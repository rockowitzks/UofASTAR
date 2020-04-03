


#include <Apuzzle.hpp>

int Matrix::moves(){
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