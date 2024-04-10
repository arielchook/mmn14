/**
 * @file firstpass.h
 * @brief First Pass Processing in Assembly Compilation.
 *
 * This header file declares the function responsible for performing the first pass
 * over an assembly source file. The first pass processes directives, labels, and
 * instructions to build the initial symbol table and set up for the second pass.
 * It's a crucial step in resolving symbols and preparing the assembly file for
 * further compilation stages.
 *
 * @authors Ariel Cohen, Jonathan Transky
 */

#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include <utils.h>

/**
 * @brief Performs the first pass of the assembler, processing each line in the input file.
 *
 * This function iterates through each line of the input assembly file, processing directives, labels,
 * and instructions. It sets the foundation for symbol table creation and prepares for the second pass
 * of assembly by updating symbol addresses and validating entries.
 *
 * @param input File pointer to the input assembly source file.
 * @return true if the first pass completes successfully without errors, false otherwise.
 */
bool firstPass(FILE *input);

#endif
