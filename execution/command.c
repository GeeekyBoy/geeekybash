//
// Created by geeekyboy on 3/23/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "command.h"
#include "../utils/path.h"
#include "../io/output.h"
#include "../utils/signal.h"
#include "../utils/file.h"

void executeCommand(Job *job) {
    pid_t child_id = fork();
    if (child_id == 0) {
        if (job->redirect != NULL) {
            String *path = getCurrentDir();
            concatLiteral(path, L"/", 1);
            concatString(path, job->redirect);
            int flags = job->redirectOverwrite ? O_CREAT | O_WRONLY | O_TRUNC : O_CREAT | O_WRONLY | O_APPEND;
            int fd = openFd(path, flags);
            if (fd == -1) {
                printLiteralError("bash: can't access the output file\n");
                exit(-1);
            }
            dup2(fd, STDOUT_FILENO);
        }
        if (job->pipeReceiver) {
            String *inPath = getCurrentDir();
            concatLiteral(inPath, L"/pipeIn", 7);
            FILE *pipeIn = openFile(inPath, "r+");
            String *outPath = getCurrentDir();
            concatLiteral(outPath, L"/pipeOut", 8);
            FILE *pipeOut = openFile(outPath, "w+");
            wchar_t ch;
            while ((ch = (wchar_t) fgetwc(pipeIn)) != EOF)
                fputwc(ch, pipeOut);
            fclose(pipeIn);
            fclose(pipeOut);
            outPath = getCurrentDir();
            concatLiteral(outPath, L"/pipeOut", 8);
            int pipeOutDescriptor = openFd(outPath, O_CREAT | O_RDONLY);
            close(STDIN_FILENO);
            dup(pipeOutDescriptor);
        }
        if (job->pipeSender) {
            String *path = getCurrentDir();
            concatLiteral(path, L"/pipeIn", 7);
            int pipeIn = openFd(path, O_WRONLY | O_CREAT | O_TRUNC);
            dup2(pipeIn, STDOUT_FILENO);
        }
        char **cArgv = argvToC(job->argv, job->argc);
        if (execvp(cArgv[0], cArgv) == -1) {
            for (int i = 0; i < job->argc; i++) {
                free(cArgv[i]);
            }
            free(cArgv);
            printStringError(job->cmd);
            printLiteralError(": command not found\n");
            exit(-1);
        }
    } else if (child_id > 0) {
        runningCount++;
        if (job->foreground) {
            int childExitStatus;
            waitpid(child_id, &childExitStatus, 0);
            lastForegroundSucceeded = childExitStatus == 0;
            handleChildExit(child_id, childExitStatus);
        } else {
            printf("[%d] %d\n", runningCount, child_id);
        }
    }
}