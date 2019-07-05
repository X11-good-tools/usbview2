/*************************************************************************
 * callbacks.c for USBView2
 * Copyright (c) 2007-2010 Luis Galdos <felipito@gmx.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * (See the included file COPYING)
 *************************************************************************/

/* The pixel buffer object was created with: */
/* gdk-pixbuf-csource --raw --name=logo logo.xpm  > ../src/logo.h */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h> /* Required for basename() */

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "logo.h"

#include "usbparse.h"
#include "usbtree.h"
#include "lsusb.h"

#define DEFAULT_REFRESH			2000
#define DEFAULT_MAX_REFRESH_SEC		100
#define DEFAULT_DEVICES_FILE		"/proc/bus/usb/devices"

/* Disable the update operation of the USB tree */
#if 0
#define ENABLE_UPDATE_DBG
#endif

/* Enable the debug messages */
#if 0
#define ENABLE_DBG
#endif
#if defined(ENABLE_DBG)
#define pr_dbg(fmt, args...)		do { g_print("[DBG] calls: " fmt, ## args); } while(0)
#else
#define pr_dbg(fmt, args...)		do { } while (0)
#endif /* ENABLE_DBG */

enum notebook_enum {
  COMPLETE,
  CONFIGURATIONS,
  INTERFACES,
  ENDPOINTS,
};

struct internal_t {
  gchar *devfile;
  gchar *devxml;	/* XML  configuration file */
  gint refresh;		/* In milliseconds */
  gboolean force;
  gboolean force_one;   /* Force the refresh only one time */

  /* Internal data */
  GtkWidget *main;
  GtkWidget *proper;
  GtkWidget *chooser;
  GtkWidget *about;
  GtkWidget *message;
  GtkNotebook *notebook;
  gint page;
  guint refresh_id;
};

static struct internal_t *in;

enum {
  COL_DEVICE_NAME = 0,
  COL_DEVICE_ID,
  COL_DEVICE_FORECOLOR,
  NUM_COLS
};

enum {
  COL_CONFIG_NUMBER,
  COL_CONFIG_IFACES,
  COL_CONFIG_ATTR,
  COL_CONFIG_POWER,
  COL_CONFIG_COLS
};

enum {
  COL_INTER_CONFIG,
  COL_INTER_NUMBER,
  COL_INTER_NAME,
  COL_INTER_ALTER,
  COL_INTER_CLASS,
  COL_INTER_SUBCLASS,
  COL_INTER_PROTO,
  COL_INTER_EPS,
  COL_INTER_NR
};

enum {
  COL_EP_CONFIG,
  COL_EP_INTER,
  COL_EP_ADDR,
  COL_EP_DIR,
  COL_EP_ATTR,
  COL_EP_TYPE,
  COL_EP_MAXSIZE,
  COL_EP_INTERV,
  COL_EP_NR
};

static void configure_treeUSB(GtkWidget *main);
static gboolean update_usb_tree(gpointer data);

static void configure_listConfigurations(GtkWidget *main);
static void configure_listInterfaces(GtkWidget *main);
static void configure_listEndpoints(GtkWidget *main);
static void add_list_column(GtkTreeView *tree, gchar *name, gint colnr);
static gint get_usb_tree_selected_id(GtkWidget *main);
static void notebook_update(void);
static void notebook_update_tab(enum notebook_enum tab, gint deviceId);
static void notebook_clear_tab(enum notebook_enum tab);

static void properties_usbfs_active(GtkWidget *window, gint active);

