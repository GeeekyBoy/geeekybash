//
// Created by geeekyboy on 3/23/22.
//

#include <stdlib.h>
#include <wchar.h>
#include "parser.h"
#include "../store/jobQueue.h"
#include "reader.h"

void parseInput(String *input) {
    int argc = 0;
    String **argv = malloc(sizeof(String *) * argc);
    String *redirectPath = NULL;
    int redirectFound = 0;
    int redirectOverwrite = 0;
    int isQuote = 0;
    int waitingForPipeReceiver = 0;
    int logicalAndFound = 0;
    int logicalOrFound = 0;
    argv = realloc(argv, sizeof(String *) * (argc + 1));
    argv[argc++] = newString(L"", 0);
    for (int i = 0; i < input->size; i++) {
        if (input->value[i] == '"') {
            if (!isQuote) {
                isQuote = 1;
            } else {
                isQuote = 0;
            }
        } else if (input->value[i] == ' ' && !isQuote && argv[argc - 1]->size > 0) {
            argv = realloc(argv, sizeof(String *) * (argc + 1));
            argv[argc++] = newString(L"", 0);
        } else if (input->value[i] == ' ' && !isQuote && redirectFound && redirectPath != NULL) {
            redirectFound = 0;
            redirectOverwrite = 0;
        } else if (input->value[i] == '>' && !isQuote) {
            redirectFound = 1;
            if (redirectPath != NULL) {
                freeString(redirectPath);
            }
            redirectPath = NULL;
            if (input->value[i + 1] == '>') {
                redirectOverwrite = 1;
                i++;
            } else {
                redirectOverwrite = 0;
            }
        } else if ((input->value[i] == '&' || input->value[i] == '\n') && isQuote) {
            String *nextLine = readNextLine();
            concatString(input, nextLine);
            freeString(nextLine);
        } else if ((input->value[i] == '&' || input->value[i] == '|' || input->value[i] == '\n') && !isQuote) {
            if (argv[argc - 1]->size > 0) {
                argv = realloc(argv, sizeof(String *) * (argc + 1));
                argv[argc] = NULL;
            } else {
                argc--;
                freeString(argv[argc]);
                argv[argc] = NULL;
            }
            Job *job = malloc(sizeof(Job));
            job->foreground = input->value[i] == '\n' || input->value[i + 1] == '&' || input->value[i] == '|';
            job->cmd = argv[0];
            job->argv = argv;
            job->argc = argc;
            job->redirect = redirectPath;
            job->pipeSender = input->value[i] == '|' && input->value[i + 1] != '|';
            job->pipeReceiver = waitingForPipeReceiver;
            job->redirectOverwrite = redirectOverwrite;
            job->precededByAnd = logicalAndFound;
            job->precededByOr = logicalOrFound;
            addJob(job);
            argc = 0;
            argv = malloc(sizeof(String *) * (argc + 1));
            argv[argc++] = newString(L"", 0);
            logicalAndFound = 0;
            logicalOrFound = 0;
            if (input->value[i] == '\n' || input->value[i + 1] == '\n') {
                break;
            } else if (input->value[i + 1] == '&') {
                logicalAndFound = 1;
                i++;
            } else if (input->value[i + 1] == '|') {
                logicalOrFound = 1;
                i++;
            } else if (input->value[i] == '|') {
                waitingForPipeReceiver = 1;
            }
        } else if (input->value[i] != ' ' || (input->value[i] == ' ' && isQuote)) {
            if (redirectFound) {
                if (redirectPath == NULL) {
                    redirectPath = newString(L"", 0);
                }
                concatChar(redirectPath, input->value[i]);
            } else {
                concatChar(argv[argc - 1], input->value[i]);
            }
        }
    }
    freeString(input);
    freeString(argv[0]);
    free(argv);
}
