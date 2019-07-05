/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_Main (void)
{
  GtkWidget *Main;
  GtkWidget *vbox1;
  GtkWidget *menubar1;
  GtkWidget *menuitem1;
  GtkWidget *menuitem1_menu;
  GtkWidget *open1;
  GtkWidget *save1;
  GtkWidget *save_as1;
  GtkWidget *separatormenuitem1;
  GtkWidget *quit1;
  GtkWidget *menuitem3;
  GtkWidget *menuitem4;
  GtkWidget *menuitem4_menu;
  GtkWidget *about1;
  GtkWidget *toolbar1;
  GtkIconSize tmp_toolbar_icon_size;
  GtkWidget *btnMainProperties;
  GtkWidget *btnMainClose;
  GtkWidget *toolitem1;
  GtkWidget *vseparator1;
  GtkWidget *btnMainAbout;
  GtkWidget *hbox2;
  GtkWidget *hpaned1;
  GtkWidget *scrolledwindow4;
  GtkWidget *treeUSB;
  GtkWidget *notebDevice;
  GtkWidget *scrolledwindow5;
  GtkWidget *viewport1;
  GtkWidget *eventbox4;
  GtkWidget *labMainDevice;
  GtkWidget *label17;
  GtkWidget *scrolledwindow7;
  GtkWidget *listConfigurations;
  GtkWidget *label10;
  GtkWidget *scrolledwindow8;
  GtkWidget *listInterfaces;
  GtkWidget *label18;
  GtkWidget *scrolledwindow9;
  GtkWidget *listEndpoints;
  GtkWidget *label19;
  GtkWidget *hbox9;
  GtkWidget *statbarMainFile;
  GtkWidget *statbarMainOp;
  GtkAccelGroup *accel_group;

  accel_group = gtk_accel_group_new ();

  Main = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (Main), _("Usbview2"));
  gtk_window_set_default_size (GTK_WINDOW (Main), 650, 400);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (Main), vbox1);
  gtk_container_set_border_width (GTK_CONTAINER (vbox1), 4);

  menubar1 = gtk_menu_bar_new ();
  gtk_widget_show (menubar1);
  gtk_box_pack_start (GTK_BOX (vbox1), menubar1, FALSE, FALSE, 0);

  menuitem1 = gtk_menu_item_new_with_mnemonic (_("_File"));
  gtk_widget_show (menuitem1);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem1);

  menuitem1_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem1), menuitem1_menu);

  open1 = gtk_image_menu_item_new_from_stock ("gtk-open", accel_group);
  gtk_widget_show (open1);
  gtk_container_add (GTK_CONTAINER (menuitem1_menu), open1);

  save1 = gtk_image_menu_item_new_from_stock ("gtk-save", accel_group);
  gtk_widget_show (save1);
  gtk_container_add (GTK_CONTAINER (menuitem1_menu), save1);

  save_as1 = gtk_image_menu_item_new_from_stock ("gtk-save-as", accel_group);
  gtk_widget_show (save_as1);
  gtk_container_add (GTK_CONTAINER (menuitem1_menu), save_as1);

  separatormenuitem1 = gtk_separator_menu_item_new ();
  gtk_widget_show (separatormenuitem1);
  gtk_container_add (GTK_CONTAINER (menuitem1_menu), separatormenuitem1);
  gtk_widget_set_sensitive (separatormenuitem1, FALSE);

  quit1 = gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
  gtk_widget_show (quit1);
  gtk_container_add (GTK_CONTAINER (menuitem1_menu), quit1);

  menuitem3 = gtk_menu_item_new_with_mnemonic (_("_View"));
  gtk_widget_show (menuitem3);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem3);

  menuitem4 = gtk_menu_item_new_with_mnemonic (_("_Help"));
  gtk_widget_show (menuitem4);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem4);

  menuitem4_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem4), menuitem4_menu);

  about1 = gtk_menu_item_new_with_mnemonic (_("_About"));
  gtk_widget_show (about1);
  gtk_container_add (GTK_CONTAINER (menuitem4_menu), about1);

  toolbar1 = gtk_toolbar_new ();
  gtk_widget_show (toolbar1);
  gtk_box_pack_start (GTK_BOX (vbox1), toolbar1, FALSE, FALSE, 0);
  gtk_toolbar_set_style (GTK_TOOLBAR (toolbar1), GTK_TOOLBAR_ICONS);
  tmp_toolbar_icon_size = gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar1));

  btnMainProperties = (GtkWidget*) gtk_tool_button_new_from_stock ("gtk-preferences");
  gtk_widget_show (btnMainProperties);
  gtk_container_add (GTK_CONTAINER (toolbar1), btnMainProperties);

  btnMainClose = (GtkWidget*) gtk_tool_button_new_from_stock ("gtk-close");
  gtk_widget_show (btnMainClose);
  gtk_container_add (GTK_CONTAINER (toolbar1), btnMainClose);

  toolitem1 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem1);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem1);
  gtk_container_set_border_width (GTK_CONTAINER (toolitem1), 2);

  vseparator1 = gtk_vseparator_new ();
  gtk_widget_show (vseparator1);
  gtk_container_add (GTK_CONTAINER (toolitem1), vseparator1);

  btnMainAbout = (GtkWidget*) gtk_tool_button_new_from_stock ("gtk-about");
  gtk_widget_show (btnMainAbout);
  gtk_container_add (GTK_CONTAINER (toolbar1), btnMainAbout);

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox2, TRUE, TRUE, 3);
  gtk_container_set_border_width (GTK_CONTAINER (hbox2), 2);

  hpaned1 = gtk_hpaned_new ();
  gtk_widget_show (hpaned1);
  gtk_box_pack_end (GTK_BOX (hbox2), hpaned1, TRUE, TRUE, 0);

  scrolledwindow4 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow4);
  gtk_paned_pack1 (GTK_PANED (hpaned1), scrolledwindow4, FALSE, TRUE);
  gtk_widget_set_size_request (scrolledwindow4, 250, -1);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow4), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow4), GTK_SHADOW_ETCHED_IN);

  treeUSB = gtk_tree_view_new ();
  gtk_widget_show (treeUSB);
  gtk_container_add (GTK_CONTAINER (scrolledwindow4), treeUSB);
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (treeUSB), TRUE);

  notebDevice = gtk_notebook_new ();
  gtk_widget_show (notebDevice);
  gtk_paned_pack2 (GTK_PANED (hpaned1), notebDevice, TRUE, TRUE);

  scrolledwindow5 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow5);
  gtk_container_add (GTK_CONTAINER (notebDevice), scrolledwindow5);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow5), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  viewport1 = gtk_viewport_new (NULL, NULL);
  gtk_widget_show (viewport1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow5), viewport1);

  eventbox4 = gtk_event_box_new ();
  gtk_widget_show (eventbox4);
  gtk_container_add (GTK_CONTAINER (viewport1), eventbox4);

  labMainDevice = gtk_label_new ("");
  gtk_widget_show (labMainDevice);
  gtk_container_add (GTK_CONTAINER (eventbox4), labMainDevice);
  GTK_WIDGET_SET_FLAGS (labMainDevice, GTK_CAN_FOCUS);
  gtk_label_set_selectable (GTK_LABEL (labMainDevice), TRUE);
  gtk_misc_set_alignment (GTK_MISC (labMainDevice), 0, 0);

  label17 = gtk_label_new (_("Complete"));
  gtk_widget_show (label17);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebDevice), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebDevice), 0), label17);

  scrolledwindow7 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow7);
  gtk_container_add (GTK_CONTAINER (notebDevice), scrolledwindow7);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow7), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow7), GTK_SHADOW_ETCHED_IN);

  listConfigurations = gtk_tree_view_new ();
  gtk_widget_show (listConfigurations);
  gtk_container_add (GTK_CONTAINER (scrolledwindow7), listConfigurations);
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (listConfigurations), TRUE);

  label10 = gtk_label_new (_("Configurations"));
  gtk_widget_show (label10);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebDevice), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebDevice), 1), label10);

  scrolledwindow8 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow8);
  gtk_container_add (GTK_CONTAINER (notebDevice), scrolledwindow8);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow8), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow8), GTK_SHADOW_ETCHED_IN);

  listInterfaces = gtk_tree_view_new ();
  gtk_widget_show (listInterfaces);
  gtk_container_add (GTK_CONTAINER (scrolledwindow8), listInterfaces);

  label18 = gtk_label_new (_("Interfaces"));
  gtk_widget_show (label18);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebDevice), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebDevice), 2), label18);

  scrolledwindow9 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow9);
  gtk_container_add (GTK_CONTAINER (notebDevice), scrolledwindow9);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow9), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow9), GTK_SHADOW_ETCHED_IN);

  listEndpoints = gtk_tree_view_new ();
  gtk_widget_show (listEndpoints);
  gtk_container_add (GTK_CONTAINER (scrolledwindow9), listEndpoints);
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (listEndpoints), TRUE);

  label19 = gtk_label_new (_("Endpoints"));
  gtk_widget_show (label19);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebDevice), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebDevice), 3), label19);

  hbox9 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox9);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox9, FALSE, FALSE, 0);

  statbarMainFile = gtk_statusbar_new ();
  gtk_widget_show (statbarMainFile);
  gtk_box_pack_start (GTK_BOX (hbox9), statbarMainFile, TRUE, TRUE, 0);
  gtk_statusbar_set_has_resize_grip (GTK_STATUSBAR (statbarMainFile), FALSE);

  statbarMainOp = gtk_statusbar_new ();
  gtk_widget_show (statbarMainOp);
  gtk_box_pack_start (GTK_BOX (hbox9), statbarMainOp, FALSE, FALSE, 0);
  gtk_widget_set_size_request (statbarMainOp, 180, -1);
  gtk_statusbar_set_has_resize_grip (GTK_STATUSBAR (statbarMainOp), FALSE);

  g_signal_connect ((gpointer) Main, "delete_event",
                    G_CALLBACK (on_Main_delete_event),
                    NULL);
  g_signal_connect ((gpointer) Main, "show",
                    G_CALLBACK (on_Main_show),
                    NULL);
  g_signal_connect ((gpointer) open1, "activate",
                    G_CALLBACK (on_open1_activate),
                    NULL);
  g_signal_connect ((gpointer) save1, "activate",
                    G_CALLBACK (on_save1_activate),
                    NULL);
  g_signal_connect ((gpointer) save_as1, "activate",
                    G_CALLBACK (on_save_as1_activate),
                    NULL);
  g_signal_connect ((gpointer) quit1, "activate",
                    G_CALLBACK (on_quit1_activate),
                    NULL);
  g_signal_connect ((gpointer) about1, "activate",
                    G_CALLBACK (on_about1_activate),
                    NULL);
  g_signal_connect ((gpointer) btnMainProperties, "clicked",
                    G_CALLBACK (on_btnMainProperties_clicked),
                    NULL);
  g_signal_connect ((gpointer) btnMainClose, "clicked",
                    G_CALLBACK (on_btnMainClose_clicked),
                    NULL);
  g_signal_connect ((gpointer) btnMainAbout, "clicked",
                    G_CALLBACK (on_btnMainAbout_clicked),
                    NULL);
  g_signal_connect ((gpointer) treeUSB, "cursor_changed",
                    G_CALLBACK (on_treeUSB_cursor_changed),
                    NULL);
  g_signal_connect ((gpointer) notebDevice, "switch_page",
                    G_CALLBACK (on_notebDevice_switch_page),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (Main, Main, "Main");
  GLADE_HOOKUP_OBJECT (Main, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (Main, menubar1, "menubar1");
  GLADE_HOOKUP_OBJECT (Main, menuitem1, "menuitem1");
  GLADE_HOOKUP_OBJECT (Main, menuitem1_menu, "menuitem1_menu");
  GLADE_HOOKUP_OBJECT (Main, open1, "open1");
  GLADE_HOOKUP_OBJECT (Main, save1, "save1");
  GLADE_HOOKUP_OBJECT (Main, save_as1, "save_as1");
  GLADE_HOOKUP_OBJECT (Main, separatormenuitem1, "separatormenuitem1");
  GLADE_HOOKUP_OBJECT (Main, quit1, "quit1");
  GLADE_HOOKUP_OBJECT (Main, menuitem3, "menuitem3");
  GLADE_HOOKUP_OBJECT (Main, menuitem4, "menuitem4");
  GLADE_HOOKUP_OBJECT (Main, menuitem4_menu, "menuitem4_menu");
  GLADE_HOOKUP_OBJECT (Main, about1, "about1");
  GLADE_HOOKUP_OBJECT (Main, toolbar1, "toolbar1");
  GLADE_HOOKUP_OBJECT (Main, btnMainProperties, "btnMainProperties");
  GLADE_HOOKUP_OBJECT (Main, btnMainClose, "btnMainClose");
  GLADE_HOOKUP_OBJECT (Main, toolitem1, "toolitem1");
  GLADE_HOOKUP_OBJECT (Main, vseparator1, "vseparator1");
  GLADE_HOOKUP_OBJECT (Main, btnMainAbout, "btnMainAbout");
  GLADE_HOOKUP_OBJECT (Main, hbox2, "hbox2");
  GLADE_HOOKUP_OBJECT (Main, hpaned1, "hpaned1");
  GLADE_HOOKUP_OBJECT (Main, scrolledwindow4, "scrolledwindow4");
  GLADE_HOOKUP_OBJECT (Main, treeUSB, "treeUSB");
  GLADE_HOOKUP_OBJECT (Main, notebDevice, "notebDevice");
  GLADE_HOOKUP_OBJECT (Main, scrolledwindow5, "scrolledwindow5");
  GLADE_HOOKUP_OBJECT (Main, viewport1, "viewport1");
  GLADE_HOOKUP_OBJECT (Main, eventbox4, "eventbox4");
  GLADE_HOOKUP_OBJECT (Main, labMainDevice, "labMainDevice");
  GLADE_HOOKUP_OBJECT (Main, label17, "label17");
  GLADE_HOOKUP_OBJECT (Main, scrolledwindow7, "scrolledwindow7");
  GLADE_HOOKUP_OBJECT (Main, listConfigurations, "listConfigurations");
  GLADE_HOOKUP_OBJECT (Main, label10, "label10");
  GLADE_HOOKUP_OBJECT (Main, scrolledwindow8, "scrolledwindow8");
  GLADE_HOOKUP_OBJECT (Main, listInterfaces, "listInterfaces");
  GLADE_HOOKUP_OBJECT (Main, label18, "label18");
  GLADE_HOOKUP_OBJECT (Main, scrolledwindow9, "scrolledwindow9");
  GLADE_HOOKUP_OBJECT (Main, listEndpoints, "listEndpoints");
  GLADE_HOOKUP_OBJECT (Main, label19, "label19");
  GLADE_HOOKUP_OBJECT (Main, hbox9, "hbox9");
  GLADE_HOOKUP_OBJECT (Main, statbarMainFile, "statbarMainFile");
  GLADE_HOOKUP_OBJECT (Main, statbarMainOp, "statbarMainOp");

  gtk_window_add_accel_group (GTK_WINDOW (Main), accel_group);

  return Main;
}

GtkWidget*
create_Chooser (void)
{
  GtkWidget *Chooser;
  GtkWidget *dialog_vbox3;
  GtkWidget *dialog_action_area3;
  GtkWidget *btnChooserCancel;
  GtkWidget *btnChooserOpen;

  Chooser = gtk_file_chooser_dialog_new (_("Usbview2 - Devices file selection"), NULL, GTK_FILE_CHOOSER_ACTION_OPEN, NULL);
  gtk_window_set_position (GTK_WINDOW (Chooser), GTK_WIN_POS_MOUSE);
  gtk_window_set_type_hint (GTK_WINDOW (Chooser), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox3 = GTK_DIALOG (Chooser)->vbox;
  gtk_widget_show (dialog_vbox3);

  dialog_action_area3 = GTK_DIALOG (Chooser)->action_area;
  gtk_widget_show (dialog_action_area3);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area3), GTK_BUTTONBOX_END);

  btnChooserCancel = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (btnChooserCancel);
  gtk_dialog_add_action_widget (GTK_DIALOG (Chooser), btnChooserCancel, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (btnChooserCancel, GTK_CAN_DEFAULT);

  btnChooserOpen = gtk_button_new_from_stock ("gtk-open");
  gtk_widget_show (btnChooserOpen);
  gtk_dialog_add_action_widget (GTK_DIALOG (Chooser), btnChooserOpen, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (btnChooserOpen, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) Chooser, "delete_event",
                    G_CALLBACK (on_Chooser_delete_event),
                    NULL);
  g_signal_connect ((gpointer) btnChooserCancel, "clicked",
                    G_CALLBACK (on_btnChooserCancel_clicked),
                    NULL);
  g_signal_connect ((gpointer) btnChooserOpen, "clicked",
                    G_CALLBACK (on_btnChooserOpen_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (Chooser, Chooser, "Chooser");
  GLADE_HOOKUP_OBJECT_NO_REF (Chooser, dialog_vbox3, "dialog_vbox3");
  GLADE_HOOKUP_OBJECT_NO_REF (Chooser, dialog_action_area3, "dialog_action_area3");
  GLADE_HOOKUP_OBJECT (Chooser, btnChooserCancel, "btnChooserCancel");
  GLADE_HOOKUP_OBJECT (Chooser, btnChooserOpen, "btnChooserOpen");

  gtk_widget_grab_default (btnChooserOpen);
  return Chooser;
}

GtkWidget*
create_About (void)
{
  GtkWidget *About;
  GtkWidget *frame1;
  GtkWidget *vbox4;
  GtkWidget *eventbox3;
  GtkWidget *logoAbout;
  GtkWidget *hseparator5;
  GtkWidget *label7;
  GtkWidget *hseparator6;
  GtkWidget *hbuttonbox1;
  GtkWidget *btnAboutOk;

  About = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (About), _("Usbview2 - About"));
  gtk_window_set_position (GTK_WINDOW (About), GTK_WIN_POS_MOUSE);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_show (frame1);
  gtk_container_add (GTK_CONTAINER (About), frame1);
  gtk_frame_set_label_align (GTK_FRAME (frame1), 0, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_ETCHED_OUT);

  vbox4 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox4);
  gtk_container_add (GTK_CONTAINER (frame1), vbox4);

  eventbox3 = gtk_event_box_new ();
  gtk_widget_show (eventbox3);
  gtk_box_pack_start (GTK_BOX (vbox4), eventbox3, FALSE, FALSE, 0);

  logoAbout = create_pixmap (About, NULL);
  gtk_widget_show (logoAbout);
  gtk_container_add (GTK_CONTAINER (eventbox3), logoAbout);

  hseparator5 = gtk_hseparator_new ();
  gtk_widget_show (hseparator5);
  gtk_box_pack_start (GTK_BOX (vbox4), hseparator5, FALSE, FALSE, 0);

  label7 = gtk_label_new (_("\n<b>Usbview2</b> (v0.3a)\nThis application is based on\nUsbview of Greg Kroah-Hartman\n\n<span weight=\"bold\">GNU General Public License</span>\n\nfelipito@gmx.net\n<span size=\"medium\">(c) 2007 Luis Galdos</span>\n"));
  gtk_widget_show (label7);
  gtk_box_pack_start (GTK_BOX (vbox4), label7, FALSE, FALSE, 0);
  gtk_label_set_use_markup (GTK_LABEL (label7), TRUE);
  gtk_label_set_justify (GTK_LABEL (label7), GTK_JUSTIFY_CENTER);

  hseparator6 = gtk_hseparator_new ();
  gtk_widget_show (hseparator6);
  gtk_box_pack_start (GTK_BOX (vbox4), hseparator6, FALSE, FALSE, 0);

  hbuttonbox1 = gtk_hbutton_box_new ();
  gtk_widget_show (hbuttonbox1);
  gtk_box_pack_end (GTK_BOX (vbox4), hbuttonbox1, FALSE, TRUE, 6);

  btnAboutOk = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (btnAboutOk);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), btnAboutOk);
  GTK_WIDGET_SET_FLAGS (btnAboutOk, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) About, "delete_event",
                    G_CALLBACK (on_About_delete_event),
                    NULL);
  g_signal_connect ((gpointer) btnAboutOk, "clicked",
                    G_CALLBACK (on_btnAboutOk_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (About, About, "About");
  GLADE_HOOKUP_OBJECT (About, frame1, "frame1");
  GLADE_HOOKUP_OBJECT (About, vbox4, "vbox4");
  GLADE_HOOKUP_OBJECT (About, eventbox3, "eventbox3");
  GLADE_HOOKUP_OBJECT (About, logoAbout, "logoAbout");
  GLADE_HOOKUP_OBJECT (About, hseparator5, "hseparator5");
  GLADE_HOOKUP_OBJECT (About, label7, "label7");
  GLADE_HOOKUP_OBJECT (About, hseparator6, "hseparator6");
  GLADE_HOOKUP_OBJECT (About, hbuttonbox1, "hbuttonbox1");
  GLADE_HOOKUP_OBJECT (About, btnAboutOk, "btnAboutOk");

  return About;
}

GtkWidget*
create_Properties (void)
{
  GtkWidget *Properties;
  GtkWidget *frame2;
  GtkWidget *vbox5;
  GtkWidget *eventbox1;
  GtkWidget *logoProperties;
  GtkWidget *hseparator7;
  GtkWidget *vbox6;
  GtkWidget *label8;
  GtkWidget *hbox7;
  GtkWidget *label9;
  GtkObject *spinPropertiesTimeout_adj;
  GtkWidget *spinPropertiesTimeout;
  GtkWidget *hbox11;
  GtkWidget *label20;
  GtkWidget *ckbtnPropertiesForce;
  GtkWidget *label10;
  GtkWidget *hbox8;
  GtkWidget *entPropertiesFile;
  GtkWidget *btnPropertiesOpen;
  GtkWidget *image8;
  GtkWidget *hseparator8;
  GtkWidget *hbuttonbox2;
  GtkWidget *btnPropertiesCancel;
  GtkWidget *btnPropertiesOk;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  Properties = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (Properties), _("Usbview2 - Properties"));
  gtk_window_set_position (GTK_WINDOW (Properties), GTK_WIN_POS_MOUSE);

  frame2 = gtk_frame_new (NULL);
  gtk_widget_show (frame2);
  gtk_container_add (GTK_CONTAINER (Properties), frame2);
  gtk_frame_set_label_align (GTK_FRAME (frame2), 0, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_ETCHED_OUT);

  vbox5 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox5);
  gtk_container_add (GTK_CONTAINER (frame2), vbox5);

  eventbox1 = gtk_event_box_new ();
  gtk_widget_show (eventbox1);
  gtk_box_pack_start (GTK_BOX (vbox5), eventbox1, FALSE, FALSE, 0);

  logoProperties = create_pixmap (Properties, NULL);
  gtk_widget_show (logoProperties);
  gtk_container_add (GTK_CONTAINER (eventbox1), logoProperties);

  hseparator7 = gtk_hseparator_new ();
  gtk_widget_show (hseparator7);
  gtk_box_pack_start (GTK_BOX (vbox5), hseparator7, FALSE, FALSE, 0);

  vbox6 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox6);
  gtk_box_pack_start (GTK_BOX (vbox5), vbox6, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (vbox6), 8);

  label8 = gtk_label_new (_("<b>Refresh</b>"));
  gtk_widget_show (label8);
  gtk_box_pack_start (GTK_BOX (vbox6), label8, FALSE, FALSE, 8);
  gtk_label_set_use_markup (GTK_LABEL (label8), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label8), 0, 0.5);

  hbox7 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox7);
  gtk_box_pack_start (GTK_BOX (vbox6), hbox7, FALSE, FALSE, 2);

  label9 = gtk_label_new (_("Time in seconds"));
  gtk_widget_show (label9);
  gtk_box_pack_start (GTK_BOX (hbox7), label9, TRUE, TRUE, 6);
  gtk_misc_set_alignment (GTK_MISC (label9), 0, 0.5);

  spinPropertiesTimeout_adj = gtk_adjustment_new (1, 1, 100, 1, 10, 10);
  spinPropertiesTimeout = gtk_spin_button_new (GTK_ADJUSTMENT (spinPropertiesTimeout_adj), 1, 0);
  gtk_widget_show (spinPropertiesTimeout);
  gtk_box_pack_start (GTK_BOX (hbox7), spinPropertiesTimeout, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, spinPropertiesTimeout, _("Refresh time in seconds"), NULL);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinPropertiesTimeout), TRUE);
  gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spinPropertiesTimeout), GTK_UPDATE_IF_VALID);

  hbox11 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox11);
  gtk_box_pack_start (GTK_BOX (vbox6), hbox11, FALSE, FALSE, 4);

  label20 = gtk_label_new (_("Force refresh"));
  gtk_widget_show (label20);
  gtk_box_pack_start (GTK_BOX (hbox11), label20, TRUE, TRUE, 6);
  gtk_misc_set_alignment (GTK_MISC (label20), 0, 0.5);

  ckbtnPropertiesForce = gtk_check_button_new_with_mnemonic ("");
  gtk_widget_show (ckbtnPropertiesForce);
  gtk_box_pack_start (GTK_BOX (hbox11), ckbtnPropertiesForce, FALSE, FALSE, 0);
  GTK_WIDGET_UNSET_FLAGS (ckbtnPropertiesForce, GTK_CAN_FOCUS);
  gtk_tooltips_set_tip (tooltips, ckbtnPropertiesForce, _("Force a refresh without looking for changes of the devices file.\n(The devices tree will be deselected every cycle)"), NULL);
  gtk_button_set_focus_on_click (GTK_BUTTON (ckbtnPropertiesForce), FALSE);

  label10 = gtk_label_new (_("<b>Devices file</b>"));
  gtk_widget_show (label10);
  gtk_box_pack_start (GTK_BOX (vbox6), label10, FALSE, FALSE, 8);
  gtk_label_set_use_markup (GTK_LABEL (label10), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label10), 0, 0.5);

  hbox8 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox8);
  gtk_box_pack_start (GTK_BOX (vbox6), hbox8, FALSE, FALSE, 2);

  entPropertiesFile = gtk_entry_new ();
  gtk_widget_show (entPropertiesFile);
  gtk_box_pack_start (GTK_BOX (hbox8), entPropertiesFile, TRUE, TRUE, 6);
  gtk_tooltips_set_tip (tooltips, entPropertiesFile, _("File with the USB devices"), NULL);
  gtk_editable_set_editable (GTK_EDITABLE (entPropertiesFile), FALSE);

  btnPropertiesOpen = gtk_button_new ();
  gtk_widget_show (btnPropertiesOpen);
  gtk_box_pack_start (GTK_BOX (hbox8), btnPropertiesOpen, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, btnPropertiesOpen, _("Chooser for the USB devices file"), NULL);

  image8 = gtk_image_new_from_stock ("gtk-open", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image8);
  gtk_container_add (GTK_CONTAINER (btnPropertiesOpen), image8);

  hseparator8 = gtk_hseparator_new ();
  gtk_widget_show (hseparator8);
  gtk_box_pack_start (GTK_BOX (vbox5), hseparator8, FALSE, TRUE, 4);

  hbuttonbox2 = gtk_hbutton_box_new ();
  gtk_widget_show (hbuttonbox2);
  gtk_box_pack_end (GTK_BOX (vbox5), hbuttonbox2, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbuttonbox2), 4);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox2), GTK_BUTTONBOX_END);
  gtk_box_set_spacing (GTK_BOX (hbuttonbox2), 10);

  btnPropertiesCancel = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (btnPropertiesCancel);
  gtk_container_add (GTK_CONTAINER (hbuttonbox2), btnPropertiesCancel);
  GTK_WIDGET_SET_FLAGS (btnPropertiesCancel, GTK_CAN_DEFAULT);

  btnPropertiesOk = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (btnPropertiesOk);
  gtk_container_add (GTK_CONTAINER (hbuttonbox2), btnPropertiesOk);
  GTK_WIDGET_SET_FLAGS (btnPropertiesOk, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) Properties, "delete_event",
                    G_CALLBACK (on_Properties_delete_event),
                    NULL);
  g_signal_connect ((gpointer) btnPropertiesOpen, "clicked",
                    G_CALLBACK (on_btnPropertiesOpen_clicked),
                    NULL);
  g_signal_connect ((gpointer) btnPropertiesCancel, "clicked",
                    G_CALLBACK (on_btnPropertiesCancel_clicked),
                    NULL);
  g_signal_connect ((gpointer) btnPropertiesOk, "clicked",
                    G_CALLBACK (on_btnPropertiesOk_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (Properties, Properties, "Properties");
  GLADE_HOOKUP_OBJECT (Properties, frame2, "frame2");
  GLADE_HOOKUP_OBJECT (Properties, vbox5, "vbox5");
  GLADE_HOOKUP_OBJECT (Properties, eventbox1, "eventbox1");
  GLADE_HOOKUP_OBJECT (Properties, logoProperties, "logoProperties");
  GLADE_HOOKUP_OBJECT (Properties, hseparator7, "hseparator7");
  GLADE_HOOKUP_OBJECT (Properties, vbox6, "vbox6");
  GLADE_HOOKUP_OBJECT (Properties, label8, "label8");
  GLADE_HOOKUP_OBJECT (Properties, hbox7, "hbox7");
  GLADE_HOOKUP_OBJECT (Properties, label9, "label9");
  GLADE_HOOKUP_OBJECT (Properties, spinPropertiesTimeout, "spinPropertiesTimeout");
  GLADE_HOOKUP_OBJECT (Properties, hbox11, "hbox11");
  GLADE_HOOKUP_OBJECT (Properties, label20, "label20");
  GLADE_HOOKUP_OBJECT (Properties, ckbtnPropertiesForce, "ckbtnPropertiesForce");
  GLADE_HOOKUP_OBJECT (Properties, label10, "label10");
  GLADE_HOOKUP_OBJECT (Properties, hbox8, "hbox8");
  GLADE_HOOKUP_OBJECT (Properties, entPropertiesFile, "entPropertiesFile");
  GLADE_HOOKUP_OBJECT (Properties, btnPropertiesOpen, "btnPropertiesOpen");
  GLADE_HOOKUP_OBJECT (Properties, image8, "image8");
  GLADE_HOOKUP_OBJECT (Properties, hseparator8, "hseparator8");
  GLADE_HOOKUP_OBJECT (Properties, hbuttonbox2, "hbuttonbox2");
  GLADE_HOOKUP_OBJECT (Properties, btnPropertiesCancel, "btnPropertiesCancel");
  GLADE_HOOKUP_OBJECT (Properties, btnPropertiesOk, "btnPropertiesOk");
  GLADE_HOOKUP_OBJECT_NO_REF (Properties, tooltips, "tooltips");

  return Properties;
}

GtkWidget*
create_Message (void)
{
  GtkWidget *Message;
  GtkWidget *frame3;
  GtkWidget *vbox7;
  GtkWidget *hbox10;
  GtkWidget *image10;
  GtkWidget *labMessage;
  GtkWidget *hseparator11;
  GtkWidget *hbuttonbox3;
  GtkWidget *btnMessageOk;

  Message = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (Message), _("Usbview2 - Messages"));
  gtk_window_set_position (GTK_WINDOW (Message), GTK_WIN_POS_CENTER_ON_PARENT);
  gtk_window_set_modal (GTK_WINDOW (Message), TRUE);

  frame3 = gtk_frame_new (NULL);
  gtk_widget_show (frame3);
  gtk_container_add (GTK_CONTAINER (Message), frame3);
  gtk_frame_set_shadow_type (GTK_FRAME (frame3), GTK_SHADOW_ETCHED_OUT);

  vbox7 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox7);
  gtk_container_add (GTK_CONTAINER (frame3), vbox7);

  hbox10 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox10);
  gtk_box_pack_start (GTK_BOX (vbox7), hbox10, TRUE, FALSE, 10);

  image10 = gtk_image_new_from_icon_name ("gtk-dialog-error", GTK_ICON_SIZE_DIALOG);
  gtk_widget_show (image10);
  gtk_box_pack_start (GTK_BOX (hbox10), image10, FALSE, FALSE, 12);

  labMessage = gtk_label_new (_("Message"));
  gtk_widget_show (labMessage);
  gtk_box_pack_start (GTK_BOX (hbox10), labMessage, TRUE, TRUE, 8);
  gtk_label_set_use_markup (GTK_LABEL (labMessage), TRUE);
  gtk_label_set_line_wrap (GTK_LABEL (labMessage), TRUE);

  hseparator11 = gtk_hseparator_new ();
  gtk_widget_show (hseparator11);
  gtk_box_pack_start (GTK_BOX (vbox7), hseparator11, FALSE, TRUE, 5);

  hbuttonbox3 = gtk_hbutton_box_new ();
  gtk_widget_show (hbuttonbox3);
  gtk_box_pack_end (GTK_BOX (vbox7), hbuttonbox3, FALSE, TRUE, 6);

  btnMessageOk = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (btnMessageOk);
  gtk_container_add (GTK_CONTAINER (hbuttonbox3), btnMessageOk);
  GTK_WIDGET_SET_FLAGS (btnMessageOk, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) Message, "delete_event",
                    G_CALLBACK (on_Message_delete_event),
                    NULL);
  g_signal_connect ((gpointer) btnMessageOk, "clicked",
                    G_CALLBACK (on_btnMessageOk_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (Message, Message, "Message");
  GLADE_HOOKUP_OBJECT (Message, frame3, "frame3");
  GLADE_HOOKUP_OBJECT (Message, vbox7, "vbox7");
  GLADE_HOOKUP_OBJECT (Message, hbox10, "hbox10");
  GLADE_HOOKUP_OBJECT (Message, image10, "image10");
  GLADE_HOOKUP_OBJECT (Message, labMessage, "labMessage");
  GLADE_HOOKUP_OBJECT (Message, hseparator11, "hseparator11");
  GLADE_HOOKUP_OBJECT (Message, hbuttonbox3, "hbuttonbox3");
  GLADE_HOOKUP_OBJECT (Message, btnMessageOk, "btnMessageOk");

  return Message;
}
