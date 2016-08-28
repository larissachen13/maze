/*
* A file to draw the avatars moving through the maze using GTK and Cairo graphics
* Much of the setting up of the refreshing was done studying the timer.c code from "http://zetcode.com/gui/gtk2/gtkevents/".
*
* Author: Benji Hannam, 2016
*/

#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "../lib/mazestruct.h"

#define WINDOW_WIDTH 830
#define WINDOW_HEIGHT 830

//global variables
extern pthread_mutex_t my_turn;

//function protoypes prototype
void* maze_drawer(void *maze_in);
gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data);
gboolean time_handler(GtkWidget *widget);

/*************** maze_drawer() ******************/
/*
* Sets up the gtk window and runs the maze animation
* Returns NULL when the window is closed.
*/
void* maze_drawer(void *maze_in){

  mazestruct_t *maze = maze_in;
  //variable to hold the window and the are to draw on
  GtkWidget *window;
  GtkWidget *darea;

  gtk_init(NULL, NULL);

  //create the new window widget
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  //set up the drawing area and add it to the window
  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);

  g_signal_connect(darea, "expose-event", G_CALLBACK(on_expose_event), maze);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  //set up the position and size of the window
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_HEIGHT, WINDOW_WIDTH);

  //set the title
  gtk_window_set_title(GTK_WINDOW(window), "Maze Drawer");

  //add the loop to draw every set amount of time
  g_timeout_add(166, (GSourceFunc) time_handler, (gpointer) window);
  gtk_widget_show_all(window);
  time_handler(window);

  //run
  gtk_main();

  return NULL;
}

/*************** time_handler() ******************/
/*
* Calls the redraw every set amount of time
* 
*/

gboolean time_handler(GtkWidget *widget) {
    
  if (widget->window == NULL){
    return false;
  }

  gtk_widget_queue_draw(widget);
  
  return true;
}

/*************** on_expose_event() ******************/
/*
* When we request the redraw, creates the new image to draw.
* returns false when done.
*/
gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data) {
  pthread_mutex_lock(&my_turn);
  cairo_t *cr;
  mazestruct_t *maze = data;
  int height = get_height(maze);
  int width = get_width(maze);

  cr = gdk_cairo_create(widget->window);

  int spot_height = 800/height;
  int indent = (WINDOW_HEIGHT - (height * spot_height))/2;
  for (int i = 0; i < width; i++){
    for (int j = 0; j < height; j++){

      //set the color and draw the grids
      if(is_dead(maze, i, j, -1)){
        cairo_set_source_rgb(cr, 0, 0, 0);
      }
      else if(is_visited(maze, i, j, -1)){
        cairo_set_source_rgb(cr, 0, 1, 1);
      }
      else{
        cairo_set_source_rgb(cr, 0.5, 0.5, 1);
      }
      cairo_rectangle(cr, i*spot_height + 1 + indent, j*spot_height + 1 + indent, spot_height - 1, spot_height -1);
      cairo_fill(cr);

      if(has_avatar(maze, i, j)){
        cairo_set_source_rgb(cr, 1, 0, 0);
        cairo_rectangle(cr, i*spot_height + 6 + indent, j*spot_height + 6 + indent, spot_height - 10, spot_height - 10);
        cairo_fill(cr);
      }

      //set the color to black and draw west walls
      if(check_wall(maze, i, j, 0)){
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_rectangle(cr, i*spot_height + indent - 1, j*spot_height + indent, 3, spot_height +1);
        cairo_fill(cr);
      }

      //set the color to black and draw north walls
      if(check_wall(maze, i, j, 1)){
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_rectangle(cr, i*spot_height + indent, j*spot_height + indent - 1, spot_height +1, 3);
        cairo_fill(cr);
      }

      //draw a south wall
      if(check_wall(maze, i, j, 2)){
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_rectangle(cr, i*spot_height + indent, (j+1)*spot_height + indent - 1, spot_height+1, 3);
        cairo_fill(cr);
      }

      //set the color to black and draw east walls
      if(check_wall(maze, i, j, 3)){
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_rectangle(cr, (i+1)*spot_height + indent - 1 , j*spot_height + indent, 3, spot_height+1);
        cairo_fill(cr);
      }
    }
  }

  cairo_destroy(cr);
  pthread_mutex_unlock(&my_turn);

  return false;
}


//gcc mazedrawer.c -o mazedrawer `pkg-config --cflags --libs gtk+-2.0`
