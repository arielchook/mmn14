#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <messages.h>
#include <stdbool.h>

/* Maximum line length in the input file */
#define MAX_LINE_LENGTH 80

void *safe_malloc(int size);
#define free_if_not_null(x) \
    if (x != NULL)          \
        free(x);

/* Trim whitespace characters from the beginning of a string */
void ltrim(char *str);
/* Trim whitespace characters from the end of a string */
void rtrim(char *str);
/* Check if a string starts with a given prefix */
int startsWith(const char *str, const char *prefix);
/* Check if a string ends with a given suffix */
int endsWith(const char *str, const char *suffix);
/* Returns the number of words in a line*/
int numWords(const char *str);
/* Function to extract the n-th word from a string
 Returns the extracted word or NULL if n is out of bounds */
char *extractWord(char *str, int n, char **pStart);
char *extractWordSeparator(char *str, int n, char **pStart, char separator);
char *strdup(const char *str);
#endif