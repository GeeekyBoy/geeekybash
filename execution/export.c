//
// Created by geeekyboy on 3/23/22.
//

#include "export.h"
#include "../store/envVar.h"
#include "../io/output.h"

void executeExport(Job *job) {
    if (job->argc == 1) {
        printLiteralError("export: not enough arguments\n");
        if (job->foreground) {
            lastForegroundSucceeded = 0;
        }
    } else if (job->argc == 2) {
        String *key = newString(L"", 0);
        String *value = newString(L"", 0);
        int keyDone = 0;
        for (int i = 0; i < job->argv[1]->size; i++) {
            if (job->argv[1]->value[i] == '=') {
                keyDone = 1;
            } else if (keyDone) {
                concatChar(value, job->argv[1]->value[i]);
            } else {
                concatChar(key, job->argv[1]->value[i]);
            }
        }
        addEnvVar(key, value);
        if (job->foreground) {
            lastForegroundSucceeded = 1;
        }
    } else {
        printLiteralError("export: too many arguments\n");
        if (job->foreground) {
            lastForegroundSucceeded = 0;
        }
    }
}