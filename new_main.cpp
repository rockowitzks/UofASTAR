//
//  main.cpp
//  A_STAR
//
//  Created by Kristopher Rockowitz on 3/16/20.
//  Copyright © 2020 r0. All rights reserved.
//

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
        // defaultly null
        this->parent = NULL;
        // using -1 as the initial tile value
        for(unsigned int i = 0; i < 9; i++){
            this->tiles[i] = -1;
        }
    }
    // defines permitted moves
    int moves(){
        //up = 1; left = 2; down = 4; right = 8;
        //e.g: up,left,right = 11;
        // blank set to first tile
        int blank = tiles[0];
        // actions initialized to -1
        int actions = 0;
        // find out how many moves avail
        switch(blank){
                // RD
            case 0: actions =  12;
                break;
                // LDR
            case 1: actions =  14;
                break;
                // LD
            case 2: actions = 6;
                break;
                // URD
            case 3: actions = 13;
                break;
                // ULDR
            case 4: actions = 15;
                break;
                // ULD
            case 5: actions = 7;
                break;
                // RU
            case 6: actions = 9;
                break;
                // LUR
            case 7: actions = 11;
                break;
                // LU
            case 8: actions = 3;
                break;
                // no action available
            default:
                actions = 0;
                break;
        }
        // return actions avail
        return actions;
    }
};

// the misplaced tiles heuristic function
int misplacedTiles(Matrix *current){
    //heuristic is the number of the misplaced tiles in the state.
    // TODO: support input of goal matrix as array in function
    int goal[9] = {8,0,1,2,3,4,5,6,7};
    int misplaced = 0;
    // if tiles don't match, increment misplaced
    for(unsigned int i = 0; i < 9; i++){
        if(current->tiles[i] != goal[i]){
            misplaced++;
        }
    }
    return misplaced;
}

