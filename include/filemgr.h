#ifndef FILEMGR_H
#define FILEMGR_H
#include <stdbool.h>

#define AS_EXTENSION ".as"
#define AM_EXTENSION ".am"
#define ENT_EXTENSION ".ent"
#define EXT_EXTENSION ".ext"
#define OB_EXTENSION ".ob"

bool processFile(char *fname);

#endif
