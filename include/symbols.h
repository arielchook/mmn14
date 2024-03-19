#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdlib.h>
#include <hashtable.h>
#include <utils.h>

#define MAX_SYMBOL_NAME 31
enum SymbolType {ST_DEFINE, ST_DATA, ST_CODE, ST_STRING, ST_ENTRY, ST_EXTERN };

/* describes an entry in the symbols hashtable */
typedef struct
{
    char *name;
    void *value;
    enum SymbolType type; /* the type of symbol. we support .define, .data and .string */
} SymbolBlock;

bool add_define(char *name, int value);
bool add_data(char *name, int value);
bool add_string(char *name, char *value);

/* Finds a symbol in the macro hashtable and returns it */
SymbolBlock *find_symbol(char *name);

bool add_data_label(char *name);
bool add_code_label(char *name);

/* Frees all memory allocated for the macro hashtable */
void free_symbol_table();

bool handle_define(char *defineStmt, int lineNumber);
bool handle_data(char *dataStmt, int lineNumber);
bool handle_string(char *stringStmt, int lineNumber);

bool isValidSymbolName(char *symName, int lineNumber);

#endif