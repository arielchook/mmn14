#ifndef FPOPPARSER_H
#define FPOPPARSER_H
#include <utils.h>
#include <machinecode.h>
#include <reserved.h>


/** 
 * @brief Counts the number of memory words needed for the operands of a statement.
 * 
 * Analyzes the operands of a machine code instruction to determine how many memory words
 * are required for them, considering the instruction properties and addressing modes.
 * 
 * @param stmt The assembly statement containing the operands.
 * @param lineNumber The current line number in the source file for error reporting.
 * @param props The properties of the instruction, including allowed addressing modes.
 * @return True if the operands are valid and processed successfully, False otherwise.
 */
bool count_operands_words(char *stmt, int lineNumber, const instruction_props *props);

#endif
