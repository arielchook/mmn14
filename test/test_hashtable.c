#include <hashtable.h>
#include <stdlib.h>
#include <stdio.h>
#include <utils.h>

#define KEY1 "key1"
#define KEY2 "key2"

#define VALUE1 -19
#define VALUE2 "hello"

int main(int argc, char *argv[])
{
    /* these must be dynamically allocated or otherwise hastable_destroy may free a local variable */
    int *e1 = safe_malloc(sizeof(int));
    char *str = strdup(VALUE2);
    void *result;

    Hashtable *ht = hashtable_create();
    *e1=VALUE1;

    hashtable_insert(ht, KEY1, e1);

    hashtable_insert(ht, KEY2, str);

    result = hashtable_get(ht, KEY1);
    if (result != NULL)
    {
        printf("%s -> %d\n", KEY1, *(int *)result);
    }
    else
    {
        printf("Value for %s not found.\n", KEY1);
        return 1;
    }

    result = hashtable_get(ht, KEY2);
    if (result != NULL)
    {
        printf("%s -> %s\n", KEY2, (char *)(result));
    }
    else
    {
        printf("Value for %s not found.\n", KEY2);
        return 1;
    }

    hashtable_destroy(ht);
    return 0;
}
