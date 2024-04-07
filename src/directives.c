#include <symbols.h>
#include <ctype.h>
#include <reserved.h>
#include <machinecode.h>
#include <entries.h>

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
    int value;

    /* trim the statement */
    ltrim(symbolStmt);
    if (strlen(symbolStmt) == 0)
    {
        printf(ERR_MISSING_DEFINE, lineNumber);
        return false;
    }
    rtrim(symbolStmt);

    /* is it in the format name=val ? */
    defName = extractWordSeparator(symbolStmt, 1, NULL, EQUAL_SEPARATOR);

    if (defName == NULL)
    {
        printf(ERR_MISSING_EQUAL, lineNumber);
        return false;
    }

    ltrim(defName);
    if (strlen(defName) == 0)
    {
        printf(ERR_MISSING_DEFINE_NAME, lineNumber);
        free_if_not_null(defName);
        return false;
    }
    rtrim(defName);

    /* make sure constant name is valid */
    if (!is_valid_symbol_name(defName, lineNumber))
    {
        free_if_not_null(defName);
        return false;
    }

    /* look for the value in the name=val */
    if ((defVal = extractWordSeparator(symbolStmt, 2, NULL, EQUAL_SEPARATOR)) == NULL)
    {
        printf(ERR_MISSING_DEFINE_VALUE, lineNumber);
        return false;
    }

    /* try to convert to int */
    value = strtol(defVal, &ptr, 10);

    /* we don't need defVal anymore */
    free_if_not_null(defVal);

    if (*ptr != '\0')
    {
        printf(ERR_DEFINE_VALUE_NOT_INT, lineNumber);
        free_if_not_null(defName);

        return false;
    }

    /* value must be in the range we can represent in memory */
    if ((value < MIN_VALUE) || (value > MAX_VALUE))
    {
        printf(ERR_INT_OUT_OF_BOUNDS, lineNumber, value, MIN_VALUE, MAX_VALUE);
        free_if_not_null(defName);
        return false;
    }

    /* store it in the symbols table if all is ok */
    add_define(defName, value);
    free_if_not_null(defName);

    return true;
}

/**
 * @brief Handles the .data directive in assembly code.
 * 
 * Parses the .data directive values, converts them to integers or resolves constants using the symbol table,
 * and serializes the values to the data section of the machine code.
 * 
 * @param dataStmt The statement containing the .data directive values.
 * @param lineNumber The current line number in the assembly file for error reporting.
 * @return True if all values are processed and serialized successfully, False on any error.
 */
bool handle_data(char *dataStmt, int lineNumber)
{
    int valCnt, intValue;
    uint16_t twoc_intValue;
    SymbolBlock *sb;
    char *ptr;
    char *value;

    ltrim(dataStmt);
    for (valCnt = 1; ((value = extractWordSeparator(dataStmt, valCnt, NULL, OP_SEPARATOR)) != NULL); valCnt++)
    {
        /* FIXME: make sure it can handle consecutive separators, e.g., ,,, */
        ltrim(value); // Trim whitespaces around each value
        if (strlen(value) == 0)
        {
            printf(ERR_MISSING_VALUE, lineNumber, directives[DATA]);
            free_if_not_null(value);
            return false;
        }
        rtrim(value);

        /* Try to convert the value to an integer */
        intValue = strtol(value, &ptr, 10);

        /* Check if the value is a string indicating it might be a constant's name */
        if (*ptr != '\0')
        {
            sb = find_symbol(value);
            if (sb == NULL || sb->type != ST_DEFINE)
            {
                printf(ERR_CANT_FIND_DEFINE, lineNumber, value);
                free_if_not_null(value);
                return false;
            }
            /* Use the constant's value */
            twoc_intValue = sb->value;
        }
        else
        {
            /* Validate the integer value can be represented in memory */
            if ((intValue < MIN_VALUE) || (intValue > MAX_VALUE))
            {
                printf(ERR_INT_OUT_OF_BOUNDS, lineNumber, intValue, MIN_VALUE, MAX_VALUE);
                free_if_not_null(value);
                return false;
            }
            twoc_intValue = to_twos_complement(intValue);
        }

        free_if_not_null(value); // The value string is no longer needed

        /* Serialize the value to the data section */
        if (!serialize_data_section(twoc_intValue))
        {
            printf(ERR_DATA_SECTION_FULL);
            return false;
        }
    }

    return true;
}


/**
 * @brief Handles the .string directive by serializing the provided string into the data section of the assembler's memory.
 *        It trims the input, validates the presence of enclosing quotes, and then iterates through each character of the string,
 *        serializing them into the data section. It also handles and reports any errors encountered during the process.
 *
 * @param stringStmt The string statement to be processed, following the ".string" directive.
 * @param lineNumber The line number in the source file where this directive is found, used for error reporting.
 * @return true If the string was successfully processed and serialized into the data section.
 * @return false If any error occurred during the processing, such as missing string value, missing enclosing quotes, or if the data section is full.
 */
bool handle_string(char *stringStmt, int lineNumber) {
    char *stringEnd;

    ltrim(stringStmt); // Trim leading whitespaces
    if (strlen(stringStmt) == 0) {
        printf(ERR_MISSING_VALUE, lineNumber, directives[STRING]); // Error for empty string statement
        return false;
    }
    rtrim(stringStmt); // Trim trailing whitespaces

    // Validate the presence of starting and ending quotes
    if (!startsWith(stringStmt, STR_ENCLOSURE) || !endsWith(stringStmt, STR_ENCLOSURE)) {
        printf(ERR_MISSING_QUOTES, lineNumber); // Error for missing quotes
        return false;
    }

    /* Iterate through the string, serializing each character into the data section.
       The starting and ending quotes are ignored, and a '\0' character is written at the end. */
    stringEnd = stringStmt + strlen(stringStmt) - 1; // Adjust pointer to skip the ending quote
    *stringEnd = '\0'; // Replace the ending quote with a null terminator to properly end the string in the data section
    for (stringStmt++; stringStmt <= stringEnd; stringStmt++) { // Start from the character after the beginning quote
        if (!serialize_data_section(*stringStmt)) { // Serialize each character and check for errors
            printf(ERR_DATA_SECTION_FULL); // Error if the data section is full and cannot accommodate more data
            return false;
        }
    }

    return true; // Return true if the entire string was successfully serialized
}

bool handle_extern(char *externStmt, int lineNumber)
{
    /* FIXME: there could be more than one label in an .extern statement. */

    ltrim(externStmt);
    if (strlen(externStmt) == 0)
    {
        printf(ERR_MISSING_VALUE, lineNumber, directives[EXTERN]);
        return false;
    }
    rtrim(externStmt);

    /* make sure extern name is valid and that there are no duplicate symbols */
    if (!is_valid_symbol_name(externStmt, lineNumber))
    {
        return false;
    }

    add_extern(externStmt);

    return true;
}

bool handle_entry(char *entryStmt, int lineNumber)
{
    ltrim(entryStmt);
    if (strlen(entryStmt) == 0)
    {
        printf(ERR_MISSING_VALUE, lineNumber, directives[ENTRY]);
        return false;
    }
    rtrim(entryStmt);

    /* we check the entry statement refers to a valid symbol only in 2nd pass since only then
    we have the entire symbol table filled up. until then we just add it to the list of entries */
    entries_append(entryStmt);
    return true;
}
