//
// Created by tplotkin on 12/21/20.
//
#include <stdio.h>
#include "../../../main.h"

int test1();

int main() {
    if (test1()) {
        printf("Test 1 failed.\n");
        return 1;
    }
    return 0;
}

int test1() {
    struct State fsm[MAX_STATES];
    int statesCount = loadFSM("FSMDefinitionFiles/test1.fsm", fsm);
    if (statesCount == ERROR) {
        return 1;
    }
    return 0;
}