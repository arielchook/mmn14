#ifndef MACHINECODE_H
#define MACHINECODE_H

#include <stdbool.h>
#include <stdint.h>
#include <mcwordtypes.h>

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

/**
 * @brief memory word contains 14 bits. we could define a struct with a variable with :14 but since 14 bits are 2 bytes
 * we are using uint16_t (which is unsigned 16 bit) and will handle the bit manipulation in the encoding.
 */
typedef uint16_t mem_word;

bool write_data_section(mem_word value);

bool write_code_word(mc_word *word);

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