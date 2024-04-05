#include <externs.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <symbols.h>
#include <linkedlist.h>

/** 
 * Global pointer to a linked list that stores external symbols and their addresses.
 */
LinkedList *externs_list = NULL;

/** 
 * @brief Appends a new external symbol and its address to the global list.
 * 
 * If the list hasn't been initialized, it creates a new linked list. Each entry in the list
 * is a string in the format "extern_name address", which is useful for printing to the .ext file.
 * 
 * @param symbol The external symbol to append.
 * @param address The address associated with the external symbol.
 */
void externs_append(char *symbol, uint16_t address)
{
    char *extern_space_address;

    if (externs_list == NULL)
    {
        externs_list = linked_list_create();
    }
    /* We only need this for printing to the .ext file at the end of the second pass.
       Therefore, each entry in the list is a string in the format "extern_name address". */
    extern_space_address = safe_malloc(sizeof(symbol) + 6);
    sprintf(extern_space_address, "%s %.4u", symbol, address);
    linked_list_append(externs_list, extern_space_address);
}

/** 
 * @brief Deletes the global list of external symbols and their addresses, freeing all associated memory.
 */
void externs_delete_list(void)
{
    if (externs_list != NULL)
    {
        linked_list_delete(externs_list, true);
        externs_list = NULL;
    }
}

/** 
 * @brief Helper function to dump an external symbol and its address to a file.
 * 
 * Writes the external symbol and its address (stored together in a string) to the given file stream.
 * 
 * @param data A void pointer to the data to write, which is cast to a char*.
 * @param f File pointer to where the data should be written.
 */
void _dump_extern(void *data, FILE *f)
{
    char *symbol_and_address = (char *)data;
    fprintf(f, "%s\n", symbol_and_address);
}

/** 
 * @brief Dumps all external symbols and their addresses to the given file stream.
 * 
 * If no file is provided, it defaults to stdout. Iterates over the external symbols list
 * and uses _dump_extern to write each symbol's name and address to the file.
 * 
 * @param f File pointer to where the external symbols and their addresses should be written.
 *          If NULL, output is written to stdout.
 */
void externs_dump(FILE *f)
{
    /* If no open file was provided, dump to stdout */
    if (f == NULL)
    {
        f = stdout;
        LOG("External symbols:\n");
    }
    linked_list_traverse_to_file(externs_list, _dump_extern, f);
}

/** 
 * @brief Checks if the externs list is empty.
 * 
 * @return True if the externs list is NULL, indicating it's empty. False otherwise.
 */
bool externs_is_empty()
{
    return externs_list == NULL;
}
