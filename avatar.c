/*
 * avatar.c
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
#define HAVE_TO_BACK_TRACK (1)
#define DONT_DO_IT (0)

/************** includes ***************/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "avatar.h"

/**************************** global functions *****************************/
/*
 * Method to move a given avatar
 * See avatar.h for full description
 */
void make_move(mazestruct_t *maze, Avatar *avatar, int comm_sock) {

    //save current position can check later if move was successful
    XYPos old_pos = avatar->pos;

    int move = get_best_move(maze, avatar->pos);
    if (!send_move(avatar->fd, move, comm_sock)) {
	perror("Error writing avatar %d's move to server.\n", avatar->id);
    }

    wait_for_response(comm_sock, msg_buff);

    //update avatar position based on response
    my_avatar->pos = ntohl(msg_buff->Pos[my_avatar->fd]);
    update_maze(maze, old_pos, move, my_avatar);
}

/*
 * Waits until the server sends a message then reads it into the given
 * 	message buffer
 *
 * Caller is responsible for allocating memory for the message buffer and
 * 	freeing it
 */
void wait_for_response(int comm_sock, AM_Message *msg_buff) {
    if ((read(comm_sock, *msg_buff, sizeof(AM_Message))) < 0) {
	     perror("Error reading message from server.\n");
    }
}

/*
 * Updates the shared map of the maze based on an avatar's last move, former
 * 	position, and current positon
 */
void update_maze(mazestruct_t *maze, XYPos old_pos, int move, Avatar *avatar) {
    //if move failed add a wall to the maze at that spot
    if ((move != NULL_MOVE) && same_pos(old_pos, avatar->pos)) {
	insert_wall(maze, avatar->pos.x, avatar->pos.y, move);
    }
    //otherwise...
    else {
	//update maze with new avatar position
	update_location(maze, old_pos.x, old_pos.y, avatar->pos.x,
		avatar->pos.y, avatar->fd);
	//if avatar was forced to backtrack it has reached a dead end
	if (move == HAVE_TO_BACK_TRACK) {
	    insert_dead_spot(maze, old_pos.x, old_pos.y);
	}
	//otherwise space hasn't been visited by avatar so update visited list
	else {
	    visited_spot(maze, avatar->pos.x, avatar->pos.y, avatar->fd);
	}
    }
}

/**************************** local functions ******************************/
/*
 * Determines an avatar's next move using the shared maze knowledge of all the
 * 	avatars and the avatar's current position
 */
static int get_best_move(mazestruct_t *maze, XYPos my_pos) {

    int best_move;

    //haven't figured out what to do here so just avoid situation
    if (false) {
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
static int get_best_move_helper(mazestruct_t *maze, XYPos my_pos) {

    int best_move = -1; //stores score of best move so far
    int move_rank; //keeps score for the current move

    for (int move = M_WEST; i < M_NUM_DIRECTIONS; move++) {
	/************* score the move *****************/
	//if the move results in running into a wall
	if (check_wall(maze, my_pos.x, my_pos.y, move)) {
	    move_rank = DONT_DO_IT;
	}
	//if the move results in potentially meeting another avatar
	else if (is_someone_adjacent(maze, my_pos.x, my_pos.y, move)) {
	    move_rank = FIRST_PRIORITY;
	}
	//if the move results in potentially visiting an unvisited space
	else if(!is_visited(maze, my_pos.x, my_pos.y, move)) {
	    move_rank = SECOND_PRIORITY;
	}
	//if the move results in potentially visiting a space visited by
	//a different avatar
	else if(!did_x_visit(maze, my_pos.x, my_pos.y, move, avatar->fd)) {
	    move_rank = THIRD_PRIORITY;
	}
	//if the move results in visiting a space I have already visited
	else {
	    move_rank = HAVE_TO_BACK_TRACK;
	}
	//check how current move compares to the best_move
	if (move_rank > best_move) {
	    best_move = move;
	}
    }
    return best_move;
}

/*
 * Sends an avatar's move to the maze server
 * Returns true if the message was sent sucessfully and false otherwise
 */
static bool send_move(uint32_t avatar_id, uint32_t direction, int comm_sock) {

    //form AM_AVATAR_MOVE message to send to server
    AM_Message my_move;
    my_move.type = htonl(AM_AVATAR_MOVE);
    my_move.avatar_move.AvatarID = htonl(avatar_ID);
    my_move.avatar_move.Direction = htonl(direction);

    //send message to server
    if (write(comm_sock, &my_move, sizeof(my_move)) < 0)  {
	return false;
    }
    return true;
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
