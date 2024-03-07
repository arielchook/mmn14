/* This is a generic hashtable implementations that maps a key of type char* to any object (of type void*) */
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* initial capacity of hashtable */
#define INITIAL_SIZE 16

/* growth factor for the hashtable when capacity is reached */
#define GROWTH_FACTOR 2

/* Each entry in the hashtable comprises of a key and a value.
The key is always a string and the value can be whatever */
typedef struct {
    char *key;
    void *value;
} KeyValuePair;

/* The hashtable is implemented using an array of KeyValuePair objects */
typedef struct {
    KeyValuePair *table;
    int size; /* number of items in the table */
    int capacity; /* maximum size of the table before it needs to be expanded */
} Hashtable;

/* creates the hashtable data structure */
Hashtable* hashtable_create();
/* destroy the hashtable data structure and free all memory */
void hashtable_destroy(Hashtable *ht);

/* insert an item into the hashtable, returning the old item that was there or NULL if it was empty */
void* hashtable_insert(Hashtable *ht, char *key, void *value);

/* retrieve an item from the hashtable using a specified key */
void* hashtable_get(Hashtable *ht, const char *key);

#endif