#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <math.h>  
#include "pong.h"
#include "handler.h"
#include "sui-gtk.h"

#define WIN1_TITRE  "PONG"
#define WIN1_LARG   500
#define WIN1_HAUT   450


void pix_charger_tous(Info *info)
{
 info->pix_raq            = sui_pixbuf_load("./images/raq.png");
 info->pix_balle          = sui_pixbuf_load("./images/balle.png");
 info->pix_xballe         = sui_pixbuf_load("./images/xballe.png");
 info->pix_fond           = sui_pixbuf_load("./images/fond.png");
 info->pix_brique3        = sui_pixbuf_load("./images/brique_tfend.png");
 info->pix_brique2        = sui_pixbuf_load("./images/brique_fend.png");
 info->pix_brique         = sui_pixbuf_load("./images/brique.png");
 info->pix_raq30          = sui_pixbuf_load("./images/raq30.png");
 info->pix_raq50          = sui_pixbuf_load("./images/raq50.png");
 info->pix_raq90          = sui_pixbuf_load("./images/raq90.png");
 info->pix_raq110         = sui_pixbuf_load("./images/raq110.png");
 info->pix_raq_stick30    = sui_pixbuf_load("./images/raq_stick30.png");
 info->pix_raq_stick50    = sui_pixbuf_load("./images/raq_stick50.png");
 info->pix_raq_stick      = sui_pixbuf_load("./images/raq_stick.png");
 info->pix_raq_stick90    = sui_pixbuf_load("./images/raq_stick90.png");
 info->pix_raq_stick110   = sui_pixbuf_load("./images/raq_stick110.png");
 info->pix_laser1         = sui_pixbuf_load("./images/laser1.png");
 info->pix_laser2         = sui_pixbuf_load("./images/laser2.png");
 info->pix_laser3         = sui_pixbuf_load("./images/laser3.png");
 info->pix_xraq           = sui_pixbuf_load("./images/xraq.png");
 info->pix_bonus1         = sui_pixbuf_load("./images/bonus1.png");
 info->pix_bonus2         = sui_pixbuf_load("./images/bonus2.png");
 info->pix_bonus3         = sui_pixbuf_load("./images/bonus3.png");
 info->pix_bonus4         = sui_pixbuf_load("./images/bonus4.png");
 info->pix_bonus5         = sui_pixbuf_load("./images/bonus5.png");
 info->pix_bonus6         = sui_pixbuf_load("./images/bonus6.png");
 info->pix_bonus7         = sui_pixbuf_load("./images/bonus7.png");
 info->pix_bonus8         = sui_pixbuf_load("./images/bonus8.png");
 info->pix_game_over      = sui_pixbuf_load("./images/game_over.png");
 info->pix_fin            = sui_pixbuf_load("./images/win.png");
}

void bonus_init(Info *info)
{
  int i; 
  info->nb_bonus = 0;
  info->bonus = malloc(info->nb_brique*sizeof(Bonus));
  for(i=0;i<info->nb_brique;i++)
  {
    info->bonus[i].b  = 0;
    info->bonus[i].x  = 0;
    info->bonus[i].y  = 0;
    info->bonus[i].dy = 1;
  } 
}

void balle_init(Info *info)
{
  info->nb_balle = 1;
  info->balle[1].x     = 0;	
  info->balle[1].y     = 0;
  info->balle[1].dx    = 2;
  info->balle[1].dy    = -2;
  info->balle[1].r     = 5;
  info->balle[1].b     = 0;
  info->balle[1].c     = 0;
}

int brique_init(Info *info)
{
  int i, lvl = 0;
  FILE *f;

  if(info->my_lvl != NULL)
    f = fopen(info->my_lvl,"r");
  else
    f = fopen("lvl.txt","r");

  while(lvl != info->lvl)
    {
      if(fscanf(f,"%d",&info->nb_brique) < 0)
	{
	  info->nb_brique = 0;
	  return 0;
	}
      info->brique = malloc(info->nb_brique*sizeof(Brique));
      assert(info->brique != NULL);
      
      for(i=0;i < info->nb_brique;i++)
	{
	  fscanf(f,"%d",&info->brique[i].x);
	  fscanf(f,"%d",&info->brique[i].y);
	  fscanf(f,"%d",&info->brique[i].f);
	  fscanf(f,"%d",&info->brique[i].bonus);
	  info->brique[i].w = 50;
	  info->brique[i].h = 15;
	}
      lvl ++;
    } 
  fclose(f);
  return 1;
}

void raquette_init (Info *info)
{
  info->raq.x  = 100;
  info->raq.y  = 100;
  info->raq.rx = 35;
  info->raq.ry = 4;
  info->raq.b  = 0;
}

void info_init (Info *info, char *lvl)
{
  info->anime        = 0;
  info->timeout1     = 0;
  info->area1_width  = 0;
  info->area1_height = 0;
  info->fl_gauche    = 0;
  info->fl_droite    = 0;
  info->vie          = 3;
  info->lvl          = 1;
  info->time         = 0;
  info->score        = 0;
  info->laser        = 0;
  info->my_lvl       = lvl;
  info->timer_xballe = NULL;
  info->timer_xraq   = NULL;
  info->etat_jeu     = E_DEPART;
  pix_charger_tous(info);
  balle_init(info);
  raquette_init(info);
  brique_init(info);
  bonus_init(info);
  charger_score(info->m_score);
  printf ("info_init ...\n");
}

