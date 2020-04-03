//
//  main.cpp
//  A_STAR
//
//  Created by Kristopher Rockowitz on 3/16/20.
//  Copyright © 2020 r0. All rights reserved.
//

/*
  "You miss 100% of the shots you don't take. -Wayne Gretzky" - Michael Scott
*/
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>

using namespace std;

// Matrix class for the 8 puzzle
class Matrix {
    public:
    // the actual tiles
        int tiles[9];
    // cost of movement
        int cost;
    // parent Matrix
        Matrix* parent;
    // constructor
    Matrix(){
        // defaultly 0
        this->cost = 0;

        // using -1 as the initial tile value
        for(unsigned int i = 0; i < 9; i++){
            this->tiles[i] = -1;
        }
        // defaultly null
        this->parent = NULL;
    }
    // defines permitted moves
    int moves(){
        //up = 1; left = 2; down = 4; right = 8;
        //e.g: up,left,right = 11;
        // blank set to first tile
        int blank = tiles[0];
        // moves initialized to -1
        int moves = 0;
        // find out how many moves avail
        switch(blank){
                // RD
            case 0: moves =  12;
                break;
                // LDR
            case 1: moves =  14;
                break;
                // LD
            case 2: moves = 6;
                break;
                // URD
            case 3: moves = 13;
                break;
                // ULDR
            case 4: moves = 15;
                break;
                // ULD
            case 5: moves = 7;
                break;
                // RU
            case 6: moves = 9;
                break;
                // LUR
            case 7: moves = 11;
                break;
                // LU
            case 8: moves = 3;
                break;
                // no action available
            default:
                moves = 0;
                break;
        }
        // return moves avail
        return moves;
    }
};

// the misplaced tiles heuristic function
int misplacedTiles(Matrix *current, int goal[]){
    //heuristic is the number of the misplaced tiles in the state.
    // int goalz[9] = {8,0,1,2,3,4,5,6,7};
    int misplaced = 0;
    // if tiles don't match, increment misplaced
    for(unsigned int i = 0; i < 9; i++){
        if(current->tiles[i] != goal[i]){
            misplaced++;
        }
    }
    return misplaced;
}

// the misplaced tiles heuristic function
int manhattanDistance(Matrix *current, int goal[]){
    //heuristic is the number of the misplaced tiles in the state.
    // int goalz[9] = {8,0,1,2,3,4,5,6,7};
    int manhattan = 0;

    //  h = abs (current_cell.x – goal.x) + 
    //  abs (current_cell.y – goal.y)

    int x1, x2, y1, y2;

    x1 = current->tiles[0] % 3;
    x2 = goal[0] % 3;

    y1 = current->tiles[0] / 3;
    y2 = goal[0] / 3;

    manhattan = abs(x1 - x2) + (y1 - y2);
    
    // if tiles don't match, increment misplaced
    return manhattan;
}
// determine if the goal has been reached
int reachedGoal(Matrix *current, int goal[]){
    // int goalz[9] = {8,0,1,2,3,4,5,6,7};

    // if even one tile doesn't match, return 0
    for(unsigned int i = 0; i < 9; i++){
        if(current->tiles[i] != goal[i]){
            return 0;
        }
    }
    return 1;
}

// updates the matrix values after a move
Matrix* update(Matrix *current, int move){
    // create a predecessor called next
    Matrix *next = new Matrix;

    // create a temporary tile
    int tile;
    // grab the blank tile
    int blank = current->tiles[0];


    // using linear indexing, m
    switch(move){
        // move up
        case 1:
            tile = blank - 3;

            break;
            
        // move left
        case 2:
            tile = blank - 1;

            break;
            
        // move down
        case 4:
            tile = blank + 3;

            break;

        // move right
        default:
            tile = blank + 1;

            break;
    }

    for(unsigned int i = 1; i < 9; i++){
        if(current->tiles[i] == tile){
            next->tiles[0] = tile;
            next->tiles[i] = blank;
        }
        else{
            next->tiles[i] = current->tiles[i];
        }
    }
    // change next's parent to this node
    next->parent = current;
    // increment cost by 1
    next->cost = current->cost + 1;
    
    return next;
}

// ******* A* algorithm set **********

// expanding the puzzle matrix graph
Matrix *expanded = new Matrix[1000000];
long int root = -1;

int explored[1000000][9];
long int root_exp = -1;

void addExpanded(Matrix *a_node, int goal[]){
    long int i = 0;
    long int j = 0;

    
    // if this is the first expansion, set the initial node
    if(root == -1){
        expanded[0] = *a_node;
    }
    // otherwise increment i for distance away from the root as long as its a better path
    else{
        while(i <= root && expanded[i].cost + misplacedTiles(&expanded[i], goal) <= a_node->cost + misplacedTiles(a_node, goal)){
            i++;
        }
        for(j = root; j >= i; --j){
            expanded[j + 1] = expanded[j];
        }
        expanded[i] = *a_node;
    }
    root++;
}

Matrix *subtractExpanded(){
    // create new subtracted Matrix
    Matrix *subtracted = new Matrix;
    // if the root is -1, found top, return NULL
    if(root == -1){
        return NULL;
    }
    
    else{
        for(unsigned long int i = 0; i < 9; i++){
            subtracted->tiles[i] = expanded[0].tiles[i];
        }
        subtracted->parent = expanded[0].parent;
        subtracted->cost = expanded[0].cost;
        for(unsigned long int i = 0; i < root; i++){
            expanded[i] = expanded[i+1];
        }
        root--;
        return subtracted;
    }
}

