#ifndef ENTRIES_H
#define ENTRIES_H

#include <stdbool.h>
#include <stdio.h>

void entries_append(char *symbol);

void entries_delete_list();

void entries_dump(FILE *f);

bool entries_is_empty();
#endif
