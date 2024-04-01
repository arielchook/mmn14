#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdlib.h>
#include <hashtable.h>
#include <utils.h>

#define MAX_SYMBOL_LEN 31 /*!< maximum length for a symbol name */

/**
 * @brief enum for the symbols supported in our symbol table
 *
 */
enum SymbolType
{
    ST_DEFINE,
    ST_DATA,
    ST_CODE,
    ST_STRING,
    ST_EXTERN
    /* ST_ENTRY - entries have their own separate list */
};

/**
 * @brief an entry in the symbols hashtable
 *
 */
typedef struct
{
    char *name;
    uint16_t value;
    enum SymbolType type; /*<! the type of this entry symbol  */
} SymbolBlock;

bool add_define(char *name, int value);
bool add_string(char *name, char *value);
bool add_extern(char *name);

bool add_data_label(char *name);
bool add_code_label(char *name);

/* Finds a symbol in the macro hashtable and returns it */
SymbolBlock *find_symbol(char *name);

/* Frees all memory allocated for the macro hashtable */
void free_symbol_table(void);

bool handle_define(char *defineStmt, int lineNumber);
bool handle_data(char *dataStmt, int lineNumber);
bool handle_string(char *stringStmt, int lineNumber);

bool is_valid_symbol_name(char *symName, int lineNumber);
void dump_symbols_table(void);
void update_data_symbols_address(void);

#endif
