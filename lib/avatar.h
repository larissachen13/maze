/*
 * avatar.h - header file for avatar.c
 *
 * Benjamin Littlejohn, August 2016
 * Team: core_dumped_in_a_maze
 */

/************* includes **************/
#include "amazing.h"
#include "mazestruct.h"
#include <netdb.h>

#ifndef _avatar_h_
#define _avatar_h_

/************* prototypes ************/
/*
 * Method to move an avatar so it can solve the Maze Challenge
 *
 * Input:
 * 	maze - database shared betweeen all avatars storing what is currently
 * 	       known about the maze
 * 	avatar - avatar that is making a move
 *
 * Algorithm:
 * 	If there is a known route between all avatars...
 * 	Follow the route to bring all avatars together
 *	If a path connecting all avatars is not yet discovered...
 * 	Run through all possible moves to see what priorities each satisfies
 * 	Choose the move that satisfies the highest priority
 *
 * Priorities:
 * 	1. Getting to the same space as another avatar
 * 	2. Visiting an unvisited space
 * 	3. Visiting a space that was visited by another avatar
 * 	4. Visiting a space I have already visited
 * 	5. Don't run into a known wall!!!
 */
void make_move(mazestruct_t *maze, Avatar *avatar, int comm_sock,
	AM_Message *msg_buff);

/*
 * Waits until the server sends a message then reads it into the given
 * 	message buffer
 *
 * Caller is responsible for allocating memory for the message buffer and
 * 	freeing it
 */
void wait_for_response(int comm_sock, AM_Message *msg_buff);

#endif
