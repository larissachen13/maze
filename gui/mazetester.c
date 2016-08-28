#include "mazedrawer.h"

#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "../lib/mazestruct.h"

int main(int argc, char *argv[]){

	FILE * fp = fopen("testfile", "w");
	if (fp == NULL){
		exit(1);
	}
	mazestruct_t *maze = maze_new(50, 50, 4, fp);
	place_avatar(maze, 5, 5, 0);
	insert_dead_spot(maze, 10, 10, 0);
	update_location(maze, 5,5,5,6, 0);
	insert_wall(maze, 5, 5, 1, 0);

	maze_drawer(maze);

}

//gcc mazetester.c -o mazetester ../lib/mazestruct.c mazedrawer.c `pkg-config --cflags --libs gtk+-2.0` -lpthread