/*************************************************************************
 ** usbtree.c for USBView2
 ** Copyright (c) 1999, 2000 by Greg Kroah-Hartman, <greg@kroah.com>
 **		  2007 by Luis Galdos <felipito@gmx.net>
 **
 **  This program is free software; you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation; either version 2 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program; if not, write to the Free Software
 **  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 **
 ** (See the included file COPYING)
 *************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <gtk/gtk.h>

#include "usbtree.h"
#include "usbparse.h"

#define MAX_LINE_SIZE	1000
#define FILENAME_SIZE	1000

static gchar previousDevicesFile[1000];
static time_t	previousChange;
const char *verifyMessage = "Please verify the following dependencies:\n\
- USB subsystem is compiled into your kernel\n\
- The USB core modules are loaded\n\
- usbdevfs filesystem is mounted\n\
  (example: mount -t usbfs none /proc/bus/usb)";


/* This external functions are from callbacks.c */
extern void update_device_infos(const gchar *data);
extern void update_operation_statusbar(const gchar *text);
extern int add_tree_device(Device *parent, Device *device);
extern void clear_devices_tree(void);
extern void display_message(const gchar *title, const gchar *mes,
			    gboolean centered, int msgtype);
extern void add_list_configurations(DeviceConfig *config);
extern void add_list_interfaces(DeviceConfig *config, DeviceInterface *iface);
extern void add_list_endpoints(DeviceConfig *config, DeviceInterface *iface,
			       DeviceEndpoint *ep);


void PopulateDevConfigs (int deviceId)
{
  Device *device;
  gint configNum;
  DeviceConfig *config;
  int deviceNumber = (deviceId >> 8);
  int busNumber = (deviceId & 0x00ff);

  if(!deviceId) return;
  device = usb_find_device (deviceNumber, busNumber);
  if (device == NULL) {
    display_message("Usbview2 - Error",
		    "Can't seem to find device info to display\n", FALSE, 0);
    return;
  }

  for (configNum = 0; configNum < MAX_CONFIGS; ++configNum) {
    if (device->config[configNum]) {
      config = device->config[configNum];
      add_list_configurations(config);
    }
    else break;
  }
}


void PopulateDevInterfaces (int deviceId)
{
  Device *device;
  gint configNum, interfaceNum;
  DeviceConfig *config;
  int deviceNumber = (deviceId >> 8);
  int busNumber = (deviceId & 0x00ff);
  DeviceInterface *interface;
  
  if(!deviceId) return;
  device = usb_find_device (deviceNumber, busNumber);
  if (device == NULL) {
    display_message("Usbview2 - Error",
		    "Can't seem to find device info to display\n", FALSE, 0);
    return;
  }

  for (configNum = 0; configNum < MAX_CONFIGS; ++configNum) {
    if (device->config[configNum]) {
      config = device->config[configNum];
      for (interfaceNum = 0; interfaceNum < MAX_INTERFACES; ++interfaceNum) {
	if (config->interface[interfaceNum]) {
	  interface = config->interface[interfaceNum];
	  add_list_interfaces(config, interface);
	}
      }
    }
    else break;
  }
}


void PopulateDevEndpoints (int deviceId)
{
  Device *device;
  gint configNum, interfaceNum, endpointNum;
  DeviceConfig *config;
  int deviceNumber = (deviceId >> 8);
  int busNumber = (deviceId & 0x00ff);
  DeviceInterface *interface;
  DeviceEndpoint *ep;
  
  if(!deviceId) return;
  device = usb_find_device (deviceNumber, busNumber);
  if (device == NULL) {
    display_message("Usbview2 - Error",
		    "Can't seem to find device info to display\n", FALSE, 0);
    return;
  }

  for (configNum = 0; configNum < MAX_CONFIGS; ++configNum) {
    if (device->config[configNum]) {
      config = device->config[configNum];
      for (interfaceNum = 0; interfaceNum < MAX_INTERFACES; ++interfaceNum) {
	if (config->interface[interfaceNum]) {
	  interface = config->interface[interfaceNum];
	  for (endpointNum = 0; endpointNum < MAX_ENDPOINTS; ++endpointNum) {
	    if (interface->endpoint[endpointNum]) {
	      ep = interface->endpoint[endpointNum];
	      add_list_endpoints(config, interface, ep);
	    }
	  }
	}
      }
    }
    else break;
  }
}


