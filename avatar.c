/*
 * avatar.c
 * Name: Benjamin Littlejohn
 * Team: core_dumped_in_a_maze
 * Date: August 2016
 * Purpose: Module of cs50 X16 project Maze Challenge
 * See avatar.h for details
 */

/************* constants ***************/
#define FOLLOW_THE_LEADER (5)
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
static bool send_move(uint32_t avatar_id, uint32_t direction, int comm_sock);
static void update_maze(mazestruct_t *maze, XYPos old_pos, 
	avatar_move *move, Avatar *avatar);
static void come_together(mazestruct_t * maze, Avatar *avatar, 
	avatar_move *best_move);
static void get_best_move_helper(mazestruct_t *maze, Avatar *avatar,
	avatar_move *best_move);
static void get_true_backtrack_route(mazestruct_t *maze, Avatar *avatar, 
	avatar_move *move);
static void should_x_backtrack(mazestruct_t *maze, Avatar *avatar, 
	avatar_move *move);
static bool same_pos(XYPos old_pos, XYPos new_pos);
static void update_leader(mazestruct_t *maze, Avatar *avatar, XYPos old_pos, 
	int direction);

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
	
	//avatar->leader = get_leader(maze, avatar->fd);

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
 * Determines an avatar's next move using the shared maze knowledge of all the
 * 	avatars and the avatar's current position
 */
