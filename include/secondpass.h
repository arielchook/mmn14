/**
 * @file secondpass.h
 * @brief Second Pass Processing in Assembly Compilation.
 *
 * Outlines the function for the second pass of the assembler, where .entry
 * directives and instructions are processed to finalize the machine code. This
 * stage resolves symbols and addresses that were not determined in the first
 * pass, ensuring the output is ready for linking or execution.
 */


#ifndef SECONDPASS_H
#define SECONDPASS_H

#include <utils.h>

/** 
 * @brief Executes the second pass of the assembler.
 * 
 * This pass processes each line in the input file, primarily focusing on .entry directives
 * and instructions, and finalizing the machine code output.
 * 
 * @param input File pointer to the input assembly source file.
 * @return True if the second pass completes successfully without errors, false otherwise.
 */
bool secondPass(FILE *input);

#endif
