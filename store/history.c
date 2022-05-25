//
// Created by geeekyboy on 3/23/22.
//

#include <stdio.h>
#include <wchar.h>
#include "history.h"
#include "../utils/path.h"
#include "../utils/file.h"

String *getHistory(int *index) {
    FILE *fp = openFile(getHistoryPath(), "r");
    if (fp == NULL) {
        *index = 0;
        return newString(L"", 0);
    }
    String *result = newString(L"", 0);
    int lineNo = 0;
    int shouldReset = 0;
    wchar_t lastChar;
    while((lastChar = (wchar_t) fgetwc(fp)) != EOF) {
        if (lastChar == '\n' || lastChar == '\0') {
            if (lineNo == *index) {
                break;
            } else {
                shouldReset = 1;
            }
        } else {
            if (shouldReset) {
                lineNo++;
                shouldReset = 0;
                emptyString(result);
                result = newString(L"", 0);
            }
            concatChar(result, lastChar);
        }
    }
    fclose(fp);
    *index = lineNo;
    return result;
}

int isHistoryExists(String *str) {
    FILE *fp = openFile(getHistoryPath(), "r");
    if (fp == NULL) {
        return 0;
    }
    String *result = newString(L"", 0);
    wchar_t lastChar;
    while((lastChar = (wchar_t) fgetwc(fp)) != EOF) {
        if (lastChar == '\n' || lastChar == '\0') {
            if (compareString(result, str)) {
                fclose(fp);
                freeString(result);
                return 1;
            } else {
                emptyString(result);
            }
        } else {
            concatChar(result, lastChar);
        }
    }
    fclose(fp);
    freeString(result);
    return 0;
}

void appendHistory(String *str) {
    if (!isHistoryExists(str)) {
        FILE *fp = openFile(getHistoryPath(), "a");
        if (fp == NULL) {
            return;
        }
        fputws(str->value, fp);
        fputws(L"\n", fp);
        fclose(fp);
    }
}
