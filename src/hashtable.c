#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hashtable.h>
#include <utils.h>

/* Generate a hash code from a string */
unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

Hashtable *hashtable_create()
{
    Hashtable *ht = (Hashtable *)safe_malloc(sizeof(Hashtable));
    ht->table = (KeyValuePair *)safe_malloc(sizeof(KeyValuePair) * INITIAL_SIZE);
    ht->size = 0;
    ht->capacity = INITIAL_SIZE;
    return ht;
}

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

void hashtable_resize(Hashtable *ht)
{
    int i;
    int new_capacity = ht->capacity * GROWTH_FACTOR;
    KeyValuePair *new_table = (KeyValuePair *)safe_malloc(sizeof(KeyValuePair) * new_capacity);
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
    ht->table[index].key = strdup(key);
    ht->table[index].value = value;
    ht->size++;
    return NULL;
}

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
