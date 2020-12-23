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

//struct Pair {
//    char input;
//    int nextState;
//};

struct State {
    int curState;
    int nextStates[52];
    //struct Pair pairs[MAX_INPUT_TYPES];
    //int pairsCount;
};

int charToIndex(char c) {
    int dif = 'a' - 'A';
    int i = (int) c - (int) 'A';
    if (c >= 'a') {
        i -= dif;
        i += 26;
    }
    return i;
}

char indexToChar(int i) {
    int dif = (int) ('a' - 'A');
    int c = i + (int) 'A';
    if (i >= 26) {
        c += dif;
        c -= 26;
    }
    return (char) c;
}

void printFSM(struct State *fsm) {
    for (int i = 0; i < 4; i++) {
        printf("CurState: %d\n", fsm[i].curState);
        for (int j = 0; j < MAX_INPUT_TYPES; j++) {
            if (fsm[i].nextStates[j] != -1) {
                printf("Input: %c\t", indexToChar(j));
                printf("NextState: %d\n", fsm[i].nextStates[j]);
            }
        }
        printf("\n");
    }
}

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
                    for (int j = 0; j < MAX_INPUT_TYPES; j++) {
                        states[i].nextStates[j] = -1;
                    }
                    statesCount++;
                }
                // add new pair to state
                states[i].nextStates[charToIndex(input[0])] = nextState;
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