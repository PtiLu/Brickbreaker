#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "editeur.h"
#include "sui-gtk.h"

#define WIN1_TITRE  "EDIT_PONG"
#define WIN1_LARG   619
#define WIN1_HAUT   442


void pix_charger_tous (My_struct *info)
{
 info->pix_fond     = sui_pixbuf_load("./images/fond_edit.png");
 info->pix_brique3  = sui_pixbuf_load("./images/brique_tfend.png");
 info->pix_brique2  = sui_pixbuf_load("./images/brique_fend.png");
 info->pix_brique   = sui_pixbuf_load("./images/brique.png");
}

void reafficher (GtkWidget *area)
{
  gdk_window_invalidate_rect (area->window, NULL, TRUE); 
}

void butt_save_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  int i, rep;
  My_struct *info = data;
  FILE *f;
  GtkWidget *dialog;

  dialog = gtk_file_chooser_dialog_new  ("          Save File          ",
					   GTK_WINDOW(info->win1),GTK_FILE_CHOOSER_ACTION_SAVE,
					   GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
					   GTK_STOCK_SAVE  ,GTK_RESPONSE_OK,NULL);
  rep = gtk_dialog_run(GTK_DIALOG(dialog)); 
  switch(rep)
    {
    case GTK_RESPONSE_OK:
      info->save =1;
      info->name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
      f = fopen(info->name,"w");
      fprintf(f,"%d\n",info->indic);
      for(i=0;i<info->indic;i++)
	{
	  fprintf(f,"%d " ,info->brique[i].x);
	  fprintf(f,"%d " ,info->brique[i].y);
	  fprintf(f,"%d " ,info->brique[i].d);
	  fprintf(f,"%d\n",info->brique[i].b);
	}
      fclose(f);
      break;
      
    default : printf("ferme\n");break;
    }
  gtk_widget_destroy (dialog);
}

void win1_on_destroy (GtkWidget * UNUSED(widget), gpointer data)
{
  int rep;
  My_struct *info = data;
  GtkWidget *message;
  
  if(info->save == 0)
    {
      message = gtk_message_dialog_new(GTK_WINDOW(info->win1), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, 
				       GTK_BUTTONS_YES_NO,"Voulez-vous sauvegarder?");
      
      rep = gtk_dialog_run(GTK_DIALOG(message));
      
      switch(rep)
	{
	case GTK_RESPONSE_YES:
	  butt_save_on_clicked (NULL, data); break;
	default: break;
	}
      gtk_widget_destroy (message);
    }
  gtk_main_quit ();
}

void butt_quit_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  int rep;
  My_struct *info = data;
  GtkWidget *message;
  
  if(info->save == 0)
    {
      message = gtk_message_dialog_new(GTK_WINDOW(info->win1), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, 
				       GTK_BUTTONS_YES_NO,"Voulez-vous sauvegarder?");
      
      rep = gtk_dialog_run(GTK_DIALOG(message));
      
      switch(rep)
	{
	case GTK_RESPONSE_YES:
	  butt_save_on_clicked (NULL, data); break;
	default: break;
	}
      gtk_widget_destroy (message);
    } 
  gtk_main_quit ();
}

void butt_clear_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  int i;
  My_struct *info = data;
  for(i=0;i<info->indic;i++)
    {
      info->brique[i].x = -1;
      info->brique[i].y = -1;
      info->brique[i].d = 0; 
      info->brique[i].b = 0;
    }
  info->indic  = 0;
  info->select =-1;
  info->save   = 0;
  reafficher (info->area1);
  printf("clear\n");
}

