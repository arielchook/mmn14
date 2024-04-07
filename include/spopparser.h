#ifndef SPOPPARSER_H
#define SPOPPARSER_H

#include <reserved.h>

/** 
 * @brief Parses operands of an assembly instruction and generates machine code words.
 * 
 * This function processes source and destination operands of an assembly instruction
 * based on their addressing modes and the instruction properties. It generates and serializes
 * the machine code words for the instruction and its operands.
 * 
 * @param stmt The assembly statement containing the operands.
 * @param lineNumber Current line number for error reporting.
 * @param props The properties of the instruction being parsed.
 * @return True if operands are processed successfully, False on error.
 */
bool parse_operands(char *stmt, int lineNumber, const instruction_props *props);

#endif
