//
// Created by geeekyboy on 3/23/22.
//

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "exit.h"
#include "../store/jobQueue.h"

void executeExit() {
    signal(SIGINT, SIG_IGN);
    kill(0, SIGINT);
    while (runningCount > 0) {
        sleep(1);
    }
    exit(0);
}