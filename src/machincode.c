#include <machinecode.h>
#include <stdio.h>
#include <symbols.h>
#include <string.h>
#include <entries.h>
#include <externs.h>

/**
 * @brief holds the data section for the compiler
 *
 */
mem_word dataSection[DATA_SECTION_SIZE];

mem_word codeSection[CODE_SECTION_SIZE];

/* instruction counter */
mem_word IC;

/* data counter */
mem_word DC;

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

bool serialize_data_section(mem_word value)
{
    write_data_word(DC, to_twos_complement(value)); /* FIXME: do we need to run to_twos_complement again? */
    return advanceDC(1);
}

int getDC(void) { return DC; }
int getIC(void) { return IC; }
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
void write_code_word(int address, mem_word value)
{
    codeSection[address - BASE_CODE_ADDRESS] = value;
}
mem_word *code_word_at(int address)
{
    return &codeSection[address - BASE_CODE_ADDRESS];
}
mem_word read_code_word(int address)
{
    return codeSection[address - BASE_CODE_ADDRESS];
}
void write_data_word(int address, mem_word value)
{
    dataSection[address - BASE_DATA_ADDRESS] = value;
}
mem_word read_data_word(int address)
{
    return dataSection[address - BASE_DATA_ADDRESS];
}
mem_word *data_word_at(int address)
{
    return &dataSection[address - BASE_DATA_ADDRESS];
}

void dump_data_section(void)
{
    int i = 0;
    LOG("\nData section:\n");
    for (i = BASE_DATA_ADDRESS; i < getDC(); i++)
    {
        LOG("%.4d: [%d]\n", i, read_data_word(i));
    }
}

void resetIC(void)
{
    IC = BASE_CODE_ADDRESS;
}
void resetDC(void)
{
    DC = BASE_DATA_ADDRESS;
}

void reset_mc_state(void)
{
    resetDC();
    resetIC();

    /* zero out our memory structures */
    memset(dataSection, 0, sizeof(dataSection));
    memset(codeSection, 0, sizeof(codeSection));
}

void cleanup_mc_state(void)
{
    /* delete the entries and externs list as well as the symbol table */
    entries_delete_list();
    externs_delete_list();
    free_symbol_table();
}

/**
 * @brief Function to write a value to specific bits in a mem_word variable
 *
 * @param word
 * @param start_bit
 * @param num_bits
 * @param value
 */
void write_bits(mem_word *word, int start_bit, int num_bits, uint16_t value)
{
    /* Perform bounds checking */
    if (start_bit < 0 || start_bit >= 14 || num_bits <= 0 || num_bits > 14 || start_bit + num_bits > 14)
    {
        /* this should happen.. */
        return;
    }

    /* Clear the bits to be written */
    *word &= ~(((1 << num_bits) - 1) << start_bit);

    /* Write the value to the specified bits */
    *word |= (value & ((1 << num_bits) - 1)) << start_bit;
}

uint16_t read_bits(mem_word *word, int start_bit, int num_bits)
{
    uint16_t mask = (1 << num_bits) - 1;

    return ((*word >> start_bit) & mask);
}

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

        LOG_AS_BINARY(getIC(), read_code_word(IC));

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
    LOG_AS_BINARY(getIC(), read_code_word(IC));

    /* advance the instruction counter by 1 */
    advanceIC(1);
    return true;
}

void LOG_AS_BINARY(mem_word address, mem_word word)
{
#ifdef DEBUG
    int j;
    LOG("%.4u: ", address);

    /* Print binary representation with spaces between each bit */
    for (j = MC_WORD_SIZE_BITS - 1; j >= 0; j--)
    {
        LOG("%2d ", (word >> j) & 1);
    }
    LOG("\t(%x)\n", word);
#endif
}

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
        LOG_AS_BINARY(i, read_code_word(i));
    }
}
