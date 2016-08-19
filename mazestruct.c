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
			//initialise the variables in spot
			new_spot->avatar = false;
			new_spot->dead = false;
			new_spot->visited = false;
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

	return new_maze;
}

/**************** maze_print() ****************/
void maze_print(mazestruct_t *maze){

	printf("   ");
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
						printf(" %d ", i);
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

/**************** place_avatar() ****************/
void place_avatar(mazestruct_t *maze, int x, int y, int avatar_number){
	//if we are within bounds
	if(x < maze->width && y < maze->width && x > -1 && y > -1){
		//mark the spot appropriately
		maze->map[x][y]->avatar = true;
		maze->map[x][y]->visited = true;
		maze->map[x][y]->visited_by[avatar_number] = 1;
		maze->map[x][y]->avatar_number[avatar_number] = 1;
		//print the update
		printf("\n********************************************************************************\n");
		printf("Inserted avatar %d at %d,%d.\n", avatar_number, x, y);
		maze_print(maze);
		printf("********************************************************************************\n");
	}
	else{
		printf("Avatar location is off the maze.\n");
	}
}

//1 is north, 3 is east, 0 is west, 2 is south

/**************** insert_wall() ****************/
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
		if(y > 0){
			maze->map[x -1][y]->east = true;
		}
		if(!maze->map[x][y]->dead){
			printf("Inserted west wall at %d,%d.\n", x, y);
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
		}
	}

	//if we arent marking it as dead
	if(!maze->map[x][y]->dead){
		maze_print(maze);
		printf("********************************************************************************\n");
	}

}

/**************** visited_spot() ****************/
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
void insert_dead_spot(mazestruct_t *maze, int x,int y){

	if(x > (maze->width - 1) || y > (maze->height - 1) || x < 0 || y < 0){
		printf("Coordinates are out of range, could not insert dead spot.\n");
		return;
	}
	else{
		//mark as dead
		maze->map[x][y]->dead = true;
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
//returns -1 if noone otherwise the direction
// UPDATE: takes in a direction and returns a bool
int is_someone_adjacent(mazestruct_t *maze, int x, int y){

	//check coords
	if(x > (maze->width - 1) || y > (maze->height - 1) || x < 0 || y < 0){
		printf("Coordinates are out of range\n");
		return -1;
	}
	//check the west
	if(x > 0){
		if(maze->map[x - 1][y]->avatar){
			return 0;
		}
	}
	//check the north
	if(y > 0){
		if(maze->map[x][y - 1]->avatar){
			return 1;
		}
	}
	//check the south
	if(y < (maze->height -1)){
		if(maze->map[x][y + 1]->avatar){
			return 2;
		}
	}
	//check the east
	if(x < (maze->width - 1)){
		if(maze->map[x + 1][y]->avatar){
			return 3;
		}
	}
	//if we make it down to here
	return -1;
}


/**************** update_location() ****************/
void update_location(mazestruct_t *maze, int init_x, int init_y, int new_x, int new_y, int avatar_number){

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
	maze->map[init_x][init_y]->avatar_number[avatar_number] = 0;
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
	maze->map[new_x][new_y]->visited_by[avatar_number] = 1;
	maze->map[new_x][new_y]->avatar_number[avatar_number] = 1;

	printf("\n********************************************************************************\n");
	printf("Moved avatar %d from %d,%d to %d,%d.\n", avatar_number, init_x, init_y, new_x, new_y);
	maze_print(maze);
	printf("********************************************************************************\n");
}

/**************** is_visited() ****************/
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
bool did_x_visit(mazestruct_t *maze, int x, int y, int direction, int avatar_number){
	//west spot
	if(direction == 0 && x > 0){
		return (maze->map[x - 1][y]->visited_by[avatar_number] == 1);
	}
	//east spot
	else if(direction == 3 && (x < (maze->width - 1))){
		return (maze->map[x + 1][y]->visited_by[avatar_number] == 1);
	}
	//north spot
	else if(direction == 1 && (y > 0)){
		return (maze->map[x][y - 1]->visited_by[avatar_number] == 1);
	}
	//south spot
	else if(direction == 2 && (y < maze->height - 1)){
		return (maze->map[x][y + 1]->visited_by[avatar_number] == 1);
	}
	//if we make it down here
	return false;
}







