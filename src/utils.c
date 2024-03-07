#include <utils.h>
#include <string.h>
#include <ctype.h>

void *safe_malloc(int size)
{
    void *p=malloc(size);
    if (p==NULL) 
    {
        fprintf(stderr,ERR_MEM_ALLOC_FAILED);
        exit(EXIT_FAILURE);
    }
    return p;
}

/* Trim whitespace characters from the beginning of a string */
void ltrim(char *str) {
    int i = 0;
    while (isspace(str[i])) {
        i++;
    }
    memmove(str, str + i, strlen(str) - i + 1);
}

/* Trim whitespace characters from the end of a string */
void rtrim(char *str) {
    int i = strlen(str) - 1;
    while (i >= 0 && isspace(str[i])) {
        i--;
    }
    str[i + 1] = '\0';
}

/* Check if a string starts with a given prefix */
int startsWith(const char *str, const char *prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

/* Check if a string ends with a given suffix */
int endsWith(const char *str, const char *suffix) {
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    if (str_len < suffix_len) {
        return 0;
    }
    return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}

int numWords(const char* str) {
    int count = 0;
    int inWord = 0;
    while (*str) {
        if (isspace(*str)) {
            inWord = 0;
        } else if (!inWord) {
            inWord = 1;
            count++;
        }
        str++;
    }
    return count;
}

/* Function to extract the n-th word from a string
 Returns the extracted word or NULL if n is out of bounds */
char* extractWord(const char* str, int n) {
    int word_count = 0,length;
    const char* start = NULL;
    const char* end = NULL;
    int in_word = 0;
    char* result;

    while (*str) {
        if (*str != ' ' && !in_word) {
            in_word = 1;
            start = str;
        } else if (*str == ' ' && in_word) {
            in_word = 0;
            word_count++;
            if (word_count == n) {
                end = str;
                break;
            }
        }
        str++;
    }

    if (in_word) {
        word_count++;
        if (word_count == n) {
            end = str;
        }
    }

    /* n-th word does not exist */
    if (word_count < n || start == NULL || end == NULL) {
        return NULL; 
    }

    length = end - start;
    result = (char*)safe_malloc(length + 1);

    strncpy(result, start, length);
    result[length] = '\0';

    return result;
}

char *strdup(const char *str) {
    /* Calculate the length of the input string */
    size_t len = strlen(str) + 1;

    /* Allocate memory for the duplicate string */
    char *dup_str = (char *)safe_malloc(len);

    /* Copy the original string into the duplicate */
    strcpy(dup_str, str);

    return dup_str;
}

