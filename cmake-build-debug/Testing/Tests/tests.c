//
// Created by tplotkin on 12/21/20.
//
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "../../../main.h"
#define TEST_OUTPUT_FILE "Testing/testOutput.txt"

int test_charToIndex();
int test_charToIndex_basic();
int test_charToIndex_lowercase();

int test_indexToChar();
int test_indexToChar_basic();
int test_indexToChar_lowercase();

int test_loadFSM();
int test_loadFSM_basic();
int test_loadFSM_negative_curState();
int test_loadFSM_invalid_char_input();
int test_loadFSM_string_input();
int test_loadFSM_too_many_states();
int test_loadFSM_duplicate_inputs();

int test_getStateIndex();
int test_getStateIndex_basic();
int test_getStateIndex_state_does_not_exist();

int test_validateFSM();
int test_validateFSM_basic();
int test_validateFSM_state_0_is_last();
int test_validateFSM_no_state_0();

int test_runFSM();
int test_runFSM_basic();
int test_runFSM_state_out_of_order();
int test_runFSM_next_state_does_not_exist();
int test_runFSM_exceed_transition_limit();
int test_runFSM_invalid_input();
int test_runFSM_input_does_not_exist();

int main() {
    // redirect stderr to /dev/null
    int dn = open("/dev/null", O_WRONLY);
    dup2(dn, 2);
    close(dn);
    // run tests and count failures
    int failures = 0;
    failures += test_charToIndex();
    failures += test_indexToChar();
    failures += test_loadFSM();
    failures += test_getStateIndex();
    failures += test_validateFSM();
    failures += test_runFSM();
    printf("\n%d total tests failed.\n", failures);
    if (failures != 0) {
        return 1;
    }
    return 0;
}

// redirect stdout to a file
int setup() {
    fflush(stdout);
    int oldout = dup(1);
    int output = open(TEST_OUTPUT_FILE, O_WRONLY | O_CREAT, 0777);
    if (output == -1) {
        char e[256] = "opening";
        strcat(e, TEST_OUTPUT_FILE);
        perror(e);
    }
    dup2(output, 1);
    close(output);
    return oldout;
}

// redirect the redirected stdout back to stdout
void teardown(int oldout) {
    fflush(stdout);
    dup2(oldout, 1);
    close(oldout);
}

// clear a file
void clearFile(char *filename) {
    FILE* f = fopen(filename, "w");
    fclose(f);
}

// not a test
// manually build fsm according to tests1.fsm
void manuallyBuildFSM(struct State *fsm) {
    for (int i = 0; i < 4; i++) {
        fsm[i].curState = i;
        for (int j = 0; j < MAX_INPUT_TYPES; j++) {
            fsm[i].nextStates[j] = DEFAULT_STATE;
        }
    }
    fsm[0].nextStates[26] = 1;
    fsm[0].nextStates[27] = 1;
    fsm[0].nextStates[28] = 2;
    fsm[0].nextStates[29] = 3;
    fsm[1].nextStates[26] = 0;
    fsm[1].nextStates[27] = 2;
    fsm[1].nextStates[28] = 2;
    fsm[1].nextStates[29] = 3;
    fsm[2].nextStates[26] = 0;
    fsm[2].nextStates[27] = 1;
    fsm[2].nextStates[28] = 2;
    fsm[2].nextStates[29] = 3;
    fsm[3].nextStates[26] = 0;
    fsm[3].nextStates[27] = 1;
    fsm[3].nextStates[28] = 0;
    fsm[3].nextStates[29] = 0;
}

void manuallyBuildScrambledFSM(struct State *fsm) {
    fsm[0].curState = 2;
    fsm[1].curState = 0;
    fsm[2].curState = 3;
    fsm[3].curState = 1;
    fsm[2].nextStates[26] = 3;
    fsm[2].nextStates[27] = 0;
    fsm[2].nextStates[28] = 0;
    fsm[2].nextStates[29] = 1;
    fsm[0].nextStates[26] = 2;
    fsm[0].nextStates[27] = 3;
    fsm[0].nextStates[28] = 1;
    fsm[0].nextStates[29] = 2;
    fsm[3].nextStates[26] = 2;
    fsm[3].nextStates[27] = 0;
    fsm[3].nextStates[28] = 3;
    fsm[3].nextStates[29] = 2;
    fsm[1].nextStates[26] = 1;
    fsm[1].nextStates[27] = 2;
    fsm[1].nextStates[28] = 2;
    fsm[1].nextStates[29] = 3;
}

