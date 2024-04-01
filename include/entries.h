#ifndef ENTRIES_H
#define ENTRIES_H

#include <utils.h>
#include <stdio.h>

void entries_append(char *symbol);

void entries_delete_list(void);

void entries_dump(FILE *f);

bool entries_is_empty(void);
#endif