void butt_load_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  int i, rep;
  gchar *name;
  My_struct *info = data;
  FILE *f;
  GtkWidget *dialog, *message;
  
  if(info->save == 0)
    {
      message = gtk_message_dialog_new(GTK_WINDOW(info->win1), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, 
				       GTK_BUTTONS_YES_NO,"Voulez-vous sauvegarder?");
      
      rep = gtk_dialog_run(GTK_DIALOG(message));
      
      switch(rep)
	{
	case GTK_RESPONSE_YES:
	  butt_save_on_clicked (NULL, data); break;
	default: break;
	}
      gtk_widget_destroy (message);
    }

  dialog = gtk_file_chooser_dialog_new  ("                         Load File",
					 GTK_WINDOW(info->win1),GTK_FILE_CHOOSER_ACTION_OPEN,
					 GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
					 GTK_STOCK_OPEN  ,GTK_RESPONSE_OK,NULL);
  rep = gtk_dialog_run(GTK_DIALOG(dialog)); 
  switch(rep)
    {
    case GTK_RESPONSE_OK:
      printf("accept\n");
      name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
      f = fopen(name,"r");
      fscanf(f,"%d",&info->indic);
      for(i=0;i<info->indic;i++)
	{
	  fscanf(f,"%d" ,&info->brique[i].x);
	  fscanf(f,"%d" ,&info->brique[i].y);
	  fscanf(f,"%d" ,&info->brique[i].d);
	  fscanf(f,"%d" ,&info->brique[i].b);
	}
      fclose(f);
      g_free (name);
      break;
      
    default : printf("ferme\n");break;
    }
  gtk_widget_destroy (dialog);
  reafficher (info->area1);
}

void butt_new_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  int rep;
  My_struct *info = data;
  GtkWidget *message;

  if(info->save == 0)
    {
      message = gtk_message_dialog_new(GTK_WINDOW(info->win1), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, 
				       GTK_BUTTONS_YES_NO,"Voulez-vous sauvegarder?");
      
      rep = gtk_dialog_run(GTK_DIALOG(message));

      switch(rep)
	{
	case GTK_RESPONSE_YES:
	  butt_save_on_clicked (NULL, data); break;
	default: break;
	}
      gtk_widget_destroy (message);
    }
  info->save = 1;
  butt_clear_on_clicked (NULL, data);
}

void butt_create_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  My_struct *info = data;
  
  reafficher (info->area1);
  if(info->mode == 0)
    {
      printf("enable create mode\n");
      sui_statusbar_write (GTK_STATUSBAR (info->stabar1), "Déplacez le curseur et faite un click gauche pour poser une brique");
      info->mode = 1;
      return;
    }
  printf("disable create mode\n");
  sui_statusbar_write (GTK_STATUSBAR (info->stabar1), "Mode édition, click gauche pour selectionner et click droit pour effacer");
  info->mode = 0;
}

void butt_bonus1_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  My_struct *info = data;
  char texte[100];

  if(info->select == -1 || info->mode == 1)
    return;
  info->save =0;
  info->brique[info->select].b = 1;
  sprintf(texte,"Brique %d ==> epaisseur : %d, bonus : agrandissement raquette"
	  ,(info->select+1),info->brique[info->select].d);
  sui_statusbar_write (GTK_STATUSBAR (info->stabar1), texte);
  printf("bonus integré\n");
}

void butt_bonus2_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  My_struct *info = data;
  char texte[100];

  if(info->select == -1 || info->mode == 1)
    return;
  info->save =0;
  info->brique[info->select].b = 2;
  sprintf(texte,"Brique %d ==> epaisseur : %d, bonus : +1 vie"
	  ,(info->select+1),info->brique[info->select].d);
  sui_statusbar_write (GTK_STATUSBAR (info->stabar1), texte);
  printf("bonus integré\n");
}

void butt_bonus3_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  My_struct *info = data;
  char texte[100];

  if(info->select == -1 || info->mode == 1)
    return;
  info->save =0;
  info->brique[info->select].b = 3;
  sprintf(texte,"Brique %d ==> epaisseur : %d, bonus : multi_balle"
	  ,(info->select+1),info->brique[info->select].d);
  sui_statusbar_write (GTK_STATUSBAR (info->stabar1), texte);
  printf("bonus integré\n");
}

void butt_bonus4_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  My_struct *info = data;
  char texte[100];

  if(info->select == -1 || info->mode == 1)
    return;
  info->save =0;
  info->brique[info->select].b = 4;
  sprintf(texte,"Brique %d ==> epaisseur : %d, bonus : -1 vie"
	  ,(info->select+1),info->brique[info->select].d);
  sui_statusbar_write (GTK_STATUSBAR (info->stabar1), texte);
  printf("bonus integré\n");
}

