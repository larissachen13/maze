/* 
* A structure file to hold the available information about the maze
*
* Created by Benji Hannam, 2016 for use in Team Core Dumped in a Maze for CS50
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/************************* global variables ******************/

/************************* local types **********************/
typedef struct spot {
	//bools for the walls
	bool north;
	bool south;
	bool east;
	bool west;
	//bools for states of the spot
	bool avatar;
	bool dead;
	bool visited;
	//arrays for more information on the spots
	int avatar_number[10];
	int visited_by[10];
} spot_t;

/************************* global types *********************/

typedef struct mazestruct{
	spot_t* map[100][100];
	int height;
	int width;
	int num_avatars;
	int move_count;
	int number_leaders;

} mazestruct_t;


/********************** global functions *********************/

/**************** maze_new() ****************/
/*
* Takes in the height and width of the maze and the number of avatars playing the game.
* Allocates memory for the maze structure and generates the 2d array of the grid spots.
* returns a pointer the the new maze structure generated.
*/
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
			//initialise the variables in spot
			new_spot->avatar = false;
			new_spot->dead = false;
			new_spot->visited = false;
			//initialise the direction bools
			new_spot->east = false;
			new_spot->north = false;
			new_spot->south = false;
			new_spot->west = false;
			//initialise the array
			for(int i = 0; i < 10; i++){
				new_spot->avatar_number[i] = 0;
			}
			
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
	new_maze->move_count = 0;
	new_maze->number_leaders = num_avatars;

	return new_maze;
}

/**************** maze_print() ****************/
/*
* Prints out the 2D representation of the maze. 
* Prints from left to right and top to bottom going row by row.
* If there are two avatars on the same spot it will print the lowest numbers avatar.
* 
*/
void maze_print(mazestruct_t *maze){

	printf("   ");

	//if the width of the maze is less that 45 make the width of each spot bigger
	if(maze->width < 45){
		for (int i = 0; i < maze->width; i ++){
			printf("  %-2d", i);
		}
		printf("\n");

		//print the top edge
		printf("   ");
		for (int j = 0; j < maze->width; j++){
			if(maze->map[j][0]->north){
				printf("+---");
			}
			//otherwise print nothing
			else{
				printf("+   +");
			}
		}
		printf("+");

		//loop through each row
		for (int i = 0; i < maze->height; i++){

			printf("\n");
			printf("%-3d", i);
			//print the east wall
			printf("|");
			// print the west walls or avatars
			for (int j = 0; j < maze->width; j++){

				if(maze->map[j][i]->avatar){
					for(int n = 0; n < 10; n++){
						if(maze->map[j][i]->avatar_number[n] == 1){
							printf(" %d ", n);
							break;
						}
					}
				}
				else if(maze->map[j][i]->dead){
					printf(" D ");
				}
				else{
					printf("   ");
				}
				if(maze->map[j][i]->east){
					printf("|");
				}
				else{
					printf(" ");
				}
			}
			printf("\n   ");

			//print any south walls
			for (int j = 0; j < maze->width; j++){
				if(maze->map[j][i]->south){
					printf("+---");
				}
				//otherwise print nothing
				else{
					printf("+   ");
				}
			}
			printf("+");
		}
		printf("\n");
	}
	//if we have more than 45 columns in the maze then scale down the width of each spot by half
	else{

		for (int i = 0; i < maze->width; i ++){
			printf(" %-2d", i);
		}
		printf("\n");

		//print the top edge
		printf("   ");
		for (int j = 0; j < maze->width; j++){
			if(maze->map[j][0]->north){
				printf("+-");
			}
			//otherwise print nothing
			else{
				printf("+ +");
			}
		}
		printf("+");

		//loop through each row
		for (int i = 0; i < maze->height; i++){

			printf("\n");
			printf("%-3d", i);
			//print the east wall
			printf("|");
			// print the west walls or avatars
			for (int j = 0; j < maze->width; j++){

				if(maze->map[j][i]->avatar){
					for(int n = 0; n < 10; n++){
						if(maze->map[j][i]->avatar_number[n] == 1){
							printf("%d", n);
							break;
						}
					}
				}
				else if(maze->map[j][i]->dead){
					printf("D");
				}
				else{
					printf(" ");
				}
				if(maze->map[j][i]->east){
					printf("|");
				}
				else{
					printf(" ");
				}
			}
			printf("\n   ");

			//print any south walls
			for (int j = 0; j < maze->width; j++){
				if(maze->map[j][i]->south){
					printf("+-");
				}
				//otherwise print nothing
				else{
					printf("+ ");
				}
			}
			printf("+");
		}
		printf("\n");
	}
}

