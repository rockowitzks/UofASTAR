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

using namespace std;

void print_vector(vector<int> vec){
    for(unsigned int i = 0; i < vec.size(); i++){
        if(i % 3 == 0) cout << endl;
        cout << vec.at(i) << " ";
    }
    cout << endl << endl;
}

void get_state(vector<int> &init){
    
    string puzzleNumbers;
    for(unsigned int i = 0; i < 9; i++)
    {

        // grab the entered numbers
        cin >> puzzleNumbers;
        // convert all non_blank tiles to ints and push em back
        if(puzzleNumbers != "_"){
            try{
                int number;
                number = stoi(puzzleNumbers);
                init.push_back(number);
            }
            // if it fails, let em know
            catch(int e){
                cout << "Failed to convert entered number to integer";
            }
        }
        // store the blank tile as 0 in init
        else{
            init.push_back(0);
        }
    }
}

void select_heuristic(string &sel){
    char selection;
    cin >> selection;
    if(selection == 'a') cout << "you selected 'a'" << endl;
    else if(selection == 'b') cout << "you selected 'b'" << endl;
    else if(selection == 'c') cout << "you selected 'c'" << endl;
    else{
        cout << "you selected an incorrect option" << endl;
        return;
    }
    sel = selection;
}

int main(int argc, const char * argv[]) {
    vector<int> initialState;
    vector<int> goalState;
    string selection;
    
    cout << "Enter the initial state" << endl;
    get_state(initialState);
    print_vector(initialState);
    cout << "Enter the goal state" << endl;
    get_state(goalState);
    print_vector(goalState);
    
    cout << "Select the heuristic\n\ta) Number of misplace tiles\n\tb) Manhattan distance\n\tc) Ain't nobody got time for that\n";
    select_heuristic(selection);
    cout << selection << endl;
    
    return 0;
}
