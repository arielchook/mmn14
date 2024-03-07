#include <macros.h>
#include <stdio.h>
#include <reserved.h>
#include <string.h>
#include <macros.h>
#include <utils.h>

/* this is the macro hashtable. it maps a macro name to an array of lines of code */
static Hashtable *macroTable = NULL;

bool add_macro(MacroBlock *m)
{
    MacroBlock *old;

    if (macroTable == NULL)
    {
        macroTable = hashtable_create();
    }
    old = hashtable_insert(macroTable, m->name, m);
    return (old == NULL);
}

MacroBlock *find_macro(char *name)
{
    /* if no macroTable it means no macros */
    if (macroTable == NULL)
    {
        return NULL;
    }
    return hashtable_get(macroTable, name);
}

void free_macro_table()
{
    if (macroTable!=NULL) hashtable_destroy(macroTable);
}

bool preprocess(FILE *input, FILE *output)
{
    bool success=true;
    bool inMacro=false;
    MacroBlock *m;
    char line[MAX_LINE_LENGTH];
    int lineNumber = 1,i;
    char *macroName, *firstWord;
    int num_words;

    /* get the next line from input file, until we reach EOF  */
    while (fgets(line,MAX_LINE_LENGTH,input)!=NULL) {

        /* remove leading and trailing whitespaces*/
        ltrim(line);
        /* skip empty lines */
        if(strlen(line)==0) {
            lineNumber++; 
            continue;
        }
        rtrim(line);

        /* examine the first word in the line */
        firstWord=extractWord(line,1);
        num_words=numWords(line);
        /* is it a macro definition*/
        if (strcmp(firstWord,reserved_words[MCR])==0) {
            if (inMacro) {
                printf(PP_ERR_NO_NESTED_MACROS,lineNumber);
                success=false;
            }
            /* make sure we only have mcr and the macro name */
            if (success && num_words>2) {
                printf(PP_ERR_EXTRA_CHARS,lineNumber);
                success=false;
            }
            /* extract the macro name */
            if (success && (macroName=extractWord(line,2))==NULL) {
                printf(PP_ERR_INVALID_MACRO_NAME,lineNumber);
                success=false;
            }
            /* check whether macro name is a reserved word */
            if (success && is_reserved_word(macroName)) {
                printf(PP_ERR_RESERVED_WORD,lineNumber, line);
                success=false;
            } 
            if (success) {
                inMacro=true;
                m=safe_malloc(sizeof(MacroBlock));
                m->name=macroName;
                /* m->lines is an array of strings containing the macro lines */
                m->lines=safe_malloc(MAX_LINE_LENGTH*MAX_LINES_IN_MACRO);
                m->num_lines=0;
            }
        /* check if it's an endmcr command */
        } else if (strcmp(firstWord, reserved_words[ENDMCR])==0) {
            /* make sure it's the only command in the line */
            if (num_words>1) {
                printf(PP_ERR_EXTRA_ENDMCR,lineNumber);
                success=false;
            }
            /* make sure we are inside a macro definition */
            if (success && !inMacro) {
                printf(PP_ERR_ENDMCR_MISLOCATION,lineNumber);
                success=false;
            }
            /* all good - add the macro to the macro table */
            if (success) {
                add_macro(m);
                inMacro=false;
            }
        } else {
            /* are we inside a macro definition? if so, just accumulate the macro lines */
            if (inMacro) {
                m->lines[m->num_lines]=strdup(line);
                m->num_lines++;

            /* not inside a macro definition. check if we are calling a macro */
            } else {
                /* calling a macro can only be done with one word in the line */
                /* search for that macro */
                if (num_words==1 && (m=find_macro(firstWord))!=NULL) {
                    /* write the macro lines to output */
                    for(i=0;i<m->num_lines;i++) {
                        fputs(m->lines[i], output);
                        fputs("\n",output);
                    }
                } else {
                    fputs(line, output);
                    fputs("\n",output);
                }
            }
        }
        lineNumber++;
    }
    free_macro_table();
    return success;
}