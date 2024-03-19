#include <symbols.h>
#include <ctype.h>
#include <reserved.h>
#include <machinecode.h>

bool handle_define(char *symbolStmt, int lineNumber)
{
    char *defName, *defVal;
    int wordCount;
    char *ptr;
    long value;

    /* Parse the name=val part */
    ltrim(symbolStmt);

    /* check for .symbol statement w/o name=val */
    if (strlen(symbolStmt) == 0)
    {
        printf(ERR_MISSING_DEFINE, lineNumber);
        return false;
    }

    /* is it in the format name=val ? */
    if ((defName=extractWordSeparator(symbolStmt, 1, &wordCount, '=')) == NULL)
    {
        printf(ERR_MISSING_EQUAL, lineNumber);
        return false;
    }

    ltrim(defName);
    if (strlen(defName) == 0)
    {
        printf(ERR_MISSING_DEFINE_NAME, lineNumber);
        return false;
    }
    
    /* make sure constant name is legal */
    if (!isValidSymbolName(defName, lineNumber))
    {
        return false;
    }

    /* look for the value in the name=val */
    if ((defVal=extractWordSeparator(symbolStmt, 2, &wordCount, '=')) == NULL)
    {
        printf(ERR_MISSING_DEFINE_VALUE, lineNumber);
        return false;
    }

    /* try to convert to int */
    value = strtol(defVal, &ptr, 10);
    if (*ptr!='\0')
    {
        printf(ERR_DEFINE_VALUE_NOT_INT, lineNumber);
        return false;
    }

    /* store it in the symbols table if all is ok */
    add_define(defName, value);

    return true;
}

bool handle_data(char *dataStmt, int lineNumber)
{
    int wordCount, valCnt, intValue;
    SymbolBlock *sb;
    char *ptr;
    char *value;

    ltrim(dataStmt);
    
    for(valCnt=1;((value=extractWordSeparator(dataStmt, valCnt, &wordCount, ',')) != NULL);valCnt++)
    {
        ltrim(value);
        if (strlen(value) == 0)
        {
            printf(ERR_MISSING_VALUE, lineNumber);
            return false;
        }

        /* try to convert to int */
        intValue = strtol(value, &ptr, 10);
        /* this means it's a string. check if it's a name of a constant */
        if (*ptr!='\0')
        {
            sb=find_symbol(value);
            if (sb==NULL || sb->type!=ST_DEFINE) {
                printf(ERR_CANT_FIND_DEFINE, lineNumber, value);    
                return false;
            }

            /* at this point we found a constant with that name. use its value */
            intValue=*(int*)(sb->value);
        }        

        /* write the value to the data section */
        if (!writeToDataSection(intValue)) {
            printf(ERR_DATA_SECTION_FULL, lineNumber);  
            return false;
        }
    }
    return true;
}

bool handle_string(char *stringStmt, int lineNumber)
{
    char *stringEnd;

    ltrim(stringStmt);
    rtrim(stringStmt);
    if (!startsWith(stringStmt,"\"")||!endsWith(stringStmt,"\""))
    {
        printf(ERR_MISSING_QUOTES, lineNumber);
        return false;
    }
    /* go through the string constant and write it to memory character by character, word by word */
    /* also write the \0 at the end */
    stringEnd=stringStmt+strlen(stringStmt);
    for(*stringEnd='\0';stringStmt<=stringEnd;stringStmt++)
    {
        /* write the value to the data section */
        if (!writeToDataSection(*stringStmt)) {
            printf(ERR_DATA_SECTION_FULL, lineNumber);  
            return false;
        }
    }
   
    return true;
}

bool handle_entry(char *entryStmt, int lineNumber)
{
    return true;
}

bool handle_extern(char *externStmt, int lineNumber)
{
    return true;
}