void butt_bonus5_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  My_struct *info = data;
  char texte[100];

  if(info->select == -1 || info->mode == 1)
    return;
  info->save =0;
  info->brique[info->select].b = 5;
  sprintf(texte,"Brique %d ==> epaisseur : %d, bonus : reduction raquette"
	  ,(info->select+1),info->brique[info->select].d);
  sui_statusbar_write (GTK_STATUSBAR (info->stabar1), texte);
  printf("bonus integré\n");
}

void butt_bonus6_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  My_struct *info = data;
  char texte[100];

  if(info->select == -1 || info->mode == 1)
    return;
  info->save =0;
  info->brique[info->select].b = 6;
  sprintf(texte,"Brique %d ==> epaisseur : %d, bonus : super_balle"
	  ,(info->select+1),info->brique[info->select].d);
  sui_statusbar_write (GTK_STATUSBAR (info->stabar1), texte);
  printf("bonus integré\n");
}

void butt_bonus7_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  My_struct *info = data;
  char texte[100];

  if(info->select == -1 || info->mode == 1)
    return;
  info->save =0;
  info->brique[info->select].b = 7;
  sprintf(texte,"Brique %d ==> epaisseur : %d, bonus : raquette collante"
	  ,(info->select+1),info->brique[info->select].d);
  sui_statusbar_write (GTK_STATUSBAR (info->stabar1), texte);
  printf("bonus integre\n");
}

void butt_bonus8_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  My_struct *info = data;
  char texte[100];

  if(info->select == -1 || info->mode == 1)
    return;
  info->save =0;
  info->brique[info->select].b = 8;
  sprintf(texte,"Brique %d ==> epaisseur : %d, bonus : super raquette"
	  ,(info->select+1),info->brique[info->select].d);
  sui_statusbar_write (GTK_STATUSBAR (info->stabar1), texte);
  printf("bonus integre\n");
}

void butt_clear_bonus_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  My_struct *info = data;
  char texte[100];
  if(info->select == -1 || info->mode == 1)
    return;
  info->save =0;
  info->brique[info->select].b = 0;
  sprintf(texte,"Brique %d ==> epaisseur : %d, pas de bonus"
	  ,(info->select+1),info->brique[info->select].d);
  sui_statusbar_write (GTK_STATUSBAR (info->stabar1), texte);
  printf("bonus retire\n");
}

void butt_aligne_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  int i;
  My_struct *info = data;
  if(info->select == -1 || info->mode == 1)
    return;
  info->save =0;
  for(i=0;i<info->indic;i++)
    {
      if(((info->brique[i].y > info->brique[info->select].y) && (info->brique[i].y < (info->brique[info->select].y + 15)))||
	 ((info->brique[i].y < info->brique[info->select].y) && (info->brique[i].y > (info->brique[info->select].y - 15))))
	info->brique[i].y = info->brique[info->select].y;
    }
  reafficher (info->area1);
}

void butt_aligne_colonne_on_clicked (GtkWidget * UNUSED(widget), gpointer data)
{
  int i;
  My_struct *info = data;
  if(info->select == -1 || info->mode == 1)
    return;
  info->save =0;
  for(i=0;i<info->indic;i++)
    {
      if(((info->brique[i].x > info->brique[info->select].x) && (info->brique[i].x < (info->brique[info->select].x + 50)))||
	 ((info->brique[i].x < info->brique[info->select].x) && (info->brique[i].x > (info->brique[info->select].x - 50))))
	info->brique[i].x = info->brique[info->select].x;
    }
  reafficher (info->area1);
}

void area1_redessiner (GdkWindow *win, My_struct *info)
{
  GdkGC *gc;
  int largeur, hauteur;

  gc = gdk_gc_new (win);
  
  gdk_drawable_get_size (win, &largeur, &hauteur);
  sui_color_rgb_set (gc, 255, 255, 255);
  sui_pixbuf_draw(win, gc, info->pix_fond, 0, 0);
  sui_color_rgb_set (gc, 0, 0, 255);
  gdk_draw_rectangle (win, gc, FALSE, 0, 0, 500, 300);

  g_object_unref (gc);
}

