/**
 * @file secondpass.c
 * @brief Contains functions related to the second pass of the assembler.
 *
 * This file provides functions for processing assembly code during the second pass of the assembler,
 * including validation of .entry statements, processing of assembly instructions, and finalization
 * of machine code output.
 *
 * @authors Ariel Cohen, Jonathan Transky
 */

#include <secondpass.h>
#include <utils.h>
#include <reserved.h>
#include <messages.h>
#include <machinecode.h>
#include <string.h>
#include <spopparser.h>
#include <symbols.h>

/**
 * @brief Checks if an entry statement is valid.
 *
 * Validates the symbol specified in an .entry statement by checking its presence and type
 * in the symbol table. .define symbols or external symbols cannot be used as .entry.
 *
 * @param entryStmt The entry statement to validate.
 * @param lineNumber The line number in the source file for error reporting.
 * @return True if the entry statement is valid, false otherwise.
 */
bool is_valid_entry(char *entryStmt, int lineNumber)
{
    SymbolBlock *sb;
    /* Locate the symbol specified in the .entry statement in the symbol table */
    ltrim(entryStmt);
    rtrim(entryStmt);
    sb = find_symbol(entryStmt);

    /* If no such symbol, it's an error */
    if (sb == NULL)
    {
        printf(ERR_ENTRY_SYMBOL_NONEXISTENT, lineNumber, entryStmt);
        return false;
    }
    /* Look at the type of symbol found */
    switch (sb->type)
    {
    /* If it's a .define - it's an error */
    case ST_DEFINE:
        printf(ERR_ENTRY_FOR_DEFINE_INVALID, lineNumber, entryStmt);
        return false;
    /* If it's an external symbol - error. Cannot have the same symbol as extern and entry */
    case ST_EXTERN:
        printf(ERR_ENTRY_EXTERN_SAME_NAME, lineNumber, entryStmt);
        return false;
    /* Any other type is OK */
    case ST_DATA:
    case ST_CODE:
    case ST_STRING:
        return true;
    }
    return true;
}

/**
 * @brief Processes a line of assembly code in the second pass of the assembler.
 *
 * Handles .define, .data, .string, and .extern directives, which were processed in the first pass,
 * and processes .entry directives and instructions.
 *
 * @param firstWord The first word of the line, potentially a label or a directive.
 * @param cmd The command or directive of the line.
 * @param pStart Pointer to the start of parameters or operands in the line.
 * @param hasLabel Indicates whether the line contains a label (1 for true, 0 for false).
 * @param lineNumber The line number in the source file for error reporting.
 * @return True if the line is processed successfully, false if an error is encountered.
 */
bool sp_process_line_internal(char *firstWord, char *cmd, char *pStart, int hasLabel, int lineNumber)
{
    const instruction_props *props;

    /* .define , .data and .string definition - was handled in first pass */
    /* .extern definition - was also handled in first-pass. in second pass we just keep track
       of the references to external symbols used in the code */
    if ((strcmp(cmd, directives[DATA]) == 0) || (strcmp(cmd, directives[STRING]) == 0) ||
        (strcmp(cmd, directives[EXTERN]) == 0) || (strcmp(firstWord, directives[DEFINE]) == 0))
    {
        return true;
    }

    /* .entry definition - need to make sure we don't have an .extern or a .define with the same name */
    if (strcmp(cmd, directives[ENTRY]) == 0)
    {
        return is_valid_entry(pStart + strlen(directives[ENTRY]), lineNumber);
    }

    /* Done with processing directives. From here on we process instructions */

    /* Locate the cmd part in the instruction table */
    props = get_instruction_props(cmd);

    /* Debug log the command */
    LOG("%s", cmd);

    return parse_operands(pStart + strlen(cmd), lineNumber, props);
}

/**
 * @brief Processes a single line of assembly code during the second pass.
 *
 * This function extracts the first word (possibly a label) and the command from a line of assembly code,
 * then calls sp_process_line_internal for further processing.
 *
 * @param line The line of assembly code to process.
 * @param lineNumber The current line number in the assembly source file for error reporting.
 * @return True if the line is processed successfully, false if an error is encountered.
 */
bool sp_process_line(char *line, int lineNumber)
{
    int hasLabel;
    char *firstWord, *cmd;
    char *pStart;
    bool success;

    /* Remove trailing whitespaces (including \n) */
    rtrim(line);

    /* Check whether it's a comment line and skip it if so */
    if (startsWith(line, directives[COMMENT]))
        return true;

    /* Assume no label */
    hasLabel = 0;

    /* Examine the first word in the line */
    firstWord = extractWord(line, 1, &pStart);
    /* Handle label definition - first word ends with : */
    if (endsWith(firstWord, LABEL_SUFFIX))
    {
        hasLabel = 1;
    }

    /* Get the 2nd word if there's a label definition or the 1st word if not */
    cmd = extractWord(line, (hasLabel + 1), &pStart);

    success = sp_process_line_internal(firstWord, cmd, pStart, hasLabel, lineNumber);

    free_if_not_null(firstWord);
    free_if_not_null(cmd);

    return success;
}

/**
 * @brief Executes the second pass of the assembler.
 *
 * This pass processes each line in the input file, primarily focusing on .entry directives
 * and instructions, and finalizing the machine code output.
 *
 * @param input File pointer to the input assembly source file.
 * @return True if the second pass completes successfully without errors, false otherwise.
 */
bool secondPass(FILE *input)
{
    bool success = true; /* Tracks if any errors occurred in the second pass */
    char line[MAX_LINE_LENGTH];
    int lineNumber;

    /* Reset the Instruction Counter since we are now writing to the actual memory array */
    resetIC();

    /* Get the next line from the input file, until we reach EOF */
    for (lineNumber = 1; fgets(line, MAX_LINE_LENGTH, input) != NULL; lineNumber++)
    {
        /* Process line by line. If one line fails processing, we keep going */
        success &= sp_process_line(line, lineNumber);
    }
    return success;
}
