/**
 * @file obwriter.c
 * @brief Function to dump the contents of the code section and data section into a .ob file.
 * 
 * This file contains the implementation of a function to dump the contents of the code section
 * and data section into a .ob file. It writes the header row indicating the size of the code
 * section and data section, followed by the binary representation of each word in the code
 * section and data section.
 */


#include <obwriter.h>
#include <machinecode.h>
#include <stdio.h>

static const char ob_encrypted[] = "*#%!";

/**
 * @brief Dump the contents of the code section and data section into a .ob file
 *
 * @param[in] f The file to write the contents to. If NULL, it will write to stdout
 *
 */
void dump_object_file(FILE *f)
{
    int i, j, k;
    uint16_t bits;
    mem_word *icword;
    mem_word *dataword;

    /* use stdout if output file was not provided */
    if (f == NULL)
        f = stdout;

    /* write header row */
    fprintf(f, "%u %u\n", (getIC() - BASE_CODE_ADDRESS), (getDC() - BASE_DATA_ADDRESS));

    /* write code section */
    for (i = BASE_CODE_ADDRESS; i < getIC(); i++)
    {
        icword = code_word_at(i);
        /* print the address of the code section word */
        fprintf(f, "%.4u ", i);

        /* for each word, we print 2 bits at a time, starting from the MSB */
        for (j = MC_WORD_SIZE_BITS - 2; j >= 0; j -= 2)
        {
            bits = read_bits(icword, j, 2);
            fputc(ob_encrypted[bits], f);
        }
        fprintf(f, "\n");
    }

    /* write data section */
    for (k = BASE_DATA_ADDRESS; k < getDC(); k++)
    {
        dataword = data_word_at(k);
        /* print the address of the code section word */
        fprintf(f, "%.4u ", i + k);

        /* for each word in data section, we print 2 bits at a time, starting from the MSB */
        for (j = MC_WORD_SIZE_BITS - 2; j >= 0; j -= 2)
        {
            bits = read_bits(dataword, j, 2);
            fputc(ob_encrypted[bits], f);
        }
        fprintf(f, "\n");
    }
}