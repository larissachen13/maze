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
#include <unistd.h>
#include <pthread.h>
#include "amazing.h"
#include "mazestruct.h"
#include "avatar.h"

/******************* local structs *****************/
typedef struct thread_data {
    int avatar_id;
    int maze_port;
    char *host_name;
} thread_data_t

/***************** global variables ****************/
thread_mutex_t my_turn = PTHREAD_MUTEX_INITIALIZER;
mazestruct_t maze = maze_new();

int main(int argc, char *argv[]) {
    //still need to fill in
    return SUCCESS;
}

/**************** generate_avatars ****************/
int generate_avatars(int num_avatars, int maze_port, char *host_name) {
  void *thread_status;
  pthread_t avatars[AM_MAX_AVATAR];

  // create each avatar's thread with avatar_thread function and its own params
  for (int i = 0; i < num_avatars, i++) {
    // generate params to pass into each thread
    thread_data_t params;
    params.avatar_id = i;
    params.maze_port = maze_port;
    params.host_name = host_name;
    if(pthread_create(&avatars[i], NULL, avatar_thread, params) < 0) {
      return AVATAR_NOT_CREATED;
    }
  }

  for (i = 0; i < num_avatars, i++) {
     pthread_join(&avatars[i], &thread_status);
  }
  return SUCCESS;
}

/**************** avatar_thread ****************/
void *avatar_thread(void *params) {

    thread_data_t *thread_data = (thread_data_t *)params;

    int ret_status = avatar(thread_data->avatar_id, maze,
	    thread_data->maze_port, thread_data->hostname);

    return((void *)ret_status));
}


/**************** avatar ****************/
int avatar(int avatar_id, mazestruct_t *maze, int maze_port,
	char *hostname) {

    //declarations
    int ret_status;
    int comm_sock;

    //initialize needed structs
    AM_message *msg_buff = malloc(sizeof(AM_message));
    Avatar *my_avatar = initialize_avatar(avatar_id);

    if (msg_buff == NULL || my_avatar == NULL) {
	     perror("Malloc error. Avatartest couldn't be run.\n");
	      ret_status = MALLOC_ERROR;
    }
    else if ((ret_status = connect_to_server(comm_sock, hostname, maze_port)
		!= SUCCESS)) {} //jump to clean up and exit
    else if ((ret_status = send_am_ready(my_avatar, msg_buff, comm_sock)
		!= SUCCESS)) {
	     close(comm_sock); //then jump to clean up and exit
    }
    else {
      int bytes_read;
      AM_Message turnReady;
      if ((bytes_read = read(comm_sock, &turnReady, sizeof(AM_Message))) < 0)
        perror("reading stream message");
      else if (bytes_read == 0) {
        printf("Ending connection\n");
        return 1;
      }
      else {
        if (ntohl(init_response->type) == AM_AVATAR_TURN) {
          printf("It's your turn %d\n", avatar_id);
        }
        else {
          printf("Code is %d\n", ntohl(init_response->type)); 
        }
      }
	    //  ret_status = solve_maze(my_avatar, maze, comm_sock);
	     close(comm_sock);
    }

    // clean up
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
    *comm_sock = socket(AF_INIT, SOCK_STREAM, 0);
    struct hostent *hostp = gethostbyname(hostname);
    struct sockaddr_in server;

    //make sure initializations worked
    if (*comm_sock < 0) {
	perror("Error creating communication socket.\n");
	return SOCKET_NOT_CREATED;
    }
    else if (hostp == NULL) {
	fprintf(stderr, "%s: unknown host '%s'\n", program, hostname);
	return UNKNOWN_HOST;
    }

    //initialize server
    server.sin_family = AF_INET;
    server.sin_port = maze_port;
    memcpy(&server.sin_addr, hostp->h_addr_list[0], hostp->h_length);

    //try to connect to server
    if (connect(*comm_sock,(struct sockaddr*)&server, sizeof(server)) < 0) {
    	perror("Connection to server failed.\n");
    	ret_status = FAILED_CONNECTION;
    }
    return SUCCESS;
}

/*
 * Sends the AM_AVATAR_READY message using the given comm_sock and msg_buff
 */
static int send_am_avatar_ready(Avatar *my_avatar, AM_message *msg_buff,
	int comm_sock) {

    //form AM_AVATAR_READY message to send to server
    msg_buff->type = htonl(AM_AVATAR_READY);
    msg_buff->AvatarID = htonl(my_avatar->fd);

    if (write(comm_sock, msg_buff, sizeof(AM_message)) < 0)  {
      perror("Error sending AM_AVATAR_READY\n");
      return MESSAGE_FAILED_TO_BE_WRITTEN;
    }
    return SUCCESS;

    // //send message to server
    // if (write(comm_sock, &my_move, sizeof(my_move)) < 0)  {
	  //    perror("Error writing avatar %d's move to server.\n", avatar->id);
	  //     return MESSAGE_FAILED_TO_BE_WRITTEN;
    // }
    // return SUCCESS;
}

/*
 * Method run by threads once everything is set up to solve the maze
 */
// static int solve_maze(Avatar *my_avatar, mazestruct_t *maze, int comm_sock){
//
//     wait_for_response(comm_sock, msg_buff);
//
//     //try to solve the maze
//     while (msg_buff->type == AM_AVATAR_TURN) {
// 	//update avatar position based on response
// 	my_avatar->pos = ntohl(msg_buff->Pos[my_avatar->fd]);
// 	//if it is your turn...
// 	if (ntohl(msg_buff->id) == my_avatar->id) {
// 	    pthread_mutex_lock(&my_turn); //make sure no one else can go
// 	    make_move(maze, my_avatar, comm_sock, server);
// 	    pthread_mutex_unlock(&my_turn); //allow next person to go
// 	}
// 	else {
// 	    wait_for_response(comm_sock, msg_buff);
// 	}
//     }
//
//     // once loop is ended, either maze has been solved or an error occured
//     switch (msg_buff->type) {
// 	case (AM_MAZE_SOLVED):
// 	    printf("The maze was solved!!!!\n");
// 	    ret_status = SUCCESS;
// 	    break;
// 	default:
// 	    if (IS_AM_ERROR(msg_buff)) {
// 		perror("Error %d.\n", msg_buff->type);
// 		ret_status = FAILED;
// 	    }
// 	    else {
// 		perror("Who knows what happened.\n");
// 		ret_status = UNCLEAR;
// 	    }
//     }
//     return ret_status;
// }
