/*
 * avatartest.c
 * Name: Benjamin Littlejohn
 * Team: core_dumped_in_a_maze
 * Date: August 2016
 * Purpose: Test for avatar.c module of cs50 X16 project Maze Challenge
 */

/***************** Return Statuses ******************/
#define SUCCESS (0)
#define FAILED (1)
#define UNCLEAR (2)
#define MALLOC_ERROR (3)
#define SOCKET_NOT_CREATED (4)
#define UNKNOWN_HOST (5)
#define FAILED_CONNECTION (6)
#define MESSAGE_FAILED_TO_BE_WRITTEN (7)
#define AVATAR_NOT_CREATED (8)

/********************* includes *********************/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "amazing.h"
#include "mazestruct.h"
#include "avatar.h"


/******************* local structs *****************/
typedef struct thread_data {
    int id;
    int maze_port;
    char *host_name;
} thread_data_t;

/************ function prototypes ************/
int generate_avatars(int num_avatars, int maze_port, char *host_name);
void *avatar_thread(void *params);
int avatar(int avatar_id, mazestruct_t *maze, int maze_port, char *hostname);
static Avatar *initialize_avatar(int id);
static int connect_to_server(int *comm_sock, char *hostname, int maze_port);
static int send_am_avatar_ready(Avatar *my_avatar, AM_Message *msg_buff,
	int comm_sock);
void wait_for_response(int comm_sock, AM_Message *msg_buff);
// static int solve_maze(Avatar *my_avatar, mazestruct_t *maze, int comm_sock,
// 	AM_Message *msg_buff);

/***************** global variables ****************/
//thread_mutex_t my_turn;
mazestruct_t *maze;

/**************** generate_avatars ****************/
int generate_avatars(int num_avatars, int maze_port, char *host_name) {
  void *thread_status;
  pthread_t avatars[AM_MAX_AVATAR];

    // generate params to pass into each thread
    thread_data_t params;
    params.maze_port = maze_port;
    params.host_name = host_name;

    for (int i = 0; i < num_avatars; i++) {
	     params.id = i;
	      pthread_create(&avatars[i], NULL, avatar_thread, &params);
	      if (avatars[i] != 0) {
	         return AVATAR_NOT_CREATED;
	      }
    }
    for (int i = 0; i < num_avatars; i++) {
	     pthread_join(avatars[i], &thread_status);
    }
    return SUCCESS;
}

/**************** avatar_thread ****************/
void *avatar_thread(void *params) {
  thread_data_t *thread_data = (thread_data_t *)params;

  int ret_status = avatar(thread_data->id, maze,
	  thread_data->maze_port, thread_data->host_name);

  return((void *)ret_status);
}

/**************** avatar ****************/
int avatar(int avatar_id, mazestruct_t *maze, int maze_port,
	char *hostname) {

    //declarations
    int ret_status;
    int comm_sock;

    //initialize needed structs
    AM_Message *msg_buff = malloc(sizeof(AM_Message));
    Avatar *my_avatar = initialize_avatar(avatar_id);

    if (msg_buff == NULL || my_avatar == NULL) {
	     perror("Malloc error. Avatartest couldn't be run.\n");
	      ret_status = MALLOC_ERROR;
    }
    else if ((ret_status = connect_to_server(&comm_sock, hostname, maze_port)
		!= SUCCESS)) {} //jump to clean up and exit
    else if ((ret_status = send_am_avatar_ready(my_avatar, msg_buff, comm_sock)
		!= SUCCESS)) {
	     close(comm_sock); //then jump to clean up and exit
    }
    else {
      wait_for_response(comm_sock, msg_buff);
      if(ntohl(msg_buff->type) == AM_AVATAR_TURN) {
        printf("It's your turn %d\n", (ntohl((msg_buff->avatar_turn).TurnId)));
      } else {
        printf("Message type is: %d\n", ntohl(msg_buff->type));
      }
      //	ret_status = solve_maze(my_avatar, maze, comm_sock);
	     close(comm_sock);
    }

    // clean up
    close(comm_sock);
    free(my_avatar);
    free(msg_buff);
    free(maze);

    return ret_status;
}

