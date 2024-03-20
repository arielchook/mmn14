#include <firstpass.h>
#include <symbols.h>
#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <machinecode.h>

int main(int argc, char *argv[])
{
    FILE *in;
    bool success;
    if (argc<2) return 1;
    printf("input:%s\n",argv[1]);
    in=fopen(argv[1], "r");
    success=firstPass(in);
    fclose(in);
    dump_symbols_table();
    dump_data_section();
    return success ? 0 : 1;
}
