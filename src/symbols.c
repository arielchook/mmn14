#include <hashtable.h>
#include <stdio.h>
#include <stdbool.h>
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
    old = hashtable_insert(symbolsTable, d->name, d->value);
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
void free_symbol_table()
{
    if (symbolsTable != NULL)
    {
        hashtable_destroy(symbolsTable);
    }
}

bool isValidSymbolName(char *symName, int lineNumber)
{
    int i;

    /* check whether symbol name is a reserved word */
    if (is_reserved_word(symName))
    {
        printf(ERR_RESERVED_WORD, lineNumber);
        return false;
    }

    /* check that symbol name starts with a letter */
    if (!isalpha(*symName) || strlen(symName)>MAX_SYMBOL_NAME)
    {
        printf(ERR_INVALID_SYMBOL_NAME, lineNumber);
        return false;
    }

    /* check that symbol name contains only alphanumeric characters */
    for(i=0;i<strlen(symName);i++) 
    {
        if (!isalnum(symName[i]))
        {
            printf(ERR_INVALID_SYMBOL_NAME, lineNumber);
            return false;
        }
    }

    /* check if symbol already exists in the symbol table */
    if (find_symbol(symName)!=NULL)
    {
        printf(ERR_DUP_SYMBOL, lineNumber, symName);
        return false;
    }

    return true;
}

bool add_define(char *name, int value) {
    SymbolBlock *sb=safe_malloc(sizeof(SymbolBlock));
    sb->name=name;
    sb->value=safe_malloc(sizeof(int));
    *(int*)(sb->value)=value;
    sb->type=ST_DEFINE;
    return add_symbol(sb);
}

bool add_data_label(char *name) {
    SymbolBlock *sb=safe_malloc(sizeof(SymbolBlock));
    sb->name=name;
    sb->value=safe_malloc(sizeof(int));
    *(int*)(sb->value)=DC;
    sb->type=ST_DATA;
    return add_symbol(sb);
}

bool add_code_label(char *name) {
    SymbolBlock *sb=safe_malloc(sizeof(SymbolBlock));
    sb->name=name;
    sb->value=safe_malloc(sizeof(int));
    *(int*)(sb->value)=IC;
    sb->type=ST_CODE;
    return add_symbol(sb);
}