/****************************************************************\

Interface to the main function (read options)

\****************************************************************/
int read_input_options(int argc, char *argv[])
{
  GError *error = NULL;
  GOptionContext* context;
  int ret;

  in = g_malloc0(sizeof(struct internal_t));

  GOptionEntry entries[] = {

#if defined(USBVIEW2_WITH_USBFS)
    { "usbfs-file", 'u', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_STRING, &in->devfile,
      "Path to the Usbfs devices file", NULL },
#endif /* USBVIEW2_WITH_USBFS */
    
    { "refresh-time", 'r', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_INT, &in->refresh,
      "Tree refresh time in seconds", NULL },
    { "force-refresh", 'f', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_NONE, &in->force,
      "Force tree refresh every cycle (don't check for changes)", NULL },
    { "xml-file", 'x', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_STRING, &in->devxml,
      "XML file with the VIDs and PIDs", NULL },
    { NULL }
  };

  context = g_option_context_new (" - This is USBView2");
  g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);
  g_option_context_add_group (context, gtk_get_option_group (TRUE));

  if (!g_option_context_parse (context, &argc, &argv, &error)) {
    char *appname;

    appname = basename(argv[0]);
    
    g_print("Error parsing command line options, %s\nTry `%s --help` for more infos.\n",
	    error->message, appname);
    exit(EXIT_FAILURE);
  }

  /* Set the default value if no XML file passed */
  if (!in->devxml) {
    pr_dbg("Using default XML file '"USBXML_DIR"/usb.xml'\n");
    in->devxml = g_strdup(USBXML_DIR"/usb.xml");
  }
  
  ret = lsusb_init_ids(in->devxml);
  if (ret) {
    g_print("Unexpected failure opening the XML file '%s'\n", in->devxml);
    exit(ret);
  }
  
  if (in->refresh>0 && in->refresh<=DEFAULT_MAX_REFRESH_SEC)    
    in->refresh = 1000*in->refresh;
  else {
    if(in->refresh<0)
      g_print("[ WARN ] Wrong input parameter for the refresh time (%i)!\n",
	      in->refresh);
    if(in->refresh>DEFAULT_MAX_REFRESH_SEC)
      g_print("[ WARN ] Refresh time (%i) exceeds %i sec.!\n",
	      in->refresh, DEFAULT_MAX_REFRESH_SEC);
    in->refresh = DEFAULT_REFRESH;
  }
 
  return EXIT_SUCCESS;
}

/****************************************************************\

Interface to Usbview1.0

\****************************************************************/
void add_list_configurations(DeviceConfig *config)
{
  gchar	*attr;
  GtkTreeModel *model;
  GtkTreeIter toplevel;
  GtkWidget *tree;
  
  tree = lookup_widget(in->main, "listConfigurations");
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree));
  gtk_list_store_append(GTK_LIST_STORE(model), &toplevel);

  attr = g_strdup_printf("%.2X", config->attributes);
  gtk_list_store_set(GTK_LIST_STORE(model), &toplevel,
                     COL_CONFIG_NUMBER, config->configNumber,
		     COL_CONFIG_IFACES, config->numInterfaces,
		     COL_CONFIG_ATTR, attr,
		     COL_CONFIG_POWER, config->maxPower,
                     -1);
  g_free(attr);
}

void add_list_interfaces(DeviceConfig *config, DeviceInterface *iface)
{
  GtkTreeModel *model;
  GtkTreeIter toplevel;
  GtkWidget *tree;
  
  tree = lookup_widget(in->main, "listInterfaces");
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree));
  gtk_list_store_append(GTK_LIST_STORE(model), &toplevel);

  gtk_list_store_set(GTK_LIST_STORE(model), &toplevel,
		     COL_INTER_CONFIG, config->configNumber,
                     COL_INTER_NUMBER, iface->interfaceNumber,
		     COL_INTER_NAME, iface->name,
		     COL_INTER_ALTER, iface->alternateNumber,
		     COL_INTER_CLASS, iface->class,
		     COL_INTER_SUBCLASS, iface->subClass,
		     COL_INTER_PROTO, iface->protocol,
		     COL_INTER_EPS, iface->numEndpoints,
                     -1);
}

