#ifndef MACHINECODE_H
#define MACHINECODE_H

#include <utils.h>
#include <mcwordtypes.h>

/* each machine code word is 14 bits */
#define MC_WORD_SIZE_BITS 14

/* each memory word is 14 bits. however, we use 2 bits for A,R,E. this means we are left with
only 12 bits to represent an address or a value. */
#define MC_ADDR_SPACE_BITS (MC_WORD_SIZE_BITS - 2)
#define MEMORY_SIZE (1 << MC_ADDR_SPACE_BITS) /* 4096 */

#define MIN_VALUE (-(1 << (MC_ADDR_SPACE_BITS - 1)))    /* -2048 */
#define MAX_VALUE ((1 << (MC_ADDR_SPACE_BITS - 1)) - 1) /* 2047 */

/* max data section size and code section size (depends on our address space) */
#define DATA_SECTION_SIZE MEMORY_SIZE
#define CODE_SECTION_SIZE MEMORY_SIZE

/**
 * @brief memory word contains 14 bits. we could define a struct with a variable with :14 but since 14 bits are 2 bytes
 * we are using uint16_t (which is unsigned 16 bit) and will handle the bit manipulation in the encoding.
 */
typedef uint16_t mem_word;

#define BASE_CODE_ADDRESS 100 /*!< code section starts at this address */
#define BASE_DATA_ADDRESS 0

/**
 * @brief return the data counter
 *
 * @return int data counter
 */
int getDC(void);

int getIC(void);
bool advanceIC(int howmuch);
bool advanceDC(int howmuch);
void write_code_word(int address, mem_word value);

mem_word read_code_word(int address);

void write_data_word(int address, mem_word value);

mem_word read_data_word(int address);

uint16_t to_twos_complement(int num);

bool serialize_data_section(mem_word value);

bool serialize_code_mc_word(mc_word *word);

/**
 * @brief prints out the contents of the data section
 *
 */
void dump_data_section(void);

void LOG_AS_BINARY(mem_word address, mem_word word);

void dump_code_section(void);

void resetIC(void);

/**
 * @brief resets the state of our machine code compiler.
 * this should be called before processing each file.
 *
 */
void reset_mc_state(void);

/**
 * @brief cleans up machine code data structures and free memory.
 * this includes entries and extern lists as well as symbol table
 *
 */
void cleanup_mc_state(void);

#endif
