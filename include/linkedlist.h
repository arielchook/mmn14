#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <utils.h>

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

/**
 * @brief Initializes a new linked list.
 * 
 * Allocates memory for a new linked list and sets the head to NULL.
 * 
 * @return A pointer to the newly created LinkedList.
 */
LinkedList *linked_list_create(void);

/**
 * @brief Appends a new node with data to the end of the linked list.
 * 
 * Creates a new node with the provided data and appends it to the end of the list.
 * If the list is empty, the new node becomes the head of the list.
 * 
 * @param list The linked list to which the new node will be appended.
 * @param data The data to store in the new node.
 */
void linked_list_append(LinkedList *list, void *data);

/**
 * @brief Deletes the entire linked list and optionally frees the data in each node.
 * 
 * Iterates through the list, freeing each node and optionally the data within each node,
 * depending on the free_data flag. Finally, frees the list structure itself.
 * 
 * @param list The linked list to be deleted.
 * @param free_data If true, also free the data stored in each node.
 */
void linked_list_delete(LinkedList *list, bool free_data);

/**
 * @brief Traverses the linked list and executes a given function for every item.
 * 
 * Iterates through the list and calls the provided function with the data from each node
 * and an output file stream.
 * 
 * @param list The linked list to traverse.
 * @param func The function to execute for each item in the list. Takes the item's data and a FILE* as parameters.
 * @param output The file stream to which the function can write output.
 */
void linked_list_traverse_to_file(LinkedList *list, void (*func)(void *, FILE *), FILE *output);

#endif