// compare if two fsm's are equal, assuming they have the same amount of states.
int equal(int statesCount, struct State *fsm1, struct State *fsm2) {
    for (int i = 0; i < statesCount; i++) {
        int foundState = 0;
        for (int j = 0; j < statesCount; j++) {
            if (fsm1[i].curState == fsm2[j].curState) {
                for (int k = 0; k < MAX_INPUT_TYPES; k++) {
                    if (fsm1[i].nextStates[k] != fsm2[j].nextStates[k]) {
                        return 0;
                    }
                }
                foundState = 1;
                break;
            }
        }
        if (!foundState) {
            return 0;
        }
    }
    return 1;
}

// all charToIndex tests
int test_charToIndex() {
    int failures = 0;
    failures += test_charToIndex_basic();
    failures += test_charToIndex_lowercase();
    printf("\n%d charToIndex tests failed.\n\n", failures);
    return failures;
}

// charToIndex uppercase letter
// should run
int test_charToIndex_basic() {
    printf("Running test_charToIndex_basic. Should run.\n");
    if (charToIndex('D') == 3) {
        return 0;
    }
    printf("test_charToIndex_basic failed.\n");
    return 1;
}

// charToIndex lowercase letter
// should run
int test_charToIndex_lowercase() {
    printf("Running test_charToIndex_lowercase. Should run.\n");
    if (charToIndex('d') == 29) {
        return 0;
    }
    printf("test_charToIndex_lowercase failed.\n");
    return 1;
}

// all indexToChar tests
int test_indexToChar() {
    int failures = 0;
    failures += test_indexToChar_basic();
    failures += test_indexToChar_lowercase();
    printf("\n%d indexToChar tests failed.\n\n", failures);
    return failures;
}

// charToIndex uppercase number
// should run
int test_indexToChar_basic() {
    printf("Running test_indexToChar_basic. Should run.\n");
    if (indexToChar(4) == 'E') {
        return 0;
    }
    printf("test_indexToChar_basic failed.\n");
    return 1;
}

// charToIndex lowercase number
// should run
int test_indexToChar_lowercase() {
    printf("Running test_indexToChar_lowercase. Should run.\n");
    if (indexToChar(30) == 'e') {
        return 0;
    }
    printf("test_indexToChar_lowercase failed.\n");
    return 1;
}

// all loadFSM tests
int test_loadFSM() {
    int failures = 0;
    failures += test_loadFSM_basic();
    clearFile(TEST_OUTPUT_FILE);
    failures += test_loadFSM_negative_curState();
    clearFile(TEST_OUTPUT_FILE);
    failures += test_loadFSM_invalid_char_input();
    clearFile(TEST_OUTPUT_FILE);
    failures += test_loadFSM_string_input();
    clearFile(TEST_OUTPUT_FILE);
    failures += test_loadFSM_too_many_states();
    clearFile(TEST_OUTPUT_FILE);
    failures += test_loadFSM_duplicate_inputs();
    clearFile(TEST_OUTPUT_FILE);
    printf("\n%d loadFSM tests failed.\n\n", failures);
    return failures;
}

// loadFSM with definition file from project description
// should run
int test_loadFSM_basic() {
    printf("Running test_loadFSM_basic. Should run.\n");
    struct State fsm[MAX_STATES];
    int oldout = setup();
    int statesCount = loadFSM("FSMDefinitionFiles/test1.fsm", fsm);
    teardown(oldout);
    struct State fsmExpected[MAX_STATES];
    manuallyBuildFSM(fsmExpected);
    if (statesCount != 4 || !equal(statesCount, fsm, fsmExpected)) {
        printf("test_loadFSM_basic failed.\n");
        printf("Output:\n");
        printFile(TEST_OUTPUT_FILE);
        return 1;
    }
    return 0;
}

// loadFSM with negative curState
// should fail
int test_loadFSM_negative_curState() {
    printf("Running test_loadFSM_negative_curState. Should error.\n");
    struct State fsm[MAX_STATES];
    int oldout = setup();
    int statesCount = loadFSM("FSMDefinitionFiles/test2.fsm", fsm);
    teardown(oldout);
    if (statesCount == ERROR) {
        return 0;
    }
    printf("test_loadFSM_negative_curState failed.\n");
    printf("Output:\n");
    printFile(TEST_OUTPUT_FILE);
    return 1;
}

// loadFSM with an input that's '#'
// should fail
int test_loadFSM_invalid_char_input() {
    printf("Running test_loadFSM_invalid_char_input. Should error.\n");
    struct State fsm[MAX_STATES];
    int oldout = setup();
    int statesCount = loadFSM("FSMDefinitionFiles/test3.fsm", fsm);
    teardown(oldout);
    if (statesCount == ERROR) {
        return 0;
    }
    printf("test_loadFSM_invalid_char_input failed.\n");
    printf("Output:\n");
    printFile(TEST_OUTPUT_FILE);
    return 1;
}

