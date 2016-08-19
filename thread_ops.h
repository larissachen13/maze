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

typedef struct thread_data thread_data_t;

/**************** generate_avatars ****************/
/*
 * Entry point function in generating the appropriate number of avatars by
 * initiating each thread responsible for each avatars communication to the server
 * Returns AVATAR_NOT_CREATED if there was an error creating an avatar's thread
 * Returns SUCCESS if each avatar's thread is successfully created.
 */
int generate_avatars(int num_avatars, int maze_port, char *host_name);

/**************** avatar_thread ****************/
/*
 * Helper function that calls main function for each thread. Maps params struct
 * to individual parameters to pass into main function "avatar".
 * Returns error or success code from "avatar". 
 */
void *avatar_thread(void *params)

/*
return data for the given key, or NULL if not found
*/
void *hashtable_find(hashtable_t *ht, char *key);

/*
return false if key already exists, and true if new item was inserted.
If ht is NULL, or you encounter other error, return false.
*/
bool hashtable_insert(hashtable_t *ht, char *key, void *data);

/* Iterate over all items in hashtable (in undefined order):
 * call itemfunc for each item, with (arg, key, data).
 */
void hashtable_iterate(hashtable_t *ht,
		       void (*itemfunc)(void *arg, const char *key,void *data),
		       void *arg);

void hashtable_delete(hashtable_t* ht);

#endif
