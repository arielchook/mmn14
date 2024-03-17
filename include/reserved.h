#ifndef RESERVED_H
#define RESERVED_H

#include <stdlib.h>
#include <utils.h>

/* enum and a static array containing all reserved words */
enum reserved_idx
{
    MCR = 0,
    ENDMCR,
    MOV
};
static char *reserved_words[] = {"mcr", "endmcr", "mov", "define"};

bool is_reserved_word(char *cmd)
{
    int i;
    long num_words;
    if (cmd == NULL)
        return false;

    /* number of strings in our reserved_words array */
    num_words = sizeof(reserved_words) / sizeof(reserved_words[0]);

    /* compare against each reserved word. return true if found */
    for (i = 0; i < num_words; i++)
    {
        if (strcmp(reserved_words[i], cmd) == 0) 
            return true;
    }
    return false;
}

#endif