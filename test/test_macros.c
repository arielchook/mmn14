#include <precomp.h>
#include <stdlib.h>
#include <stdio.h>
#include <utils.h>

int main(int argc, char *argv[])
{
    FILE *in,*out;
    bool success;
    printf("input:%s output:%s\n",argv[1],argv[2]);
    in=fopen(argv[1], "r");
    out=fopen(argv[2] ,"w");
    success=precompile(in, out);
    fclose(in);
    fclose(out);
    return success ? 0 : 1;
}
