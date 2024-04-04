#include <stdio.h>
#include <reserved.h>
#include <string.h>
#include <precomp.h>
#include <utils.h>
#include <macros.h>

bool precompile(FILE *input, FILE *output)
{
    bool success = true;
    bool inMacro = false;
    MacroBlock *m;
    char line[MAX_LINE_LENGTH];
    int lineNumber = 1;
    char *macroName, *firstWord, *anythingElse;

    /* get the next line from input file, until we reach EOF  */
    for (lineNumber = 1; fgets(line, MAX_LINE_LENGTH, input) != NULL; lineNumber++)
    {
        /* remove leading and trailing whitespaces*/
        ltrim(line);
        /* skip empty lines */
        if (strlen(line) == 0)
        {
            continue;
        }
        rtrim(line);

        /* extract the first 3 words in the line */
        firstWord = extractWord(line, 1, NULL);
        macroName = extractWord(line, 2, NULL);
        anythingElse = extractWord(line, 3, NULL);

        /* is it a macro definition*/
        if (strcmp(firstWord, directives[MCR]) == 0)
        {
            if (inMacro)
            {
                printf(PP_ERR_NO_NESTED_MACROS, lineNumber);
                success = false;
            }

            /* make sure we only have mcr and the macro name and nothing else */
            if (success && (anythingElse != NULL))
            {
                printf(PP_ERR_EXTRA_CHARS, lineNumber);
                success = false;
            }

            /* do we have a macro name? */
            if (success && (macroName == NULL))
            {
                printf(PP_ERR_INVALID_MACRO_NAME, lineNumber);
                success = false;
            }

            /* check whether macro name is a reserved word */
            if (success && is_reserved_word(macroName))
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
            /* if all good we have mcr <macro_name>  */
            if (success)
            {
                inMacro = true;
                m = safe_malloc(sizeof(MacroBlock));
                m->name = strdup(macroName);
            }
        }

        /* check if it's an endmcr command */
        else if (strcmp(firstWord, directives[ENDMCR]) == 0)
        {
            /* make sure it's the only command in the line. (macroName holds the 2nd word) */
            if (macroName != NULL)
            {
                printf(PP_ERR_EXTRA_ENDMCR, lineNumber);
                success = false;
            }

            /* make sure we are inside a macro definition */
            if (success && !inMacro)
            {
                printf(PP_ERR_ENDMCR_MISLOCATION, lineNumber);
                success = false;
            }

            /* all good - add the macro to the macro table */
            if (success)
            {
                add_macro(m);
                m = NULL;
                inMacro = false;
            }
        }
        else
        {
            /* are we inside a macro definition? if so, just accumulate the macro lines */
            if (inMacro)
            {
                macro_add_line(m, line);
            }
            /* not inside a macro definition. check if we are calling a macro */
            else
            {
                /* search for that macro */
                m = find_macro(firstWord);
                /* calling a macro can only be done with one word in the line */
                if (m != NULL)
                {
                    /* macroName holds the 2nd word. if there is a 2nd word in the line, it's an error */
                    if (macroName != NULL)
                    {
                        printf(PP_ERR_EXTRA_MACRO, lineNumber);
                        success = false;
                    }
                    else
                    {
                        /* write all the lines for the macro to the output file */
                        macro_write_lines(m, output);
                    }
                }
                else
                {
                    fprintf(output, "%s\n", line);
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
