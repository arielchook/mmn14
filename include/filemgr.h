#ifndef FILEMGR_H
#define FILEMGR_H
#include <stdbool.h>

#define AS_EXTENSION ".as"   /*!< file extension for a source file after precompilation */
#define AM_EXTENSION ".am"   /*!< file extension for a source file containing macros */
#define ENT_EXTENSION ".ent" /*!< file extension for an output file containing list of entries */
#define EXT_EXTENSION ".ext" /*!< file extension for an output file containing list of externs */
#define OB_EXTENSION ".ob"   /*!< file extension for the output object file */

bool processFile(char *fname);

#endif
