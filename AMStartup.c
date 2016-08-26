/*
 * AMStartup.c - the client startup script that makes first initial interaction
 * with server specifying parameters to setup maze game.
 *
 * exit codes: 1 - invalid arguments
 *             2 - error in socket connections
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
#include "amazing.h"
#include "thread_ops.h"

/******************** function prototypes ******************/
int send_init_message(int n_avatars, int difficulty, int comm_sock, struct sockaddr_in server);
int recv_init_response(int comm_sock, AM_Message *init_response);

/******************** global variables **********************/
mazestruct_t *maze;
pthread_mutex_t my_turn;
bool avatars_unite = false;

/*
 *  Main
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
          exit(1);
        }
        n_flag = true;
        break;
      case 'd':
        if ((sscanf(optarg, "%d", &d)) != 1) {
          printf("Usage Error: difficulty argument needs to be an int\n");
          exit(1);
        }
        d_flag = true;
        break;
      case 'h':
        hostname = optarg;
        host_flag = true;
        break;
      case '?':
        exit(1);
        break;
    }
  }

  if (!n_flag) {
    printf("Usage Error: Missing nAvatars argument: [-n nAvatars]\n");
    exit(1);
  }
  if (!d_flag) {
    printf("Usage Error: Missing difficulty argument: [-d difficulty]\n");
    exit(1);
  }
  if (!host_flag) {
    printf("Usage Error: Missing hostname argument: [-h hostname]\n");
    exit(1);
  }

  // 2. Setup initial connection w/ server
  comm_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (comm_sock < 0) {
    perror("opening socket");
    exit(2);
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(AM_SERVER_PORT));
  hostp = gethostbyname(hostname);
  if (hostp == NULL) {
    fprintf(stderr, "%s: unknown host '%s'\n", program, hostname);
    exit(3);
  }
  memcpy(&server.sin_addr, hostp->h_addr_list[0], hostp->h_length);

  // 3. Send initial message to server w/ maze specifics
  error = send_init_message(n, d, comm_sock, server);
  if (error) {
    exit (4);
  }

  // 4. Read initial response from server, reading port, and maze dimensions
  error = recv_init_response(comm_sock, &init_response);
  if (error) {
    exit(4);
  }
  maze_port = ntohl(init_response.init_ok.MazePort);
  maze_width = ntohl(init_response.init_ok.MazeWidth);
  maze_height = ntohl(init_response.init_ok.MazeHeight);
  printf("Port: %d, width: %d, height: %d\n", maze_port, maze_width, maze_height);

  // 5. Create log file
  const int len = 400; //fix this
  char filename[len];
  snprintf(filename, len, "Amazing_%s_%d_%d.log", getenv("USER"), n, d);
  logfile = fopen(filename, "w");
  time(&date);
  fprintf(logfile, "%s, 10829, %s*************************\n", getenv("USER"), ctime(&date));

  //stuff needed to create and run threads
  int kk = 1;
  void *thread_status = &kk;
  pthread_t avatars[AM_MAX_AVATAR];
  thread_data_t *params;
  if (pthread_mutex_init(&my_turn, NULL) != 0) {
      perror("Mutex creation failed.\n");
      close(comm_sock);
      exit(5);
  }
  else if ((maze = maze_new(maze_height, maze_width, n, logfile)) == NULL) {
      perror("Maze could not be created.\n");
      close(comm_sock);
      exit(6);
  }
  else {
  // error = generate_avatars(n, maze_port, hostname);
  // 6. Start the avatar threads
    for (int i = 0; i < n; i++) {

    // generate params to pass into each thread
      params = malloc(sizeof(thread_data_t));
      params->maze_port = maze_port;
      params->host_name = hostname;
      params->id = i;

      if (pthread_create(&avatars[i], NULL, avatar_thread, params) != 0) {
	  fprintf(stderr, "Thread for avatar %d could not be created.\n", i);
        //return AVATAR_NOT_CREATED;
      }
    }
      for (int i = 0; i < n; i++) {
	  pthread_join(avatars[i], &thread_status);
      }
  }


  //check for return status from threads
  switch(*(int *)thread_status) {
     case 0 :
        printf("Thread status: 0 Success!\n");
        break;
     case 1 :
        printf("Thread status: 1 Failure!\n");
        break;
     default :
       printf("Unknown thread status\n");
  }

  //delete maze
  fclose(logfile);
  delete_maze(maze);
  //free the params
  close(comm_sock);
  exit(0);
}

/************ Defined Functions ***************/
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
    return 1;
  }
  return 0;
}

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
      return 1;
    }
  }
  return 0;
}
