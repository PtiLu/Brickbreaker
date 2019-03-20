#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "sui-gtk.h"
#include "my_game.h"
#include<unistd.h>

#define WIN1_TITRE  "CASSE BRIQUE"
#define WIN1_LARG   500
#define WIN1_HAUT   417

void pix_charger_tous (My_struct *info)
{
 info->pix_fond     = sui_pixbuf_load("./images/fond_menu.png"     );
 info->pix_jouer    = sui_pixbuf_load("./images/jouer_select.png"  );
 info->pix_editeur  = sui_pixbuf_load("./images/edit_select.png"   );
 info->pix_records  = sui_pixbuf_load("./images/record_select.png" );
 info->pix_niveau   = sui_pixbuf_load("./images/niveau_select.png" );
 info->pix_quitter  = sui_pixbuf_load("./images/quit_select.png"   );
}

void reafficher (GtkWidget *area)
{
  gdk_window_invalidate_rect (area->window, NULL, TRUE); 
}

void win1_on_destroy (GtkWidget * UNUSED(widget), gpointer UNUSED(data))
{
  gtk_main_quit ();
}

void area1_redessiner (GdkWindow *win, My_struct *info)
{
  GdkGC *gc;
  int largeur, hauteur;

  gc = gdk_gc_new (win);
  
  gdk_drawable_get_size (win, &largeur, &hauteur);
  sui_color_rgb_set (gc, 255, 255, 255);
  sui_pixbuf_draw(win, gc, info->pix_fond, 0, 0);

  g_object_unref (gc);
}

