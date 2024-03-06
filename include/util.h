#include <stdlib.h>
#include <stdio.h>
#include <messages.h>

void *safe_malloc(int size)
{
    void *p=malloc(size);
    if (p==NULL) 
    {
        fprintf(stderr,ERR_MEM_ALLOC_FAILED);
        exit(EXIT_FAILURE);
    }
    return p;
}