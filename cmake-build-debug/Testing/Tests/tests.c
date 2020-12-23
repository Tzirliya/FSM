//
// Created by tplotkin on 12/21/20.
//
#include <stdio.h>
#include "../../../main.h"

int test_loadFSM();
int test_loadFSM_basic();
int test_loadFSM_negative_curState();
int test_loadFSM_invalid_char_input();
int test_loadFSM_string_input();

int test_validateFSM();
int test_validateFSM_basic();
int test_validateFSM_state_0_is_last();
int test_validateFSM_no_state_0();

int main() {
    int failures = 0;
    failures += test_loadFSM();
    failures += test_validateFSM();
    printf("\n%d total tests failed.\n", failures);
    if (failures != 0) {
        return 1;
    }
    return 0;
}

int test_loadFSM() {
    int failures = 0;
    if (test_loadFSM_basic()) {
        printf("test_loadFSM_basic failed.\n");
        failures++;
    }
    if (test_loadFSM_negative_curState()) {
        printf("test_loadFSM_negative_curState failed.\n");
        failures++;
    }
    if (test_loadFSM_invalid_char_input()) {
        printf("test_loadFSM_invalid_char_input failed.\n");
        failures++;
    }
    if (test_loadFSM_string_input()) {
        printf("test_loadFSM_string_input failed.\n");
        failures++;
    }
    printf("\n%d loadFSM tests failed.\n\n", failures);
    return failures;
}

// loadFSM with definition file from project description
// should pass
int test_loadFSM_basic() {
    printf("Running test_loadFSM_basic. Should pass.\n");
    struct State fsm[MAX_STATES];
    int statesCount = loadFSM("FSMDefinitionFiles/test1.fsm", fsm);
    if (statesCount == ERROR) {
        return 1;
    }
    return 0;
}

// loadFSM with negative curState
// should fail
int test_loadFSM_negative_curState() {
    printf("Running test_loadFSM_negative_curState. Should error.\n");
    struct State fsm[MAX_STATES];
    int statesCount = loadFSM("FSMDefinitionFiles/test2.fsm", fsm);
    if (statesCount == ERROR) {
        return 0;
    }
    return 1;
}

// loadFSM with an input that's '#'
// should fail
int test_loadFSM_invalid_char_input() {
    printf("Running test_loadFSM_invalid_char_input. Should error.\n");
    struct State fsm[MAX_STATES];
    int statesCount = loadFSM("FSMDefinitionFiles/test3.fsm", fsm);
    if (statesCount == ERROR) {
        return 0;
    }
    return 1;
}

// loadFSM with an input that's "dd"
// should fail
int test_loadFSM_string_input() {
    printf("Running test_loadFSM_string_input. Should error.\n");
    struct State fsm[MAX_STATES];
    int statesCount = loadFSM("FSMDefinitionFiles/test4.fsm", fsm);
    if (statesCount == ERROR) {
        return 0;
    }
    return 1;
}

// all validateFSM tests
int test_validateFSM() {
    int failures = 0;
    if (test_validateFSM_basic()) {
        printf("test_validateFSM_basic failed.\n");
        failures++;
    }
    if (test_validateFSM_state_0_is_last()) {
        printf("test_validateFSM_state_0_is_last failed.\n");
        failures++;
    }
    if (test_validateFSM_no_state_0()) {
        printf("test_validateFSM_no_state_0 failed.\n");
        failures++;
    }
    printf("\n%d validateFSM tests failed.\n\n", failures);
    return failures;
}

// validateFSM with states 0-3 in order
// should pass
int test_validateFSM_basic() {
    printf("Running test_validateFSM_basic. Should pass.\n");
    struct State fsm[MAX_STATES];
    int statesCount = 4;
    for (int i = 0; i < statesCount; i++) {
        fsm[i].curState = i;
    }
    if (validateFSM(statesCount, fsm) == ERROR) {
        return 1;
    }
    return 0;
}

// validateFSM with states 1,2,3,0
// should pass
int test_validateFSM_state_0_is_last() {
    printf("Running test_validateFSM_state_0_is_last. Should pass.\n");
    struct State fsm[MAX_STATES];
    int statesCount = 4;
    for (int i = 0; i < statesCount-1; i++) {
        fsm[i].curState = i;
    }
    fsm[statesCount-1].curState = 0;
    if (validateFSM(statesCount, fsm) == ERROR) {
        return 1;
    }
    return 0;
}

// validateFSM with states 1-4
// should fail
int test_validateFSM_no_state_0() {
    printf("Running test_validateFSM_no_state_0. Should error.\n");
    struct State fsm[MAX_STATES];
    int statesCount = 4;
    for (int i = 0; i < statesCount; i++) {
        fsm[i].curState = i+1;
    }
    if (validateFSM(statesCount, fsm) == ERROR) {
        return 0;
    }
    return 1;
}
