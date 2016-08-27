/*
 * AMStartup.c - the client startup script that makes first initial interaction
 * with server specifying parameters to setup maze game and then initiates threads
 * calling functions from thread_ops module.
 *
 * inputs:
 *  1. -n [nAvatars] - must be int that lies in range of server's specifications
 *  2. -d [difficulty] - must be int that lies in range of server's specifications
 *  3. -h [hostname] - valid host name that sets up maze
 *
 * exit codes: 0 - success
 *            1 - failed
 *            2 - unclear
 *            3 - malloc error
 *            4 - unknown host
 *            5 - failed connection
 *            6 - reading or writing to server failure
 *            7 - avatar not created
 *            8 - invalid number of avatars
 *            9 - invalid difficulty number
 *            10 - missing argument or invalid option argument
 *
 * Larissa Chen, August 2016
 * Team: core_dumped_in_a_maze
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <netdb.h>
#include <pthread.h>
#include <time.h>
#include "lib/amazing.h"
#include "thread_ops/thread_ops.h"

/******************** function prototypes ******************/
int send_init_message(int n_avatars, int difficulty, int comm_sock, struct sockaddr_in server);
int recv_init_response(int comm_sock, AM_Message *init_response);

/******************** global variables **********************/
mazestruct_t *maze;
pthread_mutex_t my_turn;
bool avatars_unite = false;

/*
 * Main entry point function:
 * 1. Validates input arguments, checks if there are the right number of inputs
 *      and are of correct type
 * 2. Makes initial connection with server
 * 3. Send initial message to server: number of avatars and maze difficulty
 * 4. Reads initial response from server, extracting the maze port
 * 5. Initializes log file and new maze struct
 * 6. Sets up nAvatar number of threads
 * 7. Pass control to thread_ops to handle each avatar thread and solve the maze
 * 8. Parse exit codes from thread_ops
 * 9. Clean up
 */
int main (int argc, char* argv[]) {
  char opt;
  int n, d;
  bool n_flag, d_flag, host_flag;
  char *hostname;
  char *program;
  int comm_sock;
  struct sockaddr_in server;
  struct hostent *hostp;
  int error;
  AM_Message init_response;
  int maze_port;
  int maze_width, maze_height;
  FILE *logfile;
  time_t date;


  // 1. Validate and parse arguments
  program = argv[0];
  while ((opt = getopt(argc, argv, "n:d:h:")) != -1) {
    switch (opt)
    {
      case 'n':
        if ((sscanf(optarg, "%d", &n)) != 1) {
          printf("Usage Error: n_avatars argument needs to be an int\n");
          exit(8);
        }
        n_flag = true;
        break;
      case 'd':
        if ((sscanf(optarg, "%d", &d)) != 1) {
          printf("Usage Error: difficulty argument needs to be an int\n");
          exit(9);
        }
        d_flag = true;
        break;
      case 'h':
        hostname = optarg;
        host_flag = true;
        break;
      case '?':
        exit(10);
        break;
    }
  }

  if (!n_flag) {
    printf("Usage Error: Missing nAvatars argument: [-n nAvatars]\n");
    exit(10);
  }
  if (!d_flag) {
    printf("Usage Error: Missing difficulty argument: [-d difficulty]\n");
    exit(10);
  }
  if (!host_flag) {
    printf("Usage Error: Missing hostname argument: [-h hostname]\n");
    exit(10);
  }

  // 2. Setup initial connection w/ server
  comm_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (comm_sock < 0) {
    perror("opening socket");
    exit(2);
  }
  // setup server
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(AM_SERVER_PORT));
  hostp = gethostbyname(hostname);
  if (hostp == NULL) {
    fprintf(stderr, "%s: unknown host '%s'\n", program, hostname);
    exit(4);
  }
  memcpy(&server.sin_addr, hostp->h_addr_list[0], hostp->h_length);

  // 3. Send initial message to server w/ maze specifics
  error = send_init_message(n, d, comm_sock, server);
  if (error) {
    if (error == 1)
      exit(5);
    else
      exit (6);
  }

  // 4. Read initial response from server, reading port, and maze dimensions
  error = recv_init_response(comm_sock, &init_response);
  if (error) {
    if (error == AM_INIT_TOO_MANY_AVATARS)
      exit(8);
    else if (error == AM_INIT_BAD_DIFFICULTY)
      exit(9);
    else  // error in reading from socket
      exit(6);
  }
  // parse response message, extract maze port, width, and height
  maze_port = ntohl(init_response.init_ok.MazePort);
  maze_width = ntohl(init_response.init_ok.MazeWidth);
  maze_height = ntohl(init_response.init_ok.MazeHeight);

  // 5. Create log file and print the first line 
  const int len = 400; // fix this
  char filename[len];
  snprintf(filename, len, "Amazing_%s_%d_%d.log", getenv("USER"), n, d);
  logfile = fopen(filename, "w");
  time(&date);
  fprintf(logfile, "%s, %d, %s*************************\n", getenv("USER"), maze_port,ctime(&date));

  // initialization before creating threads
  int kk = 1;
  void *thread_status = &kk;
  pthread_t avatars[AM_MAX_AVATAR];
  thread_data_t *params;
  if (pthread_mutex_init(&my_turn, NULL) != 0) {
      perror("Mutex creation failed.\n");
      close(comm_sock);
      exit(5);
  }
  // initialize maze struct
  else if ((maze = maze_new(maze_height, maze_width, n, logfile)) == NULL) {
      perror("Maze could not be created.\n");
      close(comm_sock);
      exit(6);
  }
  else {

// 6. Start the avatar threads
    for (int i = 0; i < n; i++) {

    // generate params to pass into each thread
      params = malloc(sizeof(thread_data_t));
      params->maze_port = maze_port;
      params->host_name = hostname;
      params->id = i;

  // 7. Pass control to thread_ops module through function avatar_thread
      if (pthread_create(&avatars[i], NULL, avatar_thread, params) != 0) {
	       fprintf(stderr, "Thread for avatar %d could not be created.\n", i);
         exit(7);
      }
    }
      for (int i = 0; i < n; i++) {
	       pthread_join(avatars[i], thread_status);
      }
  }

  // 8. Parse exit codes from thread_ops
  switch(*(int *)thread_status) {
     case 0 :
        printf("Thread status: 0 Success!\n");
        exit(0);
        break;
     case 1 :
        printf("Thread status: 1 Failure!\n");
        exit(1);
        break;
     case 2 :
      exit(3);
      break;
     case 3 :
      exit(3);
      break;
     case 4 :
      exit(4);
      break;
     case 5 :
      exit(5);
      break;
     case 6 :
      exit(6);
      break;
     case 7 :
      exit (7);
     case 8 :
      exit(8);
     default :
       printf("Unknown return status from threads\n");
  }

  // 9. Clean up
  if (thread_status != NULL)
    free(thread_status);
  fclose(logfile);
  delete_maze(maze);
  close(comm_sock);
  exit(0);
}

