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
