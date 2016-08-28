# Library 

## Data Structures:    
       
##Avatar Struct      
#####Important Members:      
* id
* (x, y) position 

##### Important Methods 
* `make_move`
* `get_best_move`
* `send_move`
* `update_maze `
* `come_together` 
* `same_pos`
* `update_leader`


         
## Mazestruct         
#####Important Members:         
* array of all the slots in the maze
* height, width
* number of avatars
* number of moves 
* is_solved boolean 
    
#####Important Methods    
* `maze_new`
* `maze_print`
* `place_avatar`
* `insert_wall`
* `insert_dead_spot` 
* `check_wall`
* `is_someone_adjacent`
* `is_someone_here`