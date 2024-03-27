#include <stdio.h>
#include <messages.h>
#include <filemgr.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fileno;
    /* make sure the user passed at least one parameter to the program */
    if (argc < 2)
    {
        printf(ERR_USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }

    /* We process each file given in arguments independently */
    for (fileno = 0; fileno < argc; fileno++)
    {
        processFile(argv[fileno]);
    }
    return 0;
}