//
// Created by geeekyboy on 3/23/22.
//

#ifndef GEEEKYBASH_HISTORY_H
#define GEEEKYBASH_HISTORY_H

#include "../utils/string.h"

String *getHistory(int *index);

int isHistoryExists(String *str);

void appendHistory(String *str);

#endif //GEEEKYBASH_HISTORY_H
