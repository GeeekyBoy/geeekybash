//
// Created by geeekyboy on 3/23/22.
//

#ifndef GEEEKYBASH_PATH_H
#define GEEEKYBASH_PATH_H

#include "string.h"

String *decodeHome(String *c);

String *encodeHome(String *c);

int setCurrentDir(String *dest);

int setCurrentDirToHome();

String *getCurrentDir();

String *getLogPath();

String *getHistoryPath();

String *getAbsolutePath(String *path);

#endif //GEEEKYBASH_PATH_H
