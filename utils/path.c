//
// Created by geeekyboy on 3/23/22.
//

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "path.h"

String *decodeHome(String *c) {
    if (c != NULL) {
        if (c->value[0] == '~') {
            String *result = cToWc(getenv("HOME"));
            skipAndConcatString(result, c, 1);
            return result;
        }
    }
    return c;
}

String *encodeHome(String *c) {
    if (c != NULL) {
        String *home = cToWc(getenv("HOME"));
        if (stringStartsWith(c, home)) {
            String *result = newString(L"~", 1);
            skipAndConcatString(result, c, home->size);
            freeString(home);
            return result;
        }
        freeString(home);
    }
    return c;
}

int setCurrentDir(String *dest) {
    char *cPath = wcToC(dest);
    int result = chdir(cPath);
    free(cPath);
    return result;
}

int setCurrentDirToHome() {
    return chdir(getenv("HOME"));
}

String *getCurrentDir() {
    char *cwd = malloc(sizeof(char) * 4096);
    getcwd(cwd, 4096);
    String *result = cToWc(cwd);
    free(cwd);
    return result;
}

String *getHomePath() {
    return cToWc(getenv("HOME"));
}

String *getLogPath() {
    String *result = getHomePath();
    concatLiteral(result, L"/.geeekybash_log", 16);
    return result;
}

String *getHistoryPath() {
    String *result = getHomePath();
    concatLiteral(result, L"/.geeekybash_history", 20);
    return result;
}

String *getAbsolutePath(String *path) {
    if (path->value[0] == '/') {
        return dupString(path);
    } else if (path->value[0] == '~') {
        return decodeHome(path);
    } else {
        String *result = getCurrentDir();
        concatLiteral(result, L"/", 1);
        concatString(result, path);
        return result;
    }
}