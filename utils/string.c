//
// Created by geeekyboy on 3/23/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include "string.h"

int isAlphanumeric(wchar_t c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

int isPrintable(wchar_t c) {
    return iswprint(c);
}

void insertChar(String *str, wchar_t insertion, unsigned long *pos) {
    str->value = realloc(str->value, sizeof(wchar_t) * (str->size + 2));
    for (unsigned long i = str->size; i >= *pos; i--) {
        str->value[i + 1] = str->value[i];
        if (i == 0) {
            break;
        }
    }
    str->value[*pos] = insertion;
    str->size++;
    *pos = *pos + 1;
}

void deleteChar(String *str, unsigned long *pos) {
    for (unsigned long i = *pos; i < str->size; i++) {
        str->value[i] = str->value[i + 1];
    }
    str->size--;
    str->value[str->size] = '\0';
    str->value = realloc(str->value, sizeof(wchar_t) * (str->size + 2));
    *pos = *pos - 1;
}

void printString(String *str) {
    for (int i = 0; str->value[i] != '\0'; i++) {
        printf("%lc", str->value[i]);
    }
}

char *wcToC(String *str) {
    char *result = malloc(sizeof(char) * 4096);
    result[0] = '\0';
    for (int i = 0; i < str->size; i++) {
        char temp[100] = {'\0'};
        wcrtomb(temp, str->value[i], NULL);
        strcat(result, temp);
    }
    return result;
}

String *cToWc(char *str) {
    String *result = malloc(sizeof(String));
    result->size = mbstowcs(NULL, str, 0);
    result->value = malloc(sizeof(wchar_t) * (result->size + 1));
    mbstowcs(result->value, str, result->size + 1);
    return result;
}

char **argvToC(String **argv, int argc) {
    char **result = malloc(sizeof(char*) * (argc + 1));
    result[argc] = NULL;
    for (int i = 0; i < argc; i++) {
        result[i] = wcToC(argv[i]);
    }
    return result;
}

String *newString(const wchar_t *str, unsigned long size) {
    String *result = malloc(sizeof(String));
    result->size = size;
    result->value = malloc(sizeof(wchar_t) * (result->size + 1));
    for (int i = 0; i < result->size; i++) {
        result->value[i] = str[i];
    }
    result->value[result->size] = '\0';
    return result;
}

void freeString(String *str) {
    free(str->value);
    free(str);
}

void emptyString(String *str) {
    str->size = 0;
    str->value[0] = '\0';
    str->value = realloc(str->value, sizeof(wchar_t) * 1);
}

String *dupString(String *str) {
    String *result = malloc(sizeof(String));
    result->size = str->size;
    result->value = malloc(sizeof(wchar_t) * (result->size + 1));
    for (int i = 0; i < result->size; i++) {
        result->value[i] = str->value[i];
    }
    result->value[result->size] = '\0';
    return result;
}

void moveString(String *dest, String *src) {
    dest->size = src->size;
    dest->value = realloc(dest->value, sizeof(wchar_t) * (dest->size + 1));
    for (int i = 0; i < dest->size; i++) {
        dest->value[i] = src->value[i];
    }
    dest->value[dest->size] = '\0';
    freeString(src);
}

void concatString(String *str, String *str2) {
    str->value = realloc(str->value, sizeof(wchar_t) * (str->size + str2->size + 1));
    for (int i = 0; i < str2->size; i++) {
        str->value[str->size + i] = str2->value[i];
    }
    str->value[str->size + str2->size] = '\0';
    str->size += str2->size;
}

void skipAndConcatString(String *str, String *str2, unsigned long skip) {
    if (skip < str2->size) {
        unsigned long newLen = str->size + str2->size - skip;
        str->value = realloc(str->value, sizeof(wchar_t) * (newLen + 1));
        for (unsigned long i = 0; i < str2->size - skip; i++) {
            str->value[str->size + i] = str2->value[i + skip];
        }
        str->value[newLen] = '\0';
        str->size = newLen;
    }
}

void concatLiteral(String *str, const wchar_t *str2, unsigned long size) {
    str->value = realloc(str->value, sizeof(wchar_t) * (str->size + size + 1));
    for (int i = 0; i < size; i++) {
        str->value[str->size + i] = str2[i];
    }
    str->value[str->size + size] = '\0';
    str->size += size;
}

void concatChar(String *str, wchar_t c) {
    str->value = realloc(str->value, sizeof(wchar_t) * (str->size + 2));
    str->value[str->size] = c;
    str->value[str->size + 1] = '\0';
    str->size++;
}

void insertString(String *str, String *str2, unsigned long pos) {
    str->value = realloc(str->value, sizeof(wchar_t) * (str->size + str2->size + 1));
    for (unsigned long i = str->size; i >= pos; i--) {
        str->value[i + str2->size] = str->value[i];
    }
    for (unsigned long i = 0; i < str2->size; i++) {
        str->value[pos + i] = str2->value[i];
    }
    str->size += str2->size;
}

void trimString(String *str) {
    unsigned long i = 0;
    while (str->value[i] == ' ') {
        i++;
    }
    unsigned long j = str->size - 1;
    while (str->value[j] == ' ') {
        j--;
    }
    str->size = j - i + 1;
    for (unsigned long k = 0; k < str->size; k++) {
        str->value[k] = str->value[i + k];
    }
    str->value[str->size] = '\0';
}

int stringStartsWith(String *str, String *str2) {
    if (str->size < str2->size) {
        return 0;
    }
    for (unsigned long i = 0; i < str2->size; i++) {
        if (str->value[i] != str2->value[i]) {
            return 0;
        }
    }
    return 1;
}

int compareString(String *str, String *str2) {
    if (str->size != str2->size) {
        return 0;
    }
    for (unsigned long i = 0; i < str->size; i++) {
        if (str->value[i] != str2->value[i]) {
            return 0;
        }
    }
    return 1;
}

int compareLiteral(String *str, const wchar_t *str2, unsigned long size) {
    if (str->size != size) {
        return 0;
    }
    for (unsigned long i = 0; i < str->size; i++) {
        if (str->value[i] != str2[i]) {
            return 0;
        }
    }
    return 1;
}
