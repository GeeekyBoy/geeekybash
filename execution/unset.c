//
// Created by geeekyboy on 3/23/22.
//

#include "unset.h"
#include "../store/envVar.h"
#include "../io/output.h"

void executeUnset(Job *job) {
    if (job->argc == 1) {
        printLiteralError("unset: not enough arguments\n");
        if (job->foreground) {
            lastForegroundSucceeded = 0;
        }
    } else if (job->argc == 2) {
        unsetEnvVar(job->argv[1]);
        if (job->foreground) {
            lastForegroundSucceeded = 1;
        }
    } else {
        printLiteralError("unset: too many arguments\n");
        if (job->foreground) {
            lastForegroundSucceeded = 0;
        }
    }
}