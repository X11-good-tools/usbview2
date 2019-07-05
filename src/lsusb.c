/*****************************************************************************
 *
 *      lsusb.c
 *
 *      Copyright (C) 1999-2001, 2003
 *        Thomas Sailer (t.sailer@alumni.ethz.ch)
 *      Copyright (C) 2003-2005 David Brownell
 *	Copyright (C) 2010 Luis Galdos (felipito@gmx.net)
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 *      NOTE: Parts of this file are coming from the lsusb application
 *
 *****************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <endian.h>
#include <byteswap.h>
#include <usb.h>

#include <gtk/gtk.h>

#include "usbparse.h"
#include "lsusb.h"
#include "callbacks.h"

#include <xml/parser.h>

#if 0
#define LSUSB_DBG
#endif

#if defined(LSUSB_DBG)
#define pr_dbg(fmt, args...)		do { g_print("[DBG] lsusb: " fmt, ## args); } while(0)
#else
#define pr_dbg(fmt, args...)		do { } while (0)
#endif /* LSUSB_DBG */

#define EP_INTERVAL_PATTERN			"%u ms"
#define LSUSB_BUS_ROOT				"/dev/bus/usb"
#define LSUSB_BUS_PATTERN			"/dev/bus/usb/%03u"
#define LSUSB_BUS_PATTERN_LEN			(sizeof(LSUSB_BUS_PATTERN) * \
							sizeof(gchar) + 2)
#define LSUSB_MAX_ROOT_HUBS			(32)
#define UNKNOWN_INTERFACE_NAME			"Unknown"

struct root_hub {
  struct usb_device *dev;
};

/* Internal data */
static struct hash_table *hashes = NULL;
static time_t rootPrevious;
static time_t bussesPrevious[MAX_CHILDREN];  

/* This comes from the GTK+ corner */
extern void clear_devices_tree(void);

int lsusb_init_ids(const gchar *file)
{
  int ret;
  
  hashes = g_malloc0(sizeof(*hashes));

  /* If the parser fails, then we don't have any hashes */
  ret = parse_ids_file(file, hashes);
  if (ret) {
    g_free(hashes);
    hashes = NULL;
  }

  return 0;
}

static gchar *lsusb_endpoint_types[] = {
  [USB_ENDPOINT_TYPE_CONTROL] = "Control",
  [USB_ENDPOINT_TYPE_ISOCHRONOUS] = "Iso.",
  [USB_ENDPOINT_TYPE_BULK] = "Bulk",
  [USB_ENDPOINT_TYPE_INTERRUPT] = "Int.",
};

static DeviceEndpoint *lsusb_alloc_endpoint(Device *dev,
					     struct usb_endpoint_descriptor *edesc)
{
  ssize_t len;
  DeviceEndpoint *ret;
  gchar *type;

  ret = g_malloc0(sizeof(*ret));

  ret->address = edesc->bEndpointAddress;
  ret->attribute = edesc->bmAttributes;
  ret->maxPacketSize = edesc->wMaxPacketSize;
  
  len = sizeof(EP_INTERVAL_PATTERN) + 4;
  ret->interval = g_malloc0(len * sizeof(gchar));
  snprintf(ret->interval, len, EP_INTERVAL_PATTERN, edesc->bInterval);

  ret->in = (edesc->bEndpointAddress & USB_ENDPOINT_IN) ? 1 : 0;

  /* Alloc this member cause the usbparse.c will free it */
  type = lsusb_endpoint_types[edesc->bmAttributes & USB_ENDPOINT_TYPE_MASK];
  ret->type = g_malloc0(strlen(type) + 2);
  strncpy(ret->type, type, strlen(type));
  
  return ret;
}

static DeviceInterface *lsusb_alloc_interface(Device *gdev,
					    struct usb_interface_descriptor *idesc)
{
  DeviceInterface *ret = NULL;
  ssize_t len;
  int cnt;
  DeviceEndpoint *ep;

  if (!gdev || !idesc)
    return NULL;
  
  ret = g_malloc0(sizeof(*ret));

