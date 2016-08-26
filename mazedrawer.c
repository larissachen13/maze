#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

#define WINDOW_WIDTH 830
#define WINDOW_HEIGHT 830
#define GRID_SIZE 100

gchar buf[256];
gint x = 0;


gboolean on_expose_event(GtkWidget *widget,
    GdkEventExpose *event,
    gpointer data) {
        
  cairo_t *cr;

  cr = gdk_cairo_create(widget->window);

  int spot_height = 800/GRID_SIZE;
  int indent = (WINDOW_HEIGHT - (GRID_SIZE * spot_height))/2;
  for (int i = 0; i < GRID_SIZE; i++){
    for (int j = 0; j < GRID_SIZE; j++){

      //set the color and draw the grids
      cairo_set_source_rgb(cr, 0.5, 0.5, 1);
      cairo_rectangle(cr, i*spot_height + 1 + indent, j*spot_height + 1 + indent, spot_height - 1, spot_height -1);
      cairo_fill(cr);

      //set the color to black and draw west walls
      cairo_set_source_rgb(cr, 0, 0, 0);
      cairo_rectangle(cr, i*spot_height + indent, j*spot_height + indent, 1, spot_height);
      cairo_fill(cr);

      //set the color to black and draw north walls
      cairo_set_source_rgb(cr, 0, 0, 0);
      cairo_rectangle(cr, i*spot_height + indent, j*spot_height + indent, spot_height, 1);
      cairo_fill(cr);

      //draw a south wall
      cairo_set_source_rgb(cr, 0, 0, 0);
      cairo_rectangle(cr, i*spot_height + indent, (j+1)*spot_height + indent, spot_height, 1);
      cairo_fill(cr);

      //set the color to black and draw east walls
      cairo_set_source_rgb(cr, 0, 0, 0);
      cairo_rectangle(cr, (i+1)*spot_height + indent, j*spot_height + indent, 1, spot_height);
      cairo_fill(cr);

      cairo_set_source_rgb(cr, 0, 0, 0);
      cairo_rectangle(cr, x, x, spot_height, spot_height);
      cairo_fill(cr);
    }
  }

  cairo_destroy(cr);

  return FALSE;
}

gboolean time_handler(GtkWidget *widget) {
    
  if (widget->window == NULL) return FALSE;

  x += 5;

  gtk_widget_queue_draw(widget);
  
  return TRUE;
}

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *darea;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);

  g_signal_connect(darea, "expose-event",
      G_CALLBACK(on_expose_event), NULL);
  g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_HEIGHT, WINDOW_WIDTH);

  gtk_window_set_title(GTK_WINDOW(window), "Maze Drawer");
  g_timeout_add(166, (GSourceFunc) time_handler, (gpointer) window);
  gtk_widget_show_all(window);
  time_handler(window);

  gtk_main();

  return 0;
}

//gcc mazedrawer.c -o mazedrawer `pkg-config --cflags --libs gtk+-2.0`