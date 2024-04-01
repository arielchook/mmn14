#include <utils.h>
#include <ctype.h>
#include <utils.h>
#include <hashtable.h>
#include <messages.h>
#include <symbols.h>
#include <reserved.h>
#include <directives.h>
#include <machinecode.h>
#include <fpopparser.h>
#include <entries.h>

bool fp_process_line_internal(char *firstWord, char *cmd, char *pStart, int hasLabel, int lineNumber)
{
    const instruction_props *props;

    /* handle .define - constant definition */
    if (strcmp(firstWord, directives[DEFINE]) == 0)
    {
        /* cannot have a label before a .define statement */
        if (hasLabel)
        {
            printf(ERR_DEFINE_WITH_LABEL, lineNumber);
            return false;
        }
        return handle_define(pStart + strlen(directives[DEFINE]), lineNumber);
    }

    if (hasLabel)
    {
        /* get rid of the : */
        firstWord[strlen(firstWord) - 1] = '\0';

        /* make sure the label is in legal format and that it doesn't already exist */
        if (!is_valid_symbol_name(firstWord, lineNumber))
        {
            return false;
        }
    }

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
        /* if there's label we need to add a new data label to the symbol table with the current address of DC */
        if ((hasLabel == 1) && !add_data_label(firstWord))
            return false;

        return handle_data(pStart + strlen(directives[DATA]), lineNumber);
    }

    /* .string definition */
    if (strcmp(cmd, directives[STRING]) == 0)
    {
        /* if there's a label we need to add a new data label to the symbols table */
        if ((hasLabel == 1) && !add_data_label(firstWord))
            return false;

        return handle_string(pStart + strlen(directives[STRING]), lineNumber);
    }

    /* .entry definition - just add it to the list of entries */
    /* in 2nd pass we will make sure all entries in the list relate to existing symbols */
    if (strcmp(cmd, directives[ENTRY]) == 0)
    {
        /* Cannot have a label before .entry */
        if (hasLabel)
        {
            printf(ERR_LABEL_BEFORE_ENTRY, lineNumber);
            return false;
        }
        return handle_entry(pStart + strlen(directives[ENTRY]), lineNumber);
    }

    /* .extern definition */
    if (strcmp(cmd, directives[EXTERN]) == 0)
    {
        /* Cannot have a label before .extern */
        if (hasLabel)
        {
            printf(ERR_LABEL_BEFORE_EXTERN, lineNumber);
            return false;
        }
        return handle_extern(pStart + strlen(directives[EXTERN]), lineNumber);
    }

    /* done with processing directives. from here on we process instructions */

    /* if the line has a label, add it to the symbols table with the current IC value */
    if (hasLabel)
    {
        add_code_label(firstWord);
    }

    /* locate the cmd part in the instruction table */
    props = get_instruction_props(cmd);
    /* if not found, it's an invalid command */
    if (props == NULL)
    {
        printf(ERR_UNKNOWN_CMD, lineNumber, cmd);
        return false;
    }

    /* count the number of words the current instruction will hold in the code section */
    /* we also do initial parsing here making sure the number of operands is correct etc.. */
    return count_operands_words(pStart + strlen(cmd), lineNumber, props);
}

bool fp_process_line(char *line, int lineNumber)
{
    bool success;
    int hasLabel;
    char *firstWord = NULL, *cmd = NULL;
    char *pStart;

    /* debug print the line we process */
    LOG("%d:%s\n", lineNumber, line);

    /* check whether it's a comment line and skip it if so */
    if (startsWith(line, directives[COMMENT]))
        return true;

    /* assume no label */
    hasLabel = 0;

    /* examine the first word in the line */
    firstWord = extractWord(line, 1, &pStart);

    /* handle label definition - first word ends with : */
    if (endsWith(firstWord, LABEL_SUFFIX))
    {
        hasLabel = 1;
    }

    /* get the 2nd word if there's a label definition or the 1st word if not */
    cmd = extractWord(line, (hasLabel + 1), &pStart);

    /* we call fp_process_line_internal so it can return true/false freely and we can free
    all dynamically allocated strings in this function */
    success = fp_process_line_internal(firstWord, cmd, pStart, hasLabel, lineNumber);

    free_if_not_null(firstWord);
    free_if_not_null(cmd);

    return success;
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
        success &= fp_process_line(line, lineNumber);
    }

    if (success)
    {
        /* at this point we have IC set to the end of the code section. since the data section comes
        right after the code section we should update all the data/string symbols to their correct
        address by adding IC to each's address */
        update_data_symbols_address();
    }

    return success;
}
