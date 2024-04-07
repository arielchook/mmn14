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
typedef struct
{
    char *key;
    void *value;
} KeyValuePair;

/* The hashtable is implemented using an array of KeyValuePair objects */
typedef struct
{
    KeyValuePair *table;
    int size;     /* number of items in the table */
    int capacity; /* maximum size of the table before it needs to be expanded */
} Hashtable;


/** 
 * @brief Creates a new hashtable.
 * 
 * Allocates memory for a new hashtable structure and initializes it.
 * 
 * @return Pointer to the newly created Hashtable.
 */
Hashtable *hashtable_create(void);

/** 
 * @brief Destroys a hashtable.
 * 
 * Frees all memory associated with the hashtable, including keys and values.
 * 
 * @param ht The hashtable to destroy.
 */
void hashtable_destroy(Hashtable *ht);

/** 
 * @brief Inserts a key-value pair into the hashtable.
 * 
 * If the key already exists, its value is replaced. If the load factor is too high, the table is resized.
 * 
 * @param ht The hashtable into which the key-value pair is inserted.
 * @param key The key for the key-value pair.
 * @param value The value for the key-value pair.
 * @return Returns the old value if the key already exists, NULL otherwise.
 */void *hashtable_insert(Hashtable *ht, char *key, void *value);

/** 
 * @brief Retrieves a value from the hashtable by key.
 * 
 * @param ht The hashtable from which to retrieve the value.
 * @param key The key of the value to retrieve.
 * @return The value associated with the key, or NULL if the key is not found.
 */void *hashtable_get(Hashtable *ht, const char *key);

/** 
 * @brief Iterates over all entries in the hashtable and applies a function to each one.
 * 
 * @param ht The hashtable over which to iterate.
 * @param step_function The function to apply to each KeyValuePair in the hashtable.
 */
void hashtable_iterate(Hashtable *ht, void (*step_function)(const KeyValuePair));

#endif