  len = sizeof(UNKNOWN_INTERFACE_NAME);
  ret->name = g_malloc(len * sizeof(gchar));
  strncpy(ret->name, UNKNOWN_INTERFACE_NAME, len);
  
  ret->protocol = idesc->bInterfaceProtocol;
  ret->subClass = idesc->bInterfaceSubClass;
  ret->numEndpoints = idesc->bNumEndpoints;

  len = 4; /* @XXX: Use a correct pattern */
  ret->class = g_malloc(len * sizeof(gchar));
  snprintf(ret->class, len, "%02x", idesc->bInterfaceClass);

  /* Create the endpoints */
  for (cnt = 0; cnt < idesc->bNumEndpoints; cnt++) {

    ep = lsusb_alloc_endpoint(gdev, &idesc->endpoint[cnt]);
    if (!ep)
      continue;

    ret->endpoint[cnt] = ep;

  }
  
  return ret;
}

static DeviceConfig *lsusb_add_config(Device *gdev,
				      struct usb_config_descriptor *cdesc)
{
  DeviceConfig *ret = NULL;
  int cnt, pos;
  DeviceInterface *g_iface;
  struct usb_interface *u_iface;

  if (!gdev || !cdesc)
    return NULL;
  
  ret = g_malloc0(sizeof(*ret));

  ret->maxPower = (gchar *)g_malloc0 ((CONFIG_MAXPOWER_SIZE) * sizeof(gchar));
  snprintf(ret->maxPower, CONFIG_MAXPOWER_SIZE, "%3u mA", 2 * cdesc->MaxPower);
  ret->numInterfaces = cdesc->bNumInterfaces;
  ret->configNumber = cdesc->bConfigurationValue;
  ret->attributes = cdesc->bmAttributes;

  for (cnt = 0; cnt < cdesc->bNumInterfaces; cnt++) {

    u_iface = &cdesc->interface[cnt];

    for (pos = 0; pos < u_iface->num_altsetting; pos++) {

      g_iface = lsusb_alloc_interface(gdev, &u_iface->altsetting[pos]);
      if (!g_iface)
	continue;
      
      ret->interface[cnt] = g_iface;
    }
  }
  
  return ret;
}