static void get_best_move(mazestruct_t *maze, Avatar *avatar, 
	avatar_move *best_move) {

    //if there is a common path already identified, unite
    if (avatars_unite) {
	come_together(maze, avatar, best_move);
    }
    else {
	get_best_move_helper(maze, avatar, best_move);
    }
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
 * Updates the shared map of the maze based on an avatar's last move, former
 * 	position, and current positon
 */
static void update_maze(mazestruct_t *maze, XYPos old_pos, avatar_move *move, 
	Avatar *avatar){

    //update last move so everyone knows
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
	if (move->score == HAVE_TO_BACK_TRACK && (is_someone_adjacent(maze, 
		    avatar->pos.x, avatar->pos.y, 3 - move->direction) == -1)) {
	    insert_dead_spot(maze, old_pos.x, old_pos.y);
	}

	//if avatar is now following a new avatar... 
	else if (move->score == FOLLOW_THE_LEADER && move->direction != 
		M_NULL_MOVE) {
	    update_leader(maze, avatar, old_pos, move->direction);
	    //mark the newly visited space and the fact paths have crossed
	    visited_spot(maze, avatar->pos.x, avatar->pos.y, avatar->fd);
	    cross_paths(avatar->fd, avatar->leader, maze);
	}

	//if you have successfully crossed paths with a new avatar
	else if (move->score == FIRST_PRIORITY) {
	    //find out who that avatar was
	    int adj_path = who_visited(maze, old_pos.x, old_pos.y, 
		    move->direction, avatar->fd);
	    if (adj_path != -1) {
		//alert everyone you have crossed paths with that avatar
		if (cross_paths(avatar->fd, adj_path, maze)) {
		    //if everyone has crossed paths then all avatars can meet
		    avatars_unite = true;
		}
	    }
	    //update visited list
	    visited_spot(maze, avatar->pos.x, avatar->pos.y, avatar->fd);
	}

	//otherwise just update the visited list as avatar extends path
	else {
	    visited_spot(maze, avatar->pos.x, avatar->pos.y, avatar->fd);
	}
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
    int num_bt_routes = 0; //in a perfect maze, there is only 1 backtrack route

    for (int direction = M_WEST; direction < M_NUM_DIRECTIONS; direction++) {
	/***************** score the move *****************/
	//if the move results in running into a wall
	if (check_wall(maze, avatar->pos.x, avatar->pos.y, direction)) {
	    move_rank = DONT_DO_IT;
	}
	//if the move results in potentially visiting a space visited by an 
	//avatar I haven't crossed paths with
	else if ((who_visited(maze, avatar->pos.x, avatar->pos.y, direction, 
			avatar->fd)) != -1) {
	    move_rank = FIRST_PRIORITY;
	}
	//if the move results in potentially visiting an unvisited space
	else if(!is_visited(maze, avatar->pos.x, avatar->pos.y, direction)) {
	    move_rank = SECOND_PRIORITY;
	}
	//if the move results in potentially visiting a space visited by
	//a different avatar I have crossed paths with
	else if(!did_x_visit(maze, avatar->pos.x, avatar->pos.y, direction, 
		    avatar->fd)) {
	    move_rank = THIRD_PRIORITY;
	}
	//if the move results in visiting a space I have already visited
	else {
	    move_rank = HAVE_TO_BACK_TRACK;
	    num_bt_routes++;
	}
	//check how current move compares to the best_move
	if (move_rank > best_move->score) {
	    best_move->direction = direction;
	    best_move->score = move_rank;
	}
    }
    /******************* Sanity checks ********************/
    //if there is more than one potential backtrack route, only one is valid
    if (num_bt_routes > 1) {
	//update the maze and your move so that they reflect only the valid one
	get_true_backtrack_route(maze, avatar, best_move);
    }
    //last minute check if another avatar has just backtracked to or from my 
    //best move destination; updates the move accordingly
    should_x_backtrack(maze, avatar, best_move);
}

/*
 * Decision making process run by avatars once all paths have crossed
 *
 * Priorities change so that rather than explore, avatar's follow the trail of visited
 * 	spots until they all come together
 *
 * See avatar.h for details
 */
static void come_together(mazestruct_t *maze, Avatar *avatar, 
	avatar_move *best_move) {
    int move_rank; //keeps score for the current move
    int adj_avatar; //neighboring avatar

    //if you are following someone else; just copy their last move
    if (avatar->leader != avatar->fd) {
	best_move->direction = get_last_direction(maze, avatar->leader);
	best_move->score = get_last_score(maze, avatar->leader);
	return;
    }
    //otherwise... 
    for (int direction = M_WEST; direction < M_NUM_DIRECTIONS; direction++) {
	/**************** score the move *****************/
	//if the move results in running into a wall
	if (check_wall(maze, avatar->pos.x, avatar->pos.y, direction)) {
	    move_rank = DONT_DO_IT;
	}
	//if the move results in potentially visiting an unvisited space
	else if (!is_visited(maze, avatar->pos.x, avatar->pos.y, direction)) {
	    move_rank = DONT_DO_IT;
	}
	//if the move results in potentially meeting another avatar
	else if ((adj_avatar = is_someone_adjacent(maze, avatar->pos.x, 
		    avatar->pos.y, direction)) != -1) {
	    //lowest number avatar id will be the leader
	    if (avatar->fd < adj_avatar) {
		direction = M_NULL_MOVE;
	    }
	    move_rank = FOLLOW_THE_LEADER;
	}
	//if the move results in potentially visiting a space visited by
	//a different avatar
	else if(!did_x_visit(maze, avatar->pos.x, avatar->pos.y, direction, 
		    avatar->fd)) {
	    move_rank = SECOND_PRIORITY;
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
    }
}

/*
 * In a perfect maze, there should only ever be one valid backtrack route
 *
 * Determines which route is an avatar's actual backtrack route by looking at
 * 	the avatar's last move which brought them to the current spot
 */
static void get_true_backtrack_route(mazestruct_t *maze, Avatar *avatar, 
	avatar_move *move) {

    //get the direction avatar came from; it is the actual backtrack route
    int true_bt_route = 3 - get_last_direction(maze, avatar->fd);
    //get all the directions you scored as HAVE_TO_BACK_TRACK
    for (int direction = M_WEST; direction < M_NUM_DIRECTIONS; direction++) {
	//if it is not the actual backtrack direction
	if (did_x_visit(maze, avatar->pos.x, avatar->pos.y, direction, 
		    avatar->fd) && direction != true_bt_route) {
	    //there is a wall in that direction
	    insert_wall(maze, avatar->pos.x, avatar->pos.y, direction);
	}
    }
    //if you HAVE_TO_BACK_TRACK, pick the actual backtrack direction
    if (move->score == HAVE_TO_BACK_TRACK) {
	move->direction = true_bt_route;
    }
}

/*
 * Checks if an adjacent avatar has visited more of the current region and thus
 * 	has better information about a given avatar's current best move
 *
 * If an adjacent avatar knows that the given avatar's move will lead to a dead
 * 	end the given avatar's move is updated accordingly
 *
 * If an adjacent avatar is awaree that the current avatar is backtracking from
 * 	a dead end the given avatar's move is updated accordingly
 */
static void should_x_backtrack(mazestruct_t *maze, Avatar *avatar, 
	avatar_move *move) {

    //variable declarations
    int adj_avatar;
    int adj_avatar_move;
    int adj_avatar_score;

    //see if there is an avatar on my destination
    if ((adj_avatar = is_someone_adjacent(maze, avatar->pos.x, avatar->pos.y, 
		    move->direction)) != -1) {
	adj_avatar_move = get_last_direction(maze, adj_avatar);
	adj_avatar_score = get_last_score(maze, adj_avatar);
	//if that avatar just backtracked from my current position, 
	if ((adj_avatar_move == move->direction) && (adj_avatar_score == 
		HAVE_TO_BACK_TRACK)) {
	    //backtrack with them
	    move->direction = adj_avatar_move;
	    move->score = HAVE_TO_BACK_TRACK;
	}
    }
    //see if there is an avatar at my current position
    else if ((adj_avatar = is_someone_here(maze, avatar->pos.x, 
		avatar->pos.y, avatar->fd)) != -1) {
	printf("we are on the same space\n");
	adj_avatar_move = get_last_direction(maze, adj_avatar);
	adj_avatar_score = get_last_score(maze, adj_avatar);
	//if they just backtracked from my current destination 
	if ((adj_avatar_move == (3 - move->direction)) && (adj_avatar_score == 
		HAVE_TO_BACK_TRACK)) {
	    //wait and follow them out of dead end on next turn
	    move->direction = M_NULL_MOVE;
	    move->score = FOLLOW_THE_LEADER;
	}
    }
}

/*
 * Returns true if the old_pos is equal to the new_pos
 */
static bool same_pos(XYPos old_pos, XYPos new_pos) {
    return ((old_pos.x == new_pos.x) && (old_pos.y == new_pos.y));
}

/*
 * Updates an avatar's leader and the appropriate statistics in the maze so
 * 	other avatars are aware
 */
static void update_leader(mazestruct_t *maze, Avatar *avatar, XYPos old_pos,
	int direction) {
    //update avatar's leader
    avatar->leader = is_someone_adjacent(maze, old_pos.x, old_pos.y, direction);
    //update leader statistics in maze 
    remove_leader(maze, avatar->fd);
    set_leader(maze, avatar->fd, avatar->leader);
}

