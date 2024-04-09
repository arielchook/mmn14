/**
 * @file precomp.h
 * @brief Precompilation Processing for Assembly Files.
 *
 * Defines the function for precompiling assembly files. It processes macro
 * definitions and expansions, handling 'mcr' to 'endmcr' blocks, and copies
 * non-macro lines directly to the output. The function ensures macros are
 * not nested and have valid names, trims whitespace, and skips empty lines,
 * thereby preparing the file for the main compilation stages.
 */


#ifndef PRECOMP_H
#define PRECOMP_H

#include <utils.h>

/**
 * @brief Precompiles the given input file by processing macros and writing the result to the output file.
 *
 * This function reads the input file line by line. It looks for macro definitions starting with 'mcr'
 * and ends with 'endmcr'. While processing macro definitions, it checks for errors like nested macros
 * or invalid macro names. If a line calls a defined macro, it expands that macro. All other lines are
 * copied to the output file as-is. The function also handles trimming whitespace and skipping empty lines.
 *
 * @param input Pointer to a FILE object that represents the input file to be precompiled.
 * @param output Pointer to a FILE object where the precompiled output will be written.
 * @return True if precompilation is successful without any errors, false otherwise.
 */
bool precompile(FILE *input, FILE *output);

#endif
