#include <gtk/gtk.h>
#include "sui-gtk.h"
#include <gdk-pixbuf/gdk-pixbuf.h>

void sui_pixbuf_get_size (GdkPixbuf *pix, int *w, int *h)
{
    *w = gdk_pixbuf_get_width (pix);
    *h = gdk_pixbuf_get_height (pix);
}
GdkPixbuf *sui_pixbuf_load (const char *filename)
{
    GError *error = NULL; GdkPixbuf *pix; int w, h;
    printf ("Loading %s ... ", filename); fflush (stdout);
    pix = gdk_pixbuf_new_from_file (filename, &error);
    if (pix == NULL) {
        printf ("error\n");
        fprintf (stderr, "Unable to read file: %s\n", error->message);
        g_error_free (error);
        return NULL;
    }
    sui_pixbuf_get_size (pix, &w, &h); printf ("%dx%d\n", w, h);
    return pix;
}

void sui_pixbuf_draw (GdkWindow *win, GdkGC *gc, GdkPixbuf *pix, int x, int y)
{
    gdk_draw_pixbuf (win, gc, pix, 0, 0, x, y, -1, -1, GDK_RGB_DITHER_NONE, 0, 0);
}

void sui_color_rgb_set (GdkGC *gc, int r, int g, int b)
{
  GdkColor c;
  c.pixel = 0; c.red = r << 8; c.green = g << 8; c.blue = b << 8; 
  gdk_gc_set_rgb_fg_color (gc, &c);
}

void sui_statusbar_write (GtkStatusbar *statusbar, gchar const *message)
{
  guint id = gtk_statusbar_get_context_id (statusbar, "");
  gtk_statusbar_pop  (statusbar, id);
  gtk_statusbar_push (statusbar, id , message);
}
