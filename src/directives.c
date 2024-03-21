#include <symbols.h>
#include <ctype.h>
#include <reserved.h>
#include <machinecode.h>

/**
 * @brief handle a .define statement. print out error messages and update the symbols table
 * accordingly.
 *
 * @param symbolStmt the statement string, after the ".define" part
 * @param lineNumber the current line number in the file we're processing
 * @return true if the operation was successful
 * @return false if any error occured while processing the statement
 */
bool handle_define(char *symbolStmt, int lineNumber)
{
    char *defName, *defVal;
    char *ptr;
    long value;

    /* trim the statement */
    ltrim(symbolStmt);
    if (strlen(symbolStmt) == 0)
    {
        printf(ERR_MISSING_DEFINE, lineNumber);
        return false;
    }
    rtrim(symbolStmt);

    /* is it in the format name=val ? */
    if ((defName = extractWordSeparator(symbolStmt, 1, NULL, '=')) == NULL)
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
    rtrim(defName);

    /* make sure constant name is valid */
    if (!is_valid_symbol_name(defName, lineNumber))
    {
        return false;
    }

    /* look for the value in the name=val */
    if ((defVal = extractWordSeparator(symbolStmt, 2, NULL, '=')) == NULL)
    {
        printf(ERR_MISSING_DEFINE_VALUE, lineNumber);
        return false;
    }

    /* try to convert to int */
    value = strtol(defVal, &ptr, 10);
    if (*ptr != '\0')
    {
        printf(ERR_DEFINE_VALUE_NOT_INT, lineNumber);
        return false;
    }

    /* store it in the symbols table if all is ok */
    add_define(defName, value);

    return true;
}
/**
 * @brief
 *
 * @param dataStmt
 * @param lineNumber
 * @return true
 * @return false
 */
bool handle_data(char *dataStmt, int lineNumber)
{
    int valCnt, intValue;
    SymbolBlock *sb;
    char *ptr;
    char *value;

    ltrim(dataStmt);
    for (valCnt = 1; ((value = extractWordSeparator(dataStmt, valCnt, NULL, ',')) != NULL); valCnt++)
    {
        /* trim whitespaces around each value */
        ltrim(value);
        if (strlen(value) == 0)
        {
            printf(ERR_MISSING_VALUE, lineNumber);
            return false;
        }
        rtrim(value);

        /* try to convert to int */
        intValue = strtol(value, &ptr, 10);

        /* this means it's a string. check if it's a name of a constant */
        if (*ptr != '\0')
        {
            sb = find_symbol(value);
            if (sb == NULL || sb->type != ST_DEFINE)
            {
                printf(ERR_CANT_FIND_DEFINE, lineNumber, value);
                return false;
            }

            /* at this point we found a constant with that name. use its value */
            intValue = *(int *)(sb->value);
        }

        /* write the value to the data section */
        if (!write_data_section(intValue))
        {
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
    if (strlen(stringStmt) == 0)
    {
        printf(ERR_MISSING_VALUE, lineNumber);
        return false;
    }
    rtrim(stringStmt);

    if (!startsWith(stringStmt, "\"") || !endsWith(stringStmt, "\""))
    {
        printf(ERR_MISSING_QUOTES, lineNumber);
        return false;
    }
    /* go through the string constant and write it to memory character by character, word by word */
    /* ignore the quotes and write the \0 at the end */
    stringEnd = stringStmt + strlen(stringStmt) - 1;
    *stringEnd = '\0';
    for (stringStmt++; stringStmt <= stringEnd; stringStmt++)
    {
        /* write the value to the data section */
        if (!write_data_section(*stringStmt))
        {
            printf(ERR_DATA_SECTION_FULL, lineNumber);
            return false;
        }
    }

    return true;
}

bool handle_extern(char *externStmt, int lineNumber)
{
    ltrim(externStmt);
    if (strlen(externStmt) == 0)
    {
        printf(ERR_MISSING_VALUE, lineNumber);
        return false;
    }
    rtrim(externStmt);

    /* make sure entry name is valid and that there are no duplicate symbols */
    if (!is_valid_symbol_name(externStmt, lineNumber))
    {
        return false;
    }

    add_extern(externStmt);

    return true;
}
