//
// Created by tplotkin on 12/21/20.
//

#ifndef SEMESTERPROJECT_MAIN_H
#define SEMESTERPROJECT_MAIN_H

#endif //SEMESTERPROJECT_MAIN_H

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_STATES 50
#define MAX_INPUT_TYPES 52
#define ERROR -1
//#define MAX_INPUTS 250

// TODO: create an enum for error messages so can easily confirm them when testing

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
        return -1;
    }
    int statesCount = 0;
    char curStateStr[12];
    char inputStr[12];
    char nextStateStr[12];
    int curState;
    char input;
    int nextState;
    // read fsm definition file
    while (fscanf(fsmDefFile, "%[^:]:%[^>]>%s\n", curStateStr, inputStr, nextStateStr) != EOF) {
        //printf("Cur: %d\nInput: %s\nNext: %d\n\n", curState, input, nextState);
        //printf("Cur: %s\tInput: %s\tNext: %s\n", curStateStr, inputStr, nextStateStr);
        // validate data
        for (int i = 0; i < strlen(curStateStr); i++) {
            //printf("curStateStr[%d] = %c\n", i, curStateStr[i]);
            if (!isdigit(curStateStr[i])) {
                printf("Error: invalid input found in fsm definition file 1.\n");
                return ERROR;
            }
        }
        curState = atoi(curStateStr);
        if (strlen(inputStr) > 1 || inputStr[0] < 'A' || (inputStr[0] > 'Z' && inputStr[0] < 'a') || inputStr[0] > 'z') {
            printf("Error: invalid input found in fsm definition file 2.\n");
            return ERROR;
        } else {
            input = inputStr[0];
        }
        for (int i = 0; i < strlen(nextStateStr) - 1; i++) {
            if (!isdigit(nextStateStr[i])) {
                printf("Error: invalid input found in fsm definition file 3.\n");
                return ERROR;
            }
        }
        nextState = atoi(nextStateStr);
        // add data to fsm
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
                states[i].nextStates[charToIndex(input)] = nextState;
                updated = true;
            }
        }
        if (!updated) {
            printf("Error: too many states");
            return ERROR;
        }
    }
    // close fsm definition file
    if (fclose(fsmDefFile)) {
        printf("Error closing fsm file\n");
        return ERROR;
    }
    return statesCount;
}

int validateFSM(int statesCount, struct State *fsm) {
    // ensure there's a state 0
    for (int i = 0; i < statesCount; i++) {
        if (fsm[i].curState == 0) {
            return 0;
        }
    }
    printf("Error: no state 0 in fsm definition.");
    return ERROR;
}
