//
// Created by geeekyboy on 3/23/22.
//

#include "job.h"
#include "cd.h"
#include "echo.h"
#include "export.h"
#include "unset.h"
#include "exit.h"
#include "command.h"

void executeJob() {
    Job *job = nextJob();
    if (job == NULL) {
        return;
    }
    if (job->precededByAnd && !lastForegroundSucceeded) {
        return;
    }
    if (job->precededByOr && lastForegroundSucceeded) {
        return;
    }
    if (compareLiteral(job->cmd, L"cd", 2)) {
        executeCd(job);
    } else if (compareLiteral(job->cmd, L"echo", 4)) {
        executeEcho(job);
    } else if (compareLiteral(job->cmd, L"export", 6)) {
        executeExport(job);
    } else if (compareLiteral(job->cmd, L"unset", 5)) {
        executeUnset(job);
    } else if (compareLiteral(job->cmd, L"exit", 4)) {
        executeExit();
    } else {
        executeCommand(job);
    }
    freeJob(job);
}
