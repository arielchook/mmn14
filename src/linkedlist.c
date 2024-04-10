/**
 * @file linkedlist.c
 * @brief Linked List Implementation.
 *
 * Provides functions for creating, manipulating, and destroying linked lists.
 * This module implements a generic singly linked list data structure, supporting operations
 * such as creation, appending, deletion, traversal, and applying a function to each item.
 *
 * The main functions in this file are:
 * - linked_list_create: Initializes a new linked list.
 * - linked_list_append: Appends a new node with data to the end of the linked list.
 * - linked_list_delete: Deletes the entire linked list and optionally frees the data in each node.
 * - linked_list_traverse_to_file: Traverses the linked list and executes a given function for every item.
 *
 * These functions provide essential functionality for managing collections of data in a dynamic and flexible manner.
 * Linked lists are well-suited for scenarios where efficient insertion, deletion, and traversal are required,
 * and they offer advantages such as dynamic memory allocation and simple implementation.
 *
 * @authors Ariel Cohen, Jonathan Transky
 */

#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include <linkedlist.h>

/**
 * @brief Initializes a new linked list.
 *
 * Allocates memory for a new linked list and sets the head to NULL.
 *
 * @return A pointer to the newly created LinkedList.
 */
LinkedList *linked_list_create(void)
{
    LinkedList *list = (LinkedList *)safe_malloc(sizeof(LinkedList));
    list->head = NULL;
    return list;
}

/**
 * @brief Appends a new node with data to the end of the linked list.
 *
 * Creates a new node with the provided data and appends it to the end of the list.
 * If the list is empty, the new node becomes the head of the list.
 *
 * @param list The linked list to which the new node will be appended.
 * @param data The data to store in the new node.
 */
void linked_list_append(LinkedList *list, void *data)
{
    Node *newNode = (Node *)safe_malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;

    if (list->head == NULL)
    {
        list->head = newNode;
    }
    else
    {
        Node *current = list->head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

/**
 * @brief Deletes the entire linked list and optionally frees the data in each node.
 *
 * Iterates through the list, freeing each node and optionally the data within each node,
 * depending on the free_data flag. Finally, frees the list structure itself.
 *
 * @param list The linked list to be deleted.
 * @param free_data If true, also free the data stored in each node.
 */
void linked_list_delete(LinkedList *list, bool free_data)
{
    Node *current = list->head;
    Node *next;

    while (current != NULL)
    {
        next = current->next;
        if (free_data)
            free_if_not_null(current->data);
        free(current);
        current = next;
    }

    free(list);
}

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
void linked_list_traverse_to_file(LinkedList *list, void (*func)(void *, FILE *), FILE *output)
{
    Node *current;

    if (list == NULL)
        return;

    current = list->head;

    while (current != NULL)
    {
        func(current->data, output);
        current = current->next;
    }
}
