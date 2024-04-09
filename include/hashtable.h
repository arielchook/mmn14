/**
 * @file hashtable.h
 * @brief Generic Hashtable Implementation.
 *
 * Defines a simple hashtable that maps string keys to generic values (void pointers).
 * It supports basic operations like insertion, deletion, and retrieval. The hashtable
 * is designed to grow dynamically in size when its capacity is exceeded, ensuring
 * efficient storage and retrieval. This file includes the declaration of key-value
 * pair structures, the hashtable structure itself, and functions for managing the
 * hashtable such as creation, destruction, insertion, and retrieval.
 */


#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* initial capacity of hashtable */
#define INITIAL_SIZE 16

/* growth factor for the hashtable when capacity is reached */
#define GROWTH_FACTOR 2


/**
 * @struct KeyValuePair
 * @brief A structure to represent a key-value pair.
 *
 * This structure is used to hold a key-value pair. The key is a string (char pointer), and the value is a pointer to any type (void pointer), allowing for the storage of various types of data. This struct is typically used in data structures that map keys to values, such as hash tables.
 *
 * @var KeyValuePair::key
 *     The key part of the key-value pair. It is a pointer to a character array (string). The key is used to uniquely identify the value in a collection.
 * @var KeyValuePair::value
 *     The value part of the key-value pair. It is a void pointer, allowing it to point to any type of data. The actual type of data pointed to by this void pointer should be known from the context in which the KeyValuePair is used.
 */
typedef struct
{
    char *key;   /**< Pointer to the key string of the key-value pair. */
    void *value; /**< Void pointer to the value, allowing for storage of any type of data. */
} KeyValuePair;


/**
 * @struct Hashtable
 * @brief A structure to represent a simple hash table.
 *
 * This structure represents a simple hash table that uses key-value pairs for storage. The hash table supports basic operations like insertion, deletion, and search. It dynamically grows in size when the capacity is exceeded.
 *
 * @var Hashtable::table
 *     Pointer to an array of key-value pairs (KeyValuePair) stored in the hash table. This serves as the actual storage of the hash table elements.
 * @var Hashtable::size
 *     The current number of key-value pairs stored in the hash table. This value is incremented or decremented when elements are added or removed, respectively.
 * @var Hashtable::capacity
 *     The maximum number of key-value pairs that can be stored in the hash table before it needs to be resized. Resizing involves allocating a larger array and rehashing the existing elements into it.
 */
typedef struct
{
    KeyValuePair *table; /**< Pointer to an array of key-value pairs representing the hash table storage. */
    int size;            /**< The current number of key-value pairs in the hash table. */
    int capacity;        /**< The maximum capacity of the hash table before resizing is required. */
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
