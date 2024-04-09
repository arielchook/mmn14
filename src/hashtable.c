/**
 * @file hashtable.c
 * @brief Hashtable Implementation.
 *
 * Provides functions for creating, manipulating, and destroying hash tables.
 * This module implements a generic hashtable data structure, supporting key-value pairs.
 * It includes functionality for inserting, retrieving, resizing, and iterating over entries in the hashtable.
 * 
 * The main functions in this file are:
 * - hash: Generates a hash code from a string using the djb2 algorithm.
 * - hashtable_create: Creates a new hashtable and initializes it.
 * - hashtable_destroy: Destroys a hashtable, freeing all associated memory.
 * - hashtable_resize: Resizes a hashtable when the load factor exceeds a threshold.
 * - hashtable_insert: Inserts a key-value pair into the hashtable, replacing the value if the key already exists.
 * - hashtable_get: Retrieves a value from the hashtable by key.
 * - hashtable_iterate: Iterates over all entries in the hashtable and applies a function to each one.
 * 
 * These functions are essential for efficient storage and retrieval of data, providing a flexible
 * and scalable data structure for various applications.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hashtable.h>
#include <utils.h>

/** 
 * @brief Generates a hash code from a string.
 * 
 * This function applies the djb2 algorithm by Dan Bernstein to generate a hash code.
 * 
 * @param str The string to hash.
 * @return The generated hash code as an unsigned long.
 */
unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

/** 
 * @brief Creates a new hashtable.
 * 
 * Allocates memory for a new hashtable structure and initializes it.
 * 
 * @return Pointer to the newly created Hashtable.
 */
Hashtable *hashtable_create(void)
{
    Hashtable *ht = (Hashtable *)safe_malloc(sizeof(Hashtable));
    ht->table = (KeyValuePair *)safe_malloc(sizeof(KeyValuePair) * INITIAL_SIZE);
    memset(ht->table, 0, sizeof(KeyValuePair) * INITIAL_SIZE);
    ht->size = 0;
    ht->capacity = INITIAL_SIZE;
    return ht;
}

/** 
 * @brief Destroys a hashtable.
 * 
 * Frees all memory associated with the hashtable, including keys and values.
 * 
 * @param ht The hashtable to destroy.
 */
void hashtable_destroy(Hashtable *ht)
{
    int i;
    if (ht == NULL)
        return;
    for (i = 0; i < ht->capacity; i++)
    {
        if (ht->table[i].key != NULL)
        {
            free(ht->table[i].key);
            free(ht->table[i].value);
        }
    }
    free(ht->table);
    free(ht);
}

/** 
 * @brief Resizes a hashtable.
 * 
 * Grows the hashtable when the load factor exceeds a threshold, rehashing all existing entries.
 * 
 * @param ht The hashtable to resize.
 */
void hashtable_resize(Hashtable *ht)
{
    int i;
    int new_capacity = ht->capacity * GROWTH_FACTOR;
    KeyValuePair *new_table = (KeyValuePair *)safe_malloc(sizeof(KeyValuePair) * new_capacity);
    memset(new_table, 0, sizeof(KeyValuePair) * new_capacity);
    for (i = 0; i < ht->capacity; i++)
    {
        if (ht->table[i].key != NULL)
        {
            unsigned long index = hash(ht->table[i].key) % new_capacity;
            while (new_table[index].key != NULL)
            {
                index = (index + 1) % new_capacity;
            }
            new_table[index] = ht->table[i];
        }
    }
    free(ht->table);
    ht->table = new_table;
    ht->capacity = new_capacity;
}

/** 
 * @brief Inserts a key-value pair into the hashtable.
 * 
 * If the key already exists, its value is replaced. If the load factor is too high, the table is resized.
 * 
 * @param ht The hashtable into which the key-value pair is inserted.
 * @param key The key for the key-value pair.
 * @param value The value for the key-value pair.
 * @return Returns the old value if the key already exists, NULL otherwise.
 */
void *hashtable_insert(Hashtable *ht, char *key, void *value)
{
    unsigned long index;

    if ((double)ht->size / ht->capacity > 0.7)
    {
        hashtable_resize(ht);
    }
    index = hash(key) % ht->capacity;
    while (ht->table[index].key != NULL)
    {
        if (strcmp(ht->table[index].key, key) == 0)
        {
            void *old_value = ht->table[index].value;
            ht->table[index].value = value;
            return old_value;
        }
        index = (index + 1) % ht->capacity;
    }
    ht->table[index].key = key;
    ht->table[index].value = value;
    ht->size++;
    return NULL;
}

/** 
 * @brief Retrieves a value from the hashtable by key.
 * 
 * @param ht The hashtable from which to retrieve the value.
 * @param key The key of the value to retrieve.
 * @return The value associated with the key, or NULL if the key is not found.
 */
void *hashtable_get(Hashtable *ht, const char *key)
{
    unsigned long index = hash(key) % ht->capacity;
    while (ht->table[index].key != NULL)
    {
        if (strcmp(ht->table[index].key, key) == 0)
        {
            return ht->table[index].value;
        }
        index = (index + 1) % ht->capacity;
    }

    return NULL;
}

/** 
 * @brief Iterates over all entries in the hashtable and applies a function to each one.
 * 
 * @param ht The hashtable over which to iterate.
 * @param step_function The function to apply to each KeyValuePair in the hashtable.
 */
void hashtable_iterate(Hashtable *ht, void (*step_function)(const KeyValuePair))
{
    int i;
    if (ht == NULL)
        return;
    for (i = 0; i < ht->capacity; i++)
    {
        if (ht->table[i].key != NULL)
        {
            step_function(ht->table[i]);
        }
    }
}
