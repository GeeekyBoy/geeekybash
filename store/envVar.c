//
// Created by geeekyboy on 3/23/22.
//

#include <stdlib.h>
#include "envVar.h"

static EnvVarLinkedList *envVarHead = NULL;

EnvVar *getEnvVar(String *name) {
    EnvVarLinkedList *curr = envVarHead;
    while (curr != NULL) {
        if (compareString(curr->envVar->name, name)) {
            return curr->envVar;
        }
        curr = curr->next;
    }
    return NULL;
}

String *getEnvVarValue(String *name) {
    EnvVarLinkedList *curr = envVarHead;
    while (curr != NULL) {
        if (compareString(curr->envVar->name, name)) {
            return curr->envVar->value;
        }
        curr = curr->next;
    }
    return NULL;
}

void addEnvVar(String *name, String *value) {
    EnvVar *envVar = malloc(sizeof(EnvVar));
    envVar->name = name;
    envVar->value = value;

    EnvVarLinkedList *envVarLinkedList = malloc(sizeof(EnvVarLinkedList));
    envVarLinkedList->envVar = envVar;
    envVarLinkedList->next = NULL;
    envVarLinkedList->prev = NULL;

    if (envVarHead == NULL) {
        envVarHead = envVarLinkedList;
    } else {
        EnvVarLinkedList *curr = envVarHead;
        EnvVar *existingEnvVar = getEnvVar(name);
        if (existingEnvVar != NULL) {
            existingEnvVar->value = value;
        } else {
            while (curr->next != NULL) {
                curr = curr->next;
            }
            curr->next = envVarLinkedList;
            envVarLinkedList->prev = curr;
        }
    }
}

void unsetEnvVar(String *name) {
    EnvVarLinkedList *curr = envVarHead;
    while (curr != NULL) {
        if (compareString(curr->envVar->name, name)) {
            if (curr->prev != NULL) {
                curr->prev->next = curr->next;
            }
            if (curr->next != NULL) {
                curr->next->prev = curr->prev;
            }
            if (curr == envVarHead) {
                envVarHead = curr->next;
            }
            freeString(curr->envVar->name);
            freeString(curr->envVar->value);
            free(curr->envVar);
            free(curr);
            return;
        }
        curr = curr->next;
    }
}

void loadEnvVars(char **envp) {
    int i = 0;
    while (envp[i] != NULL) {
        String *key = newString(L"", 0);
        String *value = newString(L"", 0);
        int k = 0;
        int keyDone = 0;
        while(envp[i][k] != '\0') {
            if (envp[i][k] == '=') {
                keyDone = 1;
            } else if (keyDone) {
                concatChar(value, envp[i][k]);
            } else {
                concatChar(key, envp[i][k]);
            }
            k++;
        }
        addEnvVar(key, value);
        i++;
    }
}

void evalEnvVar(String *str) {
    if (str->value[0] != '\0') {
        String *evaluatedArg = newString(L"", 0);
        int dollarFound = 0;
        String *envVarName = NULL;
        for (unsigned long j = 0; j < str->size; j++) {
            if (str->value[j] == '$' && isAlphanumeric(str->value[j + 1])) {
                dollarFound = 1;
                if (envVarName != NULL) {
                    freeString(envVarName);
                }
                envVarName = newString(L"", 0);
            } else if (dollarFound) {
                if (isAlphanumeric(str->value[j])) {
                    concatChar(envVarName, str->value[j]);
                }
                if (!isAlphanumeric(str->value[j]) || (isAlphanumeric(str->value[j]) && j == str->size - 1)) {
                    String *envVarValue = getEnvVarValue(envVarName);
                    if (envVarValue != NULL) {
                        concatString(evaluatedArg, envVarValue);
                    }
                    if (!isAlphanumeric(str->value[j])) {
                        concatChar(evaluatedArg, str->value[j]);
                    }
                    freeString(envVarName);
                    envVarName = NULL;
                    dollarFound = 0;
                }
            } else {
                concatChar(evaluatedArg, str->value[j]);
            }
        }
        moveString(str, evaluatedArg);
    }
}