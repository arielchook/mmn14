#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdio.h>
#include <stdbool.h>

/* Define the structure for a node in the linked list */
typedef struct Node
{
    void *data;        /*<! Pointer to the data stored in the node (generic) */
    struct Node *next; /*<! Pointer to the next node in the list */
} Node;

/* Define the structure for the linked list */
typedef struct
{
    Node *head; /*<! Pointer to the first node in the list */
} LinkedList;

/* Function to initialize a new linked list */
LinkedList *linked_list_create();
void linked_list_append(LinkedList *list, void *data);
void linked_list_delete(LinkedList *list, bool free_data);
void linked_list_traverse_to_file(LinkedList *list, void (*func)(void *, FILE *), FILE *output);

#endif