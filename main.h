//
// Created by tplotkin on 12/21/20.
//

#ifndef SEMESTERPROJECT_MAIN_H
#define SEMESTERPROJECT_MAIN_H

#endif //SEMESTERPROJECT_MAIN_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_STATES 50
#define MAX_INPUT_TYPES 52
//#define MAX_INPUTS 250

// TODO: create an enum for error messages so can easily confirm them when testing

struct Pair {
    char input;
    int nextState;
};

struct State {
    int curState;
    struct Pair pairs[MAX_INPUT_TYPES];
    int pairsCount;
};

int loadFSM(char *defFilename, struct State *states) {
    // open fsm definition file
    FILE* fsmDefFile = fopen(defFilename, "r");
    if (!fsmDefFile) {
        printf("Error opening fsm file\n");
        return 1;
    }
    int statesCount = 0;
    int curState;
    char input[16];
    int nextState;
    // read fsm definition file
    while (fscanf(fsmDefFile, "%d:%[^>]>%d", &curState, input, &nextState) != EOF) {
        //printf("Cur: %d\nInput: %s\nNext: %d\n\n", curState, input, nextState);
        // validate input
        if (strlen(input) > 1 || input[0] < 'A' || (input[0] > 'Z' && input[0] < 'a') || input[0] > 'z') {
            printf("Error: invalid input found in fsm definition file.\n");
            return 1;
        }
        bool updated = false;
        for (int i = 0; (i < MAX_STATES) && !updated; i++) {
            if (i == statesCount || states[i].curState == curState) {
                // check if it's a new state
                if (i == statesCount) {
                    states[i].curState = curState;
                    states[i].pairsCount = 0;
                    statesCount++;
                }
                // check for repeated input
                for (int j = 0; j < states[i].pairsCount; j++) {
                    if (states[i].pairs[j].nextState == input[0]) {
                        printf("Error: duplicate input for state.\n");
                        return 1;
                    }
                }
                // add new pair to state
                states[i].pairs[states[i].pairsCount].input = input[0];
                states[i].pairs[states[i].pairsCount].nextState = nextState;
                states[i].pairsCount++;
                updated = true;
            }
        }
        if (!updated) {
            printf("Error: too many states");
            return 1;
        }
    }
    // close fsm definition file
    if (fclose(fsmDefFile)) {
        printf("Error closing fsm file\n");
        return 1;
    }
}