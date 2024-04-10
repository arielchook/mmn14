/**
 * @file machinecode.c
 * @brief Machine Code Generation and Memory Management.
 *
 * This module contains functions for generating machine code instructions, managing the assembler's memory state,
 * and performing related operations such as serialization and debugging. It includes functions for manipulating
 * the data section and code section of the assembler's memory, as well as functions for advancing the Instruction
 * Counter (IC) and Data Counter (DC), writing words to specific addresses, and dumping the contents of memory sections.
 *
 * The main functions in this file are:
 * - to_twos_complement: Converts a signed integer to a two's complement encoded 16-bit unsigned integer.
 * - serialize_data_section: Serializes and stores a value in the data section, then advances the Data Counter (DC).
 * - serialize_code_mc_word: Serializes and stores a machine code word in the code section at the current IC position and advances IC.
 * - reset_mc_state: Resets the assembler's memory state, including data and code sections, and the counters.
 * - cleanup_mc_state: Performs cleanup operations for the assembler's memory state, such as freeing symbol tables and lists.
 * - dump_data_section: Dumps the current contents of the data section for debugging purposes.
 * - dump_code_section: Dumps the binary representation of all words in the code section for debugging purposes.
 *
 * These functions provide essential functionality for managing the memory layout of the assembler, storing machine code
 * instructions and data, and ensuring proper serialization and debugging capabilities during the assembly process.
 * Machine code generation and memory management are critical components of the assembly process, and these functions
 * facilitate the conversion of assembly code into executable machine code.
 *
 * @authors Ariel Cohen, Jonathan Transky
 */

#include <machinecode.h>
#include <stdio.h>
#include <symbols.h>
#include <string.h>
#include <entries.h>
#include <externs.h>

/**
 * @brief Data section of the assembler's memory, storing defined data.
 */
mem_word dataSection[DATA_SECTION_SIZE];

/**
 * @brief Code section of the assembler's memory, storing machine code instructions.
 */
mem_word codeSection[CODE_SECTION_SIZE];

/**
 * @brief Instruction Counter (IC), tracking the current address in the code section.
 */
mem_word IC;

/**
 * @brief Data Counter (DC), tracking the current address in the data section.
 */
mem_word DC;

/**
 * @brief Converts a signed integer to a two's complement encoded 16-bit unsigned integer.
 * @param num The integer to convert.
 * @return The two's complement encoded value.
 */
uint16_t to_twos_complement(int num)
{
    if (num < 0)
    {
        /* Determine the mask to set the leading bits to 1 */
        uint16_t mask = 1 << (MC_ADDR_SPACE_BITS - 1);
        /* Perform bitwise OR with the mask to set the leading bits to 1 */
        return (num & ((1 << MC_ADDR_SPACE_BITS) - 1)) | mask;
    }
    /* If the number is non-negative, return as is */
    return num;
}

/**
 * @brief Serializes and stores a value in the data section, then advances the Data Counter (DC).
 * @param value The value to be stored in the data section. Must be a 16-bit unsigned integer in a two-complement encoding.
 * @return True if the value was successfully stored, False if the data section is full.
 */
bool serialize_data_section(mem_word value)
{
    write_data_word(getDC(), value);
    return advanceDC(1);
}

/**
 * @brief Gets the current value of the Data Counter (DC).
 * @return The current value of DC.
 */
int getDC(void) { return DC; }

/**
 * @brief Gets the current value of the Instruction Counter (IC).
 * @return The current value of IC.
 */
int getIC(void) { return IC; }

/**
 * @brief Advances the Data Counter (DC) by a specified amount, ensuring it does not exceed the data section size.
 * @param howmuch The amount by which the DC should be advanced.
 * @return True if the DC was advanced without exceeding the limit, False otherwise.
 */
bool advanceDC(int howmuch)
{
    DC += howmuch;
    if (DC > DATA_SECTION_SIZE)
    {
        printf(ERR_DATA_SECTION_FULL);
        return false;
    }
    return true;
}

/**
 * @brief Advances the Instruction Counter (IC) by a specified amount, ensuring it does not exceed the code section size.
 * @param howmuch The amount by which the IC should be advanced.
 * @return True if the IC was advanced without exceeding the limit, False otherwise.
 */
bool advanceIC(int howmuch)
{
    IC += howmuch;
    if (IC > CODE_SECTION_SIZE)
    {
        printf(ERR_CODE_SECTION_FULL);
        return false;
    }
    return true;
}

