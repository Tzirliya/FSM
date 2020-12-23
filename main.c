#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "main.h"

//#define MAX_STATES 50;
//#define MAX_FILE_LINES 10000
//
//int loadStates(char* filename) {
//    printf("Processing FSM definition file.\n");
//    int countTransitions = 0;
//    // init graph
//    // open fsm file
//    FILE *fsmDefFile;
//    char fullFileName[19+9] = "FSMDefinitionFiles/";
//    strcat(fullFileName, filename);
//    fsmDefFile = fopen(fullFileName, "r");
//    if (fsmDefFile == NULL) {
//        printf("fopen unsuccessful.\n");
//        return -1;
//    }
//    int i = 0;
//    char *line;
//    while ((fscanf(fsmDefFile, "%s", line) != EOF) && i < MAX_FILE_LINES) {
//        char curChar;
//        // get curState
//        int j = 0;
//        int curState = 0;
//        curChar = line[j];
//        while (curChar != ":") {
//            if (curChar == "\0" || !isdigit(curChar)) {
//                printf("Syntax error in FSM definition file.\n");
//                return -1;
//            }
//            else {
//                curState *= 10;
//                curState += curChar;
//            }
//        }
//        // get input
//        j++;
//        char *input = "";
//        curChar = line[j];
//        while (curChar != ">") {
//            if (curChar == "\0") {
//                printf("Syntax error in FSM definition file.\n");
//                return -1;
//            }
//            input += curChar;
//        }
//        // get nextState
//        j++;
//        int nextState = 0;
//        curChar = line[j];
//        while (curChar != "\n") {
//            if (curChar == "\0" || !isdigit(curChar)) {
//                printf("Syntax error in FSM definition file.\n");
//                return -1;
//            }
//            else {
//                nextState *= 10;
//                nextState += curChar;
//            }
//        }
//        i++;
//    }
//    return 0;
//}


int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Error incorrect number of arguments.\n");
        return 1;
    }
//    // print main args
//    printf("argc: %d\n", argc);
//    printf("argv[0]: %s\n", argv[0]);
//    printf("argv[1]: %s\n", argv[1]);
//    printf("argv[2]: %s\n", argv[2]);

    // build fsm
    struct State fsm[MAX_STATES];
    if (loadFSM(argv[1], fsm)) {
        return 1;
    }

    // print fsm
    printFSM(fsm);

    return 0;
}
