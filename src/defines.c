#include <hashtable.h>
#include <stdio.h>
#include <stdbool.h>
#include <defines.h>

static Hashtable *definitionTable = NULL;

bool addDefine (defineBlock *d) {
    defineBlock *old;

    if (definitionTable == NULL)
    {
        definitionTable = hashtable_create();
    }
    old = hashtable_insert(definitionTable, d->name, m->val);
    return (old == NULL);
}

bool getDefine (defineBlock *d) {
    if (macroTable == NULL)
    {
        return NULL;
    }
    return hashtable_get(definitionTable, d->name);
}
