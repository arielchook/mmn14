/**
 * @file externs.h
 * @brief External Symbol Management for Assembly Processing.
 *
 * This header provides functions for managing a list of external symbols
 * in assembly language processing. It includes functionalities for appending 
 * new external symbols with their addresses to a global list, deleting 
 * the list and freeing memory, dumping the list contents to a file, and 
 * checking if the list is empty. These operations are essential for handling 
 * '.extern' directives, which declare symbols defined in other modules.
 */


#ifndef EXTERNS_H
#define EXTERNS_H

#include <stdint.h>
#include <utils.h>
#include <stdio.h>


/** 
 * @brief Appends a new external symbol and its address to the global list.
 * 
 * If the list hasn't been initialized, it creates a new linked list. Each entry in the list
 * is a string in the format "extern_name address", which is useful for printing to the .ext file.
 * 
 * @param symbol The external symbol to append.
 * @param address The address associated with the external symbol.
 */
void externs_append(char *symbol, uint16_t address);


/** 
 * @brief Deletes the global list of external symbols and their addresses, freeing all associated memory.
 */
void externs_delete_list(void);


/** 
 * @brief Dumps all external symbols and their addresses to the given file stream.
 * 
 * If no file is provided, it defaults to stdout. Iterates over the external symbols list
 * and uses _dump_extern to write each symbol's name and address to the file.
 * 
 * @param f File pointer to where the external symbols and their addresses should be written.
 *          If NULL, output is written to stdout.
 */
void externs_dump(FILE *f);


/** 
 * @brief Checks if the externs list is empty.
 * 
 * @return True if the externs list is NULL, indicating it's empty. False otherwise.
 */
bool externs_is_empty(void);

#endif
