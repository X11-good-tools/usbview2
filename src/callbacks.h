#include <gtk/gtk.h>


void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_btnMainProperties_clicked           (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_btnMainClose_clicked                (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_btnMainAbout_clicked                (GtkToolButton   *toolbutton,
                                        gpointer         user_data);

void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_Main_delete_event                   (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_Properties_delete_event             (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btnPropertiesOpen_clicked           (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_Chooser_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btnChooserCancel_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnChooserOpen_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_Main_show                           (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_btnPropertiesCancel_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnPropertiesOk_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnAboutOk_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_About_delete_event                  (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_Message_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btnMessageOk_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_treeUSB_cursor_changed              (GtkTreeView     *treeview,
                                        gpointer         user_data);

void
on_notebDevice_switch_page             (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        guint            page_num,
                                        gpointer         user_data);

gboolean
on_treeUSB_unselect_all                (GtkTreeView     *treeview,
                                        gpointer         user_data);