// loadFSM with an input that's "dd"
// should fail
int test_loadFSM_string_input() {
    printf("Running test_loadFSM_string_input. Should error.\n");
    struct State fsm[MAX_STATES];
    int oldout = setup();
    int statesCount = loadFSM("FSMDefinitionFiles/test4.fsm", fsm);
    teardown(oldout);
    if (statesCount == ERROR) {
        return 0;
    }
    printf("test_loadFSM_string_input failed.\n");
    printf("Output:\n");
    printFile(TEST_OUTPUT_FILE);
    return 1;
}

// loadFSM with 51 states
// should fail
int test_loadFSM_too_many_states() {
    printf("Running test_loadFSM_too_many_states. Should error.\n");
    struct State fsm[MAX_STATES];
    int oldout = setup();
    int statesCount = loadFSM("FSMDefinitionFiles/test5.fsm", fsm);
    teardown(oldout);
    if (statesCount == ERROR) {
        return 0;
    }
    printf("test_loadFSM_too_many_states failed.\n");
    printf("Output:\n");
    printFile(TEST_OUTPUT_FILE);
    return 1;
}

// loadFSM with an input used for a state twice
// should fail
int test_loadFSM_duplicate_inputs() {
    printf("Running test_loadFSM_duplicate_inputs. Should error.\n");
    struct State fsm[MAX_STATES];
    int oldout = setup();
    int statesCount = loadFSM("FSMDefinitionFiles/test6.fsm", fsm);
    teardown(oldout);
    if (statesCount == ERROR) {
        return 0;
    }
    printf("test_loadFSM_duplicate_inputs failed.\n");
    printf("Output:\n");
    printFile(TEST_OUTPUT_FILE);
    return 1;
}

// all getStateIndex tests
int test_getStateIndex() {
    int failures = 0;
    failures += test_getStateIndex_basic();
    failures += test_getStateIndex_state_does_not_exist();
    printf("\n%d getStateIndex tests failed.\n\n", failures);
    return failures;
}

// getStateIndex looking for state 5 with 4 states named 4-7
// should run
int test_getStateIndex_basic() {
    printf("Running test_getStateIndex_basic. Should run.\n");
    struct State fsm[MAX_STATES];
    int statesCount = 4;
    for (int i = 0; i < statesCount; i++) {
        fsm[i].curState = i+4;
    }
    if (getStateIndex(statesCount, 5, fsm) == 1) {
        return 0;
    }
    printf("test_getStateIndex_basic failed.\n");
    return 1;
}

// getStateIndex looking for state 2 with 4 states named 4-7
// should fail
int test_getStateIndex_state_does_not_exist() {
    printf("Running test_getStateIndex_state_does_not_exist. Should run.\n");
    struct State fsm[MAX_STATES];
    int statesCount = 4;
    for (int i = 0; i < statesCount; i++) {
        fsm[i].curState = i+4;
    }
    if (getStateIndex(statesCount, 2, fsm) == ERROR) {
        return 0;
    }
    printf("test_getStateIndex_state_does_not_exist failed.\n");
    return 1;
}

// all validateFSM tests
int test_validateFSM() {
    int failures = 0;
    failures += test_validateFSM_basic();
    failures += test_validateFSM_state_0_is_last();
    failures += test_validateFSM_no_state_0();
    printf("\n%d validateFSM tests failed.\n\n", failures);
    return failures;
}

// validateFSM with states 0-3 in order
// should run
int test_validateFSM_basic() {
    printf("Running test_validateFSM_basic. Should run.\n");
    struct State fsm[MAX_STATES];
    int statesCount = 4;
    for (int i = 0; i < statesCount; i++) {
        fsm[i].curState = i;
    }
    if (validateFSM(statesCount, fsm) == ERROR) {
        printf("test_validateFSM_basic failed.\n");
        return 1;
    }
    return 0;
}

