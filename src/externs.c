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

struct ExternSymbol *ent_head = NULL;
void externs_append(char *symbol, uint16_t address)
{
    struct ExternSymbol *current;
    struct ExternSymbol *newNode = (struct ExternSymbol *)safe_malloc(sizeof(struct ExternSymbol));
    newNode->symbol = strdup(symbol);
    newNode->address = address;
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

void externs_delete_list()
{
    struct ExternSymbol *current = ent_head;
    struct ExternSymbol *nextNode;
    while (current != NULL)
    {
        nextNode = current->next;
        free(current->symbol); /* Free the dynamically allocated memory for the symbol */
        free(current);
        current = nextNode;
    }
    ent_head = NULL; /* Update head to NULL */
}

void externs_dump()
{
    struct ExternSymbol *current = ent_head;
    struct ExternSymbol *nextNode;
    printf("Extern symbols:\n");

    while (current != NULL)
    {
        nextNode = current->next;
        printf("[%s]\n", current->symbol);
        current = nextNode;
    }
}