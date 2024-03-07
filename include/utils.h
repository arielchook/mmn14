#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <messages.h>
#include <stdbool.h>

void *safe_malloc(int size);
/* Trim whitespace characters from the beginning of a string */
void ltrim(char *str);
/* Trim whitespace characters from the end of a string */
void rtrim(char *str);
 /* Check if a string starts with a given prefix */
int startsWith(const char *str, const char *prefix);
/* Check if a string ends with a given suffix */
int endsWith(const char *str, const char *suffix);
/* Returns the number of words in a line*/
int numWords(const char* str);
/* Function to extract the n-th word from a string
 Returns the extracted word or NULL if n is out of bounds */
char* extractWord(const char* str, int n);
char *strdup(const char *str);
#endif