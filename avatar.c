/*
 * Avatar.c
 * Name: Benjamin Littlejohn
 * Team: core_dumped_in_a_maze 
 * Date: August 2016
 * Purpose: Module of cs50 X16 project Maze Challenge
 * See avatar.h for details
 */

/************* constants ***************/
#define FIRST_PRIORITY (4)
#define SECOND_PRIORITY	(3)
#define THIRD_PRIORITY (2)
#define BACK_TRACK (1)
#define DONT_DO_IT (0)

/************** includes ***************/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "avatar.h"

/*
 * Method to move a given avatar  
 * See avatar.h for full description
 */
void make_move(mazestruct_t *maze, Avatar *avatar) {

    int move;

    //save current position so we can later check if move was successful
    XYPos old_pos = avatar->pos;
    //get the best move
    move = get_best_move(maze, avatar->pos);
    //send the move to the server
    send_move(avatar->fd, move);

    //wait for return message signifying move was made
    wait_for_return_message; //still need to figure out

    //if move failed add a wall to the maze at that spot
    if ((move != NULL_MOVE) && same_pos(old_pos, avatar->pos)) {
	insert_wall(maze, avatar->pos.x, avatar->pos.y, move); 
    }
    //otherwise...
    else {
	//update maze with new avatar position
	place_avatar(maze, avatar->pos.x, avatar->pos.y);
	//if avatar was forced to backtrack it has reached a dead end
	if (move == BACK_TRACK) {
	    dead_spot(old_pos.x, old_pos.y);
	}
	//otherwise space hasn't been visited by avatar so update visited list
	else {
	    visited_spot(maze, avatar->pos.x, avatar->pos.y, avatar->fd);
	}
    }
}

/*
 * Determines an avatar's next move using the shared maze knowledge of all the
 * 	avatars and the avatar's current position 
 */
static int get_best_move(mazestruct_t *maze, XYPos my_pos) {
    int best_move;
    if (we_can_come_together(maze)) { //get method from Benji
	best_move = come_together();
    }
    else {
	best_move = get_best_move_helper(maze, my_pos);
    }
    return best_move;
}

/*
 * Helper method for get_best_move()
 *
 * Determines what an avatar should do if a path connecting all avatars is not 
 * 	yet discovered
 *
 * Algorithm: 
 * 	Run through all possible moves to see what priorities each satisfies 
 * 	Choose the move that satisfies the highest priority
 *
 * See avatar.h for details on priorities
 */
static int get_best_move_helper(mazestruct_t maze, XYPos my_pos) {

    int best_move = -1; //stores score of best move so far
    int move_rank; //keeps score for the current move

    for (int move = M_WEST; i < M_NUM_DIRECTIONS; move++) {
	/************* score the move *****************/
	//if the move results in running into a wall
	if (check_wall(my_pos.x, my_pos.y, move)) {
	    move_rank = DONT_DO_IT;
	}
	//if the move results in potentially meeting another avatar
	else if (someone_adjacent(my_pos.x, my_pos.y, move)) { 
	    move_rank = FIRST_PRIORITY;
	}
	//if the move results in potentially visiting an unvisited space
	else if(!is_visited(my_pos.x, my_pos.y, move)) { 
	    move_rank = SECOND_PRIORITY;
	}
	//if the move results in potentially visiting a space visited by 
	//a different avatar
	else if(!get_visited(avatar->fd, my_pos.x, my_pos.y, move)) {
	    move_rank = THIRD_PRIORITY;
	}
	//if the move results in visiting a space I have already visited
	else {
	    move_rank = BACK_TRACK;
	}
	//check how current move compares to the best_move
	if (move_rank > best_move) {
	    best_move = move;
	}
    }
    return best_move;
}

/*
 * Sends an avatar's move to the server; for now just prints a message
 * Still need to figure out
 */
static void send_move(uint32_t avatarID, uint32_t direction) {
    printf("Move was sent.\n");
}

/*
 * Returns true if the old_pos is equal to the new_pos
 */
static bool same_pos(XYPos old_pos, XYPos new_pos) {
    return ((old_pos.x == new_pos.x) && (old_pos.y == new_pos.y));
}

/*
 * Test method that will eventually be replaced with actual method to bring 
 * 	avatars together
 */
static int come_together() {
    int move = 10;
    printf("Coming together!!!\n");
    return move;
}