gboolean area1_on_expose (GtkWidget *UNUSED(area), GdkEventExpose *e, gpointer data)
{
  area1_redessiner (e->window, data);
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

gboolean area1_on_button_release (GtkWidget *UNUSED(area), GdkEventButton *UNUSED(e), gpointer data)
{  
  int i,rep,score[10];
  char *fichier, jeu[100], nom[10][100], affiche[500];
  My_struct *info = data;
  GtkWidget *dialog;
  FILE *f;

  if(info->select == 1)
    system("./pong");
  
  else if(info->select == 2)
    system("./editeur");
  
  else if(info->select == 3)
    {
      f = fopen("meilleur_score.txt","r");
      for(i=0;i<10;i++)
	fscanf(f,"%s : %d",nom[i], &score[i]);
      sprintf(affiche,"      MEILLEURS SCORES                      \n"
	              "            %s : %d\n            %s : %d\n            %s : %d\n            %s : %d\n"
	              "            %s : %d\n            %s : %d\n            %s : %d\n            %s : %d\n"
	              "            %s : %d\n            %s : %d",
	      nom[0], score[0],nom[1], score[1],nom[2], score[2],nom[3], score[3],nom[4], score[4],
	      nom[5], score[5],nom[6], score[6],nom[7], score[7],nom[8], score[8],nom[9], score[9]);
      dialog = gtk_message_dialog_new(GTK_WINDOW(info->win1), GTK_DIALOG_MODAL,
				      GTK_MESSAGE_OTHER, GTK_BUTTONS_CLOSE, "%s",affiche);
      
      rep = gtk_dialog_run(GTK_DIALOG(dialog));

      switch(rep)
	{
	case GTK_RESPONSE_CLOSE:break;
	default: break;
	}
      gtk_widget_destroy (dialog);
      fclose(f);
      
    }
  else if(info->select == 4)
    {
      dialog = gtk_file_chooser_dialog_new  (" Choisissez votre niveau",
					     GTK_WINDOW(info->win1),GTK_FILE_CHOOSER_ACTION_OPEN,
					     GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
					     GTK_STOCK_OPEN  ,GTK_RESPONSE_OK,NULL);
      rep = gtk_dialog_run(GTK_DIALOG(dialog)); 
      switch(rep)
	{
	case GTK_RESPONSE_OK:
	  fichier = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	  sprintf(jeu,"./pong %s",fichier);
	  break;
	default:gtk_widget_destroy (dialog); return TRUE;
	}
      gtk_widget_destroy (dialog);
      system(jeu);
    }
  else if(info->select == 5)
    win1_on_destroy (NULL, NULL); 

  return TRUE; 
}  

gboolean area1_on_motion_notify (GtkWidget *UNUSED(area), GdkEventMotion *e, gpointer data)
{
  My_struct *info = data;
  GdkGC *gc;

  gc = gdk_gc_new (e->window);

  if((e->x>80 && e->x<371)&&(e->y>322 && e->y<382))
    {
      sui_pixbuf_draw(e->window, gc, info->pix_quitter, 0, 0);
      info->select = 5;
    }

  else if((e->x>37 && e->x<451)&&(e->y>242 && e->y<298))
    {
      sui_pixbuf_draw(e->window, gc, info->pix_niveau, 0, 0);
      info->select = 4;
    }

  else if((e->x>68 && e->x<395)&&(e->y>158 && e->y<219))
    {
      sui_pixbuf_draw(e->window, gc, info->pix_records, 0, 0);
      info->select = 3;
    }

  else if((e->x>90 && e->x<375)&&(e->y>87 && e->y<139))
    {
      sui_pixbuf_draw(e->window, gc, info->pix_editeur, 0, 0);
      info->select = 2;
    }

  else if((e->x>111 && e->x<336)&&(e->y>0 && e->y<63))
    {
      sui_pixbuf_draw(e->window, gc, info->pix_jouer, 0, 0);  
      info->select = 1;
    }
  
  else
    {
      reafficher (info->area1);
      info->select = -1;
    }
  
  g_object_unref (gc);
  return TRUE;  
}


int main(int argc, char *argv[])
{
  My_struct info;
  
  gtk_init (&argc, &argv);
  pix_charger_tous(&info);

  info.select = -1;

  info.win1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (info.win1), WIN1_TITRE);
  gtk_window_set_default_size (GTK_WINDOW (info.win1), WIN1_LARG, WIN1_HAUT);

  SUI_ON_SIGNAL (info.win1, "destroy", win1_on_destroy, &info);

  /*~~~~~~~~~~~~~~~~CREATION VBOX~~~~~~~~~~~~~~~~~~*/
  info.vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (info.win1), info.vbox1);

 
  /*~~~~~~~~~~~~~~~~CREATION HBOX~~~~~~~~~~~~~~~~~~*/
  info.hbox1 = gtk_hbox_new (FALSE,3);
  gtk_container_add (GTK_CONTAINER(info.vbox1),info.hbox1);

  info.area1 = gtk_drawing_area_new ();
  gtk_box_pack_start (GTK_BOX (info.hbox1), info.area1, TRUE, TRUE, 0);
  
  
  GTK_WIDGET_SET_FLAGS  (info.area1, GTK_CAN_FOCUS);
  gtk_widget_set_events (info.area1, 
			 GDK_ALL_EVENTS_MASK     |
			 GDK_EXPOSURE_MASK       |
			 GDK_BUTTON_PRESS_MASK   |
			 GDK_BUTTON_RELEASE_MASK |
			 GDK_BUTTON_MOTION_MASK  |
			 GDK_KEY_PRESS_MASK      |
			 GDK_KEY_RELEASE_MASK    );
  
 
  SUI_ON_SIGNAL (info.area1, "expose_event"        , area1_on_expose        , &info);
  SUI_ON_SIGNAL (info.area1, "button_release_event", area1_on_button_release, &info);
  SUI_ON_SIGNAL (info.area1, "motion_notify_event" , area1_on_motion_notify , &info);
  SUI_ON_SIGNAL (info.area1, "configure_event"     , area1_on_configure     , &info);
  
  info.stabar1 = gtk_statusbar_new ();
  gtk_box_pack_start (GTK_BOX (info.vbox1), info.stabar1, FALSE, FALSE, 0);
  
  sui_statusbar_write (GTK_STATUSBAR (info.stabar1), "Choisissez une action");
  
  gtk_widget_show_all (info.win1);
  
  gtk_main ();
   
  return 0;
}
