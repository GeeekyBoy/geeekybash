//
// Created by geeekyboy on 3/24/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "file.h"

FILE *openFile(String *path, const char *mode) {
    char *cPath = wcToC(path);
    FILE *file = fopen(cPath, mode);
    free(cPath);
    freeString(path);
    return file;
}

int openFd(String *path, int mode) {
    char *cPath = wcToC(path);
    int fd = open(cPath, mode, 0644);
    free(cPath);
    freeString(path);
    return fd;
}