void add_list_endpoints(DeviceConfig *config, DeviceInterface *iface,
			DeviceEndpoint *ep)
{
  GtkTreeModel *model;
  GtkTreeIter toplevel;
  GtkWidget *tree;
  gchar *addr;
  
  tree = lookup_widget(in->main, "listEndpoints");
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree));
  gtk_list_store_append(GTK_LIST_STORE(model), &toplevel);

  addr = g_strdup_printf("%.2X", ep->address);
  gtk_list_store_set(GTK_LIST_STORE(model), &toplevel,
		     COL_EP_CONFIG, config->configNumber,
		     COL_EP_INTER, iface->interfaceNumber,
                     COL_EP_ADDR, addr,
		     COL_EP_DIR, (ep->in)?"in":"out",
		     COL_EP_ATTR, ep->attribute,
		     COL_EP_TYPE, ep->type,
		     COL_EP_MAXSIZE, ep->maxPacketSize,
		     COL_EP_INTERV, ep->interval,
                     -1);
  g_free(addr);
}

void update_device_infos(const gchar *data)
{
  GtkWidget *label;

  if(!data) return;
  
  label = lookup_widget(in->main, "labMainDevice");
  gtk_label_set_text(GTK_LABEL(label), data);
}

void clear_devices_tree(void)
{
  GtkTreeModel *model;
  GtkWidget *tree;
  GtkTreeSelection *selection;
  
  /* Unselect the tree and clean */  
  tree = lookup_widget(in->main, "treeUSB");
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree));
  selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(tree));
  if(gtk_tree_selection_count_selected_rows(selection))
    gtk_tree_selection_unselect_all(selection);
  
  gtk_tree_store_clear(GTK_TREE_STORE(model));
  notebook_update();
}

void update_operation_statusbar(const gchar *text)
{
  GtkWidget *bar;
  guint id;
  
  bar = lookup_widget(in->main, "statbarMainOp");
  id = gtk_statusbar_get_context_id (GTK_STATUSBAR(bar), "Notification");
  gtk_statusbar_push(GTK_STATUSBAR(bar), id, text);  
}

/* If the passed path pointer is NULL then clear the status bar */
void update_devfile_statusbar(const gchar *path)
{
  GtkWidget *bar;
  guint id;
  const gchar *pref = "Devices file: ";
  static guint last_msgid; 

  bar = lookup_widget(in->main, "statbarMainFile");
  id = gtk_statusbar_get_context_id (GTK_STATUSBAR(bar), "Notification");
  
  if (path) {
    gchar *data;
    
    data = g_malloc(strlen(path) + strlen(pref) + 1);
    sprintf(data, "%s%s", pref, path);
    last_msgid = gtk_statusbar_push(GTK_STATUSBAR(bar), id, data);
    g_free(data);
  } else if (last_msgid) {
      gtk_statusbar_remove(GTK_STATUSBAR(bar), id, last_msgid);
  }
}

int add_tree_device(Device *parent, Device *device)
{
  GtkTreeModel *model;
  GtkWidget *tree;
  gint id;
  const gchar *fore;
  
  tree = lookup_widget(in->main, "treeUSB");
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree));
  gtk_tree_store_append(GTK_TREE_STORE(model), &device->this, parent->pthis);
  device->pthis = &device->this;

  /* change the color of this leaf if there are no drivers attached to it */
  fore = (!device->driverAttached) ? "red" : "black";
  
  id = ((device->deviceNumber<<8) | (device->busNumber));
  if (!id) {
    /* @XXX: Need an error handler at this point */
    pr_dbg("Invalid ID number for device '%s'\n", device->name);
  }
  
  gtk_tree_store_set(GTK_TREE_STORE(model), &device->this,
                     COL_DEVICE_NAME, device->name,
		     COL_DEVICE_ID, id,
		     COL_DEVICE_FORECOLOR, fore,
                     -1);
  return EXIT_SUCCESS;
}

/* @ToDo: Implement msgtype for error/warning/info/etc. */
void display_message(const gchar *title, const gchar *mes, gboolean centered,
		     int msgtype)
{
  GtkWidget *label;

  if(!in->message)
    in->message = create_Message();
  
  label = lookup_widget(in->message, "labMessage");
  gtk_label_set_text(GTK_LABEL(label), mes);  
  gtk_window_set_title(GTK_WINDOW(in->message), title);  
  gtk_widget_show(in->message);
}