void test_bonus(Info *info, int i)
{
  int j;
  Bonus *b = &info->bonus[i]; 

    if(b->b == 1)
    {
      if(info->raq.rx >= 55)
      {
	b->b = 0;
	return;
      }
      info->raq.rx = info->raq.rx + 10;
      b->b = 0;
    }

    if(b->b == 2)
    {
      info->vie = info->vie+1;
      etat_jeu_afficher(info);
      b->b = 0;
    }

    if(b->b == 3)
    {
      
      info->nb_balle = info->nb_balle + 3; 
      info->balle[info->nb_balle-2].x  =  info->balle[info->nb_balle-3].x;	
      info->balle[info->nb_balle-2].y  =  info->balle[info->nb_balle-3].y;
      info->balle[info->nb_balle-2].dx = -info->balle[info->nb_balle-3].dx;
      info->balle[info->nb_balle-2].dy =  info->balle[info->nb_balle-3].dy;
      info->balle[info->nb_balle-2].r  =  info->balle[info->nb_balle-3].r;
      info->balle[info->nb_balle-2].b  =  info->balle[info->nb_balle-3].b;
      info->balle[info->nb_balle-2].c  =  info->balle[info->nb_balle-3].c;
      
      info->balle[info->nb_balle-1].x  =  info->balle[info->nb_balle-3].x;	
      info->balle[info->nb_balle-1].y  =  info->balle[info->nb_balle-3].y;
      info->balle[info->nb_balle-1].dx =  info->balle[info->nb_balle-3].dx;
      info->balle[info->nb_balle-1].dy = -info->balle[info->nb_balle-3].dy;
      info->balle[info->nb_balle-1].r  =  info->balle[info->nb_balle-3].r;
      info->balle[info->nb_balle-1].b  =  info->balle[info->nb_balle-3].b;
      info->balle[info->nb_balle-1].c  =  info->balle[info->nb_balle-3].c;
      
      info->balle[info->nb_balle].x    =  info->balle[info->nb_balle-3].x;	
      info->balle[info->nb_balle].y    =  info->balle[info->nb_balle-3].y;
      info->balle[info->nb_balle].dx   = -info->balle[info->nb_balle-3].dx;
      info->balle[info->nb_balle].dy   = -info->balle[info->nb_balle-3].dy;
      info->balle[info->nb_balle].r    =  info->balle[info->nb_balle-3].r;
      info->balle[info->nb_balle].b    =  info->balle[info->nb_balle-3].b;
      info->balle[info->nb_balle].c  =  info->balle[info->nb_balle-3].c;
      b->b = 0;
    }

    if(b->b == 4)
    {
      if(info->vie > 1)
	info->vie = info->vie-1;
      etat_jeu_afficher(info);
      b->b = 0;
    }
    
    if(b->b == 5)
    {
      if(info->raq.rx <= 15)
      {
        b->b = 0;
        return;
      }
      info->raq.rx = info->raq.rx - 10;
      b->b = 0;
    }

    if(b->b == 6)
    {
      if(info->timer_xballe == NULL)
      {
        
        info->timer_xballe = g_timer_new();

        for(j=1;j<=info->nb_balle;j++)
          info->balle[j].b = 1;
      }
      else
        g_timer_reset (info->timer_xballe);

      b->b=0;
    }
    
    if(b->b == 7)
      {      
	if(info->timer_xraq == NULL)
	  {    
	    info->timer_xraq = g_timer_new();
	    info->raq.b = 1;
	  }
	else
	  {
	    g_timer_reset (info->timer_xraq);
	    info->raq.b = 1;
	  }
	b->b=0;
      }
    
    if(b->b == 8)
      {      
	if(info->timer_xraq == NULL)
	  {    
	    info->raq.rx     = 55;
	    info->raq.b      = 2;
	  }
	else
	  {
	    g_timer_destroy(info->timer_xraq);
	    info->timer_xraq = NULL;
	    for(j=1;j<=info->nb_balle;j++)
	      info->balle[j].c = 0;
	    info->raq.rx= 55;
	    info->raq.b = 2;
	  }
	b->b=0;
      }	
    
}

void stop_bonus(Info *info)
{
  int i;
  if(info->timer_xballe != NULL)
  { 
    info->time  = g_timer_elapsed(info->timer_xballe, 0);
    if(info->time >= 10)
    {
      g_timer_destroy(info->timer_xballe);
      info->timer_xballe = NULL;
      for(i=1;i<=info->nb_balle;i++)
        info->balle[i].b = 0;
    }
  }

  if(info->timer_xraq != NULL)
    { 
      info->time  = g_timer_elapsed(info->timer_xraq, 0);
      if(info->raq.b == 1 && info->time >= 10)
	{
	  g_timer_destroy(info->timer_xraq);
	  info->timer_xraq = NULL;
	  info->raq.b = 0;
	  for(i=1;i<=info->nb_balle;i++)
	    info->balle[i].c = 0;
	}
      else if(info->raq.b == 2 && info->time >= 1)
	{
	  g_timer_destroy(info->timer_xraq);
	  info->timer_xraq = NULL;
	  info->laser = 0;
	  info->raq.b = 0;
	}
    }
}

void info_detruire (Info *UNUSED(info))
{
  printf ("info_detruire ...\n");
}

void briques_dessiner (GdkWindow *win, GdkGC *gc, Info *info)
{
  int i=0;
   for(i=0;i<info->nb_brique;i++)
     {
       if(info->brique[i].f == 1)
	 sui_pixbuf_draw(win, gc, info->pix_brique3, info->brique[i].x, info->brique[i].y);
       
       else if(info->brique[i].f == 2)
	 sui_pixbuf_draw(win, gc, info->pix_brique2, info->brique[i].x, info->brique[i].y);
	
       else if(info->brique[i].f == 3)
	 sui_pixbuf_draw(win, gc, info->pix_brique, info->brique[i].x, info->brique[i].y);
	 
     }
}

void laser_dessiner(GdkWindow *win, GdkGC *gc, Info *info)
{
  if(info->laser>0)
    {
      if(info->laser == 1)
	{
	  sui_pixbuf_draw(win, gc, info->pix_laser1, info->raq.x-25, 0);
	  info->laser++;
	}
      else if(info->laser == 2)
	{
	  sui_pixbuf_draw(win, gc, info->pix_laser2, info->raq.x-25, 0);
	  info->laser++;
	}
      else if(info->laser == 3)
	{
	  sui_pixbuf_draw(win, gc, info->pix_laser3, info->raq.x-25, 0);
	  info->laser=1;
	}
    }

} 

void balle_dessiner (GdkWindow *win, GdkGC *gc, Info *info)
{
  int i;
  for(i=1;i<=info->nb_balle;i++)
  {
    if(info->balle[i].b==0)
      sui_pixbuf_draw(win, gc, info->pix_balle, info->balle[i].x, info->balle[i].y);
    
    else if(info->balle[i].b==1)
      sui_pixbuf_draw(win, gc, info->pix_xballe, info->balle[i].x, info->balle[i].y);
  }

}  

