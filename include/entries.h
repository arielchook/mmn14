#ifndef ENTRIES_H
#define ENTRIES_H

#include <stdbool.h>


bool entries_append(char *symbol);

/**
 * @brief 
 *
 */
bool entries_resolve();
void entries_delete_list();

void entries_dump();
#endif
