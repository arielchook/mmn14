#include <externs.h>
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
struct ExternSymbol
{
    char *symbol;
    uint16_t address;
    struct ExternSymbol *next;
};

struct ExternSymbol *ext_head = NULL;
void externs_append(char *symbol, uint16_t address)
{
    struct ExternSymbol *current;
    struct ExternSymbol *newNode = (struct ExternSymbol *)safe_malloc(sizeof(struct ExternSymbol));
    newNode->symbol = strdup(symbol);
    newNode->address = address;
    newNode->next = NULL;
    if (ext_head == NULL)
    {
        ext_head = newNode;
        return;
    }
    current = ext_head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = newNode;
}

void externs_delete_list()
{
    struct ExternSymbol *current = ext_head;
    struct ExternSymbol *nextNode;
    while (current != NULL)
    {
        nextNode = current->next;
        free(current->symbol); /* Free the dynamically allocated memory for the symbol */
        free(current);
        current = nextNode;
    }
    ext_head = NULL; /* Update head to NULL */
}

void externs_dump(FILE *f)
{
    struct ExternSymbol *current = ext_head;

    /* if no open file was provided, dump to stdout */
    if (f == NULL)
    {
        f = stdout;
        fprintf(f, "Extern symbols:\n");
    }

    /* for each extrnal symbol, print its name and the address of the operand that references it */
    while (current != NULL)
    {
        fprintf(f, "%s %u\n", current->symbol, current->address);
        current = current->next;
    }
}

bool externs_is_empty()
{
    return ext_head == NULL;
}