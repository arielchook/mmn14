/**
 * @file directives.c
 * @brief Handles Assembly Language Directives.
 *
 * This file contains functions to process assembly language directives encountered during
 * the assembly process. Each function is responsible for a specific type of directive:
 * - .define: Sets constants in the symbol table.
 * - .data: Parses and stores data values in the data section.
 * - .string: Serializes strings into the data section, ensuring null termination.
 * - .extern: Registers external symbols for linkage.
 * - .entry: Marks symbols as entry points for external reference.
 *
 * The functions perform validation of the input and update relevant data structures,
 * such as symbol tables and lists of entries, accordingly. Error handling is included
 * to manage issues like incorrect formatting, invalid names, and value ranges.
 */


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

    if (strlen(defName) == 0)
    {
        printf(ERR_MISSING_DEFINE_NAME, lineNumber);
        free_if_not_null(defName);
        return false;
    }

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
        free_if_not_null(defName);

        return false;
    }

    /* try to convert to int */
    value = strtol(defVal, &ptr, 10);

    if (*ptr != '\0')
    {
        printf(ERR_DEFINE_VALUE_NOT_INT, lineNumber);
        free_if_not_null(defVal);
        free_if_not_null(defName);

        return false;
    }
    /* we don't need defVal anymore */
    free_if_not_null(defVal);

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

    /* check we don't have an empry .data command */
    if (strlen(dataStmt) == 0)
    {
        printf(ERR_MISSING_VALUE, lineNumber, directives[DATA]);
        return false;
    }

    /* parse data values one by one */
    for (valCnt = 1; ((value = extractWordSeparator(dataStmt, valCnt, NULL, OP_SEPARATOR)) != NULL); valCnt++)
    {
        if (strlen(value) == 0)
        {
            printf(ERR_MISSING_VALUE, lineNumber, directives[DATA]);
            free_if_not_null(value);
            return false;
        }

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
            /* Use the constant's value. note that it's already in two-complement form */
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

        /* no longer needed */
        free_if_not_null(value);

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
bool handle_string(char *stringStmt, int lineNumber)
{
    char *stringEnd;

    /* make sure we have something after the .string directive*/
    ltrim(stringStmt);
    if (strlen(stringStmt) == 0)
    {
        printf(ERR_MISSING_VALUE, lineNumber, directives[STRING]);
        return false;
    }
    /* Trim trailing whitespaces */
    rtrim(stringStmt);

    /* string value must be enclosed in quotes */
    if (!startsWith(stringStmt, STR_ENCLOSURE) || !endsWith(stringStmt, STR_ENCLOSURE))
    {
        printf(ERR_MISSING_QUOTES, lineNumber);
        return false;
    }

    /* Iterate through the string, serializing each character into the data section.
       The starting and ending quotes are ignored, and a '\0' character is written at the end. */
    /* Adjust pointer to skip the ending quote */
    stringEnd = stringStmt + strlen(stringStmt) - 1;
    /* Replace the ending quote with a null terminator to properly end the string in the data section */
    *stringEnd = '\0';
    /* Start from the character after the beginning quote */
    for (stringStmt++; stringStmt <= stringEnd; stringStmt++)
    {
        /* Error if the data section is full and cannot accommodate more data */
        if (!serialize_data_section(*stringStmt))
        {
            printf(ERR_DATA_SECTION_FULL);
            return false;
        }
    }

    /* Return true if the entire string was successfully serialized */
    return true;
}

/**
 * @brief Handles the .extern directive by adding the specified external symbol to the assembler's external symbol list.
 *        This function trims the input, validates that it is not empty, and checks the validity of the external symbol name.
 *        It then adds the symbol to the external symbols list for later reference during the assembly process. Note that
 *        currently, the function is designed to handle only one external symbol per .extern statement, and potential enhancements
 *        could include handling multiple symbols in a single statement.
 *
 * @param externStmt The external symbol statement to be processed, following the ".extern" directive.
 * @param lineNumber The line number in the source file where this directive is found, used for error reporting.
 * @return true If the external symbol was successfully processed and added to the external symbols list.
 * @return false If any error occurred during the processing, such as an empty external symbol statement or invalid symbol name.
 */
bool handle_extern(char *externStmt, int lineNumber)
{
    /* Trim leading whitespaces from the external symbol statement */
    ltrim(externStmt);
    if (strlen(externStmt) == 0)
    {
        printf(ERR_MISSING_VALUE, lineNumber, directives[EXTERN]); /* Error for empty external symbol statement */
        return false;
    }
    /* Trim trailing whitespaces from the external symbol statement */
    rtrim(externStmt);

    /* Validate the external symbol name */
    if (!is_valid_symbol_name(externStmt, lineNumber))
    {
        /* The symbol name is invalid; an error message is expected to be printed within is_valid_symbol_name */
        return false;
    }

    /* Add the validated external symbol to the external symbols list */
    add_extern(externStmt);

    return true; /* Return true as the external symbol has been successfully added */
}

/**
 * @brief Handles the .entry directive by adding the specified symbol to the list of entry symbols.
 *        The validation of the symbol as a valid entry point is deferred to the second pass of the assembler,
 *        at which point the entire symbol table is available for reference. This function trims the input,
 *        validates that it is not empty, and appends the symbol to the list of entries.
 *
 * @param entryStmt The entry statement to be processed, following the ".entry" directive.
 * @param lineNumber The line number in the source file where this directive is found, used for error reporting.
 * @return true If the entry statement was successfully processed and added to the list of entries.
 * @return false If any error occurred during the processing, such as an empty entry statement.
 */
bool handle_entry(char *entryStmt, int lineNumber)
{
    /* Trim whitespaces from the entry statement */
    ltrim(entryStmt);
    if (strlen(entryStmt) == 0)
    {
        printf(ERR_MISSING_VALUE, lineNumber, directives[ENTRY]);
        return false;
    }
    rtrim(entryStmt);

    /* Append the entry statement to the list of entries for later validation against the symbol table */
    entries_append(entryStmt);
    return true;
}
