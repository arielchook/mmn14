#include <machinecode.h>
#include <stdio.h>

int dataSection[DATA_SECTION_SIZE];
int IC = 0;
int DC = 0;

bool writeToDataSection(int value)
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

unsigned mc_flags = 0;

void reset_mc_state()
{
    DC = BASE_DATA_ADDRESS;
    IC = BASE_CODE_ADDRESS;
    mc_flags = 0;
}
void set_machine_code_flag(unsigned flag)
{
    mc_flags |= flag;
}
