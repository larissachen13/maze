/* 
 * mazestruct.h - header file for mazestruct
 *
 * Benji Hannam, July 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef __MAZESTRUCT_H
#define __MAZESTRUCT_H

/**************** global types ****************/
typedef struct mazestruct mazestruct_t;  // opaque to users of the module

/**************** functions ****************/

/**************** maze_new() ****************/
/*
* Takes in the height and width of the maze and the number of avatars playing the game.
* Allocates memory for the maze structure and generates the 2d array of the grid spots.
* returns a pointer the the new maze structure generated.
*/
mazestruct_t* maze_new(int height, int width, int num_avatars);

/**************** maze_print() ****************/
/*
* Prints out the 2D representation of the maze. 
* Prints from left to right and top to bottom going row by row.
* If there are two avatars on the same spot it will print the lowest numbers avatar.
* 
*/
void maze_print(mazestruct_t *maze);

/**************** place_avatar() ****************/
/*
* Takes in a pointer to a maze struct, a pair of x,y coords and the id of the avatar.
* Then marks the avatar at being in that spot referenced by the x,y coordinates provided.
* Also prints the entire maze structure to stdout.
*/
void place_avatar(mazestruct_t *maze, int x, int y, int avatar_number);

/**************** insert_wall() ****************/
/*
* Tells the maze structure where a wall has been found.
* Takes in a pointer to a maze struct, a pair of x,y coordinates and a direction denoted by:
* 0 = west, 1 = north, 2 = south and 3 = west.
*/
void insert_wall(mazestruct_t *maze, int x, int y, int direction);

/**************** visited_spot() ****************/
/*
* Marks a spot as being visited by an avatar.
* Takes in a pointer to a maze struct, a pair of x,y coords and the id of the avatar that visited it.
* 
*/
void visited_spot(mazestruct_t *maze, int x, int y, int avatar_number);

/**************** insert_dead_spot() ****************/
/*
* Marks a spot as being dead.
* Takes in a pointer to a maze struct, a pair of x,y coords and the id of the avatar that visited it.
* 
*/
void insert_dead_spot(mazestruct_t *maze, int x,int y);

/**************** check_wall() ****************/
/*
* Checks if we know for sure there is a wall on the side of a spot
* Takes in a pointer to a maze struct, a pair of x,y coords and a direction designated by:
* 0 = west, 1 = north, 2 = south and 3 = west.
* Returns true if there is a wall and false if there is not.
*/
bool check_wall(mazestruct_t *maze, int x, int y, int direction);

/**************** is_someone_adjacent() ****************/
/*
* Checks if there is someone the adjacent spot dictated by the inputted direction.
* Takes in the maze struct, x,y coordinates and the direction as parameters.
* returns true if someone is adjacent and false otherwise.
*/
bool is_someone_adjacent(mazestruct_t *maze, int x, int y, int direction);

/**************** update_location() ****************/
/*
* Moves an avatar from one spot to another one.
* Takes in a maze struct, the inital x,y and the new x,y coordinates and the id of the avatar to move.
*
*/
void update_location(mazestruct_t *maze, int init_x, int init_y, int new_x, int new_y, int avatar_number);

/**************** is_visited() ****************/
/*
* Checks if a nearby spot denoted by the direction has been visited.
* 
* returns true if it has, false if it hasn't
*/
bool is_visited(mazestruct_t *maze, int x, int y, int direction);

/**************** did_x_visit() ****************/
/*
* Checks if a spot denoted by a direction has been 
* Takes in the maze struct, x,y coordinates, the direction and an avatar id as parameters.
* returns true if the avatar has visited it otherwise returns false.
*/
bool did_x_visit(mazestruct_t *maze, int x, int y, int direction, int avatar_number);

/**************** delete_maze() ****************/
/*
* Deletes the maze preventing memory leaks.
* 
* 
*/
void delete_maze(mazestruct_t *maze);

#endif