/**
 * @brief Writes a word to a specific address in the code section.
 * @param address The address in the code section where the word should be written.
 * @param value The word to be written to the code section.
 */
void write_code_word(mem_word address, mem_word value)
{
    codeSection[address - BASE_CODE_ADDRESS] = value;
}

/**
 * @brief Retrieves a pointer to a word in the code section at a specific address.
 * @param address The address in the code section from which to retrieve the word.
 * @return A pointer to the word at the specified address, or NULL if the address is out of bounds.
 */
mem_word *code_word_at(mem_word address)
{
    /* make sure we're not trying to access an area that is beyond the code section */
    if (address < BASE_CODE_ADDRESS || address > getIC())
        return NULL;
    return &codeSection[address - BASE_CODE_ADDRESS];
}

/**
 * @brief Writes a word to a specific address in the data section.
 * @param address The address in the data section where the word should be written.
 * @param value The word to be written to the data section. Must be in two-complement encoding.
 */
void write_data_word(mem_word address, mem_word value)
{
    dataSection[address - BASE_DATA_ADDRESS] = value;
}

/**
 * @brief Retrieves a pointer to a word in the data section at a specific address.
 * @param address The address in the data section from which to retrieve the word.
 * @return A pointer to the word at the specified address, or NULL if the address is out of bounds.
 */
mem_word *data_word_at(mem_word address)
{
    /* make sure we're not trying to access an area that is beyond the data section */
    if (address < BASE_DATA_ADDRESS || address > getDC())
        return NULL;
    return &dataSection[address - BASE_DATA_ADDRESS];
}

/**
 * @brief Dumps the current contents of the data section for debugging purposes.
 *
 * This function iterates through the data section starting from the base address up to the current
 * value of the Data Counter (DC). For each address, it retrieves the data word and logs it. If the
 * data word at a given address is NULL, indicating an uninitialized or invalid address, it logs
 * "(null)". This is useful for visualizing the state of the data section at any point during
 * the assembly process.
 */
void dump_data_section(void)
{
    int i = 0;
    mem_word *data_word = NULL;
    LOG("\nData section:\n");
    for (i = BASE_DATA_ADDRESS; i < getDC(); i++)
    {
        data_word = data_word_at(i);
        if (data_word == NULL)
        {
            LOG("%.4d: (null)\n", i);
        }
        else
        {
            LOG("%.4d: [%d]\n", i, *data_word);
        }
    }
}

/**
 * @brief Resets the Instruction Counter (IC) to the base address of the code section.
 */
void resetIC(void)
{
    IC = BASE_CODE_ADDRESS;
}

/**
 * @brief Resets the Data Counter (DC) to the base address of the data section.
 */
void resetDC(void)
{
    DC = BASE_DATA_ADDRESS;
}

/**
 * @brief Resets the assembler's memory state, including data and code sections, and the counters.
 */
void reset_mc_state(void)
{
    resetDC();
    resetIC();

    /* zero out our memory structures */
    memset(dataSection, 0, sizeof(dataSection));
    memset(codeSection, 0, sizeof(codeSection));
}

/**
 * @brief Performs cleanup operations for the assembler's memory state, such as freeing symbol tables and lists.
 */
void cleanup_mc_state(void)
{
    /* delete the entries and externs list as well as the symbol table */
    entries_delete_list();
    externs_delete_list();
    free_symbol_table();
}

/**
 * @brief Writes a value to specific bits within a word, starting at a specified bit.
 * @param word Pointer to the word to be modified.
 * @param start_bit The starting bit position within the word where the value is to be written.
 * @param num_bits The number of bits to write from the value.
 * @param value The value to write into the specified bits of the word.
 */
void write_bits(mem_word *word, int start_bit, int num_bits, uint16_t value)
{
    /* Perform bounds checking */
    if (start_bit < 0 || start_bit >= 14 || num_bits <= 0 || num_bits > 14 || start_bit + num_bits > 14)
    {
        /* this should not happen.. */
        return;
    }

    /* Clear the bits to be written */
    *word &= ~(((1 << num_bits) - 1) << start_bit);

    /* Write the value to the specified bits */
    *word |= (value & ((1 << num_bits) - 1)) << start_bit;
}

/**
 * @brief Reads a value from specific bits within a word, starting at a specified bit.
 * @param word Pointer to the word from which to read the value.
 * @param start_bit The starting bit position within the word from which to read the value.
 * @param num_bits The number of bits to read.
 * @return The value read from the specified bits of the word.
 */
