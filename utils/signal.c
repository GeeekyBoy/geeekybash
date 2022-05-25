//
// Created by geeekyboy on 3/23/22.
//

#include <stdio.h>
#include <stdlib.h>
#include "signal.h"
#include <sys/wait.h>
#include "path.h"
#include "../store/jobQueue.h"
#include "file.h"

void handleChildExit(pid_t childPid, int status) {
    if (childPid > 0) {
        FILE *pf = openFile(getLogPath(), "a");
        fprintf(pf, "[%d] exited with status %d\n", childPid, status);
        fflush(pf);
        fclose(pf);
        runningCount--;
    }
}

void handleSignal(int signal) {
    if (signal == SIGCHLD) {
        while (1) {
            int status;
            pid_t childPid = wait3(&status, WNOHANG, NULL);
            if (childPid > 0) {
                handleChildExit(childPid, status);
            } else {
                break;
            }
        }
    }
}
