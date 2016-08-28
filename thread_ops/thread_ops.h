/*
* thread_ops.h header file for the threads module
* Threads module wraps all functionality of the avatar threads 
*
*
* Larissa Chen, August 2016
* Team: core_dumped_in_a_maze
*/
#include "../lib/amazing.h"
#include "../lib/mazestruct.h"
#include "../lib/avatar.h"

#ifndef __THREAD_OPS_H
#define __THREAD_OPS_H

/**************** global structs ******************/
typedef struct thread_data {
    int id;
    int maze_port;
    char *host_name;
    int *return_status;
} thread_data_t;

/**************** avatar_thread ****************/
/*
 * Helper function that calls main function for each thread. Maps params struct
 * to individual parameters to pass into main function "avatar".
 * Returns error or success code from "avatar".
 */
void *avatar_thread(void *params);

#endif
