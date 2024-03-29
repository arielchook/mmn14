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

mem_word codeSection[DATA_SECTION_SIZE];

int IC = BASE_CODE_ADDRESS;
int DC = BASE_DATA_ADDRESS;

unsigned mc_flags = 0;

/**
 * @brief Function to write a signed integer value in two's complement form using 12 bits into a 14-bit mem_word
 *
 * @param word
 * @param value
 * @return true
 * @return false
 */
bool write_signed_value(mem_word *word, int value)
{
    if (word == NULL)
        return false;

    /* Clamp value to fit within 12 bits */
    value &= 0xFFF;

    /* Sign-extend the value if negative */
    if (value & 0x800)
    {
        value |= 0xF000;
    }

    /* Write the value to the mem_word */
    *word = value;

    return true;
}

bool write_data_section(mem_word value)
{
    if (DC >= DATA_SECTION_SIZE)
    {
        return false;
    }
    write_signed_value(&dataSection[DC], value);
    DC++;
    return true;
}

void dump_data_section()
{
    int i = 0;
    printf("\nData section:\n");
    for (i = 0; i < DC; i++)
    {
        printf("%.4d: [%d]\n", i, dataSection[i]);
    }
}

void resetIC()
{
    IC = BASE_CODE_ADDRESS;
}
void resetDC()
{
    DC = BASE_DATA_ADDRESS;
}

void reset_mc_state()
{
    resetDC();
    resetIC();

    /* zero out our memory structures */
    memset(dataSection, 0, sizeof(dataSection));
    memset(codeSection, 0, sizeof(codeSection));
}

void cleanup_mc_state()
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

bool write_code_word(mc_word *word)
{
    if (word == NULL)
        return false;
    switch (word->type)
    {
    case WT_INSTRUCTION:
        write_bits(&codeSection[IC], 0, 2, word->contents.instruction.A_R_E);
        write_bits(&codeSection[IC], 2, 2, word->contents.instruction.dest_addressing);
        write_bits(&codeSection[IC], 4, 2, word->contents.instruction.src_addressing);
        write_bits(&codeSection[IC], 6, 4, word->contents.instruction.opcode);
        write_bits(&codeSection[IC], 10, 4, 0); /* unused. set to 0 */
        break;
    case WT_IMMEDIATE:
        write_bits(&codeSection[IC], 0, 2, word->contents.immediate.A_R_E);
        write_bits(&codeSection[IC], 2, 12, word->contents.immediate.value);
        break;
    case WT_DIRECT:
        write_bits(&codeSection[IC], 0, 2, word->contents.direct.A_R_E);
        write_bits(&codeSection[IC], 2, 12, word->contents.direct.address);

        /* if this word references an external symbol - add it to the list of externs */
        if (word->contents.direct.external_symbol != NULL)
        {
            externs_append(word->contents.direct.external_symbol, IC);
        }
        break;
    case WT_FIXED_INDEX:
        write_bits(&codeSection[IC], 0, 2, word->contents.fixed_index.A_R_E_1);
        write_bits(&codeSection[IC], 2, 12, word->contents.fixed_index.array);
        /* if this word references an external symbol - add it to the list of externs */
        if (word->contents.fixed_index.external_symbol != NULL)
        {
            externs_append(word->contents.fixed_index.external_symbol, IC);
        }
        printf("%.4u: ", IC);
        print_binary(codeSection[IC]);
        IC++;
        write_bits(&codeSection[IC], 0, 2, word->contents.fixed_index.A_R_E_2);
        write_bits(&codeSection[IC], 2, 12, word->contents.fixed_index.index);
        break;
    case WT_DIRECT_REG:
        write_bits(&codeSection[IC], 0, 2, word->contents.direct_reg.A_R_E);
        write_bits(&codeSection[IC], 2, 3, word->contents.direct_reg.dest);
        write_bits(&codeSection[IC], 5, 3, word->contents.direct_reg.src);
        write_bits(&codeSection[IC], 8, 6, 0);
        break;
    default:
        return false; /* TODO: say something */
    }
    /* advance the instruction counter by 1 */
    printf("%4u: ", IC);
    print_binary(codeSection[IC]);
    IC++;
    return true;
}

void print_binary(mem_word word)
{
    int j;
    /* Print binary representation with spaces between each bit */
    for (j = MC_WORD_SIZE_BITS; j >= 0; j--)
    {
        printf("%2d ", (word >> j) & 1);
    }
    printf("\t(%x)", word);
    printf("\n");
}

void dump_code_section()
{
    int i;
    printf("Code section:\n");
    printf("\t");
    /* Iterate over each element in the array */
    for (i = MC_WORD_SIZE_BITS - 1; i >= 0; i--)
    {
        printf("%2d ", i);
    }
    printf("\n");

    for (i = BASE_CODE_ADDRESS; i < IC; i++)
    {
        printf("%.4d:\t", i);
        print_binary(codeSection[i]);
    }
}
