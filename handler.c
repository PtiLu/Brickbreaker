#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>  /* pour keyvals tq GDK_q */
#include "pong.h"
#include "handler.h"
#include "sui-gtk.h"
#define TIMEOUT1 35
#define TIMEOUT2 10

void win1_on_destroy (GtkWidget * UNUSED(widget), gpointer UNUSED(data))
{
  gtk_main_quit ();
}

void butt_quit_on_clicked (GtkWidget * UNUSED(widget), gpointer UNUSED(data))
{
  gtk_main_quit ();
}

gboolean on_timeout2 (gpointer data)
{
  Info *info = data;
  
  raquette_etape_suivante(info);
  if(info->etat_jeu == E_DEPART)
  {
    balle_init(info);
    balle_preparer_service(info);
  }
  else if (info->etat_jeu == E_REBOND)
  {
    etat_jeu_afficher(info);
    briques_calculer_collisions (info);
    balle_etape_suivante(info);
    lacher_bonus(info);
    stop_bonus(info);
  }
 
    

  return TRUE;
}

gboolean on_timeout1 ( gpointer data)
{
  Info *info = data; 
  reafficher(info->area1);
  return TRUE;
}

void butt_start_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
   Info *info= data;
   if (info->anime == 0){
     info->anime = 1;
     etat_jeu_afficher(info);
     gtk_button_set_label (GTK_BUTTON (info->start), "Stop");
     gtk_widget_grab_focus(info->area1);
     if (info->timeout1 == 0 )
       info->timeout1= g_timeout_add(TIMEOUT1,on_timeout1,info);
     
     info->timeout2= g_timeout_add(TIMEOUT2,on_timeout2,info); 
   }
   
   else {
     info->anime = 0;
     etat_jeu_afficher(info);
     gtk_button_set_label (GTK_BUTTON (info->start), "Start");
     if (info->timeout1 != 0 ){
       g_source_remove(info->timeout1);
       info->timeout1 = 0;
       g_source_remove(info->timeout2);
       info->timeout2 = 0;
     }
   }
}


gboolean area1_on_expose (GtkWidget *UNUSED(area), GdkEventExpose *e, gpointer data)
{
  area1_redessiner (e->window, data);
  return TRUE;  
}


gboolean area1_on_button_press (GtkWidget *area, GdkEventButton *UNUSED(e), gpointer UNUSED(data))
{
  gtk_widget_grab_focus(area);
  return TRUE; 
}


gboolean area1_on_button_release (GtkWidget *UNUSED(area), GdkEventButton *UNUSED(e), gpointer UNUSED(data))
{  
  return TRUE;  
}

gboolean area1_on_motion_notify (GtkWidget *UNUSED(area), GdkEventMotion *UNUSED(e), gpointer UNUSED(data))
{
  return TRUE;  
}

gboolean area1_on_configure (GtkWidget *UNUSED(area), GdkEventConfigure *e, gpointer data)
{
  Info *info = data;
  printf("width = %d, height = %d\n",e->width,e->height);
  info->area1_width  = e->width;
  info->area1_height = e->height;
  raquette_adapter(&info->raq, info->area1_width, info->area1_height);
  return TRUE;
}

