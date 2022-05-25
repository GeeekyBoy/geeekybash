//
// Created by geeekyboy on 3/23/22.
//

#ifndef GEEEKYBASH_JOBQUEUE_H
#define GEEEKYBASH_JOBQUEUE_H

#include <wchar.h>
#include "../utils/string.h"

typedef struct Job {
    int foreground;
    String *cmd;
    String **argv;
    String *redirect;
    int redirectOverwrite;
    int pipeSender;
    int pipeReceiver;
    int precededByAnd;
    int precededByOr;
    int argc;
} Job;

typedef struct JobQueue {
    Job *job;
    struct JobQueue *next;
} JobQueue;

Job *nextJob();

void addJob(Job *job);

void freeJob(Job *job);

extern int runningCount;
extern int lastForegroundSucceeded;

#endif //GEEEKYBASH_JOBQUEUE_H
