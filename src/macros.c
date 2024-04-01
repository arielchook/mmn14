#include <macros.h>
#include <utils.h>
#include <hashtable.h>
#include <stdio.h>

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

void _dump_macro_line(void *data, FILE *f)
{
    fprintf(f, "%s\n", (char *)data);
}

void macro_write_lines(MacroBlock *m, FILE *output)
{
    if (m == NULL || m->lines_list == NULL)
        return;

    linked_list_traverse_to_file(m->lines_list, _dump_macro_line, output);
}

void _free_macro_lines(const KeyValuePair kvp)
{
    MacroBlock *mb = (MacroBlock *)kvp.value;
    if (mb->lines_list != NULL)
        linked_list_delete(mb->lines_list, true);
}
void free_macro_table()
{
    if (macroTable == NULL)
        return;

    /* delete the lines linkedlist for each macro first*/
    hashtable_iterate(macroTable, _free_macro_lines);
    /* delete the macro hashtable */
    hashtable_destroy(macroTable);
}