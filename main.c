#include <stdio.h>
#include <sys/wait.h>
#include <locale.h>
#include "store/envVar.h"
#include "utils/signal.h"
#include "utils/path.h"
#include "io/parser.h"
#include "io/reader.h"

_Noreturn void shell() {
    do {
        parseInput(readInput());
    } while (1);
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv, char **envp) {
    setlocale(LC_ALL, "");
    printf("Welcome to GeeekyBash %lc\n", L'\x1F60E');
    printf("Type 'exit' to exit the shell %lc\n\n", L'\x1F680');
    loadEnvVars(envp);
    signal(SIGCHLD, handleSignal);
    setCurrentDirToHome();
    shell();
}
