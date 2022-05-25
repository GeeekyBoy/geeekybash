//
// Created by geeekyboy on 3/23/22.
//

#ifndef GEEEKYBASH_STRING_H
#define GEEEKYBASH_STRING_H

typedef struct String {
    int *value;
    unsigned long size;
} String;

int isAlphanumeric(int c);

int isPrintable(int c);

void insertChar(String *str, int insertion, unsigned long *pos);

void deleteChar(String *str, unsigned long *pos);

void printString(String *str);

char *wcToC(struct String *str);

String *cToWc(char *str);

char **argvToC(String **argv, int argc);

String *newString(const int *str, unsigned long size);

void freeString(String *str);

void emptyString(String *str);

String *dupString(String *str);

void moveString(String *dest, String *src);

void concatString(String *str, String *str2);

void skipAndConcatString(String *str, String *str2, unsigned long skip);

void concatChar(String *str, int c);

void concatLiteral(String *str, const int *str2, unsigned long size);

void insertString(String *str, String *str2, unsigned long pos);

void trimString(String *str);

int stringStartsWith(String *str, String *str2);

int compareString(String *str, String *str2);

int compareLiteral(String *str, const int *str2, unsigned long size);


#endif //GEEEKYBASH_STRING_H
