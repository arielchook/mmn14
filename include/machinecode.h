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

int getIC(void);
bool advanceIC(int howmuch);
mem_word *code_word_at(mem_word address);
void write_code_word(mem_word address, mem_word value);
bool serialize_code_mc_word(mc_word *word);
void dump_code_section(void);
void resetIC(void);

#define BASE_DATA_ADDRESS 0

/**
 * @brief return the data counter
 *
 * @return int data counter
 */
int getDC(void);
bool advanceDC(int howmuch);
mem_word *data_word_at(mem_word address);
void write_data_word(mem_word address, mem_word value);
bool serialize_data_section(mem_word value);
/**
 * @brief prints out the contents of the data section
 *
 */
void dump_data_section(void);
void resetDC(void);

uint16_t to_twos_complement(int num);

/**
 * @brief Function to read a value from specific bits in a given mem_word variable.
 *
 * @param word pointer to the mem_word
 * @param start_bit the bit index where to start reading from
 * @param num_bits the number of bits to read
 *
 * @return the read value
 */
uint16_t read_bits(mem_word *word, int start_bit, int num_bits);

/**
 * @brief Function to write a value to specific bits in a given mem_word variable.
 *
 * @param word
 * @param start_bit
 * @param num_bits
 * @param value
 */
void write_bits(mem_word *word, int start_bit, int num_bits, uint16_t value);

void LOG_AS_BINARY(mem_word address);

/**
 * @brief resets the state of our machine code compiler, including IC and DC.
 * this should be called before processing each file.
 */
void reset_mc_state(void);

/**
 * @brief cleans up machine code data structures and frees memory.
 * this includes entries and extern lists as well as symbol table.
 */
void cleanup_mc_state(void);

#endif
