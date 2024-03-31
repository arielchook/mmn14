#include <entries.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <symbols.h>
#include <linkedlist.h>

LinkedList *entries = NULL;
void entries_append(char *symbol)
{
    if (entries == NULL)
    {
        entries = linked_list_create();
    }
    linked_list_append(entries, strdup(symbol));
}

void entries_delete_list()
{
    if (entries != NULL)
    {
        linked_list_delete(entries);
        entries = NULL;
    }
}

void _dump_entry(void *data, FILE *f)
{
    char *symbol = (char *)data;
    SymbolBlock *sb = find_symbol(symbol);
    fprintf(f, "%s %u\n", symbol, sb->value);
}

void entries_dump(FILE *f)
{
    /* if no open file was provided, dump to stdout */
    if (f == NULL)
    {
        f = stdout;
        LOG("Entry symbols:\n");
    }
    linked_list_traverse_to_file(entries, _dump_entry, f);
}

bool entries_is_empty()
{
    return entries == NULL;
}