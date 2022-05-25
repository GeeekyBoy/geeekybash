//
// Created by geeekyboy on 3/23/22.
//

#include <stdio.h>
#include "output.h"

void printLiteralError(const char *error) {
    printf("\e[1;31m%s\e[0m", error);
}

void printStringError(String *error) {
    printf("\e[1;31m");
    printString(error);
    printf("\e[0m");
}

void beep() {
    printf("\a");
}
