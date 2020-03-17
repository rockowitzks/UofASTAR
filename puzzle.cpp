//
//  puzzle.cpp
//  A_STAR
//
//  Created by Kristopher Rockowitz on 3/16/20.
//  Copyright © 2020 r0. All rights reserved.
//

#include "puzzle.hpp"
#include "main.cpp"

using namespace std;

bool goal_state(vector<int> init, vector<int> goal){
    
    return (init == goal);
}

int misplaced_tiles(vector<int> init, vector<int> goal){
    
    int estimate = 0;
    for(unsigned int i = 0; i < 9; i++){
        if(init.at(i) != goal.at(i)) estimate++;
    }
    return estimate;
}
