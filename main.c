#include <stdio.h>
#include "main.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Error: incorrect number of arguments.\n");
        return 1;
    }

    // build fsm
    struct State fsm[MAX_STATES];
    int statesCount = loadFSM(argv[1], fsm);
    if (statesCount == ERROR) {
        return 1;
    }

    // print fsm
    //printFSM(statesCount, fsm);

    // validate fsm
    if (validateFSM(statesCount, fsm) == ERROR) {
        return 1;
    }

    // execute fsm
    if (runFSM(statesCount, argv[2], fsm) == ERROR) {
        return 1;
    }

    return 0;
}
