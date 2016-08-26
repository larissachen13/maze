
# Maze Challenge *by Core Dumped in a Maze* 

Maze challenge is a standalone program that simulates a group of friends
finding each other after blindfoldedly being led into a perfect maze. Our client-program 
interacts with a server on the flume.cs.dartmouth.edu host and sets up and solves a maze 
with a certain difficulty and number of avatars depending on user input. More
specs about design can be found in the design-core_dumped_in_a_maze.pdf file. 

## Usage 
Compiling: `make`  
Running: `AMStartup -n [numberOfAvatars] -d [difficultMaze] -h [hostname] `      
Example: running a maze with 2 avatars on diffuculty 3: `AMStartup -n 2 -d 3 -h flume.cs.dartmouth.edu`   
Cleaning:  `make clean`

## File Structure 

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
