#ifndef GEN_TOOLS_H
#define GEN_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define MAX_CHAR 256


int popFile (char *filename, int delete_ch);
void pushbackStr (char *str, char *push_ch, const int maxLen);

void clear (void);

void slowText (const char *str);
void stringInput (char input[], int maxLen);

void getTime (char *str, const int maxLen);

#endif
