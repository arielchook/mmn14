#include <secondpass.h>
#include <stdio.h>
#include <stdbool.h>
#include <utils.h>
#include <reserved.h>
#include <messages.h>
#include <machinecode.h>
#include <string.h>
#include <spopparser.h>
#include <symbols.h>

bool is_valid_entry(char *entryStmt, int lineNumber)
{
    SymbolBlock *sb;
    /* locate the symbol specified in the .entry statement in the symbol table */
    ltrim(entryStmt);
    rtrim(entryStmt);
    sb = find_symbol(entryStmt);

    /* if no such symbol, it's an error */
    if (sb == NULL)
    {
        printf(ERR_ENTRY_SYMBOL_NONEXISTENT, lineNumber, entryStmt);
        return false;
    }
    /* look at the type of symbol found */
    switch (sb->type)
    {
    /* if it's a .define - it's an error */
    case ST_DEFINE:
        printf(ERR_ENTRY_FOR_DEFINE_INVALID, lineNumber, entryStmt);
        return false;
        break;
    /* if it's an external symbol - error. cannot have same symbol as extern and entry */
    case ST_EXTERN:
        printf(ERR_ENTRY_EXTERN_SAME_NAME, lineNumber, entryStmt);
        break;
    /* any other type is ok */
    case ST_DATA:
    case ST_CODE:
    case ST_STRING:
        return true;
    }
    return true;
}

bool sp_processLine(char *line, int lineNumber)
{
    int hasLabel;
    char *firstWord, *cmd;
    char *pStart;

    const instruction_props *props;

    /* remove trailing whitespaces (including \n) */
    rtrim(line);

    /* check whether it's a comment line and skip it if so */
    if (startsWith(line, directives[COMMENT]))
        return true;

    /* assume no label */
    hasLabel = 0;

    /* examine the first word in the line */
    firstWord = extractWord(line, 1, &pStart);

    /* .define - was already handled in first pass */
    if (strcmp(firstWord, directives[DEFINE]) == 0)
    {
        return true;
    }

    /* handle label definition - first word ends with : */
    if (endsWith(firstWord, LABEL_SUFFIX))
    {
        hasLabel = 1;
    }

    /* get the 2nd word if there's a label definition or the 1st word if not */
    cmd = extractWord(line, (hasLabel + 1), &pStart);
    ltrim(cmd);
    rtrim(cmd);

    /* .data and .string definition - was handled in first pass */
    /* .extern definition - was also handled in first-pass. in second pass we just keep track
    of the references to external symbols used in the code */
    if ((strcmp(cmd, directives[DATA]) == 0) || (strcmp(cmd, directives[STRING]) == 0) ||
        (strcmp(cmd, directives[EXTERN]) == 0))
    {
        return true;
    }

    /* .entry definition - need to make sure we don't have an extern with the same name */
    if (strcmp(cmd, directives[ENTRY]) == 0)
    {
        return is_valid_entry(pStart + strlen(directives[ENTRY]), lineNumber);
    }

    /* done with processing directives. from here on we process instructions */

    /* locate the cmd part in the instruction table */
    props = get_instruction_props(cmd);
    printf("%s", cmd);
    if (!parse_operands(pStart + strlen(cmd), lineNumber, props))
    {
        return false;
    }

    return true;
}

bool secondPass(FILE *input)
{
    bool success = true; /* tracks if any errors occured in second-pass */
    char line[MAX_LINE_LENGTH];
    int lineNumber;

    /* reset the Instruction Counter since we are now writing to the actual memory array */
    resetIC();

    /* get the next line from input file, until we reach EOF  */
    for (lineNumber = 1; fgets(line, MAX_LINE_LENGTH, input) != NULL; lineNumber++)
    {
        /* process line by line. if one line fails processing we keep going */
        success &= sp_processLine(line, lineNumber);
    }
    return success;
}