/**************** place_avatar() ****************/
/*
* Takes in a pointer to a maze struct, a pair of x,y coords and the id of the avatar.
* Then marks the avatar at being in that spot referenced by the x,y coordinates provided.
* Also prints the entire maze structure to stdout.
*/
void place_avatar(mazestruct_t *maze, int x, int y, int avatar_id){
	//if we are within bounds
	if(x < maze->width && y < maze->width && x > -1 && y > -1){
		//mark the spot appropriately
		maze->map[x][y]->avatar = true;
		maze->map[x][y]->visited = true;
		maze->map[x][y]->visited_by[avatar_id] = 1;
		maze->map[x][y]->avatar_number[avatar_id] = 1;
		//print the update
		printf("\n********************************************************************************\n");
		printf("Inserted avatar %d at %d,%d.\n", avatar_id, x, y);
		maze_print(maze);
		printf("********************************************************************************\n");
	}
	else{
		printf("Avatar location is off the maze.\n");
	}
}

/**************** insert_wall() ****************/
/*
* Tells the maze structure where a wall has been found.
* Takes in a pointer to a maze struct, a pair of x,y coordinates and a direction denoted by:
* 0 = west, 1 = north, 2 = south and 3 = west.
*/
void insert_wall(mazestruct_t *maze, int x, int y, int direction){

	if(x > (maze->width - 1) || y > (maze->height - 1) || x < 0 || y < 0){
		printf("Coordinates are out of range\n");
		return;
	}

	//if not marking as dead
	if(!maze->map[x][y]->dead){
		printf("\n********************************************************************************\n");
	}

	if(direction == 0){
		//set the west wall and the east wall of the spot to the left
		maze->map[x][y]->west = true;
		if(x > 0){
			maze->map[x -1][y]->east = true;
		}
		if(!maze->map[x][y]->dead){
			printf("Inserted west wall at %d,%d.\n", x, y);
			maze->move_count++;
		}
	}
	if(direction == 1){
		//set the north wall and the south wall of the spot;
		maze->map[x][y]->north = true;
		if(y > 0){
			maze->map[x][y - 1]->south = true;
		}
		if(!maze->map[x][y]->dead){
			printf("Inserted north wall at %d,%d.\n", x, y);
			maze->move_count++;
		}
	}
	if(direction == 2){
		//set the south wall and the north wall of the spot below
		maze->map[x][y]->south = true;
		if(y < (maze->height - 1)){
			maze->map[x][y + 1]->north = true;
		}
		if(!maze->map[x][y]->dead){
			printf("Inserted south wall at %d,%d.\n", x, y);
			maze->move_count++;
		}
	}
	if(direction == 3){
		//set the east wall and the west wall of the spot to the right
		maze->map[x][y]->east = true;
		if(x < (maze->width - 1)){
			maze->map[x + 1][y]->west = true;
		}
		if(!maze->map[x][y]->dead){
			printf("Inserted east wall at %d,%d.\n", x, y);
			maze->move_count++;
		}
	}

	//if we arent marking it as dead
	if(!maze->map[x][y]->dead){
		maze_print(maze);
		printf("********************************************************************************\n");
	}

}

/**************** visited_spot() ****************/
/*
* Marks a spot as being visited by an avatar.
* Takes in a pointer to a maze struct, a pair of x,y coords and the id of the avatar that visited it.
* 
*/
void visited_spot(mazestruct_t *maze, int x, int y, int avatar_number){

	if(x > (maze->width - 1) || y > (maze->height - 1) || x < 0 || y < 0){
		printf("Coordinates are out of range\n");
		return;
	}
	if(avatar_number < 10 && avatar_number > -1){
		maze->map[x][y]->visited_by[avatar_number] = 1;
	}
	else{
		printf("avatar number is not valid \n");
	}
}