// validateFSM with states 1,2,3,0
// should run
int test_validateFSM_state_0_is_last() {
    printf("Running test_validateFSM_state_0_is_last. Should run.\n");
    struct State fsm[MAX_STATES];
    int statesCount = 4;
    for (int i = 0; i < statesCount-1; i++) {
        fsm[i].curState = i;
    }
    fsm[statesCount-1].curState = 0;
    if (validateFSM(statesCount, fsm) == ERROR) {
        printf("test_validateFSM_state_0_is_last failed.\n");
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
    printf("test_validateFSM_no_state_0 failed.\n");
    return 1;
}

// all validateFSM tests
int test_runFSM() {
    int failures = 0;
    failures += test_runFSM_basic();
    clearFile(TEST_OUTPUT_FILE);
    failures += test_runFSM_state_out_of_order();
    clearFile(TEST_OUTPUT_FILE);
    failures += test_runFSM_next_state_does_not_exist();
    clearFile(TEST_OUTPUT_FILE);
    failures += test_runFSM_exceed_transition_limit();
    clearFile(TEST_OUTPUT_FILE);
    failures += test_runFSM_invalid_input();
    clearFile(TEST_OUTPUT_FILE);
    failures += test_runFSM_input_does_not_exist();
    clearFile(TEST_OUTPUT_FILE);
    printf("\n%d runFSM tests failed.\n\n", failures);
    return failures;
}

// runFSM according to test1.fms and test1.inputs
// should run
int test_runFSM_basic() {
    printf("Running test_runFSM_basic. Should run.\n");
    struct State fsm[MAX_STATES];
    int statesCount = 4;
    manuallyBuildFSM(fsm);
    int oldout = setup();
    if (runFSM(statesCount, "FSMInputFiles/test1.inputs", fsm) != 2) {
        teardown(oldout);
        printf("test_runFSM_basic failed.\n");
        printf("Output:\n");
        printFile(TEST_OUTPUT_FILE);
        return 1;
    }
    teardown(oldout);
    return 0;
}

// runFSM according to test1.inputs with states out of order
// should run
int test_runFSM_state_out_of_order() {
    printf("Running test_runFSM_state_out_of_order. Should run.\n");
    struct State fsm[MAX_STATES];
    int statesCount = 4;
    manuallyBuildScrambledFSM(fsm);
    int oldout = setup();
    if (runFSM(statesCount, "FSMInputFiles/test1.inputs", fsm) != 1) {
        teardown(oldout);
        printf("test_runFSM_state_out_of_order failed.\n");
        printf("Output:\n");
        printFile(TEST_OUTPUT_FILE);
        return 1;
    }
    teardown(oldout);
    return 0;
}

// runFSM according to test1.fms and test1.inputs where next state is a state that doesn't exist
// should fail
int test_runFSM_next_state_does_not_exist() {
    printf("Running test_runFSM_next_state_does_not_exist. Should error.\n");
    struct State fsm[MAX_STATES];
    int statesCount = 4;
    manuallyBuildFSM(fsm);
    fsm[1].nextStates[26] = 6;
    int oldout = setup();
    if (runFSM(statesCount, "FSMInputFiles/test1.inputs", fsm) == ERROR) {
        teardown(oldout);
        return 0;
    }
    teardown(oldout);
    printf("test_runFSM_next_state_does_not_exist failed.\n");
    printf("Output:\n");
    printFile(TEST_OUTPUT_FILE);
    return 1;
}

// runFSM according to test1.fms and test2.inputs where there are too many transitions
// should fail
int test_runFSM_exceed_transition_limit() {
    printf("Running test_runFSM_exceed_transition_limit. Should error.\n");
    struct State fsm[MAX_STATES];
    int statesCount = 4;
    manuallyBuildFSM(fsm);
    int oldout = setup();
    if (runFSM(statesCount, "FSMInputFiles/test2.inputs", fsm) == ERROR) {
        teardown(oldout);
        return 0;
    }
    teardown(oldout);
    printf("test_runFSM_exceed_transition_limit failed.\n");
    printf("Output:\n");
    printFile(TEST_OUTPUT_FILE);
    return 1;
}

// runFSM according to test1.fms and test3.inputs which has an invalid input
// should fail
int test_runFSM_invalid_input() {
    printf("Running test_runFSM_invalid_input. Should error.\n");
    struct State fsm[MAX_STATES];
    int statesCount = 4;
    manuallyBuildFSM(fsm);
    int oldout = setup();
    if (runFSM(statesCount, "FSMInputFiles/test3.inputs", fsm) == ERROR) {
        teardown(oldout);
        return 0;
    }
    teardown(oldout);
    printf("test_runFSM_invalid_input failed.\n");
    printf("Output:\n");
    printFile(TEST_OUTPUT_FILE);
    return 1;
}

// runFSM according to test1.fms and test4.inputs which has an input that doesn't exist for the current state
// should fail
int test_runFSM_input_does_not_exist() {
    printf("Running test_runFSM_input_does_not_exist. Should error.\n");
    struct State fsm[MAX_STATES];
    int statesCount = 4;
    manuallyBuildFSM(fsm);
    int oldout = setup();
    if (runFSM(statesCount, "FSMInputFiles/test3.inputs", fsm) == ERROR) {
        teardown(oldout);
        return 0;
    }
    teardown(oldout);
    printf("test_runFSM_input_does_not_exist failed.\n");
    printf("Output:\n");
    printFile(TEST_OUTPUT_FILE);
    return 1;
}
