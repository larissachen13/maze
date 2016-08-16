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

	mazestruct_t* new_maze = maze_new(10,10,1);

	printf("Number of avatars:%d\n", new_maze->num_avatars);
	printf("Height: %d, Width: %d\n", new_maze->height, new_maze->width);

	maze_print(new_maze);
	place_avatar(new_maze, 5,5);
	maze_print(new_maze);
}