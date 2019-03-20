#ifndef MY_GAME_H
#define MY_GAME_H

typedef struct {
  int area1_width, area1_height, mode, select;
  gchar *name;
  GtkWidget *win1, *area1, *vbox1, *hbox1, *stabar1;
  GdkPixbuf *pix_fond, *pix_jouer, *pix_editeur, *pix_records, *pix_niveau , *pix_quitter;
} My_struct;

#endif
