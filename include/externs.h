#ifndef EXTERNS_H
#define EXTERNS_H

#include <stdint.h>
#include <utils.h>
#include <stdio.h>

void externs_append(char *symbol, uint16_t address);

void externs_delete_list(void);

void externs_dump(FILE *f);

bool externs_is_empty(void);

#endif
