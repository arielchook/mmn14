/**
 * @file entries.c
 * @brief This file provides functionalities to manage a global list of entry symbols
 * in the context of assembly language compilation. Entry symbols are labels in
 * assembly code marked with the .entry directive, signifying they should be
 * made visible to other modules or files. The file includes functions to:
 * - Append new entry symbols to the global list.
 * - Delete the global list, freeing all associated memory.
 * - Dump all entry symbols and their corresponding values to a file.
 * - Check if the entry list is empty.
 *
 * The management of entry symbols is crucial for the linkage stage of assembly
 * processing, allowing for proper resolution of symbols that are marked as entry
 * points into the code. Each entry symbol's value is determined in the second pass
 * of the assembly process and is written to an output file alongside its name.
 *
 * @authors Ariel Cohen, Jonathan Transky
 */

#include <entries.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <symbols.h>
#include <linkedlist.h>

/**
 * Global pointer to a linked list that stores entry symbols.
 */
LinkedList *entries_list = NULL;

/**
 * @brief Appends a new entry symbol to the global list.
 *
 * If the list hasn't been initialized, it creates a new linked list.
 * The symbol is duplicated using strdup to ensure the list has its own copy.
 *
 * @param symbol The entry symbol to append to the list.
 */
void entries_append(char *symbol)
{
    if (entries_list == NULL)
    {
        entries_list = linked_list_create();
    }
    linked_list_append(entries_list, strdup(symbol));
}

/**
 * @brief Deletes the global list of entry symbols, freeing all associated memory.
 *
 * If the list exists, it's deleted using the linked list deletion function,
 * and the entries_list pointer is set back to NULL.
 */
void entries_delete_list(void)
{
    if (entries_list != NULL)
    {
        linked_list_delete(entries_list, true);
        entries_list = NULL;
    }
}

/**
 * @brief Helper function to dump an entry symbol and its value to a file.
 *
 * Retrieves the symbol's associated SymbolBlock from the symbol table
 * and writes the symbol and its value to the given file stream.
 *
 * @param data A void pointer to the entry symbol (to be cast to char*).
 * @param f File pointer to where the entry symbol and its value should be written.
 */
void _dump_entry(void *data, FILE *f)
{
    char *symbol = (char *)data;
    SymbolBlock *sb = find_symbol(symbol);
    fprintf(f, "%s %u\n", symbol, sb->value);
}

/**
 * @brief Dumps all entry symbols and their values to the given file stream.
 *
 * If no file is provided, it defaults to stdout. Iterates over the entry list
 * and uses _dump_entry to write each symbol's name and value to the file.
 *
 * @param f File pointer to where the entry symbols and their values should be written.
 *          If NULL, output is written to stdout.
 */
void entries_dump(FILE *f)
{
    /* If no open file was provided, dump to stdout */
    if (f == NULL)
    {
        f = stdout;
        LOG("Entry symbols:\n");
    }
    linked_list_traverse_to_file(entries_list, _dump_entry, f);
}

/**
 * @brief Checks if the entries list is empty.
 *
 * @return True if the entries list is NULL, indicating it's empty. False otherwise.
 */
bool entries_is_empty(void)
{
    return entries_list == NULL;
}