/****************************************************************\

Internal functions

\****************************************************************/
static gint get_usb_tree_selected_id(GtkWidget *main)
{
  GtkTreeSelection *selection;
  GtkTreeIter iter;
  GtkWidget *tree;
  gint id;
  GtkTreeModel *model;

  id = 0;
  tree = lookup_widget (main, "treeUSB");
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree));
  selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(tree));

  if (gtk_tree_selection_count_selected_rows(selection)) {
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
      gtk_tree_model_get (model, &iter,
			  COL_DEVICE_ID, &id,
			  -1);
    } else
      pr_dbg("Failed get selection of the Gtk tree.\n");
  } else
    pr_dbg("No row selected in the tree.\n");
  
  return id;
}

static void clear_list_by_name(GtkWidget *main, const gchar *name)
{
  GtkListStore *store;
  GtkWidget *tree;

  tree = lookup_widget (main, name);
  store = (GtkListStore *)gtk_tree_view_get_model(GTK_TREE_VIEW(tree));
  gtk_list_store_clear(store);
}

static void free_resources(void)
{
  if(!in) return;

  if (in->about) gtk_widget_destroy(in->about);
  if (in->proper) gtk_widget_destroy(in->proper);
  if (in->chooser) gtk_widget_destroy(in->chooser);
  if (in->main) gtk_widget_destroy(in->main);
  if (in->devfile) g_free(in->devfile);
  if (in->devxml) g_free(in->devxml);
  
  if(in->refresh_id) {
    g_source_remove(in->refresh_id);
    in->refresh_id = 0;
  }
  
  g_free(in);
}

/****************************************************************\

Menu functions

\****************************************************************/
void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}

void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}

void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}

void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  free_resources();  
  gtk_exit(0);
}

void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  on_btnMainAbout_clicked(NULL, NULL);
}

void
on_btnMainProperties_clicked           (GtkToolButton   *toolbutton,
                                        gpointer         user_data)
{
  GtkWidget *eventbox;
  GdkColor white;
  GtkWidget *entry, *spin;
  GtkWidget *ckbtn;
  gdouble val;
  gint usbfs_active;
  
  if(!in->proper) {
    in->proper = create_Properties();    
    white.red = white.green = white.blue = 65535;
    eventbox = lookup_widget(GTK_WIDGET(in->proper), "eventbox1");
    gtk_widget_modify_bg(GTK_WIDGET(eventbox), GTK_STATE_NORMAL, &white);

    GdkPixbuf *pixbuf;
    GtkWidget *image;
    image = lookup_widget(in->proper, "logoProperties");
    pixbuf = gdk_pixbuf_new_from_inline (-1, logo, FALSE, NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
  }

  if(in->devfile) {
    entry = lookup_widget(in->proper, "entPropertiesFile");
    gtk_entry_set_text(GTK_ENTRY(entry), in->devfile);
  }

  spin = lookup_widget(GTK_WIDGET(in->proper), "spinPropertiesTimeout");
  val = ((gdouble)in->refresh)/1000.0;
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), val);

  ckbtn = lookup_widget(in->proper, "ckbtnPropertiesForce");  
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ckbtn), in->force);

  /*
   * @XXX: We don't need to deactivate the usbfs widgets every time cause the support
   * is not available
   */
#if !defined(USBVIEW2_WITH_USBFS)
  usbfs_active = 0;
  
  {
    GtkWidget *usbtn;
    
    usbtn = lookup_widget(in->proper, "ckbtnPropertiesUsbfs");
    gtk_widget_set_sensitive(GTK_WIDGET(usbtn), 0);
  }
  
#else
  usbfs_active = (in->devfile) ? 1 : 0;
#endif /* USBVIEW2_WITH_USBFS */
  
  properties_usbfs_active(in->proper, usbfs_active);

  gtk_window_present(GTK_WINDOW(in->proper));
}