void raquette_dessiner (GdkWindow *win, GdkGC *gc, Info *info)
{
  sui_color_rgb_set(gc,0,0,255);

  if (info->raq.b == 1 && info->raq.rx == 15)
    sui_pixbuf_draw(win, gc, info->pix_raq_stick30, info->raq.x-info->raq.rx, info->raq.y-info->raq.ry);
  
  else if(info->raq.b == 1 && info->raq.rx == 25)
    sui_pixbuf_draw(win, gc, info->pix_raq_stick50, info->raq.x-info->raq.rx, info->raq.y-info->raq.ry);
   
  else if(info->raq.b == 1 && info->raq.rx == 35)
    sui_pixbuf_draw(win, gc, info->pix_raq_stick, info->raq.x-info->raq.rx, info->raq.y-info->raq.ry);
    
  else if(info->raq.b == 1 && info->raq.rx == 45)
    sui_pixbuf_draw(win, gc, info->pix_raq_stick90, info->raq.x-info->raq.rx, info->raq.y-info->raq.ry);
   
  else if(info->raq.b == 1 && info->raq.rx == 55)
    sui_pixbuf_draw(win, gc, info->pix_raq_stick110, info->raq.x-info->raq.rx, info->raq.y-info->raq.ry);
  
  else if(info->raq.b == 2)
    sui_pixbuf_draw(win, gc, info->pix_xraq, info->raq.x-info->raq.rx, info->raq.y-info->raq.ry); 

  else if(info->raq.rx == 35)
    sui_pixbuf_draw(win, gc, info->pix_raq, info->raq.x-info->raq.rx, info->raq.y-info->raq.ry);

  else if(info->raq.rx == 45)
    sui_pixbuf_draw(win, gc, info->pix_raq90, info->raq.x-info->raq.rx, info->raq.y-info->raq.ry); 

  else if(info->raq.rx == 55)
    sui_pixbuf_draw(win, gc, info->pix_raq110, info->raq.x-info->raq.rx, info->raq.y-info->raq.ry); 
 
  else if(info->raq.rx == 25)
    sui_pixbuf_draw(win, gc, info->pix_raq50, info->raq.x-info->raq.rx, info->raq.y-info->raq.ry); 
  
  else if(info->raq.rx == 15)
    sui_pixbuf_draw(win, gc, info->pix_raq30, info->raq.x-info->raq.rx, info->raq.y-info->raq.ry); 

}

void bonus_dessiner(GdkWindow *win, GdkGC *gc, Info *info)
{
  int i;
  sui_color_rgb_set(gc,0,0,255);
  for(i=1;i<=info->nb_bonus;i++)
    {
      if(info->bonus[i].b == 1)
	sui_pixbuf_draw(win, gc, info->pix_bonus1, info->bonus[i].x, info->bonus[i].y);
      else if(info->bonus[i].b == 2)
	sui_pixbuf_draw(win, gc, info->pix_bonus2, info->bonus[i].x, info->bonus[i].y);
      else if(info->bonus[i].b == 3)
	sui_pixbuf_draw(win, gc, info->pix_bonus3, info->bonus[i].x, info->bonus[i].y);
      else if(info->bonus[i].b == 4)
	sui_pixbuf_draw(win, gc, info->pix_bonus4, info->bonus[i].x, info->bonus[i].y);
      else if(info->bonus[i].b == 5)
	sui_pixbuf_draw(win, gc, info->pix_bonus5, info->bonus[i].x, info->bonus[i].y);
      else if(info->bonus[i].b == 6)
	sui_pixbuf_draw(win, gc, info->pix_bonus6, info->bonus[i].x, info->bonus[i].y);
      else if(info->bonus[i].b == 7)
	sui_pixbuf_draw(win, gc, info->pix_bonus7, info->bonus[i].x, info->bonus[i].y);
      else if(info->bonus[i].b == 8)
	sui_pixbuf_draw(win, gc, info->pix_bonus8, info->bonus[i].x, info->bonus[i].y);
    }
}

void raquette_adapter(Raquette *q, int largeur, int hauteur)
{
  q->x = largeur/2;
  q->y = hauteur - 5;
}

int tester_score (int score, Tab_score meilleur_score[10])
{
  int i;
  
  for(i=0;i<10;i++)
    if(score > meilleur_score[i].score)
      return i;
  return -1;
}

void decaler_score (int k, Tab_score score[10])
{
  int i;
  for(i=9;i>k;i--)
      score[i] = score[i-1];
}

void enregistrer_score(Tab_score score[10], int score_j, int k)
{
  int i;
  FILE *f;
  GtkWidget* pBoite;
  GtkWidget* pEntry;
  const gchar* new_name;

  pBoite = gtk_dialog_new_with_buttons("RECORD",
				       NULL,
				       GTK_DIALOG_MODAL,
				       GTK_STOCK_OK,GTK_RESPONSE_OK,
				       GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
				       NULL);
  pEntry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(pEntry), "Saisissez votre nom");
  
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(pBoite)->vbox), pEntry, TRUE, FALSE, 0);
  gtk_widget_show_all(GTK_DIALOG(pBoite)->vbox);
  
  
  switch (gtk_dialog_run(GTK_DIALOG(pBoite)))
    {
    case GTK_RESPONSE_OK:
      new_name = gtk_entry_get_text(GTK_ENTRY(pEntry));
      decaler_score (k,score);
      sprintf(score[k].name,"%s",(char*)new_name);
      break;
    case GTK_RESPONSE_CANCEL:  gtk_widget_destroy(pBoite);return;
    default:                   gtk_widget_destroy(pBoite);return;
    }
  gtk_widget_destroy(pBoite);

  score[k].score = score_j;
  f = fopen("meilleur_score.txt","w");
  for(i=0;i<10;i++)
    {
      if(i == k)
	{
	  fprintf(f,"%s : %d\n", score[k].name, score[i].score);
	  continue;
	}
      fprintf(f,"%s : %d\n",score[i].name, score[i].score);
    }
  fclose(f);
}

void charger_score(Tab_score score[10])
{
  int i;
  FILE *f;
  
  f = fopen("meilleur_score.txt","r");
  for(i=0;i<10;i++)
    fscanf(f,"%s : %d\n",score[i].name,&score[i].score);
  fclose(f);
}

void balle_preparer_service (Info *info)
{
  info->balle[1].x = info->raq.x - info->balle[1].r;
  info->balle[1].y = info->raq.y - info->raq.ry -2*info->balle[1].r;
}

int decoupe_raquette (double x, Info *info)
{
  double longueur;
  longueur = 2*info->raq.rx;
  x = x - (info->raq.x-info->raq.rx);
  if(x < (longueur/8))
    return 2;
  /*else if(x<(longueur/6)&&(x>longueur/8))
    return 2;*/
  else if(longueur/8 <= x && x < (2*longueur/6))
    return 3;
  else if((2*longueur/6) <= x && x < (3*longueur/6))
    return 4;
  else if((3*longueur/6) < x && x <= (4*longueur/6))
    return 6;
  else if((4*longueur/6) < x )
    return 7;
  /*else if(x>(5*longueur/6) && x<(5*longueur/8))
    return 8;*/
  else 
    return 2;
}

