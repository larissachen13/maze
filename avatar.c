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
#define THIRD_PRIORITY	(2)
#define FOURTH_PRIORITY	(1)
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
	make_wall(maze, pos, move); //get method from Benji
    }
    //otherwise update maze with new position and mark the space as visited
    else {
	update_maze(avatar->fd, avatar->pos); //get method from Benji
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
	//score the move
	if (next_to_avatar) { //get method from Benji
	    move_rank = FIRST_PRIORITY;
	}
	else if(neighbor is unvisited) { //get method from Benji
	    move_rank = SECOND_PRIORITY;
	}
	else if(visited && visitor wasn't me) { //get method from Benji
	    move_rank = THIRD_PRIORITY;
	}
	else if(neighbor is visited && vistor was me) { //get method from Benji
	    move_rank = FOURTH_PRIORITY;
	}
	else {
	    move_rank = DONT_DO_IT;
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

