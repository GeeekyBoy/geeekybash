//
// Created by geeekyboy on 3/23/22.
//

#include <stdio.h>
#include "echo.h"

void executeEcho(Job *job) {
    String *output = newString(L"", 0);
    for (int i = 1; i < job->argc; i++) {
        if (i != 1) {
            concatLiteral(output, L" ", 1);
        }
        concatString(output, job->argv[i]);
    }
    printString(output);
    printf("\n");
    freeString(output);
    if (job->foreground) {
        lastForegroundSucceeded = 1;
    }
}