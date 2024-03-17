#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    char* name;
    int val;
} defineBlock

bool getDefine (defineBlock *d);
bool addDefine (defineBlock *d);
