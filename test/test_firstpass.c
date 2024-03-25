#include <firstpass.h>
#include <symbols.h>
#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <machinecode.h>
#include <unressym.h>

int main(int argc, char *argv[])
{
    FILE *in;
    bool success = true;
    if (argc < 2)
        return 1;
    printf("input:%s\n", argv[1]);
    in = fopen(argv[1], "r");
    success = firstPass(in);

    fclose(in);
    dump_symbols_table();
    dump_data_section();
    dump_code_section();
    dump_unresolved();
    return success ? 0 : 1;
}