void
on_btnMainClose_clicked                (GtkToolButton   *toolbutton,
                                        gpointer         user_data)
{
  free_resources();  
  gtk_exit(0);
}

void
on_btnMainAbout_clicked                (GtkToolButton   *toolbutton,
                                        gpointer         user_data)
{
  GtkWidget *eventbox;
  GdkColor white;

  if(!in) return;

  if(!in->about) {
    in->about = create_About();
    white.red = white.green = white.blue = 65535;
    eventbox = lookup_widget(GTK_WIDGET(in->about), "eventbox3");
    gtk_widget_modify_bg(GTK_WIDGET(eventbox), GTK_STATE_NORMAL, &white);

    GdkPixbuf *pixbuf;
    GtkWidget *image;
    image = lookup_widget(in->about, "logoAbout");
    pixbuf = gdk_pixbuf_new_from_inline (-1, logo, FALSE, NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);

  }
  
  gtk_window_present(GTK_WINDOW(in->about));
}

gboolean
on_Main_delete_event                   (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  free_resources();
  gtk_exit(0);
  return TRUE; /* We destroy the main widget under free_resources() -> return TRUE */
}

void
on_Main_show                           (GtkWidget       *widget,
                                        gpointer         user_data)
{
  GdkColor white;
  GtkWidget *eventbox;
  gboolean force;
  
  /* Set the default configurations */
  in->main = widget;
  in->notebook = (GtkNotebook *)lookup_widget(in->main, "notebDevice");
  
  initialize_stuff(in->devfile);
  configure_treeUSB(widget);
  configure_listConfigurations(widget);
  configure_listInterfaces(widget);
  configure_listEndpoints(widget);
  
  white.red = white.green = white.blue = 65535;
  eventbox = lookup_widget(widget, "eventbox4");
  gtk_widget_modify_bg(GTK_WIDGET(eventbox), GTK_STATE_NORMAL, &white);

  /* Start the timeout function, but before force a tree update */
  force = in->force;
  in->force = TRUE;
  update_usb_tree(in);
  in->force = force;
  in->refresh_id = g_timeout_add(in->refresh, update_usb_tree, in);
}

/*
 * Return FALSE for stopping the repeated call of this function
 */
static gboolean update_usb_tree(gpointer _data)
{
  GtkWidget *tree;
  struct internal_t *data;
  int ret;
  const gchar *fl;
  gboolean force;
  
  if(!_data)
    return FALSE;

#if defined(ENABLE_UPDATE_DBG)
 {
   static int debug_update = 0;
   
   if (debug_update) {
     g_print("Stopping repeated call of %s\n", __func__);
     return FALSE;
   }
   debug_update++;
 }
#endif /* ENABLE_UPDATE_DBG */
 
  data = (struct internal_t *)_data;
  force = data->force || data->force_one;
  
  /* Check if we need to expand the tree due a modification */
#if defined(USBVIEW2_WITH_USBFS)
  if (data->devfile) {
    fl = data->devfile;
    ret = LoadUSBTree(force, data->devfile);
  } else {
    fl = data->devxml;
    ret = lsusb_load_tree(force);
  }
#else
  fl = data->devxml;
  ret = lsusb_load_tree(force);
#endif /* USBVIEW2_WITH_USBFS */

  update_devfile_statusbar(fl);
  
  if (ret > 0) {
    tree = lookup_widget(data->main, "treeUSB");
    gtk_tree_view_expand_all(GTK_TREE_VIEW(tree));
    update_operation_statusbar("Update succeeded");
  } else if (ret < 0) {
    /* @XXX: Add some error handler at this point or return FALSE? */
    update_operation_statusbar("Update failed");
  }

  data->force_one = 0;
  return TRUE;
}