void dessiner_brique (GdkWindow *win, gpointer data)
{
  int i;
  My_struct *info = data;
  GdkGC *gc;
  gc = gdk_gc_new (win);

  for(i=0;i<info->indic;i++)
    {
      if(info->brique[i].d == 1)
	sui_pixbuf_draw(win, gc, info->pix_brique3, info->brique[i].x, info->brique[i].y);

      else if(info->brique[i].d == 2)
	sui_pixbuf_draw(win, gc, info->pix_brique2, info->brique[i].x, info->brique[i].y);
      
      else if(info->brique[i].d == 3)
	sui_pixbuf_draw(win, gc, info->pix_brique, info->brique[i].x, info->brique[i].y);
    }

  sui_color_rgb_set (gc, 255, 0, 0);
  if(info->mode == 0 && info->select >= 0)
    gdk_draw_rectangle (win, gc, FALSE, info->brique[info->select].x, info->brique[info->select].y, 50, 15);
}

void effacer_brique (int indice, My_struct *info)
{
  int i;
  info->save =0;
  info->indic = info->indic - 1;
  for(i=indice;i<info->indic;i++)
    info->brique[i] = info->brique[i+1];
}

gboolean area1_on_expose (GtkWidget *UNUSED(area), GdkEventExpose *e, gpointer data)
{
  area1_redessiner (e->window, data);
  dessiner_brique  (e->window, data);
  return TRUE;  
}

int test_sortie(int x, int y)
{
  if(x+50 > 500 || x < 0)
    return 0;
  if(y+15 > 300 || y < 0)
    return 0;
  return 1;
}

int test_creation (Tableau t[], int nb_brique, int x, int y)
{
  int i;

  if(!test_sortie(x, y))return 0;
  for(i=0;i<nb_brique;i++)
    {
      if((x    >= t[i].x && x    <= (t[i].x+50) && y    >= t[i].y && y    <= (t[i].y+15))||
	 (x+50 >= t[i].x && x+50 <= (t[i].x+50) && y    >= t[i].y && y    <= (t[i].y+15))||
	 (x    >= t[i].x && x    <= (t[i].x+50) && y+15 >= t[i].y && y+15 <= (t[i].y+15))||
	 (x+50 >= t[i].x && x+50 <= (t[i].x+50) && y+15 >= t[i].y && y+15 <= (t[i].y+15)))
	return 0;
    }
  return 1;
}

int test_click (Tableau t[], int nb_brique, int x, int y)
{
   int i;

   if(x > 500)
     return -1;
   if(y > 300)
     return -1;
   for(i=0;i<nb_brique;i++)
     if(x >= t[i].x && x <= (t[i].x+50) && y >= t[i].y && y <= (t[i].y+15))
       return i;
   
   return -1;
}

gboolean area1_on_button_press (GtkWidget *area, GdkEventButton *e, gpointer data)
{
  int indice;
  char texte[100];
  My_struct *info = data;

  gtk_widget_grab_focus(area);
  
  if(info->mode == 0 && e->button == 1)
    {
      indice = test_click(info->brique, info->indic, e->x, e->y);

      if(indice >= 0)
	{
	  info->select = indice;
	  if(info->brique[info->select].b == 1)
	    sprintf(texte,"Brique %d ==> epaisseur : %d, bonus : agrandissement raquette "
		    ,(info->select+1),info->brique[info->select].d);
	  else if(info->brique[info->select].b == 2)
	     sprintf(texte,"Brique %d ==> epaisseur : %d, bonus :+1 vie "
		    ,(info->select+1),info->brique[info->select].d);
	  else if(info->brique[info->select].b == 3)
	    sprintf(texte,"Brique %d ==> epaisseur : %d, bonus : multi_balle "
		    ,(info->select+1),info->brique[info->select].d);
	  else if(info->brique[info->select].b == 4)
	    sprintf(texte,"Brique %d ==> epaisseur : %d, bonus :-1 vie "
		    ,(info->select+1),info->brique[info->select].d);
	  else if(info->brique[info->select].b == 5)
	    sprintf(texte,"Brique %d ==> epaisseur : %d, bonus :reduction raquette "
		    ,(info->select+1),info->brique[info->select].d);
	  else if(info->brique[info->select].b == 6)
	    sprintf(texte,"Brique %d ==> epaisseur : %d, bonus : super_balle "
		    ,(info->select+1),info->brique[info->select].d);
	  else if(info->brique[info->select].b == 7)
	    sprintf(texte,"Brique %d ==> epaisseur : %d, bonus : raquette collante "
		    ,(info->select+1),info->brique[info->select].d);
	  else if(info->brique[info->select].b == 8)
	    sprintf(texte,"Brique %d ==> epaisseur : %d, bonus : super raquette "
		    ,(info->select+1),info->brique[info->select].d);
	  else if(info->brique[info->select].b == 0)
	    sprintf(texte,"Brique %d ==> epaisseur : %d, pas de bonus "
		    ,(info->select+1),info->brique[info->select].d);

	  sui_statusbar_write (GTK_STATUSBAR (info->stabar1), texte);
	}
      else
	{
	  info->select = -1;
	  sui_statusbar_write (GTK_STATUSBAR (info->stabar1), 
			       "Mode édition, click gauche pour selectionner et click droit pour effacer");
	} 

      reafficher (info->area1);
    }

  else if(info->mode == 0 && e->button == 3)
    {
      indice = test_click(info->brique, info->indic, e->x, e->y);

      if(indice >= 0)
	{
	  info->save =0;
	  effacer_brique (indice, info);
	  info->select = -1;
	}

      reafficher (info->area1);
    }      
  return TRUE; 
}

