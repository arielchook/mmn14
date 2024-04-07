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