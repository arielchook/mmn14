#ifndef ENTRIES_H
#define ENTRIES_H

#include <utils.h>
#include <stdio.h>

/**
 * @brief adds a symbol to the entries list
 *
 * @param[in] symbol the symbol to be added
 */
void entries_append(char *symbol);

/**
 * @brief Deletes all entries from the list
 *
 */
void entries_delete_list(void);

/**
 * @brief Dumps the contents of the entries list to the given file
 *
 * @param[in] f the file where the entries list is dumped
 */
void entries_dump(FILE *f);

/**
 * @brief checks if the entries list is empty
 *
 * @return true if the list is empty
 * @return false if the list is not empty
 */
bool entries_is_empty(void);
#endif
