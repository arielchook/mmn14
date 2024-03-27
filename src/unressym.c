#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <symbols.h>
#include <machinecode.h>

/**
 * @brief Define the structure of a node in the linked list. The list contains all symbols that haven't yet been resolved
 * and will be resolved in the 2nd pass.
 *
 */
struct UnresolvedSymbol
{
    char *symbol;
    int address;
    int lineNumber;
    struct UnresolvedSymbol *next;
};

/* Head pointer to the unresolved symbol list */
struct UnresolvedSymbol *urs_head = NULL;

/**
 * @brief Function to create a new node
 *
 * @param symbol
 * @param address
 * @return struct UnresolvedSymbol*
 */
struct UnresolvedSymbol *create_node(char *symbol, int address, int lineNumber)
{
    struct UnresolvedSymbol *newNode = (struct UnresolvedSymbol *)safe_malloc(sizeof(struct UnresolvedSymbol));
    newNode->symbol = strdup(symbol);
    newNode->address = address;
    newNode->next = NULL;
    return newNode;
}

/* Function to add a node to the end of the list */
void append_unersolved_symbol(char *symbol, int address, int lineNumber)
{
    struct UnresolvedSymbol *current;
    struct UnresolvedSymbol *newNode = create_node(symbol, address, lineNumber);
    if (urs_head == NULL)
    {
        urs_head = newNode;
        return;
    }
    current = urs_head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = newNode;
}

bool resolve_symbols()
{
    struct UnresolvedSymbol *current = urs_head;
    SymbolBlock *sb;

    /* for each unresolved symbol we bumped into during first pass */
    while (current != NULL)
    {
        /* try and locate it now */
        sb = find_symbol(current->symbol);

        /* if not found, we have an error */
        if (sb == NULL)
        {
            printf(ERR_CANT_RESOLVE_SYMBOL, current->lineNumber, current->symbol);
            return false;
        }

        /* FIXME: what to do about entry/extern ?*/
        /* if the symbol is defined as extern, that's ok. it does not need to be
           resolved in the code */
        if (sb->type == ST_EXTERN)
        {
        }

        /* update the code section to reflect the address for that symbol */
        if (!update_symbol_in_code(*(int *)sb->value, current->address))
        {
            printf("Error updating symbol\n");
            return false;
        }

        current = current->next;
    }
    return true;
}

/* Function to delete the entire list */
void delete_unres_sym_list()
{
    struct UnresolvedSymbol *current = urs_head;
    struct UnresolvedSymbol *nextNode;
    while (current != NULL)
    {
        nextNode = current->next;
        free(current->symbol); /* Free the dynamically allocated memory for the symbol */
        free(current);
        current = nextNode;
    }
    urs_head = NULL; /* Update head to NULL */
}

void dump_unresolved()
{
    struct UnresolvedSymbol *current = urs_head;
    struct UnresolvedSymbol *nextNode;
    printf("Unresolved symbols:\n");

    while (current != NULL)
    {
        nextNode = current->next;
        printf("[%s] @ [%d]\n", current->symbol, current->address);
        current = nextNode;
    }
}
/*
int main()
{
    // Add nodes to the list
    append_unersolved_symbol("A", 100);
    append_unersolved_symbol("B", 200);
    append_unersolved_symbol("C", 300);

    // Traverse and print the list
    resolve_symbols();

    // Delete the entire list
    delete_list();

    return 0;
}
*/