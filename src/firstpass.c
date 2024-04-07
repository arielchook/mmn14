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

/*
 * Internal function to process a line of assembly code, handling labels, directives, and commands.
 *
 * This function deals with assembly language constructs such as labels, directives like .define, .data, .string, .entry,
 * and .extern, as well as machine instructions. It performs validation checks on labels, interprets directives, and
 * prepares for the translation of instructions to machine code by counting operand words and ensuring syntax correctness.
 *
 * Parameters:
 * - firstWord: The first word in the line, which could be a label or a directive.
 * - cmd: The command or directive extracted from the line, following any label.
 * - pStart: Pointer to the start of the parameters or operands in the line.
 * - hasLabel: Indicator of whether the line contains a label (1 for true, 0 for false).
 * - lineNumber: The current line number in the assembly source file, used for error reporting.
 *
 * Returns:
 * - True if the line was processed successfully, false if an error was encountered.
 */
bool fp_process_line_internal(char *firstWord, char *cmd, char *pStart, int hasLabel, int lineNumber)
{
    const instruction_props *props;
    /* Handle .define directive - constant definition */
    if (strcmp(firstWord, directives[DEFINE]) == 0)
    {
        if (hasLabel) /* Labels are not allowed before a .define statement */
        {
            printf(ERR_DEFINE_WITH_LABEL, lineNumber);
            return false;
        }
        return handle_define(pStart + strlen(directives[DEFINE]), lineNumber);
    }

    /* Process labels */
    if (hasLabel)
    {
        firstWord[strlen(firstWord) - 1] = '\0'; /* Remove the colon from the label */

        /* Validate the label's format and uniqueness */
        if (!is_valid_symbol_name(firstWord, lineNumber))
        {
            return false;
        }
    }

    /* Ensure there is a command following the label, if present */
    if (cmd == NULL)
    {
        printf(ERR_LABEL_WITH_NO_CMD, lineNumber);
        return false;
    }

    /* Process .data directive */
    if (strcmp(cmd, directives[DATA]) == 0)
    {
        if (hasLabel && !add_data_label(firstWord)) /* Add label to symbol table with current data counter (DC) address */
            return false;

        return handle_data(pStart + strlen(directives[DATA]), lineNumber);
    }

    /* Process .string directive */
    if (strcmp(cmd, directives[STRING]) == 0)
    {
        if (hasLabel && !add_data_label(firstWord)) /* Add label to symbol table */
            return false;

        return handle_string(pStart + strlen(directives[STRING]), lineNumber);
    }

    /* Process .entry directive */
    if (strcmp(cmd, directives[ENTRY]) == 0)
    {
        if (hasLabel) /* Labels are not allowed before .entry */
        {
            printf(ERR_LABEL_BEFORE_ENTRY, lineNumber);
            return false;
        }
        return handle_entry(pStart + strlen(directives[ENTRY]), lineNumber);
    }

    /* Process .extern directive */
    if (strcmp(cmd, directives[EXTERN]) == 0)
    {
        if (hasLabel) /* Labels are not allowed before .extern */
        {
            printf(ERR_LABEL_BEFORE_EXTERN, lineNumber);
            return false;
        }
        return handle_extern(pStart + strlen(directives[EXTERN]), lineNumber);
    }

    /* Process machine instructions */
    if (hasLabel)
    {
        add_code_label(firstWord); /* Add label to symbol table with current instruction counter (IC) value */
    }

    props = get_instruction_props(cmd); /* Locate the command in the instruction table */
    if (props == NULL)                  /* Invalid command */
    {
        printf(ERR_UNKNOWN_CMD, lineNumber, cmd);
        return false;
    }

    /* Count operand words and perform initial parsing for instruction syntax correctness */
    return count_operands_words(pStart + strlen(cmd), lineNumber, props);
}

/*
 * Processes a single line of assembly code, identifying and handling labels, comments, and commands.
 *
 * This function extracts the first word (possible label) and the command from a line of assembly code,
 * then delegates the detailed processing to fp_process_line_internal. It manages memory for dynamically
 * allocated strings used during line processing.
 *
 * Parameters:
 * - line: The line of assembly code to process.
 * - lineNumber: The current line number in the assembly source file, used for error reporting.
 *
 * Returns:
 * - True if the line was processed successfully, false if an error was encountered.
 */
bool fp_process_line(char *line, int lineNumber)
{
    bool success;
    int hasLabel;
    char *firstWord = NULL, *cmd = NULL;
    char *pStart;

    if (startsWith(line, directives[COMMENT])) /* Skip comment lines */
        return true;

    /* Assume no label is present */
    hasLabel = 0;

    /* Extract the first word */
    firstWord = extractWord(line, 1, &pStart);
    /* Check if the first word is a label */
    if (endsWith(firstWord, LABEL_SUFFIX))
    {
        hasLabel = 1;
    }

    /* Extract the command, considering label presence */
    cmd = extractWord(line, (hasLabel + 1), &pStart);

    /* Delegate to fp_process_line_internal for detailed processing */
    success = fp_process_line_internal(firstWord, cmd, pStart, hasLabel, lineNumber);

    /* Free dynamically allocated memory */
    free_if_not_null(firstWord);
    free_if_not_null(cmd);

    return success;
}

/*
 * Performs the first pass of the assembler, processing each line in the input file.
 *
 * This function iterates through each line of the input assembly file, processing directives, labels,
 * and instructions. It sets the foundation for symbol table creation and prepares for the second pass
 * of assembly by updating symbol addresses and validating entries.
 *
 * Parameters:
 * - input: File pointer to the input assembly source file.
 *
 * Returns:
 * - True if the first pass completes successfully without errors, false otherwise.
 */
bool firstPass(FILE *input)
{
    bool success = true; /* Tracks the success of the first pass */
    char line[MAX_LINE_LENGTH];
    int lineNumber;

    for (lineNumber = 1; fgets(line, MAX_LINE_LENGTH, input) != NULL; lineNumber++) /* Read each line */
    {
        /* Remove trailing whitespace (precompile step has already processed leading whitespace and empty lines) */
        rtrim(line);
        /* Process each line and accumulate success status */
        success &= fp_process_line(line, lineNumber);
    }

    if (success) /* Update symbol addresses in the data section if no errors occurred */
    {
        update_data_symbols_address();
    }

    return success;
}