gboolean area1_on_button_release (GtkWidget *UNUSED(area), GdkEventButton *e, gpointer data)
{  
  My_struct *info = data;

  if(info->mode == 1)
    {
      if(test_creation(info->brique, info->indic, e->x, e->y)== 1)
	{
	  info->save =0;
	  info->brique[info->indic].x = e->x;
	  info->brique[info->indic].y = e->y;
	  info->brique[info->indic].d = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (info->solid));
	  info->indic = info->indic + 1;
	  reafficher (info->area1);
	}
    }  
  return TRUE;  
}

gboolean area1_on_motion_notify (GtkWidget *UNUSED(area), GdkEventMotion *e, gpointer data)
{
  My_struct *info = data;

  if(info->mode == 0 && info->select > -1)
    {
      if(test_sortie(e->x,e->y))
	{
	  info->save =0; 
	  info->brique[info->select].x = e->x;
	  info->brique[info->select].y = e->y;
	}     
    } 
  reafficher (info->area1);
  return TRUE;  
}

gboolean area1_on_configure (GtkWidget *UNUSED(area), GdkEventConfigure *e, gpointer data)
{
  My_struct *info = data;

  printf("width = %d, height = %d\n",e->width,e->height);
  info->area1_width  = e->width;
  info->area1_height = e->height;

  return TRUE;
}

gboolean area1_on_key_press (GtkWidget *UNUSED(area), GdkEventKey *e, gpointer UNUSED(data))
{
 
  switch (e->keyval) {
  case GDK_Escape :break; 
  case GDK_q      :gtk_main_quit (); break;
  default :  break;
  }
  return TRUE;  
}

gboolean area1_on_key_release (GtkWidget *UNUSED(area), GdkEventKey *e, gpointer UNUSED(data))
{
  switch (e->keyval) {
    
  default : break;
  }  
  return TRUE;  
}

void make_my_menu(My_struct *my)
{
  my->item = gtk_menu_item_new_with_label("Nouveau");
  gtk_menu_shell_append(GTK_MENU_SHELL(my->menu), my->item);
  SUI_ON_SIGNAL(my->item, "activate", butt_new_on_clicked, my);

  my->item = gtk_menu_item_new_with_label("Ouvrir");
  SUI_ON_SIGNAL(my->item, "activate", butt_load_on_clicked, my);
  gtk_menu_shell_append(GTK_MENU_SHELL(my->menu), my->item);

  my->item = gtk_menu_item_new_with_label("Sauver");
  SUI_ON_SIGNAL(my->item, "activate", butt_save_on_clicked, my);
  gtk_menu_shell_append(GTK_MENU_SHELL(my->menu), my->item);

  my->item = gtk_menu_item_new_with_label("Quitter");
  SUI_ON_SIGNAL(my->item, "activate", win1_on_destroy, my);
  gtk_menu_shell_append(GTK_MENU_SHELL(my->menu), my->item);

  my->item = gtk_menu_item_new_with_label("Fichier");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(my->item), my->menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(my->menubar), my->item);

}

