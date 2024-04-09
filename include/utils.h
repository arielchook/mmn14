/**
 * @file util.h
 * @brief Utility Functions and Constants for Assembly Processing.
 *
 * This file contains utility functions and constants that support various
 * aspects of assembly language processing. It includes safe memory allocation,
 * string manipulation (trimming, checking prefixes/suffixes), word extraction,
 * and debugging support. Additionally, it defines commonly used constants
 * such as maximum line length, parsing characters for labels, operands,
 * immediate values, and array notation, as well as a custom boolean type.
 * These utilities facilitate parsing, memory management, and diagnostic
 * logging within the assembler.
 */


#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <messages.h>
#include <stdint.h>
#include <stdarg.h>

#define DEBUG

/* defines for bool */
#define bool uint8_t
#define true 1
#define false 0

/* Maximum line length in the input file + 1 more for the \n */
#define MAX_LINE_LENGTH 81

/** parsing constants */
#define LABEL_SUFFIX ":"           /*!< label definition suffix character */
#define EQUAL_SEPARATOR '='        /*!< separator in name=value */
#define OP_SEPARATOR ','           /*!< operands separator character */
#define IMMEDIATE_VALUE_PREFIX "#" /*!< Immediate values must start with this prefix */
#define ARRAY_OPEN_CHAR '['        /*!< Array subscription character */
#define ARRAY_CLOSE_CHAR "]"       /*!< Array suffix */
#define STR_ENCLOSURE "\""         /*!< strings enclosure characters */
#define SPACE ' '                  /* !< space separator character */

/**
 * @brief Allocates memory safely.
 * 
 * This function allocates memory of a specified size and checks for allocation failure.
 * If memory allocation fails, the program prints an error message and exits.
 * 
 * @param size The size of memory to allocate.
 * @return A pointer to the allocated memory.
 */
void *safe_malloc(int size);
#define free_if_not_null(x) \
    if (x != NULL)          \
    {                       \
        free(x);            \
        x = NULL;           \
    }

/**
 * @brief Trims leading whitespace characters from a string.
 * 
 * @param str The string to trim.
 */
void ltrim(char *str);

/**
 * @brief Trims trailing whitespace characters from a string.
 * 
 * @param str The string to trim.
 */
void rtrim(char *str);

/**
 * @brief Checks if a string starts with a specified prefix.
 * 
 * @param str The string to check.
 * @param prefix The prefix to look for.
 * @return True if the string starts with the prefix, false otherwise.
 */
bool startsWith(const char *str, const char *prefix);

/**
 * @brief Checks if a string ends with a specified suffix.
 * 
 * @param str The string to check.
 * @param suffix The suffix to look for.
 * @return True if the string ends with the suffix, false otherwise.
 */
bool endsWith(const char *str, const char *suffix);

/**
 * @brief Extracts the n-th word from a string, using space as the default separator.
 * 
 * @param str The string from which to extract the word.
 * @param n The word position to extract (1-based).
 * @param pStart Pointer to where to store the starting position of the extracted word in the original string.
 * @return The extracted word, or NULL if n is out of bounds.
 */
char *extractWord(char *str, int n, char **pStart);

/**
 * @brief Extracts the n-th word from a string, using a specified separator.
 * 
 * Note: This function doesn't handle consecutive separators and should return an empty string in such a case.
 * 
 * @param str The string from which to extract the word.
 * @param n The word position to extract (1-based).
 * @param pStart Pointer to where to store the starting position of the extracted word in the original string.
 * @param separator The character used to separate words in the string.
 * @return The extracted word, or NULL if n is out of bounds.
 */
char *extractWordSeparator(char *str, int n, char **pStart, char separator);

/**
 * @brief Duplicates a string.
 * 
 * @param str The string to duplicate.
 * @return A pointer to the duplicated string.
 */
char *strdup(const char *str);

/**
 * @brief debug logging function. prints to standard output only if the DEBUG symbol is defined
 *
 * @param format
 * @param ...
 */
void LOG(const char *format, ...);

#endif
