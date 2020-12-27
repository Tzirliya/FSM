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
#define MAX_INPUTS 250
#define DEFAULT_STATE (-1)
#define ERROR (-1)

// TODO: create an enum for error messages so can easily confirm them when testing
// TODO: use fprintf to print errors to stderr
// TODO: capture stdout when running tests and only print them when tests fail

// holds all necessary details about a state
struct State {
    int curState;
    int nextStates[52];
};

// converts an input character to it's corresponding index for nextStates in struct State
// returns the index
int charToIndex(char c) {
    int dif = 'a' - 'A';
    int i = (int) c - (int) 'A';
    if (c >= 'a') {
        i -= dif;
        i += 26;
    }
    return i;
}

// converts an index to it's corresponding input character for nextStates in struct State
// returns the input character
char indexToChar(int i) {
    int dif = (int) ('a' - 'A');
    int c = i + (int) 'A';
    if (i >= 26) {
        c += dif;
        c -= 26;
    }
    return (char) c;
}

// prints an fsm in a readable format
// returns nothing
void printFSM(int statesCount, struct State *fsm) {
    for (int i = 0; i < statesCount; i++) {
        printf("CurState: %d\n", fsm[i].curState);
        for (int j = 0; j < MAX_INPUT_TYPES; j++) {
            if (fsm[i].nextStates[j] != DEFAULT_STATE) {
                printf("Input: %c\t", indexToChar(j));
                printf("NextState: %d\n", fsm[i].nextStates[j]);
            }
        }
        printf("\n");
    }
}

// builds an fsm by reading an fsm definition file and loading the data into a list of struct States
// returns the amount of states and ERROR if failed
int loadFSM(char *defFilename, struct State *fsm) {
    printf("processing FSM inputs file %s\n", defFilename);

    // open fsm definition file
    FILE* fsmDefFile = fopen(defFilename, "r");
    if (!fsmDefFile) {
        printf("Error opening fsm definition file.\n");
        return ERROR;
    }

    // declare variables
    int transitionCount = 0;
    int statesCount = 0;
    char curStateStr[12];
    char inputStr[12];
    char nextStateStr[12];
    int curState;
    char input;
    int nextState;

    // read fsm definition file
    while (fscanf(fsmDefFile, "%[^:]:%[^>]>%s\n", curStateStr, inputStr, nextStateStr) != EOF) {
        // validate data
        for (int i = 0; i < strlen(curStateStr); i++) {
            //printf("curStateStr[%d] = %c\n", i, curStateStr[i]);
            if (!isdigit(curStateStr[i])) {
                printf("Error: invalid curState found in fsm definition file.\n");
                return ERROR;
            }
        }
        curState = atoi(curStateStr);
        if (strlen(inputStr) != 1 || inputStr[0] < 'A' || (inputStr[0] > 'Z' && inputStr[0] < 'a') || inputStr[0] > 'z') {
            printf("Error: invalid input found in fsm definition file.\n");
            return ERROR;
        }
        input = inputStr[0];
        for (int i = 0; i < strlen(nextStateStr) - 1; i++) {
            if (!isdigit(nextStateStr[i])) {
                printf("Error: invalid nextState found in fsm definition file.\n");
                return ERROR;
            }
        }
        nextState = atoi(nextStateStr);

        // add data to fsm
        bool updated = false;
        for (int i = 0; (i < MAX_STATES) && !updated; i++) {
            if (i == statesCount || fsm[i].curState == curState) {
                // check if it's a new state
                if (i == statesCount) {
                    fsm[i].curState = curState;
                    // load nextStates with default values
                    for (int j = 0; j < MAX_INPUT_TYPES; j++) {
                        fsm[i].nextStates[j] = DEFAULT_STATE;
                    }
                    statesCount++;
                }
                // add new pair to state
                fsm[i].nextStates[charToIndex(input)] = nextState;
                updated = true;
            }
        }

        // if the loop exited without updating, then the MAX_STATES limit was exceeded
        if (!updated) {
            printf("Error: too many states\n");
            return ERROR;
        }
        transitionCount++;
    }

    printf("FSM has %d transitions\n", transitionCount);

    // close fsm definition file
    if (fclose(fsmDefFile)) {
        printf("Error closing fsm definition file.\n");
        return ERROR;
    }

    return statesCount;
}

// given a state's name gets the index of the state in an array of struct States
// returns the state's index
int getStateIndex(int statesCount, int curState, struct State *fsm) {
    for (int i = 0; i < statesCount; i++) {
        if (fsm[i].curState == curState) {
            return i;
        }
    }
    return ERROR;
}

// validates an fsm by checking that state 0 exists
// returns 0 if valid and ERROR otherwise
int validateFSM(int statesCount, struct State *fsm) {
    // ensure there's a state 0
    if (getStateIndex(statesCount, 0, fsm) == ERROR) {
        printf("Error: no state 0 in fsm definition.\n");
        return ERROR;
    }
    return 0;
}

// runs an fsm by reading an fsm inputs file and transitioning through the states in the fsm
// returns 0 if ran successfully and ERROR otherwise
int runFSM(int statesCount, char *inputsFilename, struct State *fsm) {
    printf("processing FSM inputs file %s\n", inputsFilename);

    // open fsm inputs file
    FILE* fsmInputsFile = fopen(inputsFilename, "r");
    if (!fsmInputsFile) {
        printf("Error opening fsm inputs file.\n");
        return ERROR;
    }

    // declare variables
    int stepCount = 0;
    int curState = 0;
    int curStateIndex = getStateIndex(statesCount, curState, fsm);
    char inputStr[12];
    char input;
    int nextState;

    // read fsm input file
    while (fscanf(fsmInputsFile, "%s", inputStr) != EOF) {
        // ensure don't exceed MAX_INPUTS limit
        if (stepCount == MAX_INPUTS) {
            printf("Error: exceeded maximum transitions allowed.\n");
            return ERROR;
        }

        // validate data
        if (strlen(inputStr) != 1 || inputStr[0] < 'A' || (inputStr[0] > 'Z' && inputStr[0] < 'a') || inputStr[0] > 'z') {
            printf("Error: invalid input found in fsm inputs file.\n");
            return ERROR;
        }
        input = inputStr[0];

        // transition to next state
        nextState = fsm[curStateIndex].nextStates[charToIndex(input)];
        curStateIndex = getStateIndex(statesCount, nextState, fsm);
        if (curStateIndex == ERROR) {
            printf("Error: could not find next state.\n");
            return ERROR;
        }
        printf("\tat step %d, input %c transitions FSM from state %d to state %d\n", stepCount, input, curState, nextState);
        curState = nextState;
        stepCount++;
    }

    printf("after %d steps, state machine finished successfully at state %d\n", stepCount, curState);

    // close fsm definition file
    if (fclose(fsmInputsFile)) {
        printf("Error closing fsm inputs file.\n");
        return ERROR;
    }

    return curState;
}
