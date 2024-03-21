#include <machinecode.h>
#include <stdio.h>

/**
 * @brief holds the data section for the compiler
 *
 */
int dataSection[DATA_SECTION_SIZE];

int codeSection[DATA_SECTION_SIZE];

int IC = BASE_CODE_ADDRESS;
int DC = BASE_DATA_ADDRESS;
unsigned mc_flags = 0;

bool write_data_section(int value)
{
    if (DC >= DATA_SECTION_SIZE)
    {
        return false;
    }
    dataSection[DC++] = value;
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

void reset_mc_state()
{
    DC = BASE_DATA_ADDRESS;
    IC = BASE_CODE_ADDRESS;
    mc_flags = 0;
}
void set_machine_code_flag(unsigned bitmask)
{
    mc_flags |= bitmask;
}
bool is_mc_flag_set(unsigned bitmask)
{
    /* this will return true of the bits that are on in the bitmask are also tuend on in the machine code state flags */
    return (mc_flags & bitmask);
}
