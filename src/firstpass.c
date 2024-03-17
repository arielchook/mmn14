#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <hashtable.h>

static int IC, DC = 0;


bool firstPass(FILE *input, FILE *output)
{
    bool success = true;
    char line[MAX_LINE_LENGTH];
    int lineNumber = 1, i;
    int num_words, definitionVal;
    char *definitionName, *firstWord;
    void *definitionValue;

    /* get the next line from input file, until we reach EOF  */
    while (fgets(line, MAX_LINE_LENGTH, input) != NULL)
    {
        /* remove all whitespace from the left*/
        ltrim(line);
        /* skip empty lines */
        if (strlen(line) == 0)
        {
            lineNumber++;
            continue;
        }
        /* remove all whitespaces from the right*/
        rtrim(line);

        /* examine the first word in the line */
        firstWord = extractWord(line, 1);
        num_words = numWords(line);
        /* in define */
        if (strcmp(firstWord, ".define") == 0)
        {
            if (num_words != 3)
            {
                printf(ERR_INVALID_DEFINE, lineNumber);
                success = false;
            }
            definitionName = extractWordSeparator(line, 2, ' ');
            definitionVal = extractWordSeparator(line, 3, "=");
            if (is_reserved_word(definitionName))
            {
                printf(ERR_RESERVED_WORD, lineNumber);
                success = false;
            }
            if(!isalpha(definitionName)) 
            {
                printf(ERR_DEF_NOT_STRING, lineNumber);
                success = false;
            }
            /*
                check if definition word is already defined
                if all ok, map name of definition to int value and continue to next line
            */

        }
    }
    /*replace all instances of definition word to value*/
    while (fgets(line, MAX_LINE_LENGTH, input) != NULL)
    {
        int idx = 0;
        while (extractWord(line, idx) != NULL)
        {
            if (strcmp(extractWord(line, idx), definitionWord) == 0)
                fputs(line, definitionValue);
            idx++;
        }
    }
}
