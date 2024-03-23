#include <utils.h>
#include <string.h>
#include <ctype.h>

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

/* Trim whitespace characters from the beginning of a string */
void ltrim(char *str)
{
    int i = 0;
    while (isspace(str[i]))
    {
        i++;
    }
    memmove(str, str + i, strlen(str) - i + 1);
}

/* Trim whitespace characters from the end of a string */
void rtrim(char *str)
{
    int i = strlen(str) - 1;
    while (i >= 0 && isspace(str[i]))
    {
        i--;
    }
    str[i + 1] = '\0';
}

/* Check if a string starts with a given prefix */
int startsWith(const char *str, const char *prefix)
{
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

/* Check if a string ends with a given suffix */
int endsWith(const char *str, const char *suffix)
{
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    if (str_len < suffix_len)
    {
        return 0;
    }
    return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}

int numWords(const char *str)
{
    int count = 0;
    int inWord = 0;
    while (*str)
    {
        if (isspace(*str))
        {
            inWord = 0;
        }
        else if (!inWord)
        {
            inWord = 1;
            count++;
        }
        str++;
    }
    return count;
}

/* Function to extract the n-th word from a string
 Returns the extracted word or NULL if n is out of bounds */
char *extractWord(char *str, int n, char **pStart)
{
    return extractWordSeparator(str, n, pStart, ' ');
}
/* FIXME: doesn't handle 2 consecutive seprators. should return an empty string in such a case */
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
        else if (*str == separator && in_word)
        {
            in_word = 0;
            wordCount++;
            if (wordCount == n)
            {
                end = str;
                break;
            }
        }
        str++;
    }

    if (in_word)
    {
        (wordCount)++;
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

    return result;
}

char *strdup(const char *str)
{
    /* Calculate the length of the input string */
    size_t len = strlen(str) + 1;

    /* Allocate memory for the duplicate string */
    char *dup_str = (char *)safe_malloc(len);

    /* Copy the original string into the duplicate */
    strcpy(dup_str, str);

    return dup_str;
}

/*
void main()
{
    char *s = "a,,b";
    printf("[%s]\n", extractWordSeparator(s, 2, NULL, ','));
}*/