/* Create a new device from the passed usb_device */
#define DEVICE_NAME_PATTERN		"%s [%s]"
#define DEVICE_SRNR_PATTERN		"%2x.%02x"
static Device *lsusb_alloc_device(struct usb_device *udev, Device *parent)
{
  Device *gdev;
  gchar vid[VID_STR_LEN], pid[PID_STR_LEN];
  struct product_t *product;
  struct vendor_t *vendor;
  ssize_t len;
  gchar *pstr, *vstr;
  struct usb_config_descriptor *cfg;
  int cnt;
  
  gdev = (Device *)(g_malloc0 (sizeof(Device)));

  ++parent->maxChildren;
  parent->child[parent->maxChildren-1] = gdev;
    
  snprintf(vid, sizeof(vid), "%04x", udev->descriptor.idVendor);
  snprintf(pid, sizeof(pid), "%04x", udev->descriptor.idProduct);

  /* Search for the vendor if we have the hashes */
  vendor = hashes ? parse_search_vendor(vid, hashes) : NULL;
  vstr = vendor ? vendor->name : vid;
  if (!vendor)
    pr_dbg("Couldn't find the vendor with ID '%s'\n", vid);
  
  /* Search for the product */
  product = hashes ? parse_search_product(pid, vendor) : NULL;
  pstr = product ? product->name : pid;
  if (!product)
    pr_dbg("Couldn't find the product with ID '%s'\n", pid);

  /* Build the device name */
  if (product) {
    len = strlen(pstr) + 2;
    gdev->name = g_malloc0(len * sizeof(gchar));
    snprintf(gdev->name, len, "%s", pstr);
  } else {
    /*
     * If no product name is provided, then use the pattern VID:PID for
     * identifying the device
     */
    len = strlen(pstr) + strlen(vstr) + 4;
    gdev->name = g_malloc0(len * sizeof(gchar));
    snprintf(gdev->name, len, "%s:%s", vstr, pstr);
  }

  /* Build the device properties */
  gdev->vendorId = udev->descriptor.idVendor;
  gdev->productId = udev->descriptor.idProduct;
  gdev->deviceNumber = g_ascii_strtoull(udev->filename, NULL, 10);
  gdev->busNumber = g_ascii_strtoull(udev->bus->dirname, NULL, 10);

  /* Need to alloc this value, cause usbparse.c is freeing it */
  len = strlen(vstr) * sizeof(gchar) + 2;
  gdev->manufacturer = g_malloc(len);
  strncpy(gdev->manufacturer,vstr, len);

  len = strlen(DEVICE_SRNR_PATTERN) + 4; /* @XXX: Use a correct string pattern */
  gdev->revisionNumber = g_malloc0(len * sizeof(gchar));
  snprintf(gdev->revisionNumber, len,
	   DEVICE_SRNR_PATTERN,
	   udev->descriptor.bcdDevice >> 8, udev->descriptor.bcdDevice & 0xff);

  len = strlen(DEVICE_SRNR_PATTERN) + 4;
  gdev->version = g_malloc0(len * sizeof(gchar));
  snprintf(gdev->version, len,
	   DEVICE_SRNR_PATTERN,
	   udev->descriptor.bcdUSB >> 8, udev->descriptor.bcdUSB & 0xff);

  len = 5; /* @XXX: Quick and dirty! */
  gdev->class = g_malloc0(len * sizeof(gchar));
  snprintf(gdev->class, len, "%02x", udev->descriptor.bDeviceClass);
  gdev->subClass = g_malloc0(len * sizeof(gchar));
  snprintf(gdev->subClass, len, "%02x", udev->descriptor.bDeviceSubClass);
  gdev->protocol = g_malloc0(len * sizeof(gchar));
  snprintf(gdev->protocol, len, "%02x", udev->descriptor.bDeviceProtocol);


  gdev->maxPacketSize = udev->descriptor.bMaxPacketSize0;
  gdev->numConfigs = udev->descriptor.bNumConfigurations;

  
  /* @XXX: Where can we obtain this information from? */
  gdev->driverAttached = 1;

  /* Add the device address */
  gdev->busAddress = g_ascii_strtoull(udev->filename, NULL, 10);  
  
  /* Add the configurations */
  for (cnt = 0; cnt < udev->descriptor.bNumConfigurations; cnt++) {
    cfg = &udev->config[cnt];
    gdev->config[cnt] = lsusb_add_config(gdev, cfg);
  }

  add_tree_device(parent, gdev);
  
  return gdev;
}

static int lsusb_list_hub(struct usb_device *hub, int level, Device *parent)
{
  struct usb_device *dev;
  struct usb_device_descriptor *desc;
  int nrdevs;
  Device *gdev;

  if (!hub) {
    g_print("%s(): NULL pointer found!\n", __func__);
    return -1;
  }

  gdev = lsusb_alloc_device(hub, parent);
  
  nrdevs = 0;
  for (dev = hub->prev; dev; dev = dev->prev) {

    desc = &dev->descriptor;
    if (desc->bDeviceClass == USB_CLASS_HUB) {
      int pos;

      /* This device is a hub, so pass our own structure as parent */
      nrdevs = lsusb_list_hub(dev, level + 1, gdev);
      if (nrdevs < 0) {
	g_print("Failure detected, aborting hub\n");
	return 0;
      }
      
      /* Skip all the devices of this HUB */
      for (pos = 0; pos < nrdevs; pos++)
	dev = dev->prev;
    } else {
      nrdevs++;
      lsusb_alloc_device(dev, gdev);
    }
  }

  return nrdevs;
}