void addExplored(int *tile){
    root_exp++;
    for(int i = 0; i < 9; i++){
        explored[root_exp][i] = tile[i];
    }
}

int alreadyExplored(int *tile){
    int count = 0;
    for(unsigned long int i = 0; i <= root_exp; i++){
        count = 0;
        for(int j = 0; j < 9; j++){
            if(explored[i][j] == tile[j]){
                count++;
            }
        }
        if(count == 9){
            return 1;
        }
    }
    return 0;
}

void getMove(int array[]){
  /*
  for(unsigned int i = 0; i < 9; i++){
    switch(errrrrrrrrrrrr) {
      case NUT:
        break;
      case Squashy:
        break;
      default:
        break;
    }
  }
  */
}
void astar(Matrix initial_state, int goal[]){
    // create a new matrix pointer
    Matrix *initial = new Matrix;
    // point to the initial node
    initial = &initial_state;

    Matrix *temp_matrix;
    Matrix *new_matrix;
    
    int moves;
    int moves_avail[4];

    addExpanded(initial, goal);

    while(1){
        // reached the end without finding the goal state
        if(root == -1){
            cout<<"no solution found"<<endl;
            return;
        }
        temp_matrix = subtractExpanded();
        addExplored(temp_matrix->tiles);
        if(reachedGoal(temp_matrix, goal)){
            cout<<"solution found"<<endl;
            while(temp_matrix){
                int reverse[9];
                //Switch index with value
                for(int i = 0; i < 9; i++){
                    reverse[temp_matrix->tiles[i]] = i;
                }
                // printing the actual values at their indices
                for(int i = 0; i < 9; i++){
                    if(i != 0 && i % 3 == 0) cout << endl;
                    cout << reverse[i] << " ";
                }
                cout << endl << endl;
                temp_matrix = temp_matrix->parent;
            }
            return;
        }

        moves = temp_matrix->moves();
        switch(moves)
        {
            case 12:  // RD
                moves_avail[0]=0;
                moves_avail[1]=0;
                moves_avail[2]=1;
                moves_avail[3]=1;
                break;
            case 14: // LRD
                moves_avail[0]=0;
                moves_avail[1]=1;
                moves_avail[2]=1;
                moves_avail[3]=1;
                break;
            case 6: // LD
                moves_avail[0]=0;
                moves_avail[1]=1;
                moves_avail[2]=1;
                moves_avail[3]=0;
                break;
            case 13: // URD
                moves_avail[0]=1;
                moves_avail[1]=0;
                moves_avail[2]=1;
                moves_avail[3]=1;
                break;
            case 15: // ULRD
                moves_avail[0]=1;
                moves_avail[1]=1;
                moves_avail[2]=1;
                moves_avail[3]=1;
                break;
            case 7: // ULD
                moves_avail[0]=1;
                moves_avail[1]=1;
                moves_avail[2]=0;
                moves_avail[3]=0;
                break;
            case 9: // UR
                moves_avail[0]=1;
                moves_avail[1]=0;
                moves_avail[2]=0;
                moves_avail[3]=1;
                break;
            case 11: // LUR
                moves_avail[0]=1;
                moves_avail[1]=1;
                moves_avail[2]=0;
                moves_avail[3]=1;
                break;
            case 3: // LU
                moves_avail[0]=1;
                moves_avail[1]=1;
                moves_avail[2]=0;
                moves_avail[3]=0;
                break;
        }
        
        for(int i = 0; i < 4; i++)
        {
            // for every available move, update the new matrix
            if(moves_avail[i]){
                // update the new matrix
                new_matrix = update(temp_matrix,pow(2,i));
                // if the new matrix is not part of the explored set, expand and explore it
                if(alreadyExplored(new_matrix->tiles) == 0)
                    addExpanded(new_matrix, goal);
            }
        }

    }
    
}


int main(int argc, const char * argv[]) {     
      
    Matrix initial_state;
    int value;
    

    cout<<"8 Puzzle Solver"<<endl;
    cout<<"Please Enter the Initial State"<<endl;

    for(unsigned int i = 0; i < 9; i++){
        cin>>value;
//        initial_state.tiles[i] = value;
        initial_state.tiles[value] = i;
    }
    
    value = 0;
    int goal[9];
    
    cout<<"Please Enter the Goal State"<<endl;
    for(unsigned int i = 0; i < 9; i++){
            cin >> value;
    //        initial_state.tiles[i] = value;
            goal[value] = i;
        }
    
    //cout << "You entered ";
    //for(unsigned int i = 0; i < 9; i++){
    //     cout << goal[i] << " ";
    //}
    //cout << endl;
    
    //Determine heuristic
    function<void()> h_function;
    int h_funct_val = 0;
    
    cout << "Please Enter 0 for misplaced tiles or 1 for Manhattan" << endl;
    cin >> h_funct_val;
    
    if (h_funct_val == 1){
      h_function = &misplacedTiles;
      cout << "You chose Manhattan Heuristic" << endl;
    }
    else {
      h_function = &manhattanDistance;
      cout << "You chose Misplaced Tiles heuristic" << endl;
    }


    // TODO: print out just the moves
    // TODO: refactor the shit outta this code
    // TODO: allow user entry of the goal state
    cout<<"Processing..." << endl;

    astar(initial_state, goal);

    
    
    return 0;
}
