#include "mazedrawer.h"

#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "../lib/mazestruct.h"

int main(int argc, char *argv[]){

	mazestruct_t *maze = maze_new(50, 50, 4, NULL);

	maze_drawer(maze);
}

//gcc mazetester.c -o mazetester ../lib/mazestruct.c mazedrawer.c `pkg-config --cflags --libs gtk+-2.0`