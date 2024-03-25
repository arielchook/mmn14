#include <entries.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <symbols.h>
#include <machinecode.h>

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
bool entries_append(char *symbol)
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
    return true;
}

/**
 * @brief
 *
 */
bool entries_resolve()
{
    /* go over all items in the list and look each symbol in the symbol table.
    if found make sure it's a code or data lable (but not a define). if not, print an error */
    return true;
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

void entries_dump()
{
    struct EntrySymbol *current = ent_head;
    struct EntrySymbol *nextNode;
    printf("Entry symbols:\n");

    while (current != NULL)
    {
        nextNode = current->next;
        printf("[%s]\n", current->symbol);
        current = nextNode;
    }
}