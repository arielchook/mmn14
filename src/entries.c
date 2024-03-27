#include <entries.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <symbols.h>
#include <machinecode.h>
#include <stdint.h>

/**
 * @brief
 *
 */
struct EntrySymbol
{
    char *symbol;
    struct EntrySymbol *next;
};

struct EntrySymbol *ent_head = NULL;
void entries_append(char *symbol)
{
    struct EntrySymbol *current;
    struct EntrySymbol *newNode = (struct EntrySymbol *)safe_malloc(sizeof(struct EntrySymbol));
    newNode->symbol = strdup(symbol);

    newNode->next = NULL;
    if (ent_head == NULL)
    {
        ent_head = newNode;
        return;
    }
    current = ent_head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = newNode;
}

void entries_delete_list()
{
    struct EntrySymbol *current = ent_head;
    struct EntrySymbol *nextNode;
    while (current != NULL)
    {
        nextNode = current->next;
        free(current->symbol); /* Free the dynamically allocated memory for the symbol */
        free(current);
        current = nextNode;
    }
    ent_head = NULL; /* Update head to NULL */
}

void entries_dump(FILE *f)
{
    struct EntrySymbol *current = ent_head;
    SymbolBlock *sb;

    /* if no open file was provided, dump to stdout */
    if (f == NULL)
    {
        f = stdout;
        fprintf(f, "Entry symbols:\n");
    }

    /* for each entry symbol, print its name and its address in the code/data section */
    while (current != NULL)
    {
        sb = find_symbol(current->symbol);
        fprintf(f, "%s %u\n", current->symbol, *(int *)sb->value);
        current = current->next;
    }
}

bool entries_is_empty()
{
    return ent_head == NULL;
}