void PopulateListBox (int deviceId)
{
  Device  *device;
  char    *tempString;
  int     configNum;
  int     interfaceNum;
  int     endpointNum;
  int     deviceNumber = (deviceId >> 8);
  int     busNumber = (deviceId & 0x00ff);
  gchar *output, *tout;
  
  if(deviceId==0) return;  
  
  device = usb_find_device (deviceNumber, busNumber);
  if (device == NULL) {
    display_message("Usbview2 - Error",
		    "Can't seem to find device info to display\n", FALSE, 0);
    return;
  }

  /* add the manufacturer if we have one */
  output = tout = NULL;
  if (device->manufacturer != NULL)
    output = g_strdup_printf("\nManufacturer: %s", device->manufacturer);

  /* add the serial number if we have one */
  if (device->serialNumber != NULL) {
    if(output) {
      tout = output;
      output = g_strdup_printf("%s\nSerial Number: %s", tout, device->serialNumber);
      g_free(tout);
    }
    else
      output = g_strdup_printf("\nSerial Number: %s", device->serialNumber);
  }
    
  /* add speed */
  switch (device->speed) {
  case 1 :        tempString = "1.5Mb/s (low)";   break;
  case 12 :       tempString = "12Mb/s (full)";   break;
  case 480 :      tempString = "480Mb/s (high)";  break;
  default :       tempString = "unknown";         break;
  }

  if(output) {
    tout = output;
    output = g_strdup_printf("%s\nSpeed: %s", tout, tempString);
    g_free(tout);
  }
  else {
    output = g_strdup_printf("\nSpeed: %s", tempString);
  }
  
  /* add ports if available */
  if (device->maxChildren) {
    tout = output;
    output = g_strdup_printf("%s\nNumber of Ports: %i", tout, device->maxChildren);
    g_free(tout);
  }

  /* add the bandwidth info if available */
  if (device->bandwidth != NULL) {
    tout = output;
    output = g_strdup_printf("%s\nBandwitch allocated: %i / %i (%i%%)\
\nTotal number of interrupt requests: %i\
\nTotal number of isochronous requests: %i",
		    tout, device->bandwidth->allocated, device->bandwidth->total,
		    device->bandwidth->percent,
		    device->bandwidth->numInterruptRequests,
		    device->bandwidth->numIsocRequests);
    g_free(tout);
  }

  /* add the USB version, device class, subclass, protocol,
     max packet size, and the number of configurations (if it is there) */
  if (device->version) {
    tout = output;
    output = g_strdup_printf("%s\nUSB Version: %s\
\nDevice Class: %s\
\nDevice Subclass: %s\
\nDevice Protocol: %s\
\nMaximum Default Endpoint Size: %i\
\nNumber of Configurations: %i", tout, device->version, device->class,
			     device->subClass, device->protocol,
			     device->maxPacketSize, device->numConfigs);
    g_free(tout);
  }

  /* add the vendor id, product id, and revision number (if it is there) */
  if (device->vendorId) {
    tout = output;
    output = g_strdup_printf("%s\nVendor Id: %.4x\
\nProduct Id: %.4x\
\nRevision Number: %s", tout, device->vendorId, device->productId,
			     device->revisionNumber);
    g_free(tout);
  }

  /* display all the info for the configs */
  for (configNum = 0; configNum < MAX_CONFIGS; ++configNum) {
    if (device->config[configNum]) {
      DeviceConfig *config = device->config[configNum];

      tout = output;
      output = g_strdup_printf("%s\n\nConfig Number: %i\
\n\tNumer of Interfaces: %i\
\n\tAttributes: %.2x\
\n\tMax Power needed: %s", tout, config->configNumber,
			config->numInterfaces, config->attributes, config->maxPower);
      g_free(tout);
      
      /* show all of the interfaces for this config */
      for (interfaceNum = 0; interfaceNum < MAX_INTERFACES; ++interfaceNum) {
	if (config->interface[interfaceNum]) {
	  DeviceInterface *interface = config->interface[interfaceNum];

	  tout = output;
	  output = g_strdup_printf("%s\n\n\tInterface Number: %i",
				   tout, interface->interfaceNumber);	  
	  g_free(tout);
	  
	  if (interface->name != NULL) {

	    tout = output;
	    output = g_strdup_printf("%s\n\t\tName: %s\
\n\t\tAlternate Number: %i\
\n\t\tClass: %s\
\n\t\tSub Class: %i\
\n\t\tProtocol: %i\
\n\t\tNumber of Endpoints: %i", tout, interface->name, interface->alternateNumber,
				     interface->class, interface->subClass,
				     interface->protocol, interface->numEndpoints);
	    g_free(tout);
	    
	    /* show all of the endpoints for this interface */
	    for (endpointNum = 0; endpointNum < MAX_ENDPOINTS; ++endpointNum) {
	      if (interface->endpoint[endpointNum]) {
		DeviceEndpoint  *endpoint = interface->endpoint[endpointNum];
		
		tout = output;
		output = g_strdup_printf("%s\n\n\t\t\tEndpoint Address: %.2x\
\n\t\t\tDirection: %s\
\n\t\t\tAttribute: %i\
\n\t\t\tType: %s\
\n\t\t\tMax Packet Size: %i\
\n\t\t\tInterval: %s", tout, endpoint->address, (endpoint->in)?"in":"out",
					 endpoint->attribute, endpoint->type,
					 endpoint->maxPacketSize, endpoint->interval);
		g_free(tout);
	      }
	    }
	  }
	}
      }
    }
  }

  if(output) {
    update_device_infos(output);
    g_free(output);
  }
  return;
}