/* Maximal number of supported root hubs */
int lsusb_list_devices(void)
{
  struct usb_bus *bus;
  struct usb_device *dev, *last_dev = NULL;
  struct usb_device *root_hubs[LSUSB_MAX_ROOT_HUBS];
  int status, pos;
  struct usb_device_descriptor *desc;
  
  memset(root_hubs, 0x00, sizeof(root_hubs));
  status = 1;
  pos = 0;
  
  for (bus = usb_busses; bus; bus = bus->next) {

    /* Find the last device, this is the root HUB */
    for (dev = bus->devices; dev; dev = dev->next) {
      last_dev = dev;
    }

    /* Add this device to our root hubs */
    root_hubs[pos++] = last_dev;
    if (pos >= LSUSB_MAX_ROOT_HUBS) {
      g_print("Max. number of root hubs reached, stopping %s\n", __func__);
      break;
    }
  }

  /* List the root hubs */
  for (pos = 0; root_hubs[pos]; pos++) {
    struct usb_device *hub;
    
    hub = root_hubs[pos];
    desc = &hub->descriptor;

    /* This is the first level of the tree */
    lsusb_list_hub(hub, 1, rootDevice);
  }
  
  return status;
}

static void lsusb_tree_save_times(Device *root)
{
  Device *dev;
  int ret, cnt;
  struct stat file_info;
  gchar bus_path[LSUSB_BUS_PATTERN_LEN];

  /* First check if a new root hub with a new bus was added */
  ret = stat(LSUSB_BUS_ROOT, &file_info);
  if (ret) {
    pr_dbg("stat() failed by bus root, %s\n", strerror(errno));
    return ;
  }
  rootPrevious = file_info.st_ctime;
  
  for (cnt = 0; cnt < MAX_CHILDREN && root->child[cnt]; cnt++) {
    dev = root->child[cnt];
    snprintf(bus_path, sizeof(bus_path), LSUSB_BUS_PATTERN, dev->busNumber);
    ret = stat (bus_path, &file_info);
    if (ret)
      continue;
    bussesPrevious[cnt] = file_info.st_ctime;
  }
 
}

/*
 * Return zero if the three with the corresponding busses has not changed
 * @XXX: Keep it simple by checking the time stamp of the USB bus
 */
static int lsusb_tree_has_changed(Device *root)
{
  int cnt, ret;
  Device *dev;
  struct stat file_info;
  gchar bus_path[LSUSB_BUS_PATTERN_LEN];
  int has_changed;

  /* First check if a new root hub with a new bus was added */
  ret = stat (LSUSB_BUS_ROOT, &file_info);
  if (ret) {
    pr_dbg("stat() failed by bus root, %s\n", strerror(errno));
    return 0;
  }

  if (file_info.st_ctime != rootPrevious) {
    pr_dbg("BUS directory has changed\n");
    has_changed = 1;
    goto exit_changed;
  }

  /* Check for root hubs modifications */
  has_changed = 0;
  for (cnt = 0; cnt < MAX_CHILDREN && root->child[cnt]; cnt++) {

    dev = root->child[cnt];
    snprintf(bus_path, sizeof(bus_path), LSUSB_BUS_PATTERN, dev->busNumber);

    ret = stat (bus_path, &file_info);
    if (ret) {
      pr_dbg("@ERROR: stat() failed, %s\n", strerror(errno));
      break;
    }

    if (file_info.st_ctime != bussesPrevious[cnt]) {      
      has_changed = 1;
      break;
    }
  }

 exit_changed:
  return has_changed;
}

/*
 * Call this function for updating the root device using lsusb. Return values:
 * TRUE  : Reload the tree
 * FALSE : Tree refresh not required
 */
int lsusb_load_tree(int force_refresh)
{

  /* By forced refresh only save the root directory time stamp */
  if (!force_refresh) {
    if (!lsusb_tree_has_changed(rootDevice))
      return FALSE;
  }

  /* Call these libusb functions for refreshing the devices list */
  usb_find_busses();
  usb_find_devices();
  
  /*
   * This will clear the USB devices and alloc 'rootDevice'. 
   */
  usb_initialize_list();

  /* Now clear the tree before starting to list the devices tree */
  clear_devices_tree();
  
  /* Now list the available devices */
  lsusb_list_devices();

  /* Save the different time stamps */
  lsusb_tree_save_times(rootDevice);

  return TRUE;
}
