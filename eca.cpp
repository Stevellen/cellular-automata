#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <stdint.h>
#include <time.h>

using namespace std;

void show_ca(vector<vector<uint8_t>> vec) {
    /*
     * This function just prints the matrix representing the
     * behavior of the automaton to stdout for easy viewing.
    */
    char c;
    for (vector<uint8_t> xs : vec){
        for(int x : xs){
            cout << (x==1 ? "\u25A0" : "\u00A6") << " ";
        }
        cout << endl;
    }
        cout << "\n";
    return;
}

void checkParams(int N, int iter, int k, unsigned long rule) {
    // need to check for usable parameter values
    if ((iter <= 0) || (N <= 0) || (rule < 0) || (k <= 0) || (k >= 64)) { 
        cout << "I need positive integers, yo.\n";
        cout << "You gave me this shit: \n\t";
        cout << iter << ", " << N << ", " << rule << ", " << k <<  endl;
        exit(1); // we obviously can't continue at this point
    }
}

auto randomStart(int N) {
    /*
     * Creates a random starting row for cellular automata
     * 
     * @return vector of randomly-activated "cells"
    */
    srand(time(0)); // seed rand
    int idx;
    int active_cells = rand() % N;  // determine number of active cells
    vector<uint8_t> row(N, 0);      // empty row

    for(int i = 0; i < active_cells; i++) { // randomly activate rows
        idx = rand() % N;
        row[idx] = 1;
    }

    return row;
}

auto eca(int N, int iter, unsigned long rule, int k = 3, bool random = false){
    /*
     * Generates simple rules-based cellular automata. Generalized to any number rule
     * that can be represented in a 64-bit integer.
     * 
     * TODO: Add calculation for 'k' parameter
     * 
     * @param N Integer representing the number of nodes (columns) of the automaton
     * @param iter Integer representing the number of iterations to perform
     * @param rule Integer representing the rule to be applied
     * @return matrix of values generated using params and basic CA rules
    */
    checkParams(N, iter, k, rule);     // sanity checking inputs
    
    const unsigned long mask = 1;      // single active bit for shifting and masking
    vector<vector<uint8_t>> world(iter, vector<uint8_t>(N, 0));
    k = (k&1 ? k : k+1);  // even, or odd? i like odd

    if(random) {
        world[0] = randomStart(N);
    } else {
    world[0][N / 2] = 1; // start with a single 1 in the middle    
    }

    // main loop for updating ECA matrix
    for(int i = 0; i < iter-1; i++) {
        for(int j = 0; j < N; j++) {
            // assuming window k=3 for now; may need to loop in the future
            unsigned int val = world[i][(j-1+N) % N] << 2 |
                    world[i][j] << 1 | 
                    world[i][(j+1+N) % N];

            if(rule & (mask<<val)) { // turn the cell on maybe?
                world[i+1][j] = 1;
            }
        }
    }
    return world;
}




int main(int argc, char *argv[]) {
    if(argc < 4) {  // just sanity checking args
        cout << "Need at least 3 arguments!" << endl;
        cout << "I need values for rows cols rule..." << endl;
        exit(1);
    }
    // read command line args
    unsigned int iter = atoi(argv[1]);   // iterations
    unsigned int N = atoi(argv[2]);   // width (# of cells)
    const unsigned long rule = atoi(argv[3]);   // rule number

    // testing the function version of the eca
    auto world = eca(N, iter, rule);
    // print to stdout
    show_ca(world);

    return 0;
}
