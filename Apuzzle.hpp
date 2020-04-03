


//
//  puzzle.hpp
//  A_STAR
//
//  Created by Alex Sisson on 4/2/20.
//  Copyright © 2020 r0. All rights reserved.
//

#ifndef Apuzzle_hpp
#define Apuzzle_hpp

#include "new_main.cpp"
#include <stdio.h>

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
    int moves();
};

#endif /* Apuzzle_hpp */