int balle_touche_raquette(Balle balle, Info *info)
{
   if((balle.y + balle.r) >= (info->raq.y - balle.r) && 
      (balle.y + balle.r) <= (info->raq.y )&&
      (balle.x + balle.r) >= (info->raq.x - info->raq.rx) && 
      (balle.x + balle.r) <= (info->raq.x + info->raq.rx))
     return decoupe_raquette(balle.x, info);  
   else if((balle.y + balle.r) >= (info->raq.y - balle.r) && 
	   (balle.y + balle.r) <= (info->raq.y )&&
	   (balle.x) >(info->raq.x + info->raq.rx - balle.r) && 
	   (balle.x) <= (info->raq.x + info->raq.rx))
     return 9; 
   else if((balle.y + balle.r)   >= (info->raq.y - balle.r) && 
	   (balle.y + balle.r)   <= (info->raq.y )&&
	   (balle.x + 2*balle.r) < (info->raq.x - info->raq.rx + balle.r) && 
	   (balle.x + 2*balle.r) >= (info->raq.x -info->raq.rx))
     return 1; 
   
 return -1;
}

int bonus_touche_raquette(Bonus b, Info *info)
{
  if(((b.y+20>=(info->raq.y) && b.y+20<=(info->raq.y)) ||
      (b.y>=(info->raq.y) && b.y<=(info->raq.y))) &&
      (b.x>=(info->raq.x-info->raq.rx-45)&&b.x<=(info->raq.x+info->raq.rx)))
      return 1;  
 return 0;
}

void reorganise_bonus(Info *info, int num)
{
  int i;
  info->nb_bonus = info->nb_bonus-1;
  for(i=num;i<=info->nb_bonus;i++)
      info->bonus[i] = info->bonus[i+1];
}

void lacher_bonus (Info *info)
{
  int i;
  for(i=1;i<=info->nb_bonus;i++)
  {
    info->bonus[i].y = info->bonus[i].y + info->bonus[i].dy;   
    if(bonus_touche_raquette(info->bonus[i], info))
      {
	if(info->bonus[i].b!=4&&info->bonus[i].b!=5)
	  info->score = info->score +5;
	test_bonus(info, i);
	reorganise_bonus(info,i);
      }
    else if(info->bonus[i].y > info->raq.y + info->raq.ry)
      {
	reorganise_bonus(info,i);
      }
  }
}

int toucher_laser(int x, Brique brique)
{
  if((brique.x>= x-25 && brique.x<= x+25)||((brique.x+brique.w)>=x-25&&(brique.x+brique.w)<=x+25))
    return 1;
  return 0;
}