// determine if the goal has been reached
int reachedGoal(Matrix *current){
    // TODO: support input of goal matrix as array in function
    int goal[9] = {8,0,1,2,3,4,5,6,7};
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

    int blank = current->tiles[0];
    // create a new tile
    int tile;

    switch(move){
        // move up
        case 1:
            tile = blank - 3;
            for(unsigned int i = 1; i < 9; i++){
               if(current->tiles[i] == tile){
                   next->tiles[0] = tile;
                   next->tiles[i] = blank;
               }
               else{
                   next->tiles[i] = current->tiles[i];
               }
            }
            break;
            
        // move left
        case 2:
            tile = blank - 1;
            for(unsigned int i = 1; i < 9; i++){
                if(current->tiles[i] == tile){
                    next->tiles[0] = tile;
                    next->tiles[i] = blank;
                }
                else{
                    next->tiles[i] = current->tiles[i];
                }
            }
            break;
            
        // move down
        case 4:
            tile = blank + 3;
            for(unsigned int i = 1; i < 9; i++){
                if(current->tiles[i] == tile){
                    next->tiles[0] = tile;
                    next->tiles[i] = blank;
                }
                else{
                    next->tiles[i] = current->tiles[i];
                }
            }
            break;

        // move right
        default:
            tile = blank + 1;
            for(unsigned int i = 1; i < 9; i++){
                if(current->tiles[i] == tile){
                    next->tiles[0] = tile;
                    next->tiles[i] = blank;
                }
                else{
                    next->tiles[i] = current->tiles[i];
                }
            }
            break;
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

void addExpanded(Matrix *a_node){
    long int i = 0;
    long int j = 0;
    
    // if this is the first expansion, set the initial node
    if(root == -1){
        expanded[0] = *a_node;
    }
    // otherwise increment i for distance away from the root as long as its a better path
    else{
        while(i <= root && expanded[i].cost + misplacedTiles(&expanded[i]) <= a_node->cost + misplacedTiles(a_node)){
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
    Matrix *popped = new Matrix;
    if(root == -1){
        return NULL;
    }
    else{
        for(int i=0; i<9; i++)
            popped->tiles[i] = expanded[0].tiles[i];
        popped->parent = expanded[0].parent;
        popped->cost = expanded[0].cost;
        for(long int i=0; i<root; i++)
            expanded[i] = expanded[i+1];
        root--;
        return popped;
    }
}

void addExplored(int * state){
    root_exp++;
    for(int i = 0; i < 9; i++){
        explored[root_exp][i] = state[i];
    }
}

int alreadyExplored(int * state){
    int count = 0;
    for(long int i=0; i<=root_exp; i++)
    {
        count = 0;
        for(int j = 0; j < 9; j++){
            if(explored[i][j] == state[j]){
                count++;
            }
        }
        if(count == 9)
            return 1;
    }
    return 0;
}

void astar(Matrix initial_node){
    // create a new matrix pointer
    Matrix *initial = new Matrix;
    // point to the initial node
    initial = &initial_node;

    Matrix *temp;
    Matrix *gen_node;
    
    int actions;
    int possible_actions[4];

    addExpanded(initial);

    while(true){
        if(root == -1){
            //cout<<"failure"<<endl;
            cout<<"no solution found"<<endl;
            return;
        }
        temp = subtractExpanded();
        addExplored(temp->tiles);
        if(reachedGoal(temp)){
            cout<<"solution found"<<endl;
            while(temp){
                int reverse[9];
                for(int i=0; i<9; i++){
                    reverse[temp->tiles[i]] = i;
                }
                for(int i = 0; i < 9; i++){
                    if(i != 0 && i % 3 == 0) cout << endl;
                    cout<<reverse[i]<<" ";
                }
                cout<<endl<<endl;
                temp = temp->parent;
            }
            return;
        }

        actions = temp->moves();
        switch(actions)
        {
            case 12:  // RD
                possible_actions[0]=0;
                possible_actions[1]=0;
                possible_actions[2]=1;
                possible_actions[3]=1;
                break;
            case 14: // LDR
                possible_actions[0]=0;
                possible_actions[1]=1;
                possible_actions[2]=1;
                possible_actions[3]=1;
                break;
            case 6: // LD
                possible_actions[0]=0;
                possible_actions[1]=1;
                possible_actions[2]=1;
                possible_actions[3]=0;
                break;
            case 13: // URD
                possible_actions[0]=1;
                possible_actions[1]=0;
                possible_actions[2]=1;
                possible_actions[3]=1;
                break;
            case 15: // ULDR
                possible_actions[0]=1;
                possible_actions[1]=1;
                possible_actions[2]=1;
                possible_actions[3]=1;
                break;
            case 7: // ULD
                possible_actions[0]=1;
                possible_actions[1]=1;
                possible_actions[2]=0;
                possible_actions[3]=0;
                break;
            case 9: // RU
                possible_actions[0]=1;
                possible_actions[1]=0;
                possible_actions[2]=0;
                possible_actions[3]=1;
                break;
            case 11: // LUR
                possible_actions[0]=1;
                possible_actions[1]=1;
                possible_actions[2]=0;
                possible_actions[3]=1;
                break;
            case 3: // LU
                possible_actions[0]=1;
                possible_actions[1]=1;
                possible_actions[2]=0;
                possible_actions[3]=0;
                break;
        }
        
        for(int i = 0; i < 4; i++)
        {
            if(possible_actions[i] == 1){
                // generate a node and update the board
                gen_node = update(temp,pow(2,i));
                // if generated node not explored, expand it
                if(alreadyExplored(gen_node->tiles) == 0)
                    addExpanded(gen_node);
            }
        }

    }
    
}

// returns true if goal state found, false if not found
//bool goal_state(vector<int> init, vector<int> goal){
//    return (init == goal);
//}

// returns the number of misplaced tiles
//int misplaced_tiles(vector<int> init, vector<int> goal){
//
//    int estimate = 0;
//    for(unsigned int i = 0; i < 9; i++){
//        if(init.at(i) != goal.at(i)) estimate++;
//    }
//    return estimate;
//}

//// prints a vector
//void print_vector(vector<int> vec){
//    for(unsigned int i = 0; i < vec.size(); i++){
//        if(i % 3 == 0) cout << endl;
//        cout << vec.at(i) << " ";
//    }
//    cout << endl << endl;
//}
//
//// turns the user input for state into a vector
//void get_state(vector<int> &state){
//
//    string puzzleNumbers;
//    for(unsigned int i = 0; i < 9; i++)
//    {
//
//        // grab the entered numbers
//        cin >> puzzleNumbers;
//        // convert all non_blank tiles to ints and push em back
//        if(puzzleNumbers != "_"){
//            try{
//                int number;
//                number = stoi(puzzleNumbers);
//                state.push_back(number);
//            }
//            // if it fails, let em know
//            catch(int e){
//                cout << "Failed to convert entered number to integer";
//            }
//        }
//        // store the blank tile as 0 in init
//        else{
//            state.push_back(0);
//        }
//    }
//}
//
//// sets the heuristic used by the program
//void select_heuristic(string &sel){
//    char selection;
//    cin >> selection;
//    if(selection == 'a') cout << "you selected 'a'" << endl;
//    else if(selection == 'b') cout << "you selected 'b'" << endl;
//    else if(selection == 'c') cout << "you selected 'c'" << endl;
//    else{
//        cout << "you selected an incorrect option" << endl;
//        return;
//    }
//    sel = selection;
//}

int main(int argc, const char * argv[]) {
    vector<int> initialState;
    vector<int> goalState;
//    string selection;
//
//    cout << "Enter the initial state" << endl;
//    get_state(initialState);
//    print_vector(initialState);
//    cout << "Enter the goal state" << endl;
//    get_state(goalState);
//    print_vector(goalState);
//
//    cout << "Select the heuristic\n\ta) Number of misplace tiles\n\tb) Manhattan distance\n\tc) Ain't nobody got time for that\n";
//    select_heuristic(selection);
//    cout << selection << endl;
    
    Matrix initial;
    int pos;

    cout<<"Welcome!"<<endl;
    cout<<"Enter the positions of the tiles!"<<endl;

    for(unsigned int i = 0; i < 9; i++){
        cin>>pos;
//        initial.tiles[i] = pos;
        initial.tiles[pos] = i;
    }
    
    // TODO: print out just the moves
    // TODO: refactor the shit outta this code
    // TODO: allow user entry of the goal state
    cout<<"Processing..." << endl;

    astar(initial);
    
    return 0;
}
