#ifndef MACROS_H
#define MACROS_H

#include <stdlib.h>
#include <hashtable.h>
#include <utils.h>

/* Maximum line length in the as file */
#define MAX_LINE_LENGTH 80
/* Maximum number of lines in a macro */
#define MAX_LINES_IN_MACRO 100

/* describes an entry in the macros hashtable */
typedef struct
{
    char *name;
    char **lines;
    int num_lines;
} MacroBlock;

/* Add a macro to the macro hashtable */
bool add_macro(MacroBlock *m);
/* Finds a macro in the macro hashtable and returns it */
MacroBlock *find_macro(char *name);
/* Frees all memory allocated for the macro hashtable */
void free_macro_table();

/* Preprocess a .am file to generate a .as file where all macros are expanded */
bool preprocess(FILE *input, FILE *output);

#endif