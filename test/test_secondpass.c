#include <firstpass.h>
#include <symbols.h>
#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <machinecode.h>
#include <secondpass.h>
#include <entries.h>
#include <externs.h>

int main(int argc, char *argv[])
{
    FILE *in;
    bool success = true;
    if (argc < 2)
        return 1;
    printf("input:%s\n", argv[1]);
    in = fopen(argv[1], "r");
    success = firstPass(in);
    if (!success)
    {
        printf("firstpass failed..\n");
        return 1;
    }

    fseek(in, 0, SEEK_SET);
    success = secondPass(in);
    fclose(in);
    dump_symbols_table();
    dump_code_section();
    dump_data_section();
    entries_dump(NULL);
    externs_dump(NULL);
    return success ? 0 : 1;
}
