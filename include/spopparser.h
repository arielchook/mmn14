/**
 * @file spopparser.h
 * @brief Assembly Instruction Operand Parsing.
 *
 * Defines a function for parsing the operands of an assembly instruction
 * and generating the relevant machine code words. It takes into account the
 * addressing modes of the operands and the specific properties of the
 * instruction to ensure accurate machine code serialization. This is a
 * key part of the assembly process, translating assembly language into
 * machine-readable code.
 */


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