/**************** insert_dead_spot() ****************/
/*
* Marks a spot as being dead.
* Takes in a pointer to a maze struct, a pair of x,y coords and the id of the avatar that visited it.
* 
*/
void insert_dead_spot(mazestruct_t *maze, int x,int y){

	if(x > (maze->width - 1) || y > (maze->height - 1) || x < 0 || y < 0){
		printf("Coordinates are out of range, could not insert dead spot.\n");
		return;
	}
	else{
		//mark as dead
		maze->map[x][y]->dead = true;
		maze->map[x][y]->visited = false;
		//add west wall
		insert_wall(maze, x, y, 0);
		//add north wall
		insert_wall(maze, x, y, 1);
		//add south wall
		insert_wall(maze, x, y, 2);
		//add east wall
		insert_wall(maze, x, y, 3);

	}

	printf("\n********************************************************************************\n");
	printf("Inserted dead spot at %d,%d.\n", x, y);
	maze_print(maze);
	printf("********************************************************************************\n");
}

/**************** check_wall() ****************/
/*
* Checks if we know for sure there is a wall on the side of a spot
* Takes in a pointer to a maze struct, a pair of x,y coords and a direction designated by:
* 0 = west, 1 = north, 2 = south and 3 = west.
* Returns true if there is a wall and false if there is not.
*/
bool check_wall(mazestruct_t *maze, int x, int y, int direction){

	//check coords
	if(x > (maze->width - 1) || y > (maze->height - 1) || x < 0 || y < 0){
		printf("Coordinates are out of range, cannot check wall.\n");
		return false;
	}
	//return east wall status
	if(direction == 0){
		
		return maze->map[x][y]->west;
	}
	//return north wall status
	else if(direction == 1){

		return maze->map[x][y]->north;
	}
	//return south wall status
	else if(direction == 2){
	
		return maze->map[x][y]->south;
		
	}
	//return west wall status
	else if(direction == 3){
		
		return maze->map[x][y]->east;
	}
	else{
		return false;
	}

}
/**************** is_someone_adjacent() ****************/
/*
* Checks if there is someone the adjacent spot dictated by the inputted direction.
* Takes in the maze struct, x,y coordinates and the direction as parameters.
* returns the lowest avatar id if someone is adjacent and -1 if noone is found.
*/
int is_someone_adjacent(mazestruct_t *maze, int x, int y, int direction){

	//check coords
	if(x > (maze->width - 1) || y > (maze->height - 1) || x < 0 || y < 0){
		printf("Coordinates are out of range\n");
		return -1;
	}

	//west spot
	if(direction == 0 && x > 0){
		//loop through and check for each avatar return -1 if none.
		for(int i = 0; i < maze->num_avatars){
			 if (maze->map[x - 1][y]->avatar_number[i] == 1){
			 	return i;
			 }
			 return -1;
		} 
	}
	//east spot
	else if(direction == 3 && (x < (maze->width - 1))){
		
		for(int i = 0; i < maze->num_avatars){
			 if (maze->map[x + 1][y]->avatar_number[i] == 1){
			 	return i;
			 }
			 return -1;
		} 
	}
	//north spot
	else if(direction == 1 && (y > 0)){

		for(int i = 0; i < maze->num_avatars){
			 if (maze->map[x][y - 1]->avatar_number[i] == 1){
			 	return i;
			 }
			 return -1;
		} 
	}
	//south spot
	else if(direction == 2 && (y < maze->height - 1)){

		for(int i = 0; i < maze->num_avatars){
			 if (maze->map[x][y + 1]->avatar_number[i] == 1){
			 	return i;
			 }
			 return -1;
		} 
	}
	else{
		return -1;
	}
}