/************ User-defined Functions ***************/

/*
* send_init_message: asks server to create maze with difficulty and n_avatars,
* sets up maze-initialization message and writes it to server
*
* @n_avatars: int representing number of avatars
* @difficulty: difficulty of maze, must be an int
* @comm_sock
* @server: struct sockaddr_in representing the server, members are already populated
*
* Returns 0 on success, 1 if error in connected to stream socket, and 2 if error
* in writing to socket
*/
int send_init_message(int n_avatars, int difficulty, int comm_sock, struct sockaddr_in server) {
  AM_Message init_message;
  if (connect(comm_sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
    perror("connecting stream socket");
    return 1;
  }
  printf("Connected!\n");

    // form initial message to send to server
  init_message.type = htonl(AM_INIT);
  init_message.init.nAvatars = htonl(n_avatars);
  init_message.init.Difficulty = htonl(difficulty);
  if (write(comm_sock, &init_message, sizeof(init_message)) < 0)  {
    perror("writing on stream socket");
    return 2;
  }
  return 0;
}

/*
* recv_init_response: reads response from server which indicates maze dimensions
* and which port the maze is set up on the server.
*
* @comm_sock
* @init_response: pointer to store the response message in, allocated but empty
*
* Returns 0 on success otherwise returns error
*/
int recv_init_response(int comm_sock, AM_Message *init_response) {
  int bytes_read;

  if ((bytes_read = read(comm_sock, init_response, sizeof(AM_Message))) < 0)
    perror("reading stream message");
  else if (bytes_read == 0) {
    printf("Ending connection\n");
    return 1;
  }
  else  {
    if (IS_AM_ERROR(ntohl(init_response->type))) {
      int error = ntohl(init_response->init_failed.ErrNum);
      if (error == AM_INIT_TOO_MANY_AVATARS) {
        printf("Usage Error: Too many avatars specified as arugment\n");
      }
      if (error == AM_INIT_BAD_DIFFICULTY) {
        printf("Usage Error: Difficulty argument needs to be between 0 and 9 inclusive\n");
      }
      return error;
    }
  }
  return 0;
}
