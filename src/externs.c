#include <externs.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <symbols.h>
#include <linkedlist.h>

LinkedList *externs = NULL;
void externs_append(char *symbol, uint16_t address)
{
    char *symbol_space_address;

    if (externs == NULL)
    {
        externs = linked_list_create();
    }
    /* we only need this for printing to the .ext file at the end of secondpass
    therefore each entry in the list is a string in the form of:
    extern_name address */
    symbol_space_address = safe_malloc(sizeof(symbol) + 6);
    sprintf(symbol_space_address, "%s %.4u", symbol, address);
    linked_list_append(externs, symbol_space_address);
}

void externs_delete_list()
{
    if (externs != NULL)
    {
        linked_list_delete(externs);
        externs = NULL;
    }
}

void _dump_extern(void *data, FILE *f)
{
    char *symbol_and_address = (char *)data;
    fprintf(f, "%s\n", symbol_and_address);
}

void externs_dump(FILE *f)
{
    /* if no open file was provided, dump to stdout */
    if (f == NULL)
    {
        f = stdout;
        LOG("External symbols:\n");
    }
    linked_list_traverse_to_file(externs, _dump_extern, f);
}

bool externs_is_empty()
{
    return externs == NULL;
}