#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hashtable.h>
#include <messages.h>

char *strdup(const char *s) {
    size_t size = strlen(s) + 1;
    char *p = malloc(size);
    if (p) {
        memcpy(p, s, size);
    }
    return p;
}

unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

Hashtable* createHashtable() {
    Hashtable *ht = (Hashtable*)malloc(sizeof(Hashtable));
    if (ht == NULL) {
        fprintf(stderr, ERR_MEM_ALLOC_FAILED);
        exit(EXIT_FAILURE);
    }
    ht->table = (KeyValuePair*)malloc(sizeof(KeyValuePair) * INITIAL_SIZE);
    if (ht->table == NULL) {
        fprintf(stderr, ERR_MEM_ALLOC_FAILED);
        exit(EXIT_FAILURE);
    }
    ht->size = 0;
    ht->capacity = INITIAL_SIZE;
    return ht;
}

void destroyHashtable(Hashtable *ht) {
    if (ht == NULL) return;
    free(ht->table);
    free(ht);
}

void resizeHashtable(Hashtable *ht) {
    int i;
    int new_capacity = ht->capacity * GROWTH_FACTOR;
    KeyValuePair *new_table = (KeyValuePair*)malloc(sizeof(KeyValuePair) * new_capacity);
    if (new_table == NULL) {
        fprintf(stderr, ERR_MEM_ALLOC_FAILED);
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < ht->capacity; i++) {
        if (ht->table[i].key != NULL) {
            unsigned long index = hash(ht->table[i].key) % new_capacity;
            while (new_table[index].key != NULL) {
                index = (index + 1) % new_capacity;
            }
            new_table[index] = ht->table[i];
        }
    }
    free(ht->table);
    ht->table = new_table;
    ht->capacity = new_capacity;
}

void insert(Hashtable *ht, const char *key, Entry value) {
    unsigned long index;
    if ((double)ht->size / ht->capacity > 0.7) {
        resizeHashtable(ht);
    }
    index = hash(key) % ht->capacity;
    while (ht->table[index].key != NULL) {
        if (strcmp(ht->table[index].key, key) == 0) {
            ht->table[index].value = value;
            return;
        }
        index = (index + 1) % ht->capacity;
    }
    ht->table[index].key = strdup(key);
    ht->table[index].value = value;
    ht->size++;
}

Entry* get(Hashtable *ht, const char *key) {
    unsigned long index = hash(key) % ht->capacity;
    while (ht->table[index].key != NULL) {
        if (strcmp(ht->table[index].key, key) == 0) {
            return &ht->table[index].value;
        }
        index = (index + 1) % ht->capacity;
    }
    return NULL;
}
