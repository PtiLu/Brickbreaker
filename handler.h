#ifndef HANDLER_H
#define HANDLER_H

void win1_on_destroy (GtkWidget *widget, gpointer data);
void butt_quit_on_clicked (GtkWidget *widget, gpointer data);
void butt_start_on_clicked (GtkWidget *widget, gpointer data);
gboolean on_timeout1 ( gpointer data);
gboolean on_timeout2 ( gpointer data);
gboolean area1_on_expose (GtkWidget *area, GdkEventExpose *e, gpointer data);
gboolean area1_on_button_press (GtkWidget *area, GdkEventButton *e, gpointer data);
gboolean area1_on_button_release (GtkWidget *area, GdkEventButton *e, gpointer data);
gboolean area1_on_motion_notify (GtkWidget *area, GdkEventMotion *e, gpointer data);
gboolean area1_on_key_press (GtkWidget *area, GdkEventKey *e, gpointer data);
gboolean area1_on_key_release (GtkWidget *area, GdkEventKey *e, gpointer data);
gboolean area1_on_configure (GtkWidget *area, GdkEventConfigure *e, gpointer data);

#endif
