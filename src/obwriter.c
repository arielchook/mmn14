#include <obwriter.h>
#include <machinecode.h>
#include <stdio.h>

static char ob_encrypted[] = "*#%!";

bool dump_object_file(FILE *f)
{
    int i, j, k;
    uint16_t bits;
    mem_word *icword;
    mem_word *dataword;

    /* use stdout if output file was not provided */
    if (f == NULL)
        f = stdout;

    /* write header row */
    fprintf(f, "%u %u\n", (getIC() - 100), getDC());

    for (i = BASE_CODE_ADDRESS; i < getIC(); i++)
    {
        icword = code_word_at(i);
        fprintf(f, "%.4u ", i);
        for (j = MC_WORD_SIZE_BITS - 2; j >= 0; j -= 2)
        {
            bits = read_bits(icword, j, 2);
            fputc(ob_encrypted[bits], f);
        }
        fprintf(f, "\n");
    }

    for (k = BASE_DATA_ADDRESS; k < getDC(); k++)
    {
        dataword = data_word_at(k);
        fprintf(f, "%.4u ", i + k);

        for (j = MC_WORD_SIZE_BITS - 2; j >= 0; j -= 2)
        {
            bits = read_bits(dataword, j, 2);
            fputc(ob_encrypted[bits], f);
        }
        fprintf(f, "\n");
    }
    return true;
}