void briques_calculer_collisions (Info *info)
{
  int i,brique_casse = 0,j;

  for(i=0;i<info->nb_brique;i++)
    {
      if(info->laser >0 && info->brique[i].f > 0)
	{
	  if(toucher_laser(info->raq.x, info->brique[i]))
	    {
	      info->brique[i].f--;
	      info->score = info->score + 20;
	      continue;
	    }
	}
      for(j=1;j<=info->nb_balle;j++) 
	{
	  /*~~~~~~~~~~~BALLE DESCEND VERS DROITE~~~~~~~~~~~~~~~~~~~~~~~*/
	  
	  if(info->balle[j].dx > 0 && info->balle[j].dy > 0)
	    {
	      /*collision angle haut gauche*/
	      if ((info->balle[j].x + info->balle[j].r)  >= (info->brique[i].x - info->balle[j].r) && 
		  (info->balle[j].x + info->balle[j].r)  <=  info->brique[i].x &&
		  (info->balle[j].y + info->balle[j].r)  >= (info->brique[i].y - info->balle[j].r) &&  
		  (info->balle[j].y + info->balle[j].r)  <=  info->brique[i].y)
		{
                 
		  if(info->balle[j].b == 1 && info->brique[i].f >= 1)
		    {
		      info->brique[i].f = 0;
		      info->score       = info->score + 60;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }

		  if(info->brique[i].f >= 1)
		    {
		      info->brique[i].f--;
		      if(info->brique[i].f == 0) info->score = info->score + 20;
		      info->balle[j].y     =  info->brique[i].y-info->balle[j].r;
		      info->balle[j].dy    = -info->balle[j].dy;
		      info->balle[j].y     =  info->balle[j].y + info->balle[j].dy;
		      info->balle[j].dx    = -info->balle[j].dx;
		      info->balle[j].x     =  info->balle[j].x + info->balle[j].dx;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		}
	      
	      /*collision bas balle[j] =)*/
	      else if((info->balle[j].y + info->balle[j].r)>=(info->brique[i].y - info->balle[j].r)&&
		      (info->balle[j].y + info->balle[j].r)<=(info->brique[i].y )&&
		      (info->balle[j].x + info->balle[j].r)>(info->brique[i].x)&&
		      (info->balle[j].x + info->balle[j].r)<(info->brique[i].x + info->brique[i].w ))
		{
		  if(info->balle[j].b == 1 && info->brique[i].f >= 1)
		    {
		      info->brique[i].f = 0;
		      info->score       = info->score + 60;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		  if(info->brique[i].f >= 1)
		    {
		      info->brique[i].f--;
		      if(info->brique[i].f == 0) info->score = info->score + 20;
		      info->balle[j].y     =  info->brique[i].y-2*info->balle[j].r;
		      info->balle[j].dy    = -info->balle[j].dy;
		      info->balle[j].y     =  info->balle[j].y + info->balle[j].dy;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		}
	      
	      /*collision droite balle[j]*/
	      else if((info->balle[j].x + info->balle[j].r)>=(info->brique[i].x - info->balle[j].r)&&
		      (info->balle[j].x + info->balle[j].r)<=(info->brique[i].x)&&
		      (info->balle[j].y + info->balle[j].r)> (info->brique[i].y)&&
		      (info->balle[j].y + info->balle[j].r)< (info->brique[i].y + info->brique[i].h))
		{  
		  if(info->balle[j].b == 1 && info->brique[i].f >= 1)
		    {
		      info->brique[i].f = 0;
		      info->score       = info->score + 60;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		  if(info->brique[i].f >= 1)
		    {
		      info->brique[i].f--;
		      if(info->brique[i].f == 0) info->score = info->score + 20;
		      info->balle[j].x     = info->brique[i].x-2*info->balle[j].r;
		      info->balle[j].dx    = -info->balle[j].dx;
		      info->balle[j].x     = info->balle[j].x + info->balle[j].dx;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		} 
	    }
	  
	  /*~~~~~~~~~~~~~~~~~~~~~~~BALLE MONTE VERS DROITE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	  
	  else if(info->balle[j].dx > 0 && info->balle[j].dy < 0)
	    {   
	      /*collision angle bas gauche*/
	      if ((info->balle[j].x + info->balle[j].r)  >= (info->brique[i].x - info->balle[j].r) && 
		  (info->balle[j].x + info->balle[j].r)  <= (info->brique[i].x) &&
		  (info->balle[j].y + info->balle[j].r)  >= (info->brique[i].y + info->brique[i].h) &&  
		  (info->balle[j].y + info->balle[j].r)  <= (info->brique[i].y + info->brique[i].h + info->balle[j].r))
		{
		  if(info->balle[j].b == 1 && info->brique[i].f >= 1)
		    {
		      info->brique[i].f = 0;
		      info->score       = info->score + 60;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		  if(info->brique[i].f >= 1)
		    {
		      info->brique[i].f--;
		      if(info->brique[i].f == 0) info->score = info->score + 20;
		      info->balle[j].y     =  info->brique[i].y + info->brique[i].h + info->balle[j].r;
		      info->balle[j].dy    = -info->balle[j].dy;
		      info->balle[j].y     =  info->balle[j].y + info->balle[j].dy;
		      info->balle[j].dx    = -info->balle[j].dx;
		      info->balle[j].x     =  info->balle[j].x + info->balle[j].dx;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		}
	      
	      /*collision droite balle[j]*/
	      else if((info->balle[j].x + info->balle[j].r)>=(info->brique[i].x - info->balle[j].r)&&
		      (info->balle[j].x + info->balle[j].r)<=(info->brique[i].x)&&
		      (info->balle[j].y + info->balle[j].r)> (info->brique[i].y)&&
		      (info->balle[j].y + info->balle[j].r)< (info->brique[i].y + info->brique[i].h))
		{  
		  if(info->balle[j].b == 1 && info->brique[i].f >= 1)
		    {
		      info->brique[i].f = 0;
		      info->score       = info->score + 60;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		  if(info->brique[i].f >= 1)
		    {
		      info->brique[i].f--;
		      if(info->brique[i].f == 0) info->score = info->score + 20;
		      info->balle[j].x     = info->brique[i].x-2*info->balle[j].r;
		      info->balle[j].dx    = -info->balle[j].dx;
		      info->balle[j].x     = info->balle[j].x + info->balle[j].dx;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		}
	      
	      /*collision haut balle[j]*/
	      else if((info->balle[j].y + info->balle[j].r)>=(info->brique[i].y + info->brique[i].h)&&
		      (info->balle[j].y + info->balle[j].r)<=(info->brique[i].y + info->brique[i].h + info->balle[j].r)&&
		      (info->balle[j].x + info->balle[j].r)> (info->brique[i].x)&&
		      (info->balle[j].x + info->balle[j].r)< (info->brique[i].x + info->brique[i].w))
		{
		  if(info->balle[j].b == 1 && info->brique[i].f >= 1)
		    {
		      info->brique[i].f = 0;
		      info->score       = info->score + 60;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		  if(info->brique[i].f >= 1)
		    {
		      info->brique[i].f--;
		      if(info->brique[i].f == 0) info->score = info->score + 20;
		      info->balle[j].y     = info->brique[i].y + info->brique[i].h;
		      info->balle[j].dy    = -info->balle[j].dy;
		      info->balle[j].y     = info->balle[j].y + info->balle[j].dy;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		}
	    }
	  
	  /*~~~~~~~~~~~~~~~~~~BALLE MONTE VERS GAUCHE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	  
	  else if(info->balle[j].dx < 0 && info->balle[j].dy < 0)
	    {
	      
	      /*collision angle bas droite*/
	      if ((info->balle[j].x + info->balle[j].r)  >= (info->brique[i].x + info->brique[i].w) && 
		  (info->balle[j].x + info->balle[j].r)  <= (info->brique[i].x + info->brique[i].w + info->balle[j].r)  &&
		  (info->balle[j].y + info->balle[j].r)  >= (info->brique[i].y + info->brique[i].h) &&  
		  (info->balle[j].y + info->balle[j].r)  <= (info->brique[i].y + info->brique[i].h + info->balle[j].r))
		{
		  if(info->balle[j].b == 1 && info->brique[i].f >= 1)
		    {
		      info->brique[i].f = 0;
		      info->score       = info->score + 60;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		  if(info->brique[i].f >= 1)
		    {
		      info->brique[i].f--;
		      if(info->brique[i].f == 0) info->score = info->score + 20;
		      info->balle[j].y     =  info->brique[i].y + info->brique[i].h + info->balle[j].r;
		      info->balle[j].dy    = -info->balle[j].dy;
		      info->balle[j].y     =  info->balle[j].y + info->balle[j].dy;
		      info->balle[j].dx    = -info->balle[j].dx;
		      info->balle[j].x     =  info->balle[j].x + info->balle[j].dx;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		}
	      
	      /*collision gauche balle*/
	      else if((info->balle[j].x + info->balle[j].r)>=(info->brique[i].x + info->brique[i].w)&&
		      (info->balle[j].x + info->balle[j].r)<=(info->brique[i].x + info->brique[i].w + info->balle[j].r)&&
		      (info->balle[j].y + info->balle[j].r)> (info->brique[i].y)&&
		      (info->balle[j].y + info->balle[j].r)< (info->brique[i].y + info->brique[i].h))
		{  
		  if(info->balle[j].b == 1 && info->brique[i].f >= 1)
		    {
		      info->brique[i].f = 0;
		      info->score       = info->score + 60;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		  if(info->brique[i].f >= 1)
		    {
		      info->brique[i].f--;
		      if(info->brique[i].f == 0) info->score = info->score + 20;
		      info->balle[j].x     = info->brique[i].x+info->brique[i].w;
		      info->balle[j].dx    = -info->balle[j].dx;
		      info->balle[j].x     = info->balle[j].x + info->balle[j].dx;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		}
	      
	      /*collision haut balle[j]*/
	      else if((info->balle[j].y + info->balle[j].r)>=(info->brique[i].y + info->brique[i].h)&&
		      (info->balle[j].y + info->balle[j].r)<=(info->brique[i].y + info->brique[i].h + info->balle[j].r)&&
		      (info->balle[j].x + info->balle[j].r)> (info->brique[i].x)&&
		      (info->balle[j].x + info->balle[j].r)< (info->brique[i].x + info->brique[i].w))
		{
		  if(info->balle[j].b == 1 && info->brique[i].f >= 1)
		    {
		      info->brique[i].f = 0;
		      info->score       = info->score + 60;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		  if(info->brique[i].f >= 1)
		    {
		      info->brique[i].f--;
		      if(info->brique[i].f == 0) info->score = info->score + 20;
		      info->balle[j].y     = info->brique[i].y + info->brique[i].h;
		      info->balle[j].dy    = -info->balle[j].dy;
		      info->balle[j].y     = info->balle[j].y + info->balle[j].dy;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		}
	    }
	  
	  /*~~~~~~~~~~~~~~~~~~~~~~~BALLE DESCEND VERS GAUCHE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	  
	  else if(info->balle[j].dx < 0 && info->balle[j].dy > 0)
	    {
	      
	      /*collision angle haut droit*/
	      if ((info->balle[j].x + info->balle[j].r)  >= (info->brique[i].x + info->brique[i].w) && 
		  (info->balle[j].x + info->balle[j].r)  <= (info->brique[i].x + info->brique[i].w + info->balle[j].r) &&
		  (info->balle[j].y + info->balle[j].r)  >= (info->brique[i].y - info->balle[j].r) &&  
		  (info->balle[j].y + info->balle[j].r)  <= (info->brique[i].y))
		{
		  if(info->balle[j].b == 1 && info->brique[i].f >= 1)
		    {
		      info->brique[i].f = 0;
		      info->score       = info->score + 60;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		  if(info->brique[i].f >= 1)
		    {
		      info->brique[i].f--;
		      if(info->brique[i].f == 0) info->score = info->score + 20;
		      info->balle[j].y     =  info->brique[i].y-info->balle[j].r;
		      info->balle[j].dy    = -info->balle[j].dy;
		      info->balle[j].y     =  info->balle[j].y + info->balle[j].dy;
		      info->balle[j].dx    = -info->balle[j].dx;
		      info->balle[j].x     =  info->balle[j].x + info->balle[j].dx;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		}
	      
	      /*collision bas balle =)*/
	      else if((info->balle[j].y + info->balle[j].r)>=(info->brique[i].y - info->balle[j].r)&&
		      (info->balle[j].y + info->balle[j].r)<=(info->brique[i].y )&&
		      (info->balle[j].x + info->balle[j].r)>(info->brique[i].x)&&
		      (info->balle[j].x + info->balle[j].r)<(info->brique[i].x + info->brique[i].w ))
		{
		  if(info->balle[j].b == 1 && info->brique[i].f >= 1)
		    {
		      info->brique[i].f = 0;
		      info->score       = info->score + 60;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		  if(info->brique[i].f >= 1)
		    {
		      info->brique[i].f--;
		      if(info->brique[i].f == 0) info->score = info->score + 20;
		      info->balle[j].y     =  info->brique[i].y-2*info->balle[j].r;
		      info->balle[j].dy    = -info->balle[j].dy;
		      info->balle[j].y     =  info->balle[j].y + info->balle[j].dy;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		}
	      /*collision gauche balle[j]*/
	      else if((info->balle[j].x + info->balle[j].r)>=(info->brique[i].x + info->brique[i].w)&&
		      (info->balle[j].x + info->balle[j].r)<=(info->brique[i].x + info->brique[i].w + info->balle[j].r)&&
		      (info->balle[j].y + info->balle[j].r)> (info->brique[i].y)&&
		      (info->balle[j].y + info->balle[j].r)< (info->brique[i].y + info->brique[i].h))
		{  
		  if(info->balle[j].b == 1 && info->brique[i].f >= 1)
		    {
		      info->brique[i].f = 0;
		      info->score       = info->score + 60;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		  if(info->brique[i].f >= 1)
		    {
		      info->brique[i].f--;
		      if(info->brique[i].f == 0) info->score = info->score + 20;
		      info->balle[j].x     = info->brique[i].x + info->brique[i].w;
		      info->balle[j].dx    = -info->balle[j].dx;
		      info->balle[j].x     = info->balle[j].x + info->balle[j].dx;
		      if(info->brique[i].bonus > 0 && info->brique[i].f == 0)
			{
			  info->nb_bonus = info->nb_bonus+1;
			  info->bonus[info->nb_bonus].b = info->brique[i].bonus;
			  info->bonus[info->nb_bonus].x = info->brique[i].x;
			  info->bonus[info->nb_bonus].y = info->brique[i].y;
			}
		      break;
		    }
		}
	    }
	}
    }
  
  for(i=0;i<info->nb_brique;i++)
    if(info->brique[i].f == 0)
      brique_casse++;
  if(brique_casse == info->nb_brique)
    etat_jeu_changer (info, E_GAGNE);
}

double calcul_norme (double x, double y)
{
  double norme;
  
  norme = sqrt(x*x+y*y);
  
  return norme;
}

void reorganise_balle(int num, Info *info)
{
  int i = num;
  while(i != info->nb_balle+1)
    {
      info->balle[i] = info->balle[i+1];
      i++;
    }
} 

void balle_etape_suivante (Info *info)
{  
  int rebond_raquette;
  double norme1, norme2;
  int j;
 
  for(j=1;j<=info->nb_balle;j++)
    {
      rebond_raquette = balle_touche_raquette(info->balle[j],info);

      if(info->balle[j].c == 1)
	continue;

      else if( (info->balle[j].x + info->balle[j].dx) > info->area1_width - 2*info->balle[j].r)
	{
	  info->balle[j].x  = info->area1_width - 2*info->balle[j].r;
	  info->balle[j].dx = -info->balle[j].dx;
	}

      else if(info->balle[j].x + info->balle[j].dx < 0)
	{
	  info->balle[j].x  = 0;
	  info->balle[j].dx = -info->balle[j].dx;
	}
      info->balle[j].x = info->balle[j].x + info->balle[j].dx;
      
      if( rebond_raquette > 0) 
	{
	  if(info->raq.b == 1 && info->balle[j].dy > 0)
	    {
	      info->balle[j].c = 1;
	      continue;
	    }
	  norme1 = calcul_norme(info->balle[j].dx, info->balle[j].dy);
	  if(rebond_raquette == 1)
	    {  
	      info->balle[j].dx = -3;
	      info->balle[j].y  = (info->raq.y - 2*info->balle[j].r);
	      info->balle[j].dy = -info->balle[j].dy;
	      norme2            = calcul_norme(info->balle[j].dx, info->balle[j].dy);
	      info->balle[j].dx = (norme1*info->balle[j].dx)/norme2;
	      info->balle[j].dy = (norme1*info->balle[j].dy)/norme2;
	      info->balle[j].y = info->balle[j].y + info->balle[j].dy;
	      return;
	    }
	  else if(rebond_raquette == 2&&info->balle[j].dx<0)
	    {  
	      info->balle[j].dx = -3;
	      info->balle[j].y  = (info->raq.y - 2*info->balle[j].r);
	      info->balle[j].dy = -info->balle[j].dy;
	      norme2            = calcul_norme(info->balle[j].dx, info->balle[j].dy);
	      info->balle[j].dx = (norme1*info->balle[j].dx)/norme2;
	      info->balle[j].dy = (norme1*info->balle[j].dy)/norme2;
	      info->balle[j].y = info->balle[j].y + info->balle[j].dy;
	      return;
	    }
	  else if(rebond_raquette == 2&&info->balle[j].dx>0)
	    {  
	      info->balle[j].dx = 3;
	      info->balle[j].y  = (info->raq.y - 2*info->balle[j].r);
	      info->balle[j].dy = -info->balle[j].dy;
	      norme2            = calcul_norme(info->balle[j].dx, info->balle[j].dy);
	      info->balle[j].dx = (norme1*info->balle[j].dx)/norme2;
	      info->balle[j].dy = (norme1*info->balle[j].dy)/norme2;
	      info->balle[j].y = info->balle[j].y + info->balle[j].dy;
	      return;
	    }
	  else if((rebond_raquette == 3 && info->balle[j].dx<0)||(rebond_raquette == 7&&info->balle[j].dx<0))
	    {
	      info->balle[j].dx = -2;
	      info->balle[j].y  = (info->raq.y - 2*info->balle[j].r);
	      info->balle[j].dy = -info->balle[j].dy;
	      norme2            = calcul_norme(info->balle[j].dx, info->balle[j].dy);
	      info->balle[j].dx = (norme1*info->balle[j].dx)/norme2;
	      info->balle[j].dy = (norme1*info->balle[j].dy)/norme2;
	      info->balle[j].y = info->balle[j].y + info->balle[j].dy;
	      info->balle[j].x = info->balle[j].x + info->balle[j].dx;
	      return;
	      }
	  else if((rebond_raquette == 4 && info->balle[j].dx<0)||(rebond_raquette == 6 && info->balle[j].dx<0))
	    {  
	      info->balle[j].dx = -1;
	      info->balle[j].y  = (info->raq.y - 2*info->balle[j].r);
	      info->balle[j].dy = -info->balle[j].dy;
	      norme2            = calcul_norme(info->balle[j].dx, info->balle[j].dy);
	      info->balle[j].dx = (norme1*info->balle[j].dx)/norme2;
	      info->balle[j].dy = (norme1*info->balle[j].dy)/norme2;
	      info->balle[j].y = info->balle[j].y + info->balle[j].dy;
	      return;
	    }
	   else if((rebond_raquette == 6 && info->balle[j].dx>0)||(rebond_raquette == 4 && info->balle[j].dx>0))
	    {	
	      info->balle[j].dx = 1;
	      info->balle[j].y  = (info->raq.y - 2*info->balle[j].r);
	      info->balle[j].dy = -info->balle[j].dy;
	      norme2            = calcul_norme(info->balle[j].dx, info->balle[j].dy);
	      info->balle[j].dx = (norme1*info->balle[j].dx)/norme2;
	      info->balle[j].dy = (norme1*info->balle[j].dy)/norme2;
	      info->balle[j].y  = info->balle[j].y + info->balle[j].dy;
	      info->balle[j].x  = info->balle[j].x + info->balle[j].dx;
	      return;
	    }

	   else if((rebond_raquette == 7&&info->balle[j].dx>0)||(rebond_raquette == 3 && info->balle[j].dx>0))
	    {	
	      info->balle[j].dx = 2;
	      info->balle[j].y  = (info->raq.y - 2*info->balle[j].r);
	      info->balle[j].dy = -info->balle[j].dy;
	      norme2            = calcul_norme(info->balle[j].dx, info->balle[j].dy);
	      info->balle[j].dx = (norme1*info->balle[j].dx)/norme2;
	      info->balle[j].dy = (norme1*info->balle[j].dy)/norme2;
	      info->balle[j].y  = info->balle[j].y + info->balle[j].dy;
	      info->balle[j].x  = info->balle[j].x + info->balle[j].dx;
	      return;
	    }
	   else if(rebond_raquette == 9)
	    {	
	      info->balle[j].dx = 3;
	      info->balle[j].y  = (info->raq.y - 2*info->balle[j].r);
	      info->balle[j].dy = -info->balle[j].dy;
	      norme2            = calcul_norme(info->balle[j].dx, info->balle[j].dy);
	      info->balle[j].dx = (norme1*info->balle[j].dx)/norme2;
	      info->balle[j].dy = (norme1*info->balle[j].dy)/norme2;
	      info->balle[j].y  = info->balle[j].y + info->balle[j].dy;
	      info->balle[j].x  = info->balle[j].x + info->balle[j].dx;
	      return;
	    }

	  info->balle[j].y  = (info->raq.y - 2*info->balle[j].r);
	  info->balle[j].dy = -info->balle[j].dy;
	}
      else if(info->balle[j].y + info->balle[j].dy < 0) 
	{
	  info->balle[j].y  = 0;
	  info->balle[j].dy = -info->balle[j].dy;
	}
      
      else if(info->balle[j].dy == 0)
	info->balle[j].dy = info->balle[j].dy + 4;

      else if(info->balle[j].y > info->area1_height)  
	{
	  info->nb_balle = info->nb_balle - 1;
	  if(info->nb_balle == 0)
	    {
	      etat_jeu_changer (info, E_PERDU);
	      info->vie--;
	      return;
	    }
	  reorganise_balle(j,info);
	}
	  
      info->balle[j].y = info->balle[j].y + info->balle[j].dy;
    } 
}
   
void raquette_etape_suivante(Info *info)
{
  int i;
  if(info->fl_gauche == 1 && info->fl_droite == 1) return;
  
  if(info->fl_gauche == 1 && (info->raq.x - info->raq.rx) > 5)
    {
      info->raq.x = info->raq.x-6;
      for(i=1;i<=info->nb_balle;i++)
	if(info->balle[i].c == 1)
	    info->balle[i].x = info->balle[i].x-6;
    }

  else if(info->fl_droite == 1 && (info->raq.x + info->raq.rx) < info->area1_width - 5)
    {
      info->raq.x = info->raq.x+6;
      for(i=1;i<=info->nb_balle;i++)
	if(info->balle[i].c == 1)
	  info->balle[i].x = info->balle[i].x+6;
    }
}
   
void area1_redessiner (GdkWindow *win, Info *info)
{
  GdkGC *gc;
  int largeur, hauteur;

  gc = gdk_gc_new (win);
  
  gdk_drawable_get_size (win, &largeur, &hauteur);
  sui_color_rgb_set (gc, 255, 255, 255);
  sui_pixbuf_draw(win, gc, info->pix_fond, 0, 0);
  balle_dessiner    (win, gc, info);
  raquette_dessiner (win, gc, info);
  briques_dessiner  (win, gc, info);

  if(info->nb_bonus>0)
      bonus_dessiner(win, gc, info);
  if(info->laser >0)
    laser_dessiner(win, gc, info);
  if(info->vie <=0)
    sui_pixbuf_draw(win, gc, info->pix_game_over, 0, 0); 
  if(info->etat_jeu == E_FIN)
    sui_pixbuf_draw(win, gc, info->pix_fin, 0, 0);

  g_object_unref (gc);
}

void reafficher (GtkWidget *area)
{
  gdk_window_invalidate_rect (area->window, NULL, TRUE); 
}

void etat_jeu_afficher(Info *info)
{
  char option[100];
  if(info->etat_jeu == E_FIN)
    sprintf(option,"                                 ****SCORE : %d  + %d points de vie****",
	    info->score,(info->vie*10)); 
  else if(info->raq.b==2)
    sprintf(option,"Raquette chargée appuyez sur espace pour tirer!!!!"); 
  else
    sprintf(option,"Niveau %d  ===> Vies : %d   Score : %d",info->lvl,info->vie,info->score);
 
  if (info->anime== 0) 
  {
   sui_statusbar_write (GTK_STATUSBAR (info->stabar1), "");
   return;
  }
  switch (info->etat_jeu){
  case E_DEPART : sui_statusbar_write (GTK_STATUSBAR (info->stabar1), "Appuyez sur espace ou sur les fleches");break;
  case E_REBOND : sui_statusbar_write (GTK_STATUSBAR (info->stabar1), option);break;
  case E_PERDU  : sui_statusbar_write (GTK_STATUSBAR (info->stabar1), "C'est Perdu!! Appuyez sur espace");break;
  case E_GAGNE  : sui_statusbar_write (GTK_STATUSBAR (info->stabar1), "C'est Gagné!! Appuyez sur espace");break;
  case E_FIN    : sui_statusbar_write (GTK_STATUSBAR (info->stabar1), option);break;  
  default : break;
  }
}

void etat_jeu_changer (Info *info, int etat)
{
  info->etat_jeu = etat;
  etat_jeu_afficher(info);
}

int main (int argc, char *argv[])
{
    GtkWidget *tmp;
    Info info;

    gtk_init (&argc, &argv);

    if(argv[1] != NULL)
      info_init (&info, argv[1]); 
    else
      info_init (&info,NULL);

    info.win1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (info.win1), WIN1_TITRE);
    gtk_window_set_default_size (GTK_WINDOW (info.win1), WIN1_LARG, WIN1_HAUT);

 
    SUI_ON_SIGNAL (info.win1, "destroy", win1_on_destroy, &info);


    info.vbox1 = gtk_vbox_new (FALSE, 3);
    gtk_container_add (GTK_CONTAINER (info.win1), info.vbox1);


    info.hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (info.vbox1), info.hbox1, FALSE, FALSE, 0);

    tmp = gtk_button_new_with_label ("Quit");
    gtk_box_pack_start (GTK_BOX (info.hbox1), tmp, FALSE, FALSE, 0);
    SUI_ON_SIGNAL (tmp, "clicked", butt_quit_on_clicked, &info);
    

    info.start=gtk_button_new_with_label ("Start");
    gtk_box_pack_start (GTK_BOX (info.hbox1), info.start, FALSE, FALSE, 0);
    SUI_ON_SIGNAL (info.start, "clicked", butt_start_on_clicked, &info);

    info.area1 = gtk_drawing_area_new ();
    gtk_box_pack_start (GTK_BOX (info.vbox1), info.area1, TRUE, TRUE, 0);

    GTK_WIDGET_SET_FLAGS  (info.area1, GTK_CAN_FOCUS);
    gtk_widget_set_events (info.area1, 
                           GDK_EXPOSURE_MASK       |
                           GDK_BUTTON_PRESS_MASK   |
                           GDK_BUTTON_RELEASE_MASK |
                           GDK_BUTTON_MOTION_MASK  |
                           GDK_KEY_PRESS_MASK      |
                           GDK_KEY_RELEASE_MASK    );


    SUI_ON_SIGNAL (info.area1, "expose_event"        , area1_on_expose        , &info);
    SUI_ON_SIGNAL (info.area1, "button_press_event"  , area1_on_button_press  , &info);
    SUI_ON_SIGNAL (info.area1, "button_release_event", area1_on_button_release, &info);
    SUI_ON_SIGNAL (info.area1, "motion_notify_event" , area1_on_motion_notify , &info);
    SUI_ON_SIGNAL (info.area1, "key_press_event"     , area1_on_key_press     , &info);
    SUI_ON_SIGNAL (info.area1, "key_release_event"   , area1_on_key_release   , &info);
    SUI_ON_SIGNAL (info.area1, "configure_event"     , area1_on_configure     , &info);
    
    info.stabar1 = gtk_statusbar_new ();
    gtk_box_pack_start (GTK_BOX (info.vbox1), info.stabar1, FALSE, FALSE, 0);
    sui_statusbar_write (GTK_STATUSBAR (info.stabar1), "Welcome");

 
    gtk_widget_show_all (info.win1);
 
    gtk_main ();

    info_detruire (&info);

    return 0;
}


