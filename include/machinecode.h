#ifndef MACHINECODE_H
#define MACHINECODE_H

#include <stdbool.h>

#define DATA_SECTION_SIZE 4096

/* instruction counter */
extern int IC;

/* data counter */
extern int DC;

bool writeToDataSection(int value);
void advanceDC(int howmuch);

#endif