static void DisplayDevice (Device *parent, Device *device)
{
  int		i;
  int		configNum;
  int		interfaceNum;
  gboolean	driverAttached = TRUE;

  
  if (device == NULL)
    return;

  /* determine if this device has drivers attached to all interfaces */
  for (configNum = 0; configNum < MAX_CONFIGS; ++configNum) {
    if (device->config[configNum]) {
      DeviceConfig    *config = device->config[configNum];
      for (interfaceNum = 0; interfaceNum < MAX_INTERFACES; ++interfaceNum) {
	if (config->interface[interfaceNum]) {
	  DeviceInterface *interface = config->interface[interfaceNum];
	  if (interface->driverAttached == FALSE) {
	    driverAttached = FALSE;
	    break;
	  }
	}
      }
    }
  }

  device->driverAttached = driverAttached;
  add_tree_device(parent, device);
  
  /* create all of the children's leafs */
  for (i = 0; i < MAX_CHILDREN; ++i) {
    DisplayDevice (device, device->child[i]);
  }

  return;
}


static void FileError (gchar *devfile)
{
  gchar *tempString;

  clear_devices_tree();
  tempString = g_malloc0(strlen(verifyMessage) + strlen(devfile) + 50);
  sprintf(tempString, "Can not open the file %s\n\n%s", devfile,
	  verifyMessage);
  display_message("Usbview2 - Error", tempString, FALSE, 0);
  update_operation_statusbar("Error opening devices file");
  g_free (tempString);
  return;
}


static int FileHasChanged (gchar *devfile)
{
  struct stat	file_info;
  int		result;

  if (strcmp (previousDevicesFile, devfile) == 0) {
    /* we've looked at this filename before, so check the file time of the file */
    result = stat (devfile, &file_info);
    if (result) {
      /* something wrong in looking for this file */
      return 0;
    }
		
    if (file_info.st_ctime == previousChange) {
      /* no change */
      return 0;
    } else {
      /* something changed */
      previousChange = file_info.st_ctime;
      return 1;
    }
  } else {
    /* filenames are different, so save the name for the next time */
    strcpy (previousDevicesFile, devfile);
    return 1;
  }
}


int LoadUSBTree (int refresh, gchar *devfile)
{
  FILE            *usbFile;
  char            *dataLine;
  int             finished;
  int             i;
  static gboolean already_err = FALSE;
  
  /* if refresh is selected, then always do a refresh,
     otherwise look at the file first */
  if (!refresh) {
    if (!FileHasChanged(devfile)) {
      return FALSE;
    }
  }

  usbFile = fopen (devfile, "r");
  if (usbFile == NULL) {
    if(!already_err) {
      FileError(devfile);
      already_err = TRUE;
    }
    return FALSE;
  }
  
  finished = 0;

  usb_initialize_list ();

  dataLine = (char *)g_malloc (MAX_LINE_SIZE);
  while (!finished) {
    /* read the line in from the file */
    fgets (dataLine, MAX_LINE_SIZE, usbFile);

    if (dataLine[strlen(dataLine)-1] == '\n')
      usb_parse_line (dataLine);

    if (feof (usbFile))
      finished = 1;
  }

  g_free (dataLine);

  usb_name_devices ();

  clear_devices_tree();
  update_operation_statusbar("Reading devices file");
  for (i = 0; i < rootDevice->maxChildren; ++i) {
    DisplayDevice (rootDevice, rootDevice->child[i]);
  }

  /* Save the time of the previous change! */
  FileHasChanged(devfile);
  already_err = FALSE;
  return TRUE;
}


/* Set the default devices file to avoid an initial second refresh */
void initialize_stuff (const gchar *devfile)
{
  if(devfile==NULL)
    memset (&previousDevicesFile[0], 0x00, sizeof(previousDevicesFile));
  else
    strncpy(previousDevicesFile, devfile, sizeof(previousDevicesFile)-1);
  
  previousChange = 0;
  return;
}

