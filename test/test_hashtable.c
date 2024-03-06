#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    Entry e1={10, 20, 3.14},e2={30, 40, 6.28},*result;
    Hashtable *ht = createHashtable();
    
    insert(ht, "key1", e1);

    insert(ht, "key2", e2);

    result = get(ht, "key1");
    if (result != NULL) {
        printf("Value found: (%d, %d, %f)\n", result->int1, result->int2, result->dbl);
    } else {
        printf("Value not found.\n");
    }

    destroyHashtable(ht);
    return 0;
}