/*
 * Creates a new avatar struct with the given id
 */
static Avatar *initialize_avatar(int id) {
    Avatar *avatar = malloc(sizeof(Avatar));
    if (avatar == NULL) {
	return NULL;
    }
    else {
	     avatar->fd = id;
	      return avatar;
    }
 }

/*
 * Trys to connect to the server specified by hostname at port maze_port
 * 	using socket at address of comm_sock
 *
 * If successful then the given socket is properly connected to the given
 * 	server
 *
 * Returns 0 if the operation was successful and positive integer error codes
 * 	if not as well as prints error messages
 *
 * See return statuses in head-of-file comments for details
 */
static int connect_to_server(int *comm_sock, char *hostname, int maze_port) {

    //initialize necessary args
    *comm_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct hostent *hostp = gethostbyname(hostname);
    struct sockaddr_in server;

    //make sure initializations worked
    if (*comm_sock < 0) {
	     perror("Error creating communication socket.\n");
	      return SOCKET_NOT_CREATED;
    }
    else if (hostp == NULL) {
	     fprintf(stderr, "%s: unknown host\n", hostname);
	     return UNKNOWN_HOST;
    }

    //initialize server
    server.sin_family = AF_INET;
    server.sin_port = htons(maze_port);
    memcpy(&server.sin_addr, hostp->h_addr_list[0], hostp->h_length);

    //try to connect to server
    if (connect(*comm_sock,(struct sockaddr*)&server, sizeof(server)) < 0) {
    	perror("Connection to server failed.\n");
    	return FAILED_CONNECTION;
    }
    printf("Connected to server.\n");
    return SUCCESS;
}

/*
 * Sends the AM_AVATAR_READY message using the given comm_sock and msg_buff
 */
static int send_am_avatar_ready(Avatar *my_avatar, AM_Message *msg_buff,
	int comm_sock) {

    //form AM_AVATAR_READY message to send to server
    msg_buff->type = htonl(AM_AVATAR_READY);
    msg_buff->avatar_ready.AvatarId = htonl(my_avatar->fd);

    //send message to server
    if (write(comm_sock, msg_buff, sizeof(AM_Message)) < 0) {
	     fprintf(stderr, "Error writing avatar %d's move to server.\n",
		     my_avatar->fd);
         return MESSAGE_FAILED_TO_BE_WRITTEN;
    }
    printf("Avatar %d sent AM_AVATAR_READY.\n", my_avatar->fd);
    return SUCCESS;
}

/*
 * Method run by threads once everything is set up to solve the maze
 */
/*
static int solve_maze(Avatar *my_avatar, mazestruct_t *maze, int comm_sock,
	AM_Message *msg_buff) {
    int ret_status;

    wait_for_response(comm_sock, msg_buff);

    //try to solve the maze
    while (msg_buff->type == AM_AVATAR_TURN) {
	//update avatar position based on response
	my_avatar->pos = ntohl((msg_buff->avatar_turn).Pos[my_avatar->fd]);
	//if it is your turn...
	if (ntohl((msg_buff->avatar_turn).TurnId) == my_avatar->fd) {
	    //pthread_mutex_lock(&my_turn); //make sure no one else can go
	    make_move(maze, my_avatar, comm_sock, msg_buff);
	    //pthread_mutex_unlock(&my_turn); //allow next person to go
	}
	else {
	    wait_for_response(comm_sock, msg_buff);
	}
    }

    // once loop is ended, either maze has been solved or an error occured
    switch (msg_buff->type) {
	case (AM_MAZE_SOLVED):
	    printf("The maze was solved!!!!\n");
	    ret_status = SUCCESS;
	    break;
	default:
	    if (IS_AM_ERROR(msg_buff->type)) {
		fprintf(stderr, "Error %d.\n", msg_buff->type);
		ret_status = FAILED;
	    }
	    else {
		perror("Who knows what happened.\n");
		ret_status = UNCLEAR;
	    }
    }
    return ret_status;
}
*/


void wait_for_response(int comm_sock, AM_Message *msg_buff) {
    if ((read(comm_sock, msg_buff, sizeof(AM_Message))) < 0) {
	     perror("Error reading message from server.\n");
    }
}