/**************** update_location() ****************/
/*
* Moves an avatar from one spot to another one.
* Takes in a maze struct, the inital x,y and the new x,y coordinates and the id of the avatar to move.
*
*/
void update_location(mazestruct_t *maze, int init_x, int init_y, int new_x, int new_y, int avatar_id){

	//check the initial coords
	if(init_x > (maze->width - 1) || init_y > (maze->height - 1) || init_x < 0 || init_y < 0){
		printf("Initial coordinates are out of range, did not update_location.\n");
		return;
	}

	//check the new coords
	if(new_x > (maze->width - 1) || new_y > (maze->height - 1) || new_x < 0 || new_y < 0){
		printf("New coordinates are out of range, did not update location.\n");
		return;
	}

	//update the initial spot
	maze->map[init_x][init_y]->avatar_number[avatar_id] = 0;
	//assume there are no more avatars at that spot
	maze->map[init_x][init_y]->avatar = false;
	//check if there are more and set the bool to true if so
	for(int i = 0; i < 10; i++){
		if(maze->map[init_x][init_y]->avatar_number[i] == 1){
			maze->map[init_x][init_y]->avatar = true;
			break;
		}
	}

	//update the new spot
	maze->map[new_x][new_y]->avatar = true;
	maze->map[new_x][new_y]->visited = true;
	maze->map[new_x][new_y]->visited_by[avatar_id] = 1;
	maze->map[new_x][new_y]->avatar_number[avatar_id] = 1;

	printf("\n********************************************************************************\n");
	printf("Moved avatar %d from %d,%d to %d,%d.\n", avatar_id, init_x, init_y, new_x, new_y);
	maze->move_count++;
	maze_print(maze);
	printf("********************************************************************************\n");
}

/**************** is_visited() ****************/
/*
* Checks if a nearby spot denoted by the direction has been visited.
* returns true if it has, false if it hasn't
*/
bool is_visited(mazestruct_t *maze, int x, int y, int direction){

	//west spot
	if(direction == 0 && x > 0){
		return maze->map[x - 1][y]->visited;
	}
	//east spot
	else if(direction == 3 && (x < (maze->width - 1))){
		return maze->map[x + 1][y]->visited;
	}
	//north spot
	else if(direction == 1 && (y > 0)){
		return maze->map[x][y - 1]->visited;
	}
	//south spot
	else if(direction == 2 && (y < maze->height - 1)){
		return maze->map[x][y + 1]->visited;
	}
	else{
		return false;
	}

}

/**************** did_x_visit() ****************/
/*
* Checks if a spot denoted by a direction has been 
* Takes in the maze struct, x,y coordinates, the direction and an avatar id as parameters.
* returns true if the avatar has visited it otherwise returns false.
*/
bool did_x_visit(mazestruct_t *maze, int x, int y, int direction, int avatar_id){
	//west spot
	if(direction == 0 && x > 0){
		return (maze->map[x - 1][y]->visited_by[avatar_id] == 1);
	}
	//east spot
	else if(direction == 3 && (x < (maze->width - 1))){
		return (maze->map[x + 1][y]->visited_by[avatar_id] == 1);
	}
	//north spot
	else if(direction == 1 && (y > 0)){
		return (maze->map[x][y - 1]->visited_by[avatar_id] == 1);
	}
	//south spot
	else if(direction == 2 && (y < maze->height - 1)){
		return (maze->map[x][y + 1]->visited_by[avatar_id] == 1);
	}
	//if we make it down here
	return false;
}

/**************** delete_maze() ****************/
/*
* Deletes the maze preventing memory leaks.
* 
* 
*/
void delete_maze(mazestruct_t *maze){

	if(maze != NULL){
		//print the total number of moves
		printf("TOTAL NUMBER OF MOVES: %d\n", maze->move_count);
		printf("Number of leaders: %d\n", maze->number_leaders);

		//go through each spot and free the memory associated with it
		for (int i = 0; i < maze->width; i++){
			for (int j = 0; j < maze->height; j++){
				if(maze->map[i][j] != NULL){
					free(maze->map[i][j]);
				}
			}
		}
		free(maze);
	}
}

/**************** remove_leader() ****************/
/*
* Decrements the number of leaders by 1
* 
*/
void remove_leader(mazestruct_t *maze){
	maze->number_leaders--;
}






