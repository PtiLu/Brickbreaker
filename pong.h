#ifndef PONG_H
#define PONG_H

#define MAX_BRIQUES  50

typedef struct {
  int  r, b, c;
  double x, y, dx, dy;
}Balle;

typedef struct {
  int x,y,rx,ry,b;
}Raquette;

typedef struct{
  int x, y, w, h, f, bonus;
}Brique;

typedef struct{
  int b;
  int x, y, dy;
}Bonus;

typedef struct
{
  char name[500];
  int  score;
}Tab_score;

typedef enum { E_DEPART, E_REBOND, E_PERDU, E_GAGNE, E_FIN}Coin;

typedef struct {
  int anime, area1_width, area1_height, fl_gauche, fl_droite, etat_jeu, nb_brique, 
    vie, lvl, nb_balle, nb_bonus, score, laser;
  char *my_lvl;
  float time;
  gint timeout1, timeout2;
  GtkWidget *win1, *area1, *vbox1, *hbox1, *stabar1, *start;
  Balle balle[600];
  Bonus *bonus;
  Raquette raq;
  Brique *brique;
  Tab_score m_score[10];
  GTimer *timer_xballe, *timer_xraq;
  GdkPixbuf *pix_raq, *pix_raq90, *pix_raq110, *pix_fond, *pix_balle, 
    *pix_brique,*pix_brique2, *pix_brique3, *pix_bonus1, *pix_bonus2, *pix_bonus3,
    *pix_bonus4, *pix_bonus5, *pix_bonus6, *pix_bonus7, *pix_bonus8, *pix_game_over, *pix_fin,
    *pix_raq50, *pix_raq30, *pix_xraq, *pix_laser1, *pix_laser2, *pix_laser3, *pix_raq_stick30,
    *pix_raq_stick50, *pix_raq_stick, *pix_raq_stick70, *pix_raq_stick90, *pix_raq_stick110, *pix_xballe;
} Info;

void area1_redessiner (GdkWindow *win, Info *info);
void balle_etape_suivante(Info *info);
void raquette_etape_suivante(Info *info);
void reafficher (GtkWidget *area);
void raquette_adapter(Raquette *q, int largeur, int hauteur);
void balle_preparer_service (Info *info);
void etat_jeu_changer (Info *info, int etat);
void etat_jeu_afficher(Info *info);
void balle_init(Info *info);
int brique_init(Info *info);
void briques_calculer_collisions (Info *info);
void pix_charger_tous(Info *info);
void test_bonus(Info *info, int i);
void raquette_init (Info *info);
void bonus_dessiner(GdkWindow *win, GdkGC *gc, Info *info);
void lacher_bonus (Info *info);
void bonus_init(Info *info);
void stop_bonus(Info *info);
int tester_score (int score, Tab_score meilleur_score[10]);
void enregistrer_score(Tab_score score[10], int score_j, int k);
void charger_score(Tab_score score[10]);
void info_init (Info *info, char *lvl);
#endif
