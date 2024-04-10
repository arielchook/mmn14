/**
 * @file filemgr.h
 * @brief Assembly File Management.
 *
 * Defines the process for handling assembly source files through various
 * compilation stages including precompilation, first pass, and second pass.
 * It manages the generation of intermediate and output files with specific
 * extensions (.am, .ent, .ext, .ob) relevant to the assembly compilation
 * process. Functions within facilitate the entire assembly file processing
 * lifecycle from source to object code.
 *
 * @authors Ariel Cohen, Jonathan Transky
 */

#ifndef FILEMGR_H
#define FILEMGR_H
#include <utils.h>

#define AS_EXTENSION ".as"   /*!< file extension for a source file after precompilation */
#define AM_EXTENSION ".am"   /*!< file extension for a source file containing macros */
#define ENT_EXTENSION ".ent" /*!< file extension for an output file containing list of entries */
#define EXT_EXTENSION ".ext" /*!< file extension for an output file containing list of externs */
#define OB_EXTENSION ".ob"   /*!< file extension for the output object file */

/**
 * @brief Processes an assembly source file through various compilation stages.
 *
 * The function performs precompilation, first pass, and second pass on the given file.
 * It generates intermediate files (.am, .ent, .ext, .ob) throughout the process.
 *
 * @param fname The base name of the file to process, without any extension.
 * @return True if the processing was successful, false if any errors were encountered.
 */
bool processFile(char *fname);

#endif
