/* 
 * mazedrawer.h - header file for mazedrawer
 *
 * Benji Hannam, August 2016
 */

#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "../lib/mazestruct.h"
 
#ifndef __MAZEDRAWER_H
#define __MAZEDRAWER_H

void* maze_drawer(void *maze_in);
gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data);
gboolean time_handler(GtkWidget *widget);

#endif

//gcc mazetester.c -o mazetester mazestruct.c mazedrawer.c `pkg-config --cflags --libs gtk+-2.0`
