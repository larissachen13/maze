# Testing 
## Scripts 
Compile scripts from root-level: `chmod +x ./test_scripts/compile.sh; ./test_scripts/compile.sh`   
Now able to run scripts at root-level of directory with....

1. `./test_scripts/test_arg.sh`
 * Tests validating command-line arguments 

2. `./test_scripts/test_ascii.sh`
 * Tests the basic ASCII representation on mazes: 
  * -n 2 -d 1 (with sleep)
  * -n 2 -d 2 (without sleep) 
  * -n 2 -d 4 (without sleep 
 
3. `./test_scripts/test_gtk.sh`
 * Tests the gui GTK representation on mazes: 
  * -n 7 -d 5

4. `./test_scripts/populate_results.sh`
 * populates the result directory with logfiles of running the maze w/ -n 4 and -d 0 through 9 