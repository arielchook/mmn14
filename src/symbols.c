/**
 * @file symbols.h
 * @brief Function prototypes and definitions for symbol table management.
 */



#include <hashtable.h>
#include <utils.h>
#include <symbols.h>
#include <ctype.h>
#include <reserved.h>
#include <machinecode.h>

static Hashtable *symbolsTable = NULL;

/**
 * @brief Adds a symbol to the symbol hashtable.
 * 
 * @param d Pointer to the SymbolBlock structure to add.
 * @return True if the symbol was added successfully, False if the symbol already exists.
 */
bool add_symbol(SymbolBlock *d)
{
    SymbolBlock *old;

    if (symbolsTable == NULL)
    {
        symbolsTable = hashtable_create();
    }
    old = hashtable_insert(symbolsTable, d->name, d);
    return (old == NULL);
}

/**
 * @brief Finds a symbol in the symbol hashtable and returns it.
 * 
 * @param name Name of the symbol to find.
 * @return Pointer to the SymbolBlock if found, NULL otherwise.
 */
SymbolBlock *find_symbol(char *name)
{
    if (symbolsTable == NULL)
    {
        return NULL;
    }
    return hashtable_get(symbolsTable, name);
}

/**
 * @brief Frees all memory allocated for the symbol hashtable.
 */
void free_symbol_table(void)
{
    if (symbolsTable != NULL)
    {
        hashtable_destroy(symbolsTable);
    }
}

/**
 * @brief Checks if a given symbol name is valid.
 * 
 * Validates the symbol name against reserved words, length, character types, and duplicates.
 * 
 * @param symName Symbol name to validate.
 * @param lineNumber Current line number for error reporting.
 * @return True if the symbol name is valid, False otherwise.
 */
bool is_valid_symbol_name(char *symName, int lineNumber)
{
    int i;
    /* Check whether symbol name is a reserved word */
    if (is_reserved_word(symName))
    {
        printf(ERR_RESERVED_WORD, lineNumber);
        return false;
    }

    /* Check that symbol name starts with a letter */
    if (!isalpha(symName[0]) || strlen(symName) > MAX_SYMBOL_LEN)
    {
        printf(ERR_INVALID_SYMBOL_NAME, lineNumber);
        return false;
    }

    /* Check that symbol name contains only alphanumeric characters */
    for (i = 0; i < strlen(symName); i++)
    {
        if (!isalnum(symName[i]))
        {
            printf(ERR_INVALID_SYMBOL_NAME, lineNumber);
            return false;
        }
    }

    /* Check if symbol already exists in the symbol table */
    if (find_symbol(symName) != NULL)
    {
        printf(ERR_DUP_SYMBOL, lineNumber, symName);
        return false;
    }

    return true;
}

/**
 * @brief Adds a define symbol to the symbol table.
 * 
 * @param name Name of the define symbol.
 * @param value Integer value of the define symbol.
 * @return True if the symbol was added successfully, False otherwise.
 */
bool add_define(char *name, int value)
{
    SymbolBlock *sb = safe_malloc(sizeof(SymbolBlock));

    sb->name = strdup(name);
    sb->value = to_twos_complement(value);
    sb->type = ST_DEFINE;
    return add_symbol(sb);
}

/**
 * @brief Adds an extern symbol to the symbol table.
 * 
 * @param name Name of the extern symbol.
 * @return True if the symbol was added successfully, False otherwise.
 */
bool add_extern(char *name)
{
    SymbolBlock *sb = safe_malloc(sizeof(SymbolBlock));

    /* Create a symbol block for .extern definition */
    sb->name = strdup(name);
    sb->value = 0; /* .extern has no value */
    sb->type = ST_EXTERN;

    return add_symbol(sb);
}

/**
 * @brief Adds a data label symbol to the symbol table.
 * 
 * @param name Name of the data label symbol.
 * @return True if the symbol was added successfully, False otherwise.
 */
bool add_data_label(char *name)
{
    SymbolBlock *sb = safe_malloc(sizeof(SymbolBlock));
    sb->name = strdup(name);
    sb->value = getDC();
    sb->type = ST_DATA;
    return add_symbol(sb);
}

/**
 * @brief Adds a code label symbol to the symbol table.
 * 
 * @param name Name of the code label symbol.
 * @return True if the symbol was added successfully, False otherwise.
 */
bool add_code_label(char *name)
{
    SymbolBlock *sb = safe_malloc(sizeof(SymbolBlock));
    sb->name = strdup(name);
    sb->value = getIC(); /* IC is the current Instruction Counter */
    sb->type = ST_CODE;
    return add_symbol(sb);
}

/**
 * @brief Updates the addresses of data and string symbols in the symbol table.
 * 
 * This function is executed for every symbol in the symbols table. If it's a ST_DATA or ST_STRING,
 * it adds IC to its address so when written to file, it can be placed after the code part properly.
 * 
 * @param kvp The KeyValuePair structure containing the symbol to update.
 */
void _update_address(const KeyValuePair kvp)
{
    SymbolBlock *sb = (SymbolBlock *)kvp.value;
    if ((sb->type == ST_DATA) || (sb->type == ST_STRING))
    {
        sb->value += getIC();
    }
}

/**
 * @brief Iterates over the symbol table and updates the addresses of data and string symbols.
 */
void update_data_symbols_address(void)
{
    hashtable_iterate(symbolsTable, _update_address);
}

/**
 * @brief Dumps the symbol table for debugging purposes.
 * 
 * @param kvp The KeyValuePair structure containing the symbol to dump.
 */
void _dump_symbol(const KeyValuePair kvp)
{
    SymbolBlock *sb = (SymbolBlock *)kvp.value;

    LOG("%s -> ", kvp.key);
    switch (sb->type)
    {
    case ST_DEFINE:
        LOG("(define) %d", sb->value);
        break;
    case ST_DATA:
        LOG("(data label) %d", sb->value);
        break;
    case ST_CODE:
        LOG("(code label) %d", sb->value);
        break;
    case ST_STRING:
        LOG("(string label) %d", sb->value);
        break;
    case ST_EXTERN:
        LOG("(extern)");
        break;
    default:
        LOG("unknown symbol type (%d)", sb->type);
    }
    LOG("\n");
}

/**
 * @brief Logs the contents of the symbol table to assist with debugging.
 */
void dump_symbols_table(void)
{
    if ((symbolsTable == NULL) || (symbolsTable->size == 0))
    {
        LOG("Symbol table is empty!\n");
        return;
    }

    LOG("\nSymbols table:\n");
    hashtable_iterate(symbolsTable, _dump_symbol);
}
