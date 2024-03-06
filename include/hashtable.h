#define INITIAL_SIZE 16
#define GROWTH_FACTOR 2

typedef struct {
    int int1;
    int int2;
    double dbl;
} Entry;

typedef struct {
    char *key;
    Entry value;
} KeyValuePair;

typedef struct {
    KeyValuePair *table;
    int size;
    int capacity;
} Hashtable;

Hashtable* createHashtable();
void destroyHashtable(Hashtable *ht);
void insert(Hashtable *ht, const char *key, Entry value);
Entry* get(Hashtable *ht, const char *key);

