#ifndef EXTERNS_H
#define EXTERNS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

void externs_append(char *symbol, uint16_t address);

void externs_delete_list();

void externs_dump(FILE *f);

bool externs_is_empty();

#endif
