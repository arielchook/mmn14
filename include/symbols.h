/**
 * @file symbols.h
 * @brief Symbol Management for Assemblers.
 *
 * Outlines structures and functions for managing a symbol table in assembly
 * processing, including adding, finding, and handling different types of symbols
 * (like defines, externs, data, and code labels). The symbol table is central to
 * resolving references in assembly language, supporting the assembly process by
 * providing a mapping from symbol names to their values or addresses. Additional
 * utilities include validating symbol names, dumping the symbol table for debugging,
 * and updating symbol addresses post-processing.
 */


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

/**
 * @brief Adds a define symbol to the symbol table.
 * 
 * @param name Name of the define symbol.
 * @param value Integer value of the define symbol.
 * @return True if the symbol was added successfully, False otherwise.
 */
bool add_define(char *name, int value);


bool add_string(char *name, char *value);

/**
 * @brief Adds an extern symbol to the symbol table.
 * 
 * @param name Name of the extern symbol.
 * @return True if the symbol was added successfully, False otherwise.
 */
bool add_extern(char *name);

/**
 * @brief Adds a data label symbol to the symbol table.
 * 
 * @param name Name of the data label symbol.
 * @return True if the symbol was added successfully, False otherwise.
 */
bool add_data_label(char *name);

/**
 * @brief Adds a code label symbol to the symbol table.
 * 
 * @param name Name of the code label symbol.
 * @return True if the symbol was added successfully, False otherwise.
 */
bool add_code_label(char *name);

/**
 * @brief Finds a symbol in the symbol hashtable and returns it.
 * 
 * @param name Name of the symbol to find.
 * @return Pointer to the SymbolBlock if found, NULL otherwise.
 */
SymbolBlock *find_symbol(char *name);

/**
 * @brief Frees all memory allocated for the symbol hashtable.
 */
void free_symbol_table(void);

/**
 * @brief handle a .define statement. print out error messages and update the symbols table
 * accordingly.
 *
 * @param symbolStmt the statement string, after the ".define" part
 * @param lineNumber the current line number in the file we're processing
 * @return true if the operation was successful
 * @return false if any error occured while processing the statement
 */
bool handle_define(char *defineStmt, int lineNumber);

/**
 * @brief Handles the .data directive in assembly code.
 * 
 * Parses the .data directive values, converts them to integers or resolves constants using the symbol table,
 * and serializes the values to the data section of the machine code.
 * 
 * @param dataStmt The statement containing the .data directive values.
 * @param lineNumber The current line number in the assembly file for error reporting.
 * @return True if all values are processed and serialized successfully, False on any error.
 */
bool handle_data(char *dataStmt, int lineNumber);

/**
 * @brief Handles the .string directive by serializing the provided string into the data section of the assembler's memory.
 *        It trims the input, validates the presence of enclosing quotes, and then iterates through each character of the string,
 *        serializing them into the data section. It also handles and reports any errors encountered during the process.
 *
 * @param stringStmt The string statement to be processed, following the ".string" directive.
 * @param lineNumber The line number in the source file where this directive is found, used for error reporting.
 * @return true If the string was successfully processed and serialized into the data section.
 * @return false If any error occurred during the processing, such as missing string value, missing enclosing quotes, or if the data section is full.
 */
bool handle_string(char *stringStmt, int lineNumber);

/**
 * @brief Checks if a given symbol name is valid.
 * 
 * Validates the symbol name against reserved words, length, character types, and duplicates.
 * 
 * @param symName Symbol name to validate.
 * @param lineNumber Current line number for error reporting.
 * @return True if the symbol name is valid, False otherwise.
 */
bool is_valid_symbol_name(char *symName, int lineNumber);

/**
 * @brief Logs the contents of the symbol table to assist with debugging.
 */
void dump_symbols_table(void);

/**
 * @brief Iterates over the symbol table and updates the addresses of data and string symbols.
 */
void update_data_symbols_address(void);

#endif
