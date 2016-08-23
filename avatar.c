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
#include <netdb.h>
#include <unistd.h>
#include "avatar.h"

/******************** global variables **********************/
extern bool avatars_unite;

/*********************** local function prototypes ************************/
static void get_best_move(mazestruct_t *maze, Avatar *avatar, 
	avatar_move *best_move);
static void get_best_move_helper(mazestruct_t *maze, Avatar *avatar,
	avatar_move *best_move);
static void update_maze(mazestruct_t *maze, XYPos old_pos, 
	avatar_move *move, Avatar *avatar);
static bool send_move(uint32_t avatar_id, uint32_t direction, int comm_sock);
static bool same_pos(XYPos old_pos, XYPos new_pos);
static void come_together(mazestruct_t * maze, Avatar *avatar, avatar_move *best_move);

/**************************** global functions *****************************/
/*
 * Method to move a given avatar
 * See avatar.h for full description
 */
void make_move(mazestruct_t *maze, Avatar *avatar, int comm_sock, 
	AM_Message *msg_buff) {

    avatar_move *move;
    
    if ((move = malloc(sizeof(avatar_move))) == NULL) {
	fprintf(stderr, "Avatar %d could not make move due to malloc error.\n",
		avatar->fd);
    }
    else {
	//save current position can check later if move was successful
	XYPos old_pos = avatar->pos;
	
	get_best_move(maze, avatar, move);
	sleep(1);
	if (!send_move(avatar->fd, move->direction, comm_sock)) {
	    fprintf(stderr, "Error writing avatar %d's move to server.\n", 
		avatar->fd);
	}
	
	wait_for_response(comm_sock, msg_buff);
	
	if (ntohl(msg_buff->type) == AM_AVATAR_TURN) {
	    //update avatar position based on response
	    avatar->pos.x = ntohl(msg_buff->avatar_turn.Pos[avatar->fd].x);
	    avatar->pos.y = ntohl(msg_buff->avatar_turn.Pos[avatar->fd].y);
	    update_maze(maze, old_pos, move, avatar);
	}
	free(move);
    }
}

/*
 * Waits until the server sends a message then reads it into the given
 * 	message buffer
 *
 * Caller is responsible for allocating memory for the message buffer and
 * 	freeing it
 */
void wait_for_response(int comm_sock, AM_Message *msg_buff) {
    if ((read(comm_sock, msg_buff, sizeof(AM_Message))) < 0) {
	     perror("Error reading message from server.\n");
    }
}

/**************************** local functions ******************************/
/*
 * Updates the shared map of the maze based on an avatar's last move, former
 * 	position, and current positon
 */
