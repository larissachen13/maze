/* 
 * mazestruct.h - header file for mazestruct
 *
 * Benji Hannam, July 2016
 */

#ifndef __MAZESTRUCT_H
#define __MAZESTRUCT_H

/**************** global types ****************/
typedef struct maze mazestruct_t;  // opaque to users of the module

/**************** functions ****************/
/*Creates a new maze
*
*/
mazestruct_t maze_new(int height, int width, int num_avatars);

#endif // __MAZESTRUCT_H