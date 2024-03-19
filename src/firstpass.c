#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <hashtable.h>
#include <messages.h>
#include <symbols.h>
#include <reserved.h>
#include <directives.h>

bool processLine(char *line, int lineNumber) 
{
    int hasLabel;
    int num_words;
    char *firstWord, *cmd;

    /* check whether it's a comment line and skip it if so */
    if (startsWith(line,reserved_words[COMMENT])) return true;

    /* assume no label */
    hasLabel=0;

    /* examine the first word in the line */
    firstWord = extractWord(line, 1, &num_words);

    /* handle .define - constant definition */
    if (strcmp(firstWord, reserved_words[DEFINE]) == 0)
    {
        if (!handle_define(line+strlen(reserved_words[DEFINE]), lineNumber))
            return false;
    }

    /* handle label definition - first word ends with : */
    if (endsWith(firstWord,":"))
    { 
        hasLabel=1;

        /* get rid of the : */
        firstWord[strlen(firstWord)-1]='\0';

        /* make sure the label is in legal format and add it to the symbols map if so */
        if (!isValidSymbolName(firstWord, lineNumber)) 
        {
            return false;
        }
    }

    /* get the 2nd word if there's a label definition or the 1st word if not */
    if ((cmd=extractWord(line, (hasLabel+1), &num_words))==NULL)
    {
        /* having a label only in a line is not allowed */
        printf(ERR_LABEL_WITH_NO_CMD,lineNumber);
        return false;
    }

    /* .data definition */
    if (strcmp(cmd,reserved_words[DATA])==0)
    {
        if (!handle_data(cmd+strlen(reserved_words[DATA]), lineNumber)) return false;

        if ((hasLabel==1) && !add_data_label(firstWord)) return false;

        return true;
    }

    /* .string definition */
    if (strcmp(cmd,reserved_words[STRING])==0)
    {
        if (!handle_string(cmd+strlen(reserved_words[DATA]), lineNumber)) return false;
        if ((hasLabel==1) && !add_data_label(firstWord)) return false;
        return true;
    }

    /* .entry definition */
    if (strcmp(cmd,reserved_words[ENTRY])==0)
    {
        if (!handle_entry(cmd+strlen(reserved_words[ENTRY]), lineNumber)) return false;
        if ((hasLabel==1) && !add_code_label(firstWord)) return false;
        return true;
    }

    /* .extern definition */
    if (strcmp(cmd,reserved_words[EXTERN])==0)
    {
        if (!handle_extern(cmd+strlen(reserved_words[EXTERN]), lineNumber)) return false;
        if ((hasLabel==1) && !add_code_label(firstWord)) return false;
        return true;
    }
    return true;
}

bool firstPass(FILE *input)
{
    bool success=true; /* holds if any errors occured in first-pass */
    char line[MAX_LINE_LENGTH];
    int lineNumber;

    /* get the next line from input file, until we reach EOF  */
    for(lineNumber=1;fgets(line, MAX_LINE_LENGTH, input)!=NULL;lineNumber++)
    {
        /* note that we don't have to remove all whitespaces in the beginning and end of the line
        since it was already done in the Precompile step. Also empty lines were skipped in precompile */

        success&=processLine(line, lineNumber);
    }
    return success;
}
