/*
* thread_ops.h header file for the threads module
*
*
* Larissa Chen, July 2016
* Team: core_dumped_in_a_maze
*/
#include "amazing.h"
#include "mazestruct.h"
#include "avatar.h"

#ifndef __THREAD_OPS_H
#define __THREAD_OPS_H

/**************** global structs ******************/
typedef struct thread_data {
    int id;
    int maze_port;
    char *host_name;
} thread_data_t;

/**************** generate_avatars ****************/
/*
 * Entry point function in generating the appropriate number of avatars by
 * initiating each thread responsible for each avatars communication to the server
 * Returns AVATAR_NOT_CREATED if there was an error creating an avatar's thread
 * Returns SUCCESS if each avatar's thread is successfully created.
 */
//int generate_avatars(int num_avatars, int maze_port, char *host_name);

/**************** avatar_thread ****************/
/*
 * Helper function that calls main function for each thread. Maps params struct
 * to individual parameters to pass into main function "avatar".
 * Returns error or success code from "avatar".
 */
void *avatar_thread(void *params);

#endif
