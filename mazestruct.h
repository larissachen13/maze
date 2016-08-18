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
void maze_print(mazestruct_t *maze);
void place_avatar(mazestruct_t *maze, int x, int y, int avatar_number);
void insert_wall(mazestruct_t *maze, int x, int y, int direction);
void visited_spot(mazestruct_t *maze, int x, int y, int avatar_number);
void insert_dead_spot(mazestruct_t *maze, int x,int y);
bool check_wall(mazestruct_t *maze, int x, int y, int direction);
int is_someone_adjacent(mazestruct_t *maze, int x, int y);
void update_location(mazestruct_t *maze, int init_x, int init_y, int new_x, int new_y, int avatar_number);
bool is_visited(mazestruct_t *maze, int x, int y, int direction);
bool did_x_visit(mazestruct_t *maze, int x, int y, int direction, int avatar_number);

#endif // __MAZESTRUCT_H