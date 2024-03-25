#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>

/* Head pointer to the unresolved symbol list */
struct UnresolvedSymbol *urs_head = NULL;

/**
 * @brief Define the structure of a node in the linked list. The list contains all symbols that haven't yet been resolved
 * and will be resolved in the 2nd pass.
 *
 */
struct UnresolvedSymbol
{
    char *symbol;
    int address;
    struct UnresolvedSymbol *next;
};

/**
 * @brief Function to create a new node
 *
 * @param symbol
 * @param address
 * @return struct UnresolvedSymbol*
 */
struct UnresolvedSymbol *create_node(char *symbol, int address)
{
    struct UnresolvedSymbol *newNode = (struct UnresolvedSymbol *)safe_malloc(sizeof(struct UnresolvedSymbol));
    newNode->symbol = strdup(symbol);
    newNode->address = address;
    newNode->next = NULL;
    return newNode;
}

/* Function to add a node to the end of the list */
void append_unersolved_symbol(char *symbol, int address)
{
    struct UnresolvedSymbol *current;
    struct UnresolvedSymbol *newNode = create_node(symbol, address);
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

/* Function to traverse and print the list */
void resolve_symbols()
{
    struct UnresolvedSymbol *current = urs_head;
    printf("Linked List Contents:\n");
    while (current != NULL)
    {
        printf("Symbol: %s, Address: %d\n", current->symbol, current->address);
        current = current->next;
    }
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