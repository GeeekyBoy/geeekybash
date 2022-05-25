//
// Created by geeekyboy on 3/23/22.
//

#ifndef GEEEKYBASH_SIGNAL_H
#define GEEEKYBASH_SIGNAL_H

void handleChildExit(pid_t childPid, int status);

void handleSignal(int signal);

#endif //GEEEKYBASH_SIGNAL_H
