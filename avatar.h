/*
 * avatar.h - header file for avatar.c
 * Name: Benjamin Littlejohn
 * Team: core_dumped_in_a_maze 
 * Date: August 2016
 * Purpose: Module of cs50 X16 project Maze Challenge
 */

/************* includes **************/
#include "amazing.h"
#include "mazestruct.h"

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
void make_move(mazestruct_t *maze, Avatar avatar);

#endif
