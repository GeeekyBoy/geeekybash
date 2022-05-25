//
// Created by geeekyboy on 3/23/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <wchar.h>
#include <fcntl.h>
#include "reader.h"
#include "../utils/path.h"
#include "output.h"
#include "../store/history.h"
#include "../store/envVar.h"

static String *buf = NULL;
static unsigned long bufPos = 0;
static int historyPos = -1;
static int isTerminalSetUp = 0;
static int isNextLine = 0;
static struct termios oldt;
static int oldf;

void setupTerminal() {
    if (!isTerminalSetUp) {
        struct termios newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        isTerminalSetUp = 1;
    }
}

void resetTerminal() {
    if (isTerminalSetUp) {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        isTerminalSetUp = 0;
        isNextLine = 0;
    }
}

void printPrefix() {
    if (isNextLine) {
        printf("\33[2K\r> ");
    } else {
        printf("\33[2K\r\e[1;33m%s@%s\e[0m:\e[1;34m", getenv("USER"), getenv("NAME"));
        String *currDir = getCurrentDir();
        String *homeEncoded = encodeHome(currDir);
        printString(homeEncoded);
        freeString(currDir);
        freeString(homeEncoded);
        printf("\e[0m$ ");
    }
}

void initializeBuffer() {
    if (buf == NULL) {
        buf = newString(L"", 0);
    }
}

void loadHistory(int up) {
    int currHistoryPos = historyPos;
    if (up) {
        if (historyPos != 0) {
            historyPos = historyPos - 1;
        } else {
            beep();
            return;
        }
    } else {
        historyPos = historyPos + 1;
    }
    String *history = getHistory(&historyPos);
    if ((!up && historyPos == currHistoryPos) || (currHistoryPos < 0 && historyPos == 0 && history->size == 0)) {
        beep();
        return;
    }
    freeString(buf);
    buf = NULL;
    initializeBuffer();
    if (history != NULL) {
        concatString(buf, history);
    }
    freeString(history);
    printPrefix();
    printString(buf);
    bufPos = buf->size;
}

int getCaretDelta() {
    int result = 0;
    for (unsigned long i = bufPos; i < buf->size; i++) {
        if (buf->value[i] > 0xFFF) {
            result += 2;
        } else {
            result++;
        }
    }
    return result;
}

void handleArrowUp() {
    loadHistory(1);
}

void handleArrowDown() {
    loadHistory(0);
}

void handleArrowLeft() {
    if (bufPos > 0) {
        int caretDelta = buf->value[--bufPos] > 0xFFF ? 2 : 1;
        printf("\033[%dD", caretDelta);
    } else {
        beep();
    }
}

void handleArrowRight() {
    if (bufPos < buf->size) {
        int caretDelta = buf->value[bufPos++] > 0xFFF ? 2 : 1;
        printf("\033[%dC", caretDelta);
    } else {
        beep();
    }
}

void handleBackspace() {
    if (bufPos > 0) {
        int shouldGoToTail = buf->size == bufPos;
        deleteChar(buf, &bufPos);
        printPrefix();
        printString(buf);
        printf("\033[%dD", getCaretDelta() - shouldGoToTail);
    } else {
        beep();
    }
}

void handleNewData(wchar_t data) {
    insertChar(buf, data, &bufPos);
    printPrefix();
    printString(buf);
    printf("\033[%dD", getCaretDelta() - 1);
}

String *handleSubmit() {
    bufPos = buf->size;
    historyPos = -1;
    appendHistory(buf);
    insertChar(buf, '\n', &bufPos);
    printPrefix();
    printString(buf);
    printf("\033[%dD", getCaretDelta() - 1);
    String *result = dupString(buf);
    freeString(buf);
    buf = NULL;
    bufPos = 0;
    return result;
}

String *readNextLine() {
    isNextLine = 1;
    return readInput();
}

String *readInput() {
    setupTerminal();
    initializeBuffer();
    fflush(stdin);
    fflush(stdout);
    printPrefix();
    while (1) {
        wchar_t lastChar;
        if ((lastChar = (wchar_t) getwc(stdin)) == 27) { // if the first value is esc
            getwc(stdin); // skip the [
            switch (getwc(stdin)) { // the real value
                case 'A':
                    handleArrowUp();
                    break;
                case 'B':
                    handleArrowDown();
                    break;
                case 'C':
                    handleArrowRight();
                    break;
                case 'D':
                    handleArrowLeft();
                    break;
            }
        } else if (lastChar == 127 || lastChar == 8) {
            handleBackspace();
        } else if (isPrintable(lastChar)) {
            handleNewData(lastChar);
        } else if (lastChar == '\n') {
            if (buf->size > 0) {
                String *result = handleSubmit();
                evalEnvVar(result);
                resetTerminal();
                return result;
            } else {
                printf("\n");
                printPrefix();
            }
        }
    }
}


