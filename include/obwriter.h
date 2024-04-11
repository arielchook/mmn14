/**
 * @file obwriter.h
 * @brief Object File Writer for Assembled Code.
 *
 * Defines the function responsible for writing the final assembled code
 * and data sections to an object file (.ob format). This is a crucial step
 * in the assembly process, as it produces the output file that can be used
 * for linking or loading into memory for execution. The function supports
 * writing to a specified file or to stdout if no file is provided.
 * 
 * @authors Ariel Cohen, Jonathan Transky
 */


#ifndef OBWRITER_H
#define OBWRITER_H

#include <utils.h>

/**
 * @brief Dump the contents of the code section and data section into a .ob file
 *
 * @param[in] f The file to write the contents to. If NULL, it will write to stdout
 *
 */
void dump_object_file(FILE *f);

#endif