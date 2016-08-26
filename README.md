
# Maze Challenge *by Core Dumped in a Maze* 

Maze challenge is a standalone program that simulates a group of friends
finding each other after blindfoldedly being led into a perfect maze. Our client-program 
interacts with a server on the flume.cs.dartmouth.edu host and sets up and solves a maze 
with a certain difficulty and number of avatars depending on user input. More
specs about design can be found in the design.pdf file. 

## Usage 
* Compiling: `make`  
* Running: `AMStartup -n [numberOfAvatars] -d [difficultMaze] -h [hostname] `      
* Example: running a maze with 2 avatars on diffuculty 3: `AMStartup -n 2 -d 3 -h flume.cs.dartmouth.edu`   
* Cleaning:  `make clean`


## Inputs 
`AMStartup -n [numberOfAvatars] -d [difficultyMaze] -h [hostname] `    

* -n numberOfAvatars - number of avatars to place in maze
 * must be an int
 * must lie in range specified at server with hostname (in case of flume.cs.dartmouth.edu: 1 <= numberOfAvatars <= 10
* -d difficultyMaze - difficulty of the maze
 * must be an int 
 * must lie in range specified at server with hostname (in case of flume.cs.dartmouth.edu: 0 <= difficultyMaze <= 9

## Exit Statuses 
Exit Statuses:     

* 0 - success 
* 1 - failed 
* 2 - unclear 
* 3 - malloc error
* 4 - unknown host 
* 5 - failed connection 
* 6 - message to server failed to be created 
* 7 - avatar not created 
* 8 - invalid number of avatars 
* 9 - invalid difficulty number 


## File Structure
* **AMStartup**: main entry point   
* **thread_ops**: thread_ops module called by AMStartup and uses our data structs in lib.  
* **lib**: *amazing.h* and our defined data structures *avatar.c*, *mazestruct.c*, with their corresponding .h files. 

*see design.pdf for more info about each module* 

create new repository:

- git clone https://gitlab.cs.dartmouth.edu/benjihannam/maze.git
- cd maze
- touch README.md
- git add README.md
- git commit -m "add README"
- git push -u origin master

Existing folder or git repository:

- git clone https://gitlab.cs.dartmouth.edu/benjihannam/maze.git
- cd maze
- touch README.md
- git add README.md
- git commit -m "add README"
- git push -u origin master
