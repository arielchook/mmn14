#ifndef MACHINECODE_H
#define MACHINECODE_H

#include <stdbool.h>

#define DATA_SECTION_SIZE 4096

/* instruction counter */
extern int IC;

/* data counter */
extern int DC;

#define BASE_CODE_ADDRESS 100
#define BASE_DATA_ADDRESS 0

/* these flags help maintain state for the machine code compiler */
#define MC_HAS_EXTERNS 0x1
#define MC_HAS_ENRTIES 0x2

bool writeToDataSection(int value);
void dump_data_section();
void reset_mc_state();
void set_machine_code_flag(unsigned flag);
#endif