static void add_list_column(GtkTreeView *tree, gchar *name, gint colnr)
{
  GtkTreeViewColumn *col1;
  GtkCellRenderer *rend1;

  rend1 = gtk_cell_renderer_text_new ();
  col1 = gtk_tree_view_column_new_with_attributes (_(name), rend1,
                           "text", colnr, NULL);
  gtk_tree_view_column_set_resizable (col1, TRUE);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col1);
}

static void configure_listInterfaces(GtkWidget *main)
{
  GtkWidget *tree;
  GtkListStore *store;

  tree = lookup_widget(main, "listInterfaces");

  store = gtk_list_store_new (COL_INTER_NR,
			      G_TYPE_INT,
			      G_TYPE_INT,
			      G_TYPE_STRING,
			      G_TYPE_INT,
			      G_TYPE_STRING,
			      G_TYPE_INT,
			      G_TYPE_INT,
			      G_TYPE_INT);

  gtk_tree_view_set_model (GTK_TREE_VIEW(tree), GTK_TREE_MODEL(store));
  g_object_unref (store);

  add_list_column(GTK_TREE_VIEW(tree), "Configuration", COL_INTER_CONFIG);
  add_list_column(GTK_TREE_VIEW(tree), "Number", COL_INTER_NUMBER);
  add_list_column(GTK_TREE_VIEW(tree), "Name", COL_INTER_NAME);
  add_list_column(GTK_TREE_VIEW(tree), "Alternate", COL_INTER_ALTER);
  add_list_column(GTK_TREE_VIEW(tree), "Class", COL_INTER_CLASS);
  add_list_column(GTK_TREE_VIEW(tree), "Sub Class", COL_INTER_SUBCLASS);
  add_list_column(GTK_TREE_VIEW(tree), "Protocol", COL_INTER_PROTO);
  add_list_column(GTK_TREE_VIEW(tree), "Endpoints", COL_INTER_EPS);
}

static void configure_listConfigurations(GtkWidget *main)
{
  GtkWidget *tree;
  GtkTreeModel *model;

  tree = lookup_widget(main, "listConfigurations");
  model = (GtkTreeModel *)gtk_list_store_new(COL_CONFIG_COLS,
					     G_TYPE_INT,
					     G_TYPE_INT,
					     G_TYPE_STRING,
					     G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(tree), model);
  g_object_unref(model);
  
  add_list_column(GTK_TREE_VIEW(tree), "Number", COL_CONFIG_NUMBER);
  add_list_column(GTK_TREE_VIEW(tree), "Interfaces", COL_CONFIG_IFACES);
  add_list_column(GTK_TREE_VIEW(tree), "Attributes", COL_CONFIG_ATTR);
  add_list_column(GTK_TREE_VIEW(tree), "Max. Power", COL_CONFIG_POWER);
    
  gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(tree)),
                              GTK_SELECTION_SINGLE);
}

static void configure_listEndpoints(GtkWidget *main)
{
  GtkWidget *tree;
  GtkTreeModel *model;

  tree = lookup_widget(main, "listEndpoints");
  model = (GtkTreeModel *)gtk_list_store_new(COL_EP_NR,
					     G_TYPE_INT,
					     G_TYPE_INT,
					     G_TYPE_STRING,
					     G_TYPE_STRING,
					     G_TYPE_INT,
					     G_TYPE_STRING,
					     G_TYPE_INT,
					     G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(tree), model);
  g_object_unref(model);

  add_list_column(GTK_TREE_VIEW(tree), "Configuration", COL_EP_CONFIG);
  add_list_column(GTK_TREE_VIEW(tree), "Interface", COL_EP_INTER);
  add_list_column(GTK_TREE_VIEW(tree), "Address", COL_EP_ADDR);
  add_list_column(GTK_TREE_VIEW(tree), "Direction", COL_EP_DIR);
  add_list_column(GTK_TREE_VIEW(tree), "Attribute", COL_EP_ATTR);
  add_list_column(GTK_TREE_VIEW(tree), "Type", COL_EP_TYPE);
  add_list_column(GTK_TREE_VIEW(tree), "Max. Packet", COL_EP_MAXSIZE);
  add_list_column(GTK_TREE_VIEW(tree), "Interval", COL_EP_INTERV);
    
  gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(tree)),
                              GTK_SELECTION_SINGLE);
}

