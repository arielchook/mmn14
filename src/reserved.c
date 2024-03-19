#include <reserved.h>
#include <string.h>

char *reserved_words[] = {"mcr", "endmcr", ".define",";",".data",".string", ".entry", ".extern",
                                 "mov","cmp","add","sub","lea",
                                 "not","clr","inc","dec","jmp","bne","red","prn","jsr",
                                 "rts","hlt"};

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