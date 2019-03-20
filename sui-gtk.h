	#ifndef SUI_GTK_H 
#define SUI_GTK_H 

#define IS_HOMOGEN     TRUE
#define IS_NOT_HOMOGEN FALSE

#define DO_FILL      TRUE
#define DO_NOT_FILL  FALSE

#define DO_EXPAND     TRUE
#define DO_NOT_EXPAND FALSE
#define UNUSED(PARAM) PARAM##_UNUSED   __attribute__ ((unused))

#define SUI_ON_SIGNAL(w,n,c,d)					\
                  (g_signal_connect(G_OBJECT(w), (n), G_CALLBACK(c), (d) ))
GdkGC *sui_gc_new(void);

void sui_pixbuf_get_size (GdkPixbuf *pix, int *w, int *h);
GdkPixbuf *sui_pixbuf_load (const char *filename);
void sui_pixbuf_draw (GdkWindow *win, GdkGC *gc, GdkPixbuf *pix, int x, int y);
void sui_color_rgb_set (GdkGC *gc, int r, int g, int b);
void sui_statusbar_write (GtkStatusbar *statusbar, gchar const *message);

#endif
