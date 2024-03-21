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

bool write_data_section(int value);

/**
 * @brief prints out the contents of the data section
 *
 */
void dump_data_section();

/**
 * @brief resets the state of our machine code compiler.
 * this should be called before processing each file.
 *
 */
void reset_mc_state();

/**
 * @brief Set/unset flags in the machine code state
 *
 * @param flag
 */
void set_machine_code_flag(unsigned flag);
bool is_mc_flag_set(unsigned flag);
#endif