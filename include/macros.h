#ifndef MACROS_H
#define MACROS_H
#include <linkedlist.h>
#include <stdbool.h>
#include <stdio.h>

/* describes an entry in the macros hashtable */
typedef struct
{
    char *name;
    LinkedList *lines_list;
    int num_lines;
} MacroBlock;

/* Add a macro to the macro hashtable */
bool add_macro(MacroBlock *m);
/* Finds a macro in the macro hashtable and returns it */
MacroBlock *find_macro(char *name);
/* Frees all memory allocated for the macro hashtable */
void free_macro_table();

void macro_add_line(MacroBlock *m, char *line);

void macro_write_lines(MacroBlock *m, FILE *output);
#endif