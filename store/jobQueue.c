//
// Created by geeekyboy on 3/23/22.
//

#include <stdlib.h>
#include "jobQueue.h"
#include "../execution/job.h"

static JobQueue *queueHead = NULL;
static JobQueue *queueTail = NULL;

int runningCount = 0;
int lastForegroundSucceeded = 0;

void addJob(Job *job) {
    if (queueHead == NULL) {
        queueHead = malloc(sizeof(JobQueue));
        queueHead->job = job;
        queueHead->next = NULL;
        queueTail = queueHead;
        executeJob();
    } else if (queueHead->next == NULL) {
        queueHead->next = malloc(sizeof(JobQueue));
        queueHead->next->job = job;
        queueHead->next->next = NULL;
        queueTail = queueHead->next;
    } else {
        queueTail->next = malloc(sizeof(JobQueue));
        queueTail->next->job = job;
        queueTail->next->next = NULL;
        queueTail = queueTail->next;
    }
}

Job *nextJob() {
    if (queueHead == NULL) {
        return NULL;
    } else if (queueTail == NULL) {
        Job *job = queueHead->job;
        free(queueHead);
        queueHead = NULL;
        return job;
    } else {
        Job *job = queueHead->job;
        JobQueue *temp = queueHead;
        queueHead = queueHead->next;
        free(temp);
        if (queueHead == queueTail) {
            free(queueTail);
            queueTail = NULL;
        }
        return job;
    }
}

void freeJob(Job *job) {
    freeString(job->cmd);
    for (int i = 1; i < job->argc; i++) {
        freeString(job->argv[i]);
    }
    free(job->argv);
    if (job->redirect != NULL) {
        freeString(job->redirect);
    }
    free(job);
}