static void configure_treeUSB(GtkWidget *main)
{
  GtkWidget *tree;
  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;
  GtkTreeModel *model;

  tree = lookup_widget(main, "treeUSB");

  /* Create and pack tree view column into tree view */
  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Devices");
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col);

  /* Pack cell renderer into tree view column */
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);

  /* Connect the 'text' property of the cell renderer to
   * model column that contains the device name */  
  gtk_tree_view_column_add_attribute(col, renderer, "text", COL_DEVICE_NAME);
  gtk_tree_view_column_add_attribute(col, renderer, "foreground",
				     COL_DEVICE_FORECOLOR);
  
  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "ID");
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col);
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, "text", COL_DEVICE_ID);
  gtk_tree_view_column_set_visible(col, FALSE);
 
  model = (GtkTreeModel *)gtk_tree_store_new(NUM_COLS,
					     G_TYPE_STRING,
					     G_TYPE_INT,
					     G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(tree), model);
  g_object_unref(model);
  gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(tree)),
                              GTK_SELECTION_SINGLE);
}

void
on_treeUSB_cursor_changed              (GtkTreeView     *treeview,
                                        gpointer         user_data)
{
  GtkWidget *top;
  gint id;
  
  top = gtk_widget_get_toplevel(GTK_WIDGET(treeview));
  if ((id = get_usb_tree_selected_id(top))==0) {
    pr_dbg("Got invalid ID number from the tree.\n");
    return;
  }
  
  notebook_update();
}

/****************************************************************\

File chooser

\****************************************************************/
gboolean
on_Chooser_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  if(in) in->chooser = NULL;
  return FALSE;
}

void
on_btnChooserCancel_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
  if(!in) return;
  if(!in->chooser) return;

  gtk_widget_hide(in->chooser);  
}

void
on_btnChooserOpen_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  gchar *file;
  GtkWidget *entry;

  if(!in) return;
  
  file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(in->chooser));

  entry = lookup_widget(in->proper, "entPropertiesFile");
  gtk_entry_set_text(GTK_ENTRY(entry), file);
  gtk_widget_hide(in->chooser);
}

/****************************************************************\

Properties dialog

\****************************************************************/
static void properties_usbfs_active(GtkWidget *window, gint active)
{
  GtkWidget *ent, *opbtn, *usbtn;
  
  opbtn = lookup_widget(window, "btnPropertiesOpen");
  ent = lookup_widget(window, "entPropertiesFile");
  usbtn = lookup_widget(in->proper, "ckbtnPropertiesUsbfs");

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(usbtn), active);
  gtk_widget_set_sensitive(GTK_WIDGET(opbtn), active);
  gtk_widget_set_sensitive(GTK_WIDGET(ent), active);
}

void
on_btnPropertiesOpen_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
  if(!in->chooser)
    in->chooser = create_Chooser();

  gtk_window_present(GTK_WINDOW(in->chooser));
}

void
on_ckbtnPropertiesUsbfs_toggled        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GtkWidget *ckbtn;
  gint active;
  GtkWidget *proper;

  proper = gtk_widget_get_toplevel(GTK_WIDGET(togglebutton));
  ckbtn = lookup_widget(proper, "ckbtnPropertiesUsbfs");
  active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ckbtn));

  /* Enable/Disable the widgets for the usbfs */
  properties_usbfs_active(proper, active);
}

void
on_btnPropertiesCancel_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_hide(in->proper);
}

static gint properties_usbfs_enabled(GtkWidget *window)
{
  GtkWidget *btn;
  
  btn = lookup_widget(window, "ckbtnPropertiesUsbfs");
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(btn));
}

