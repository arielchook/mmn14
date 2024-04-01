#include <hashtable.h>
#include <utils.h>
#include <symbols.h>
#include <ctype.h>
#include <reserved.h>
#include <machinecode.h>

static Hashtable *symbolsTable = NULL;

/* Add a symbol to the symbol hashtable */
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

/* Finds a symbol in the macro hashtable and returns it */
SymbolBlock *find_symbol(char *name)
{
    if (symbolsTable == NULL)
    {
        return NULL;
    }
    return hashtable_get(symbolsTable, name);
}

/* Frees all memory allocated for the symbol hashtable */
void free_symbol_table(void)
{
    if (symbolsTable != NULL)
    {
        /* FIXME: hashtable_destroy frees the key and the value but the value contains the key. no issue here?*/
        hashtable_destroy(symbolsTable);
    }
}

bool is_valid_symbol_name(char *symName, int lineNumber)
{
    int i;

    /* check whether symbol name is a reserved word */
    if (is_reserved_word(symName))
    {
        printf(ERR_RESERVED_WORD, lineNumber);
        return false;
    }

    /* check that symbol name starts with a letter */
    if (!isalpha(symName[0]) || strlen(symName) > MAX_SYMBOL_LEN)
    {
        printf(ERR_INVALID_SYMBOL_NAME, lineNumber);
        return false;
    }

    /* check that symbol name contains only alphanumeric characters */
    for (i = 0; i < strlen(symName); i++)
    {
        if (!isalnum(symName[i]))
        {
            printf(ERR_INVALID_SYMBOL_NAME, lineNumber);
            return false;
        }
    }

    /* check if symbol already exists in the symbol table */
    if (find_symbol(symName) != NULL)
    {
        printf(ERR_DUP_SYMBOL, lineNumber, symName);
        return false;
    }

    return true;
}

bool add_define(char *name, int value)
{
    SymbolBlock *sb = safe_malloc(sizeof(SymbolBlock));

    sb->name = name;
    sb->value = to_twos_complement(value);
    sb->type = ST_DEFINE;
    return add_symbol(sb);
}

bool add_extern(char *name)
{
    SymbolBlock *sb = safe_malloc(sizeof(SymbolBlock));

    /* create a symbol block for .extern definition */
    sb->name = name;
    sb->value = 0; /* .extern has no value */
    sb->type = ST_EXTERN;

    /* set the flag to mark there was at least one .extern */
    return add_symbol(sb);
}

bool add_data_label(char *name)
{
    SymbolBlock *sb = safe_malloc(sizeof(SymbolBlock));
    sb->name = name;
    sb->value = getDC();
    sb->type = ST_DATA;
    return add_symbol(sb);
}

bool add_code_label(char *name)
{
    SymbolBlock *sb = safe_malloc(sizeof(SymbolBlock));
    sb->name = name;
    sb->value = getIC(); /* IC is the current InstructionCounter */
    sb->type = ST_CODE;
    return add_symbol(sb);
}

/**
 * @brief this funcion is executed for every symbol in the symbols table.
 * if it's a ST_DATA or ST_STRING it add IC to its address so when written to file
 * it can be placed after the code part properly
 *
 * @param kvp
 */
void _update_address(const KeyValuePair kvp)
{
    SymbolBlock *sb = (SymbolBlock *)kvp.value;
    if ((sb->type == ST_DATA) || (sb->type == ST_STRING))
    {
        sb->value += getIC();
    }
}

void update_data_symbols_address(void)
{
    hashtable_iterate(symbolsTable, _update_address);
}

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
        LOG("unkown symbol type (%d)", sb->type);
    }
    LOG("\n");
}

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
