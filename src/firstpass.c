#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <hashtable.h>
#include <messages.h>
#include <symbols.h>
#include <reserved.h>
#include <directives.h>
#include <machinecode.h>
#include <op_parser.h>

#define LABEL_SUFFIX ":"

bool processLine(char *line, int lineNumber)
{
    int hasLabel;
    char *firstWord, *cmd;
    char *pStart;
    int keepDC, keepIC;
    const instruction_props *props;

    /* check whether it's a comment line and skip it if so */
    if (startsWith(line, directives[COMMENT]))
        return true;

    /* assume no label */
    hasLabel = 0;

    /* examine the first word in the line */
    firstWord = extractWord(line, 1, &pStart);

    /* handle .define - constant definition */
    if (strcmp(firstWord, directives[DEFINE]) == 0)
    {
        return (handle_define(pStart + strlen(directives[DEFINE]), lineNumber));
    }

    /* handle label definition - first word ends with : */
    if (endsWith(firstWord, LABEL_SUFFIX))
    {
        hasLabel = 1;

        /* get rid of the : */
        firstWord[strlen(firstWord) - 1] = '\0';

        /* make sure the label is in legal format and that it doesn't already exist */
        if (!is_valid_symbol_name(firstWord, lineNumber))
        {
            return false;
        }
    }

    /* get the 2nd word if there's a label definition or the 1st word if not */
    cmd = extractWord(line, (hasLabel + 1), &pStart);
    if (cmd == NULL)
    {
        /* having a label only in a line is not allowed */
        printf(ERR_LABEL_WITH_NO_CMD, lineNumber);
        return false;
    }
    ltrim(cmd);
    rtrim(cmd);

    /* .data definition */
    if (strcmp(cmd, directives[DATA]) == 0)
    {
        /* need to save value of DC so we can add the correct address in case of a label */
        keepDC = DC;

        if (!handle_data(pStart + strlen(directives[DATA]), lineNumber))
            return false;

        if ((hasLabel == 1) && !add_data_label(firstWord, keepDC))
            return false;

        return true;
    }

    /* .string definition */
    if (strcmp(cmd, directives[STRING]) == 0)
    {
        /* need to save value of DC so we can add the correct address in case of a label */
        keepDC = DC;
        if (!handle_string(pStart + strlen(directives[STRING]), lineNumber))
            return false;
        if ((hasLabel == 1) && !add_data_label(firstWord, keepDC))
            return false;
        return true;
    }

    /* .entry definition should be handled in 2nd pass */
    if (strcmp(cmd, directives[ENTRY]) == 0)
    {
        return true;
    }

    /* .extern definition */
    if (strcmp(cmd, directives[EXTERN]) == 0)
    {
        /* need to save value of IC so we can add the correct address in case of a label */
        keepIC = IC;
        if (!handle_extern(pStart + strlen(directives[EXTERN]), lineNumber))
            return false;
        if ((hasLabel == 1) && !add_code_label(firstWord, keepIC))
            return false;
        return true;
    }

    /* done with processing directives. from here on we process instructions */

    /* if the line has a label, add it to the symbols table with the current IC value */
    if (hasLabel)
    {
        add_code_label(firstWord, IC);
    }

    /* locate the cmd part in the instruction table */
    props = get_instruction_props(cmd);
    /* if not found, it's an invalid command */
    if (props == NULL)
    {
        printf(ERR_UNKNOWN_CMD, lineNumber, cmd);
        return false;
    }
    printf("%s", cmd);
    if (!parse_operands(pStart + strlen(cmd), lineNumber, props))
    {
        return false;
    }

    return true;
}

bool firstPass(FILE *input)
{
    bool success = true; /* tracks if any errors occured in first-pass */
    char line[MAX_LINE_LENGTH];
    int lineNumber;

    /* get the next line from input file, until we reach EOF  */
    for (lineNumber = 1; fgets(line, MAX_LINE_LENGTH, input) != NULL; lineNumber++)
    {
        /* note that we don't have to remove all whitespaces in the beginning and end of the line
        since it was already done in the Precompile step. Also empty lines were skipped in precompile */
        rtrim(line);
        /* process line by line. if one line fails processing we keep going */
        success &= processLine(line, lineNumber);
    }
    return success;
}