int main (int argc, char *argv[])
{
  int i;
  My_struct info;
  GtkWidget *p_image;

  gtk_init (&argc, &argv);

  pix_charger_tous(&info);

  info.mode   = 0;
  info.indic  = 0;
  info.select = -1;
  info.save   = 1;
  info.name   = NULL;
  info.brique = malloc(210*sizeof(Tableau));
  assert(info.brique != NULL);
  for(i=0;i<210;i++)
    {
      info.brique[i].x = -1;
      info.brique[i].y = -1;
      info.brique[i].d =  0;
      info.brique[i].b =  0;
    }

  info.win1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (info.win1), WIN1_TITRE);
  gtk_window_set_default_size (GTK_WINDOW (info.win1), WIN1_LARG, WIN1_HAUT);

  SUI_ON_SIGNAL (info.win1, "destroy", win1_on_destroy, &info);

  /*~~~~~~~~~~~~~~~~CREATION VBOX~~~~~~~~~~~~~~~~~~*/
  info.vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (info.win1), info.vbox1);

  /*~~~~~~~~~~~~~~~~CREATION MENUBAR~~~~~~~~~~~~~~~*/
  info.menubar   = gtk_menu_bar_new();
  gtk_box_pack_start(GTK_BOX(info.vbox1),info.menubar,FALSE,FALSE,0);
  info.menu = gtk_menu_new();
  make_my_menu(&info);

  /*~~~~~~~~~~~~~~~~CREATION HBOX~~~~~~~~~~~~~~~~~~*/
  info.hbox1 = gtk_hbox_new (FALSE,3);
  gtk_container_add (GTK_CONTAINER(info.vbox1),info.hbox1);

  /*~~~~~~~~~~~~~~~~CREATION TABLE~~~~~~~~~~~~~~~~~*/
  info.table = gtk_table_new(10, 2, TRUE);
  info.clear = gtk_button_new_with_label ("      EFFACER     ");
  gtk_table_attach(GTK_TABLE(info.table), info.clear,  0, 2, 0, 1, TRUE, FALSE, 0, 0);
  SUI_ON_SIGNAL (info.clear, "clicked", butt_clear_on_clicked, &info);
  
  info.create = gtk_toggle_button_new_with_label ("Ajouter");
  gtk_table_attach(GTK_TABLE(info.table), info.create, 0, 1, 1, 2, FALSE, FALSE, 0, 0);
  SUI_ON_SIGNAL (info.create, "clicked", butt_create_on_clicked, &info);
   
  info.solid = gtk_spin_button_new_with_range(1,3,1);
  gtk_table_attach(GTK_TABLE(info.table), info.solid,  1, 2, 1, 2, FALSE, FALSE, 0, 0);

  info.clear_bonus = gtk_button_new_with_label (" PAS DE BONUS ");
  gtk_table_attach(GTK_TABLE(info.table), info.clear_bonus, 0, 2, 3, 4, TRUE, FALSE, 0, 0);
  SUI_ON_SIGNAL (info.clear_bonus, "clicked", butt_clear_bonus_on_clicked, &info);

  info.bonus1 = gtk_button_new();
  gtk_table_attach(GTK_TABLE(info.table), info.bonus1, 0, 1, 4, 5, TRUE, FALSE, 0, 0);
  p_image =  gtk_image_new_from_file("./images/bonus1.png");
  gtk_container_add(GTK_CONTAINER(info.bonus1),p_image);
  SUI_ON_SIGNAL (info.bonus1, "clicked", butt_bonus1_on_clicked, &info);
  
  info.bonus2 = gtk_button_new();
  gtk_table_attach(GTK_TABLE(info.table), info.bonus2, 1, 2, 4, 5, TRUE, FALSE, 0, 0);
  p_image =  gtk_image_new_from_file("./images/bonus2.png");
  gtk_container_add(GTK_CONTAINER(info.bonus2),p_image);
  SUI_ON_SIGNAL (info.bonus2, "clicked", butt_bonus2_on_clicked, &info);

  info.bonus3 = gtk_button_new();
  gtk_table_attach(GTK_TABLE(info.table), info.bonus3, 0, 1, 5, 6, TRUE, FALSE, 0, 0);
  p_image =  gtk_image_new_from_file("./images/bonus3.png");
  gtk_container_add(GTK_CONTAINER(info.bonus3),p_image);
  SUI_ON_SIGNAL (info.bonus3, "clicked", butt_bonus3_on_clicked, &info);

  info.bonus4 = gtk_button_new();
  gtk_table_attach(GTK_TABLE(info.table), info.bonus4, 1, 2, 5, 6, TRUE, FALSE, 0, 0);
  p_image =  gtk_image_new_from_file("./images/bonus4.png");
  gtk_container_add(GTK_CONTAINER(info.bonus4),p_image);
  SUI_ON_SIGNAL (info.bonus4, "clicked", butt_bonus4_on_clicked, &info);

  info.bonus5 = gtk_button_new();
  gtk_table_attach(GTK_TABLE(info.table), info.bonus5, 0, 1, 6, 7, TRUE, FALSE, 0, 0);
  p_image =  gtk_image_new_from_file("./images/bonus5.png");
  gtk_container_add(GTK_CONTAINER(info.bonus5),p_image);
  SUI_ON_SIGNAL (info.bonus5, "clicked", butt_bonus5_on_clicked, &info);

  info.bonus6 = gtk_button_new();
  gtk_table_attach(GTK_TABLE(info.table), info.bonus6, 1, 2, 6, 7, TRUE, FALSE, 0, 0);
  p_image =  gtk_image_new_from_file("./images/bonus6.png");
  gtk_container_add(GTK_CONTAINER(info.bonus6),p_image);
  SUI_ON_SIGNAL (info.bonus6, "clicked", butt_bonus6_on_clicked, &info);

  info.bonus7 = gtk_button_new();
  gtk_table_attach(GTK_TABLE(info.table), info.bonus7, 0, 1, 7, 8, TRUE, FALSE, 0, 0);
  p_image =  gtk_image_new_from_file("./images/bonus7.png");
  gtk_container_add(GTK_CONTAINER(info.bonus7),p_image);
  SUI_ON_SIGNAL (info.bonus7, "clicked", butt_bonus7_on_clicked, &info);
 
  info.bonus8 = gtk_button_new();
  gtk_table_attach(GTK_TABLE(info.table), info.bonus8, 1, 2, 7, 8, TRUE, FALSE, 0, 0);
  p_image =  gtk_image_new_from_file("./images/bonus8.png");
  gtk_container_add(GTK_CONTAINER(info.bonus8),p_image);
  SUI_ON_SIGNAL (info.bonus8, "clicked", butt_bonus8_on_clicked, &info);
  
  info.aligne_ligne = gtk_button_new_with_label (" ALIGNER LIGNE");
  gtk_table_attach(GTK_TABLE(info.table), info.aligne_ligne, 0, 2, 8, 9, TRUE, FALSE, 0, 0);
  SUI_ON_SIGNAL (info.aligne_ligne, "clicked", butt_aligne_on_clicked, &info);

  info.aligne_colonne = gtk_button_new_with_label (" ALIGNER COLO ");
  gtk_table_attach(GTK_TABLE(info.table), info.aligne_colonne, 0, 2, 9, 10, TRUE, FALSE, 0, 0);
  SUI_ON_SIGNAL (info.aligne_colonne, "clicked", butt_aligne_colonne_on_clicked, &info);
  gtk_box_pack_start (GTK_BOX (info.hbox1), GTK_WIDGET(info.table), FALSE, FALSE, 0);  
 
  info.area1 = gtk_drawing_area_new ();
  gtk_box_pack_start (GTK_BOX (info.hbox1), info.area1, TRUE, TRUE, 0);
  
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
  
  sui_statusbar_write (GTK_STATUSBAR (info.stabar1), "Editeur de niveau");
  
  gtk_widget_show_all (info.win1);
  
  gtk_main ();
   
  return 0;
}