static void update_maze(mazestruct_t *maze, XYPos old_pos, avatar_move *move, 
	Avatar *avatar) {
    insert_last_move(maze, move->direction, move->score, avatar->fd);
    //if move failed add a wall to the maze at that spot
    if ((move->direction != M_NULL_MOVE) && same_pos(old_pos, avatar->pos)) {
	    insert_wall(maze, avatar->pos.x, avatar->pos.y, move->direction);
    }
    //otherwise...
    else {
	//update maze with new avatar position
	update_location(maze, old_pos.x, old_pos.y, avatar->pos.x,
		avatar->pos.y, avatar->fd);
	//if avatar was forced to backtrack it has reached a dead end
	if (move->score == HAVE_TO_BACK_TRACK && is_someone_adjacent(maze, 
		    avatar->pos.x, avatar->pos.y, 3 - move->direction == -1)) {
	    insert_dead_spot(maze, old_pos.x, old_pos.y);
	}
	//if avatar is now following a new avatar, update its leader and 
	//mark the new space as visited 
	else if (move->score == FIRST_PRIORITY && move->direction != 
		M_NULL_MOVE) {
	    avatar->leader = is_someone_adjacent(maze, old_pos.x, 
		    old_pos.y, move->direction);
	    remove_leader(maze, avatar->fd);
	    set_leader(maze, avatar->fd, avatar->leader);
	    visited_spot(maze, avatar->pos.x, avatar->pos.y, avatar->fd);
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
static void get_best_move(mazestruct_t *maze, Avatar *avatar, 
	avatar_move *best_move) {

    //if there is a common path already identified, unite
    if (avatars_unite) {
	come_together(maze, avatar, best_move);
    }
    //if you recognize that all avatar's have a common path
    else if ((get_number_leaders(maze) == 2) && have_paths_crossed(maze)) {
	avatars_unite = true;
	come_together(maze, avatar, best_move);
    }
    else {
	get_best_move_helper(maze, avatar, best_move);
    }
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
static void get_best_move_helper(mazestruct_t *maze, Avatar *avatar, 
	avatar_move *best_move) {

    best_move->score = -1; //stores score of best move so far
    int move_rank; //keeps score for the current move
    int adj_avatar;

    if (avatar->leader != avatar->fd) {
	best_move->direction = get_last_direction(maze, avatar->leader);
	best_move->score = get_last_score(maze, avatar->leader);
    }

    else {

    for (int direction = M_WEST; direction < M_NUM_DIRECTIONS; direction++) {
	/************* score the move *****************/
	//if the move results in running into a wall
	if (check_wall(maze, avatar->pos.x, avatar->pos.y, direction)) {
	    move_rank = DONT_DO_IT;
	}
	//if the move results in potentially meeting another avatar
	//needs editing!!!!
	else if ((adj_avatar = is_someone_adjacent(maze, avatar->pos.x, 
		    avatar->pos.y, direction)) != -1) {
	    if (avatar->fd < adj_avatar) {
		direction = M_NULL_MOVE;
	    }
	    move_rank = FIRST_PRIORITY;
	}
	//if the move results in potentially visiting an unvisited space
	else if(!is_visited(maze, avatar->pos.x, avatar->pos.y, direction)) {
	    move_rank = SECOND_PRIORITY;
	}
	//if the move results in potentially visiting a space visited by
	//a different avatar
	else if(!did_x_visit(maze, avatar->pos.x, avatar->pos.y, direction, 
		    avatar->fd)) {
	    move_rank = THIRD_PRIORITY;
	}
	//if the move results in visiting a space I have already visited
	else {
	    move_rank = HAVE_TO_BACK_TRACK;
	}
	//check how current move compares to the best_move
	if (move_rank > best_move->score) {
	    best_move->direction = direction;
	    best_move->score = move_rank;
	}
    }}
}

/*
 * Sends an avatar's move to the maze server
 * Returns true if the message was sent sucessfully and false otherwise
 */
static bool send_move(uint32_t avatar_id, uint32_t direction, int comm_sock) {

    //form AM_AVATAR_MOVE message to send to server
    AM_Message my_move;
    my_move.type = htonl(AM_AVATAR_MOVE);
    my_move.avatar_move.AvatarId = htonl(avatar_id);
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
static void come_together(mazestruct_t *maze, Avatar *avatar, avatar_move *best_move) {
    printf("Coming together!!!\n");
    best_move->score = -1; //stores score of best move so far
    int move_rank; //keeps score for the current move
    int adj_avatar;

    if (avatar->leader != avatar->fd) {
	best_move->direction = get_last_direction(maze, avatar->leader);
	best_move->score = get_last_score(maze, avatar->leader);
    }

    else {

    for (int direction = M_WEST; direction < M_NUM_DIRECTIONS; direction++) {
	/************* score the move *****************/
	//if the move results in running into a wall
	if (check_wall(maze, avatar->pos.x, avatar->pos.y, direction)) {
	    move_rank = DONT_DO_IT;
	}
	//if the move results in potentially meeting another avatar
	else if ((adj_avatar = is_someone_adjacent(maze, avatar->pos.x, 
		    avatar->pos.y, direction)) != -1) {
	    if (avatar->fd < adj_avatar) {
		direction = M_NULL_MOVE;
	    }
	    move_rank = FIRST_PRIORITY;
	}
	//if the move results in potentially visiting an unvisited space
	else if (!is_visited(maze, avatar->pos.x, avatar->pos.y, direction)) {
	    move_rank = DONT_DO_IT;
	}
	//if the move results in potentially visiting a space visited by
	//a different avatar
	else if(!did_x_visit(maze, avatar->pos.x, avatar->pos.y, direction, 
		    avatar->fd)) {
	    move_rank = FIRST_PRIORITY;
	}
	//if the move results in visiting a space I have already visited
	else {
	    move_rank = HAVE_TO_BACK_TRACK;
	}
	//check how current move compares to the best_move
	if (move_rank > best_move->score) {
	    best_move->direction = direction;
	    best_move->score = move_rank;
	}
    }}
    
}

