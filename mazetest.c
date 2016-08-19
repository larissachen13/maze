/* 
* A test file for mazestruct.c
*
* Created by Benji Hannam, 2016 for use in Team Core Dumped in a Maze for CS50
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mazestruct.c"

int main(int argc, char *argv[]){

	mazestruct_t* new_maze = maze_new(4,4,1);

	printf("Number of avatars:%d\n", new_maze->num_avatars);
	printf("Height: %d, Width: %d\n", new_maze->height, new_maze->width);

	place_avatar(new_maze, 1, 0, 0);
	place_avatar(new_maze, 1, 0, 1);
	insert_dead_spot(new_maze, 3, 3);
	insert_wall(new_maze, 0, 0, 3);
	update_location(new_maze, 1, 0, 1, 1, 0);
	update_location(new_maze, 1, 0, 1, 1, 1);
}