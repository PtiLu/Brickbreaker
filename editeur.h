#ifndef EDITEUR_H
#define EDITEUR_H

typedef struct
{
  int x, y, d, b;
}Tableau;

typedef struct {
  int area1_width, area1_height, mode, indic, select, save;
  gchar *name;
  GtkWidget *win1, *area1, *vbox1, *hbox1, *stabar1, *clear, *create, *solid, *menu, *item, *menubar,*table,
    *bonus1, *bonus2, *bonus3, *bonus4, *bonus5, *bonus6, *bonus7, *bonus8, *clear_bonus, *aligne_ligne, *aligne_colonne;
  GdkPixbuf *pix_fond, *pix_brique, *pix_brique2, *pix_brique3;
  Tableau *brique;
} My_struct;

#endif
