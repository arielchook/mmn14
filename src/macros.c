#include <macros.h>
#include <utils.h>
#include <hashtable.h>
#include <stdio.h>

/* Global variable for storing macros. It maps macro names to their respective code lines. */
static Hashtable *macroTable = NULL;

/**
 * @brief Adds a macro to the global macro table.
 * 
 * If the macro table doesn't exist yet, it's created. This function attempts to insert
 * a new macro into the table. If a macro with the same name already exists, it's overwritten.
 * 
 * @param m Pointer to the MacroBlock representing the macro to add.
 * @return True if the macro was successfully added without replacing an existing one, false otherwise.
 */
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

/**
 * @brief Retrieves a macro from the global macro table by name.
 * 
 * If the macro table hasn't been initialized or the macro doesn't exist, NULL is returned.
 * 
 * @param name The name of the macro to find.
 * @return Pointer to the MacroBlock of the found macro, or NULL if not found.
 */
MacroBlock *find_macro(char *name)
{
    if (macroTable == NULL)
    {
        return NULL;
    }
    return hashtable_get(macroTable, name);
}

/**
 * @brief Adds a line of code to a macro's line list.
 * 
 * Initializes the line list if it doesn't exist yet and appends the new line to it.
 * This function also increments the macro's line count.
 * 
 * @param m Pointer to the MacroBlock to which the line will be added.
 * @param line The line of code to add to the macro.
 */
void macro_add_line(MacroBlock *m, char *line)
{
    if (m == NULL)
        return;
    if (m->lines_list == NULL)
    {
        m->lines_list = linked_list_create();
        m->num_lines = 0;
    }
    linked_list_append(m->lines_list, strdup(line));
    m->num_lines++;
}

/**
 * @brief Helper function to write a macro line to a file.
 * 
 * Used internally by the macro_write_lines function to write each line of a macro to the output file.
 * 
 * @param data Pointer to the line data to write.
 * @param f File pointer to the output file where the line will be written.
 */
void _dump_macro_line(void *data, FILE *f)
{
    fprintf(f, "%s\n", (char *)data);
}

/**
 * @brief Writes all lines of a macro to an output file.
 * 
 * Iterates through the macro's lines list and writes each line to the specified output file.
 * 
 * @param m Pointer to the MacroBlock whose lines will be written.
 * @param output File pointer to the output file.
 */
void macro_write_lines(MacroBlock *m, FILE *output)
{
    if (m == NULL || m->lines_list == NULL)
        return;

    linked_list_traverse_to_file(m->lines_list, _dump_macro_line, output);
}

/**
 * @brief Helper function to free memory used by macro lines.
 * 
 * Invoked by free_macro_table to free the linked list of lines associated with each macro.
 * 
 * @param kvp A key-value pair where the value is a pointer to a MacroBlock to be cleaned up.
 */
void _free_macro_lines(const KeyValuePair kvp)
{
    MacroBlock *mb = (MacroBlock *)kvp.value;
    if (mb->lines_list != NULL)
    {
        linked_list_delete(mb->lines_list, true);
        mb->lines_list = NULL;
    }
}

/**
 * @brief Frees the global macro table and all associated resources.
 * 
 * Iterates through the macro table and frees the linked lists of lines for each macro.
 * Then, it destroys the macro table itself.
 */
void free_macro_table()
{
    if (macroTable == NULL)
        return;

    /* delete the lines linkedlist for each macro first*/
    hashtable_iterate(macroTable, _free_macro_lines);
    /* delete the macro hashtable */
    hashtable_destroy(macroTable);
}
