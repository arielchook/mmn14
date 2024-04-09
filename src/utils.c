/**
 * @file utils.c
 * @brief Utility functions for string manipulation, memory allocation, and logging.
 */


#include <utils.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Allocates memory safely.
 *
 * This function allocates memory of a specified size and checks for allocation failure.
 * If memory allocation fails, the program prints an error message and exits. There is not point in trying to properly free all the allocated memory
 * thus far in such case as the os will free up all memory consumed by the program upon exit and this is a critical error.
 *
 * @param size The size of memory to allocate.
 * @return A pointer to the allocated memory.
 */
void *safe_malloc(int size)
{
    void *p = malloc(size);
    if (p == NULL)
    {
        printf(ERR_MEM_ALLOC_FAILED);
        exit(EXIT_FAILURE);
    }
    return p;
}

/**
 * @brief Trims leading whitespace characters from a string.
 *
 * @param str The string to trim.
 */
void ltrim(char *str)
{
    int i = 0;
    if (str == NULL)
        return;
    while (isspace(str[i]))
    {
        i++;
    }
    memmove(str, str + i, strlen(str) - i + 1);
}

/**
 * @brief Trims trailing whitespace characters from a string.
 *
 * @param str The string to trim.
 */
void rtrim(char *str)
{
    int i = strlen(str) - 1;
    if (str == NULL)
        return;

    while (i >= 0 && isspace(str[i]))
    {
        i--;
    }
    str[i + 1] = '\0';
}

/**
 * @brief Checks if a string starts with a specified prefix.
 *
 * @param str The string to check.
 * @param prefix The prefix to look for.
 * @return True if the string starts with the prefix, false otherwise.
 */
bool startsWith(const char *str, const char *prefix)
{
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

/**
 * @brief Checks if a string ends with a specified suffix.
 *
 * @param str The string to check.
 * @param suffix The suffix to look for.
 * @return True if the string ends with the suffix, false otherwise.
 */
bool endsWith(const char *str, const char *suffix)
{
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    if (str_len < suffix_len)
    {
        return 0;
    }
    return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}

/**
 * @brief Extracts the n-th word from a string, using space as the default separator.
 *
 * @param str The string from which to extract the word.
 * @param n The word position to extract (1-based).
 * @param pStart Pointer to where to store the starting position of the extracted word in the original string.
 * @return The extracted word, or NULL if n is out of bounds.
 */
char *extractWord(char *str, int n, char **pStart)
{
    return extractWordSeparator(str, n, pStart, SPACE);
}

/**
 * @brief Extracts the n-th word from a string, using a specified separator.
 * Note: if there are consecutive separators in the string, the function returns an empty string ("") for that word. This only applies
 * if the separator is not a space.
 *
 * @param str The string from which to extract the word.
 * @param n The word position to extract (1-based).
 * @param pStart Pointer to where to store the starting position of the extracted word in the original string.
 * @param separator The character used to separate words in the string.
 * @return The extracted word trimmed of leading and trailing whitespace, or NULL if n is out of bounds.
 */
char *extractWordSeparator(char *str, int n, char **pStart, char separator)
{
    int wordCount = 0;
    int length;
    char *start = NULL;
    char *end = NULL;
    int in_word = 0;
    char *result;

    while (*str)
    {
        if (*str != separator && !in_word)
        {
            in_word = 1;
            start = str;
        }
        else if (*str == separator)
        {
            if (in_word)
            {
                in_word = 0;
                wordCount++;
                if (wordCount == n)
                {
                    end = str;
                    break;
                }
            }
            /* Two consecutive non-space separators */
            else if (separator != SPACE)
            {
                wordCount++;
                if (wordCount == n)
                {
                    start = str;
                    end = str;
                    break;
                }
            }
        }
        str++;
    }

    if (in_word)
    {
        wordCount++;
        if (wordCount == n)
        {
            end = str;
        }
    }

    /* n-th word does not exist */
    if (wordCount < n || start == NULL || end == NULL)
    {
        return NULL;
    }

    length = end - start;
    result = (char *)safe_malloc(length + 1);

    strncpy(result, start, length);
    result[length] = '\0';

    /* this will return a pointer to the start of the word that was extracted */
    if (pStart != NULL)
    {
        *pStart = start;
    }

    /* Trim the result */
    ltrim(result);
    rtrim(result);

    return result;
}

/**
 * @brief Duplicates a string.
 *
 * @param str The string to duplicate.
 * @return A pointer to the duplicated string.
 */
char *strdup(const char *str)
{
    size_t len = strlen(str) + 1;
    char *dup_str = (char *)safe_malloc(len);
    strcpy(dup_str, str);
    return dup_str;
}

/**
 * @brief Logs a formatted message, intended for debugging purposes.
 *
 * Note: This function only operates when DEBUG is defined.
 *
 * @param format The format string for the message.
 * @param ... Variable number of arguments to format the message.
 */
void LOG(const char *format, ...)
{
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
#else
    // Do nothing if DEBUG is not defined.
#endif
}
