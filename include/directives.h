#ifndef DIRECTIVES_H
#define DIRECTIVES_H

/**
 * @brief Handles a .define statement, updates the symbols table accordingly, and prints out error messages.
 *
 * @param symbolStmt The statement string, after the ".define" part.
 * @param lineNumber The current line number in the file being processed.
 * @return True if the operation was successful, false if any error occurred while processing the statement.
 */
bool handle_define(char *symbolStmt, int lineNumber);

/**
 * @brief Handles the .data directive in assembly code by parsing the directive values, converting them to integers or resolving constants, and serializing the values to the data section of the machine code.
 *
 * @param dataStmt The statement containing the .data directive values.
 * @param lineNumber The current line number in the assembly file for error reporting.
 * @return True if all values are processed and serialized successfully, False on any error.
 */
bool handle_data(char *dataStmt, int lineNumber);

/**
 * @brief Handles the .string directive by serializing the given string into the data section of the machine code.
 *
 * @param stringStmt The string statement to handle, excluding the .string directive itself.
 * @param lineNumber The current line number in the assembly file for error reporting.
 * @return True if the string is processed successfully, False on any error.
 */
bool handle_string(char *stringStmt, int lineNumber);

/**
 * @brief Handles the .extern directive by adding the provided symbol as an external symbol in the symbols table.
 *
 * @param externStmt The statement string, after the ".extern" part.
 * @param lineNumber The current line number in the file being processed.
 * @return True if the operation was successful, false if any error occurred while processing the statement.
 */
bool handle_extern(char *externStmt, int lineNumber);

/**
 * @brief Handles the .entry directive by appending the provided entry to the list of entries. The validation of the entry against the symbol table is deferred to the second pass.
 *
 * @param entryStmt The entry statement to handle, excluding the .entry directive itself.
 * @param lineNumber The current line number in the assembly file for error reporting.
 * @return True if the entry is added to the list successfully, False on any error.
 */
bool handle_entry(char *entryStmt, int lineNumber);

#endif