gboolean area1_on_key_press (GtkWidget *UNUSED(area), GdkEventKey *e, gpointer data)
{
  Info *info = data;
  int i, temp;
 
  switch (e->keyval) {
  case GDK_Escape :gtk_main_quit (); break; 
  case GDK_q      :gtk_main_quit (); break;
  case GDK_Left   :if(info->laser==0)info->fl_gauche = 1;break;
  case GDK_Right  :if(info->laser==0)info->fl_droite = 1;break;  
  case GDK_Return :  
    if (info->etat_jeu == E_REBOND)
      butt_start_on_clicked(NULL, data);
    break;
  case GDK_space  : 
    if(info->etat_jeu == E_DEPART)
      {
	etat_jeu_changer(info, E_REBOND);
	break;
      }
    else if (info->etat_jeu == E_REBOND)
      {
	if(info->raq.b == 1)
	  for(i=1;i<=info->nb_balle;i++)
	    {
	      if(info->balle[i].c == 1)
		{
		  info->balle[i].c = 0;
		  info->balle[i].y = (info->raq.y - 2*info->balle[i].r);
		  info->balle[i].dy= -info->balle[i].dy;
		  info->balle[i].y = info->balle[i].y + info->balle[i].dy;
		}
	    }
	else if(info->raq.b == 2)
	  {
	    info->timer_xraq = g_timer_new();
	    info->laser      = 1;
	  }
	      
	break;
      }

    else if (info->etat_jeu == E_PERDU && info->vie <= 0) 
      {
	if(info->my_lvl == NULL)
	  {
	    temp = tester_score (info->score, info->m_score);
	    if(temp >= 0)
	      enregistrer_score(info->m_score, info->score, temp);
	  }
	info->vie    = 3;
	info->lvl    = 1;
	info->score  = 0;
	info->laser  = 0;
	brique_init(info); 
	bonus_init(info);
	raquette_init (info);
	raquette_adapter(&info->raq, info->area1_width, info->area1_height);
	etat_jeu_changer(info, E_DEPART);
	break;
      }
    else if (info->etat_jeu == E_PERDU)
      {
	info->timer_xballe = NULL;
	info->timer_xraq   = NULL;
	info->balle[1].c = 0;
	info->laser      = 0;
	etat_jeu_changer(info, E_DEPART);
	bonus_init(info);
	raquette_init (info);
	raquette_adapter(&info->raq, info->area1_width, info->area1_height);
	break;
      }
    else if (info->etat_jeu == E_GAGNE)
      {
	info->lvl++;
	info->laser = 0;
	if(!brique_init(info))
	  {
	    etat_jeu_changer (info, E_FIN);
	    break;
	  }
	bonus_init(info);
	raquette_init (info);
	info->balle[1].c   = 0;
	info->timer_xballe = NULL;
	info->timer_xraq   = NULL;
	raquette_adapter(&info->raq, info->area1_width, info->area1_height);
	etat_jeu_changer(info, E_DEPART);
	break;
      }
    else if (info->etat_jeu == E_FIN)
      {
	info->score = info->score + (info->vie*10);
	if(info->my_lvl == NULL)
	  {
	    temp = tester_score (info->score, info->m_score);
	    if(temp >= 0)
	      enregistrer_score(info->m_score, info->score, temp);
	  }
	info->vie    = 3; 
	info->lvl    = 1;
	info->score  = 0;
	info->balle[1].c = 0;
	info->timer_xballe = NULL;
	info->timer_xraq   = NULL;
	brique_init(info); 
	bonus_init(info);
	raquette_init (info);
	raquette_adapter(&info->raq, info->area1_width, info->area1_height);
	etat_jeu_changer(info, E_DEPART);
	break;
      }
    break;
  case GDK_r : brique_init(info);info->raq.rx = 35;break;
  case GDK_3 : info->bonus[1].b = 3;test_bonus(info,1);break;
  case GDK_2 : info->bonus[1].b = 2;test_bonus(info,1);break; 
  case GDK_1 : info->bonus[1].b = 1;test_bonus(info,1);break;
  case GDK_4 : info->bonus[1].b = 4;test_bonus(info,1);break;
  case GDK_5 : info->bonus[1].b = 5;test_bonus(info,1);break;
  case GDK_6 : info->bonus[1].b = 6;test_bonus(info,1);break;
  case GDK_7 : info->bonus[1].b = 7;test_bonus(info,1);break; 
  case GDK_8 : info->bonus[1].b = 8;test_bonus(info,1);break;     
  
  default : break;
  }
  
  return TRUE;  
}

gboolean area1_on_key_release (GtkWidget *UNUSED(area), GdkEventKey *e, gpointer data)
{
  Info *info = data;

  switch (e->keyval) {
  case GDK_Left   :info->fl_gauche = 0;break;
  case GDK_Right  :info->fl_droite = 0;break;  
    
  default :  break;
  }
  
  return TRUE;  
}
