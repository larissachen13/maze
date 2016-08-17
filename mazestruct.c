/* 
* A structure file to hold the available information about the maze
*
* Created by Benji Hannam, 2016 for use in Team Core Dumped in a Maze for CS50
*/

//DEVELOPMENT NOTES
/*
* Add in an array to hold the avatar locations for each avatar when needed
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/************************* global variables ******************/

/************************* local types **********************/
typedef struct spot {
	bool north;
	bool south;
	bool east;
	bool west;
	bool avatar;
	visited_by int[10];
} spot_t;

/************************* global types *********************/

typedef struct mazestruct{
	spot_t* map[100][100];
	int height;
	int width;
	int num_avatars;

} mazestruct_t;


/********************** global functions *********************/

/**************** maze_new() ****************/
mazestruct_t* maze_new(int height, int width, int num_avatars){

	mazestruct_t *new_maze = malloc(sizeof(mazestruct_t));
	if(new_maze == NULL){
		printf("Error allocating memory to new maze struct, exiting..\n");
		exit(2);
	}

	//insert all the spots into the map
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){

			spot_t *new_spot = malloc(sizeof(spot_t));
			if(new_spot == NULL){
				printf("Error allocaitng memory to a new spot struct, exiting..\n");
				exit(2);
			}
			new_spot->avatar = false;
			for(int i = 0; i < 10; i++){
				new_spot->visited_by[i] = 0;
			}
			new_maze->map[i][j] = new_spot;
		}
	}

	//insert the walls on the north edge
	for (int i = 0; i < width; i++){
		new_maze->map[i][0]->north = true;
	}

	//insert the walls on the south edge
	for (int i = 0; i < width; i++){
		new_maze->map[i][height - 1]->south = true;
	}
	//insert the walls on the east edge
	for (int i = 0; i < height; i++){
		new_maze->map[width -1][i]->east = true;
	}

	//insert the walls on the west edge
	for (int i = 0; i < height; i++){
		new_maze->map[0][i]->west = true;
	}

	//initialise the variables
	//new_maze->map = temp;
	new_maze->height = height;
	new_maze->width = width;
	new_maze->num_avatars = num_avatars;

	return new_maze;
}

/**************** maze_print() ****************/
void maze_print(mazestruct_t *maze){

	printf("   ");
	for (int i = 0; i < maze->width; i ++){
		printf("  %-2d ", i);
	}
	printf("\n");

	//loop through each row
	for (int i = 0; i < maze->height; i++){

		//print any north walls
		printf("   ");
		for (int j = 0; j < maze->width; j++){
			if(maze->map[j][i]->north){
				printf("+---+");
			}
			//otherwise print nothing
			else{
				printf("+   +");
			}
		}
		printf("\n");
		printf("%-3d", i);
		//print any east, west walls or avatars
		for (int j = 0; j < maze->width; j++){

			if(maze->map[j][i]->west){
				printf("| ");
			}
			else{
				printf("  ");
			}
			if(maze->map[j][i]->avatar){
				printf("A");
			}
			else{
				printf(" ");
			}
			if(maze->map[j][i]->east){
				printf(" |");
			}
			else{
				printf("  ");
			}
		}
		printf("\n   ");

		//print any south walls
		for (int j = 0; j < maze->width; j++){
			if(maze->map[j][i]->south){
				printf("+---+");
			}
			//otherwise print nothing
			else{
				printf("+   +");
			}
		}
		printf("\n");
	}
	printf("\n");
}

/**************** spot_print() ****************/
void place_avatar(mazestruct_t *maze, int x, int y){
	if(x < maze->width && y < maze->width){
		maze->map[x][y]->avatar = true;
	}
	else{
		printf("Avatar location is off the maze.\n");
	}
}

//1 is north, 3 is east, 0 is west, 2 is south

/**************** insert_wall() ****************/
void insert_wall(mazestruct_t *maze, int x, int y, int direction){

	if(direction == 0){
		maze->map[x][y]->west = true;
	}
	if(direction == 1){
		maze->map[x][y]->north = true;
	}
	if(direction == 2){
		maze->map[x][y]->south = true;
	}
	if(direction == 3){
		maze->map[x][y]->east = true;
	}

}

/**************** visited_spot() ****************/
void visited_spot(mazestruct_t *maze, int x,y, int avatar_number){

	if(avatar_number < 10 && avatar_number > -1){
		maze->visited_by[avatar_number] = 1;
	}
	else{
		printf("avatar number is not valid \n");
	}

}

//dead_spot(x,y)

//get_visited

//check_wall(x,y, direction)

//someone_adjacent(x,y)
//returns -1 if noone otherwise the direction

//update_location(init x, init y, current x, current y)