void
on_btnPropertiesOk_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *ent;
  GtkWidget *proper;
  GtkWidget *spin;
  GtkWidget *ckbtn;
  const gchar *devfile;
  gint newref;
  GtkWidget *tree;
  GtkTreeSelection *selection;
  gint usbfs;

  proper = gtk_widget_get_toplevel(GTK_WIDGET(button));
  ent = lookup_widget(proper, "entPropertiesFile");
  spin = lookup_widget(proper, "spinPropertiesTimeout");
  ckbtn = lookup_widget(proper, "ckbtnPropertiesForce");
  
  usbfs = properties_usbfs_enabled(proper);

  devfile = gtk_entry_get_text(GTK_ENTRY(ent));

  /* When the Usbfs is enabled and no devices file is defined display an error */
  if (usbfs && !strlen(devfile)) {
    display_message("Configuration error",
		    "Usbfs enabled but no devices file defined.\n", 1, 0);
    return;
  }
  
  in->force = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ckbtn));

  /* First free the old devfile */
  if (in->devfile) {
    g_free(in->devfile);
    in->devfile = NULL;
  }

  /* If usbfs is enabled, then only duplicate the text entry value */
  if (usbfs)
    in->devfile = g_strdup(devfile);

  /* Ok, unselect the tree if one row is selected */
  tree = lookup_widget (in->main, "treeUSB");
  selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(tree));
  if(gtk_tree_selection_count_selected_rows(selection))
    gtk_tree_selection_unselect_all(selection);
  
  /* Stop the timeout function and restart with the new value */
  newref = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin));
  if(in->refresh!=newref) {
    if(in->refresh_id) g_source_remove(in->refresh_id);
    in->refresh = newref*1000;
    in->refresh_id = g_timeout_add(in->refresh, (GSourceFunc)update_usb_tree, in);
  }
  
  gtk_widget_hide(in->proper);

  /* Force the update one time */
  in->force_one = 1;
  update_usb_tree(in);
  notebook_update();
}

gboolean
on_Properties_delete_event             (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  if(in) in->proper = NULL;
  return FALSE;
}

/****************************************************************\

About window

\****************************************************************/
void
on_btnAboutOk_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  if(!in) return;
  gtk_widget_hide(in->about);
}

gboolean
on_About_delete_event                  (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  if(in) in->about = NULL;
  return FALSE;
}

/****************************************************************\

Message window

\****************************************************************/
gboolean
on_Message_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  in->message = NULL;
  return FALSE;
}

void
on_btnMessageOk_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_hide(in->message);
}

/****************************************************************\

Main notebook

\****************************************************************/
static void notebook_update(void)
{
  gint id;
  gint tab;

  /* Don't use gtk_notebook_get_current_page() to get the page number */
  id = get_usb_tree_selected_id(in->main);
  tab = in->page;
  
  notebook_clear_tab(tab);
  notebook_update_tab(tab, id);
}

/* Function called for updating the different TABs of the main window */
static void notebook_update_tab(enum notebook_enum tab, gint deviceId)
{  
  if (!deviceId)
    return;
  
  switch(tab) {
  case COMPLETE:
    PopulateListBox (deviceId);
    break;
  case CONFIGURATIONS:
    PopulateDevConfigs (deviceId);
    break;
  case INTERFACES:
    PopulateDevInterfaces(deviceId);
    break;
  case ENDPOINTS:
    PopulateDevEndpoints(deviceId);
    break;
  default:
    break;
  }
}

static void notebook_clear_tab(enum notebook_enum tab)
{
  switch(tab) {
  case COMPLETE:
    update_device_infos("");
    break;
  case CONFIGURATIONS:
    clear_list_by_name(in->main, "listConfigurations");
    break;
  case INTERFACES:
    clear_list_by_name(in->main, "listInterfaces");
    break;
  case ENDPOINTS:
    clear_list_by_name(in->main, "listEndpoints");
    break;
  default:
    break;
  }
}

void
on_notebDevice_switch_page             (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        guint            page_num,
                                        gpointer         user_data)
{
  /* Save the current page (troubles with gtk_notebook_get_current_page()) */
  in->page = page_num;  
  notebook_update();
}

