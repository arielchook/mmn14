#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include <linkedlist.h>

/* Function to initialize a new linked list */
LinkedList *linked_list_create()
{
    LinkedList *list = (LinkedList *)safe_malloc(sizeof(LinkedList));
    list->head = NULL;
    return list;
}

/* Function to add a new node with data to the end of the linked list */
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

/* Function to delete the entire linked list */
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

/* Function to traverse the linked list and execute a given function for every item */
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
