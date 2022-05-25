//
// Created by geeekyboy on 3/23/22.
//

#ifndef GEEEKYBASH_ENVVAR_H
#define GEEEKYBASH_ENVVAR_H

#include <wchar.h>
#include "../utils/string.h"

typedef struct EnvVar {
    String *name;
    String *value;
} EnvVar;

typedef struct EnvVarLinkedList {
    EnvVar *envVar;
    struct EnvVarLinkedList *next;
    struct EnvVarLinkedList *prev;
} EnvVarLinkedList;

EnvVar *getEnvVar(String *name);

String *getEnvVarValue(String *name);

void addEnvVar(String *name, String *value);

void unsetEnvVar(String *name);

void loadEnvVars(char **envp);

void evalEnvVar(String *str);

#endif //GEEEKYBASH_ENVVAR_H
