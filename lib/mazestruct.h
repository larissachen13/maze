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

typedef struct avatar_move {
    int direction;
    int score;
} avatar_move;

/**************** functions ****************/

/**************** maze_new() ****************/
/*
* Takes in the height and width of the maze and the number of avatars playing the game.
* Allocates memory for the maze structure and generates the 2d array of the grid spots.
* returns a pointer the the new maze structure generated.
*/
mazestruct_t* maze_new(int height, int width, int num_avatars, FILE *fp);

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
void insert_wall(mazestruct_t *maze, int x, int y, int direction, int avatar_id);

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
void insert_dead_spot(mazestruct_t *maze, int x,int y, int avatar_id);

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
int is_someone_adjacent(mazestruct_t *maze, int x, int y, int direction);

/**************** is_someone_here() ****************/
/*
* Checks if there is an avatar other than the one with the given id at the
* 	current spot dictated by x and y.
* Takes in the maze struct, x,y coordinates and the id of the avatar checking
* 	if another avatar is present.
* Returns the lowest avatar id if another avatar is on the spot
* 	and -1 if none is found.
*/
int is_someone_here(mazestruct_t *maze, int x, int y, int id);

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

/**************** remove_leader() ****************/
/*
* Decrements the number of leaders by 1 only if the avatar was previously a leader
*
*/
void remove_leader(mazestruct_t *maze, int avatar_id);

/**************** insert_last_move() ****************/
/*
* updates the last move for an avatar
*
*/
void insert_last_move(mazestruct_t *maze, int direction, int score, int avatar_id);

/**************** get_last_direction() ****************/
/*
* Returns the last direction in which an avatar moved
*
*/
int get_last_direction(mazestruct_t *maze, int avatar_id);

/**************** get_last_score() ****************/
/*
* Returns the last score of the last move for an avatar
*
*/
int get_last_score(mazestruct_t *maze, int avatar_id);

/**************** get_number_leaders() ****************/
/*
* Returns the number of leaders
*
*/
int get_number_leaders(mazestruct_t *maze);

/**************** set_leader() ****************/
/*
* Sets the leader of the given avatar
*
*/
void set_leader(mazestruct_t *maze, int avatar_id, int leader_id);

/**************** get_leader() ****************/
/*
* Gets the leader of the given avatar
*
*/
int get_leader(mazestruct_t *maze, int avatar_id);


/**************** have_paths_crossed() ****************/
/*
* Returns if the paths of the two leaders have crossed
*
*/
bool have_paths_crossed(mazestruct_t *maze);

/**************** who_visited() ****************/
/*
* Returns the lowest numbered of id of the an avatar who's path is adjacent but
* has not crossed paths with the current avatar.
* Returns -1 if no such avatar exist.
*/
int who_visited(mazestruct_t *maze, int x, int y, int direction, int avatar_id);

/**************** cross_paths() ****************/
/*
* Takes in the the ids of two avatars and marks that anyone the first avatar has crossed with,
* the second has also crossed with and vice versa.
* returns true if all avatars have crossed paths with each other, false otherwise.
*/
bool cross_paths (int id1, int id2, mazestruct_t *maze);

/****************** is_dead() ***********************/
/*
 * returns whether or not the spot in the a given direction from
 * 	the spot at (x,y) is dead
 */
bool is_dead(mazestruct_t *maze, int x, int y, int direction);

/**************** print_solved() ****************/
/*
* Prints the maze solved message
*
*/
void print_solved(mazestruct_t *maze);

#endif
