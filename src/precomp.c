/**
 * @file precomp.h
 * @brief Precompiles the given input file by processing macros and writing the result to the output file.
 * 
 * This file contains the implementation of a function to precompile an input file by processing macros
 * and writing the resulting output to another file. The precompilation process involves reading the input
 * file line by line, identifying macro definitions and macro calls, expanding macros, and copying non-macro
 * lines to the output file. The function also handles error checking for nested macros, invalid macro names,
 * duplicate macros, and other precompilation errors.
 */


#include <stdio.h>
#include <reserved.h>
#include <string.h>
#include <precomp.h>
#include <utils.h>
#include <macros.h>

/**
 * @brief Precompiles the given input file by processing macros and writing the result to the output file.
 *
 * This function reads the input file line by line. It looks for macro definitions starting with 'mcr'
 * and ends with 'endmcr'. While processing macro definitions, it checks for errors like nested macros
 * or invalid macro names. If a line calls a defined macro, it expands that macro. All other lines are
 * copied to the output file as-is. The function also handles trimming whitespace and skipping empty lines.
 *
 * @param input Pointer to a FILE object that represents the input file to be precompiled.
 * @param output Pointer to a FILE object where the precompiled output will be written.
 * @return True if precompilation is successful without any errors, false otherwise.
 */
bool precompile(FILE *input, FILE *output)
{
    bool success = true;                        /* Indicates the success of the precompilation process. */
    bool inMacro = false;                       /* Flag to track if currently within a macro definition. */
    MacroBlock *m = NULL;                       /* Pointer to hold the current macro block being defined. */
    char line[MAX_LINE_LENGTH];                 /* Buffer to hold the current line from the input file. */
    char *macroName, *firstWord, *anythingElse; /* Variables to parse the line. */
    int lineNumber, len;

    /* Loop through each line in the input file until we reach EOF or any failure */
    for (lineNumber = 1; success && fgets(line, MAX_LINE_LENGTH, input) != NULL; lineNumber++)
    {
        /* if we got back MAX_LINE_LENGTH length string and the last character in the line is not \n it means the line is too long */
        len = strlen(line);
        if ((len >= MAX_LINE_LENGTH - 1) && (line[len - 1] != '\n'))
        {
            printf(PP_ERR_MAX_LENGTH_EXCEEDED, lineNumber, MAX_LINE_LENGTH);
            success = false;
            break;
        }

        ltrim(line); /* Trim leading whitespace. */
        if (strlen(line) == 0)
            continue; /* Skip empty lines. */
        rtrim(line);  /* Trim trailing whitespace. */

        /* Parse the current line into key components. */
        firstWord = extractWord(line, 1, NULL);
        macroName = extractWord(line, 2, NULL);
        anythingElse = extractWord(line, 3, NULL);

        /* Check if the line is a macro definition start ('mcr'). */
        if (strcmp(firstWord, directives[MCR]) == 0)
        {
            if (inMacro)
            {
                /* Error handling for nested macros. */
                printf(PP_ERR_NO_NESTED_MACROS, lineNumber);
                success = false;
            }
            else if (success && anythingElse != NULL)
            {
                /* Error handling for additional characters after macro name. */
                printf(PP_ERR_EXTRA_CHARS, lineNumber);
                success = false;
            }
            else if (success && macroName == NULL)
            {
                /* Error handling for missing macro name. */
                printf(PP_ERR_INVALID_MACRO_NAME, lineNumber);
                success = false;
            }
            else if (success && is_reserved_word(macroName))
            {
                printf(PP_ERR_RESERVED_WORD, lineNumber, macroName);
                success = false;
            }
            /* make sure this macro doesn't already exists */
            if (success && find_macro(macroName))
            {
                printf(PP_ERR_DUPLICATE_MACRO, lineNumber, macroName);
                success = false;
            }

            if (success)
            {
                /* Start of a new macro definition. */
                inMacro = true;
                m = safe_malloc(sizeof(MacroBlock));
                m->name = strdup(macroName);
            }
        }
        else if (strcmp(firstWord, directives[ENDMCR]) == 0)
        {
            /* Check if the line is the end of a macro definition ('endmcr'). */
            if (macroName != NULL)
            {
                /* Error handling for extra characters after 'endmcr'. */
                printf(PP_ERR_EXTRA_ENDMCR, lineNumber);
                success = false;
            }
            else if (!inMacro)
            {
                /* Error handling for 'endmcr' without a corresponding 'mcr'. */
                printf(PP_ERR_ENDMCR_MISLOCATION, lineNumber);
                success = false;
            }

            if (success)
            {
                /* Successfully ending a macro definition. */
                add_macro(m);
                m = NULL;
                inMacro = false;
            }
        }
        else
        {
            /* Process lines within macro definitions or expand macros. */
            if (inMacro)
            {
                /* Accumulate lines within a macro definition. */
                macro_add_line(m, line);
            }
            else
            {
                /* Check for macro calls and expand them. */
                m = find_macro(firstWord);
                if (m != NULL && macroName == NULL)
                {
                    /* Expand the macro. */
                    macro_write_lines(m, output);
                }
                else if (m == NULL)
                {
                    /* Write non-macro lines directly to the output. */
                    fprintf(output, "%s\n", line);
                }
                else
                {
                    /* Error handling for incorrect macro usage. */
                    printf(PP_ERR_EXTRA_MACRO, lineNumber);
                    success = false;
                }
            }
        }

        /* free any dynamically allocated memory in this loop cycle */
        free_if_not_null(firstWord);
        free_if_not_null(macroName);
        free_if_not_null(anythingElse);
    }

    /* just in case we exited the loop in the middle of a macro */
    if (m != NULL)
    {
        free_if_not_null(m->name);
        free_if_not_null(m);
    }

    /* free memory */
    free_macro_table();

    return success;
}
