#ifndef UNRESSYM_H
#define UNRESSYM_H

/**
 * @brief Appends a symbol to the list of unresovled symbols that are discovered in the first pass
 * 
 * @param symbol 
 * @param address 
 */
void append_unersolved_symbol(char *symbol, int address);

/**
 * @brief Go over the list of unresolved symbols from first pass and replace each symbol with its address
 * in the machine code memory
 * 
 */
void resolve_symbols();
void delete_unres_sym_list();

#endif
