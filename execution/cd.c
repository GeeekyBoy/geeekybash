//
// Created by geeekyboy on 3/23/22.
//

#include <stdlib.h>
#include "cd.h"
#include "../utils/path.h"
#include "../io/output.h"

void executeCd(Job *job) {
    if (job->argc == 1) {
        String *homePath = cToWc(getenv("HOME"));
        setCurrentDir(homePath);
        freeString(homePath);
        if (job->foreground) {
            lastForegroundSucceeded = 1;
        }
    } else if (job->argc == 2) {
        String *absolutePath = getAbsolutePath(job->argv[1]);
        int result = setCurrentDir(absolutePath);
        freeString(absolutePath);
        if (result != 0) {
            printLiteralError("cd: ");
            printStringError(job->argv[1]);
            printLiteralError(": No such file or directory\n");
        }
        if (job->foreground) {
            lastForegroundSucceeded = result == 0;
        }
    } else {
        printLiteralError("cd: too many arguments\n");
        if (job->foreground) {
            lastForegroundSucceeded = 0;
        }
    }
}