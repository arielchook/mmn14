#include <entries.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <symbols.h>
#include <linkedlist.h>

LinkedList *entries_list = NULL;
void entries_append(char *symbol)
{
    if (entries_list == NULL)
    {
        entries_list = linked_list_create();
    }
    linked_list_append(entries_list, strdup(symbol));
}

void entries_delete_list()
{
    if (entries_list != NULL)
    {
        linked_list_delete(entries_list, true);
        entries_list = NULL;
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
    linked_list_traverse_to_file(entries_list, _dump_entry, f);
}

bool entries_is_empty()
{
    return entries_list == NULL;
}