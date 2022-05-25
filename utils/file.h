//
// Created by geeekyboy on 3/24/22.
//

#ifndef GEEEKYBASH_FILE_H
#define GEEEKYBASH_FILE_H

#include "string.h"

FILE *openFile(String *path, const char *mode);

int openFd(String *path, int mode);

#endif //GEEEKYBASH_FILE_H
