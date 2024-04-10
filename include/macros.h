/**
 * @file macros.h
 * @brief Macro Management in Assembly Processing.
 *
 * Defines structures and functions for managing macros in assembly
 * programming. A macro, represented by a MacroBlock struct, consists
 * of a name and a list of lines. This file provides functionalities
 * to add macros to a table, find macros by name, add lines to macros,
 * and write macro lines to a file. The operations ensure efficient
 * handling and retrieval of macros, which are commonly used for code
 * reuse and simplification in assembly language.
 *
 * @authors Ariel Cohen, Jonathan Transky
 */

#ifndef MACROS_H
#define MACROS_H
#include <linkedlist.h>
#include <utils.h>
#include <stdio.h>

/**
 * @brief represents a macro in the macro table
 *
 */
typedef struct
{
    char *name;             /**< name of the macro */
    LinkedList *lines_list; /**< linked list of lines @see linkedlist.h */
    int num_lines;          /**< number of lines in the macro (=size of the linked list) */
} MacroBlock;

/**
 * @brief adds a macro to the macro table
 *
 * @param[in] m the macro to be added
 * @return true if this a new addition to the table
 * @return false if there was already a macro with the same name
 */
bool add_macro(MacroBlock *m);

/**
 * @brief Finds a macro in the macro table by name and returns it
 *
 * @param name name of the macro to be located
 * @return MacroBlock* pointer to the @ref "MacroBlock" struct or NULL if not found
 */
MacroBlock *find_macro(char *name);

/**
 * @brief Frees all memory allocated for the macro hashtable. It also frees
 * the linked list implemented for the macro's lines.
 *
 */
void free_macro_table(void);

/**
 * @brief Adds a line to a given macro. Each macro's lines are implemented in a linkedlist.
 * @see linkedlist.h
 *
 * @param m the macro to add the line to
 * @param line the line to be added
 */
void macro_add_line(MacroBlock *m, char *line);

/**
 * @brief Writes the macro's lines into a file
 *
 * @param m the macro whose lines to be printed
 * @param output the file to write the lines to (must be opened for writing)
 */
void macro_write_lines(MacroBlock *m, FILE *output);

#endif