uint16_t read_bits(mem_word *word, int start_bit, int num_bits)
{
    uint16_t mask = (1 << num_bits) - 1;

    return ((*word >> start_bit) & mask);
}

/**
 * @brief Serializes and stores a machine code word in the code section at the current IC position and advances IC.
 * @param word Pointer to the mc_word structure representing the machine code word to be serialized.
 * @return True if the word was successfully serialized and stored, False otherwise.
 */
bool serialize_code_mc_word(mc_word *word)
{
    mem_word *word_at_IC;
    if (word == NULL)
        return false;
    word_at_IC = code_word_at(IC);
    switch (word->type)
    {
    case WT_INSTRUCTION:
        write_bits(word_at_IC, 0, 2, word->contents.instruction.A_R_E);
        write_bits(word_at_IC, 2, 2, word->contents.instruction.dest_addressing);
        write_bits(word_at_IC, 4, 2, word->contents.instruction.src_addressing);
        write_bits(word_at_IC, 6, 4, word->contents.instruction.opcode);
        write_bits(word_at_IC, 10, 4, 0); /* unused. set to 0 */
        break;
    case WT_IMMEDIATE:
        write_bits(word_at_IC, 0, 2, word->contents.immediate.A_R_E);
        write_bits(word_at_IC, 2, 12, word->contents.immediate.value);
        break;
    case WT_DIRECT:
        write_bits(word_at_IC, 0, 2, word->contents.direct.A_R_E);
        write_bits(word_at_IC, 2, 12, word->contents.direct.address);

        /* if this word references an external symbol - add it to the list of externs */
        if (word->contents.direct.external_symbol != NULL)
        {
            externs_append(word->contents.direct.external_symbol, getIC());
        }
        break;
    case WT_FIXED_INDEX:
        write_bits(word_at_IC, 0, 2, word->contents.fixed_index.A_R_E_1);
        write_bits(word_at_IC, 2, 12, word->contents.fixed_index.array);
        /* if this word references an external symbol - add it to the list of externs */
        if (word->contents.fixed_index.external_symbol != NULL)
        {
            externs_append(word->contents.fixed_index.external_symbol, getIC());
        }

        LOG_AS_BINARY(getIC());

        /* advance the instruction counter by 1, since a fixed index addressing takes 2 words */
        advanceIC(1);
        word_at_IC = code_word_at(IC);
        write_bits(word_at_IC, 0, 2, word->contents.fixed_index.A_R_E_2);
        write_bits(word_at_IC, 2, 12, word->contents.fixed_index.index);
        break;
    case WT_DIRECT_REG:
        write_bits(word_at_IC, 0, 2, word->contents.direct_reg.A_R_E);
        write_bits(word_at_IC, 2, 3, word->contents.direct_reg.dest);
        write_bits(word_at_IC, 5, 3, word->contents.direct_reg.src);
        write_bits(word_at_IC, 8, 6, 0);
        break;
    default:
        return false;
    }

    /* debug print the word in memory */
    LOG_AS_BINARY(getIC());

    /* advance the instruction counter by 1 */
    advanceIC(1);
    return true;
}

/**
 * @brief Logs the binary representation of a word at a specific address in the code section for debugging purposes.
 * @param address The address of the word in the code section to be logged.
 */
void LOG_AS_BINARY(mem_word address)
{
#ifdef DEBUG
    int j;
    mem_word *word = code_word_at(address);
    LOG("%.4u: ", address);

    if (word == NULL)
    {
        LOG("(null)\n");
        return;
    }

    /* Print binary representation with spaces between each bit */
    for (j = MC_WORD_SIZE_BITS - 1; j >= 0; j--)
    {
        LOG("%2d ", (*word >> j) & 1);
    }
    LOG("\t(%u)\n", *word);
#endif
}

/**
 * @brief Dumps the binary representation of all words in the code section for debugging purposes.
 */
void dump_code_section(void)
{
    int i;
    LOG("Code section:\n\t");
    LOG("\t");
    /* Iterate over each element in the array */
    for (i = MC_WORD_SIZE_BITS - 1; i >= 0; i--)
    {
        LOG("%2d ", i);
    }
    LOG("\n");

    for (i = BASE_CODE_ADDRESS; i < getIC(); i++)
    {
        LOG_AS_BINARY(i);
    }
}
