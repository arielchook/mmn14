#include <stdio.h>
#include <messages.h>
#include <utils.h>
#include <string.h>
#include <precomp.h>
#include <firstpass.h>
#include <secondpass.h>
#include <machinecode.h>

bool processFile(char *fname)
{
    FILE *asFile, *amFile;
    char *fnameWext;

    /* resets the state for the compiler */
    reset_mc_state();

    fnameWext = (char *)safe_malloc(strlen(fname) + 4);
    /* add .as extension and try to open the input file */
    sprintf(fnameWext, "%s.as", fname);
    if ((asFile = fopen(fnameWext, "r")) == NULL)
    {
        printf(ERR_FILE_CANT_BE_READ, fnameWext);
        free(fnameWext);
        return false;
    }

    /* add .am extension and try to open the macro output file.
    no need to allocate a new file name since it's the same length */
    sprintf(fnameWext, "%s.am", fname);
    if ((amFile = fopen(fnameWext, "w")) == NULL)
    {
        printf(ERR_FILE_CANT_BE_WRITTEN, fnameWext);
        free(fnameWext);
        return false;
    }

    /* run precompiler to generate .am file from .as file */
    if (!precompile(asFile, amFile))
    {
        printf(ERR_FOUND_IN_PRECOMP);
        return false;
    }

    /* we don't need access to the .as file anymore - close file */
    if (fclose(asFile) != 0)
    {
        printf(ERR_CLOSING_FILE, fname, ".as");
        return false;
    }

    /* close the .am file and reopen it for for reading */
    if (fclose(amFile) != 0)
    {
        printf(ERR_CLOSING_FILE, fname, ".am");
        return false;
    }
    if ((amFile = fopen(fnameWext, "r")) == NULL)
    {
        printf(ERR_FILE_CANT_BE_READ, fnameWext);
        free(fnameWext);
        return false;
    }

    /* run first pass on the .am file */
    if (!firstPass(amFile))
    {
        printf(ERR_FOUND_IN_FIRSTPASS);

        if (fclose(amFile) != 0)
        {
            printf(ERR_CLOSING_FILE, fname, ".am");
        }

        return false;
    }

    if (!secondPass(asFile, amFile))
    {
        printf(ERR_FOUND_IN_SECONDPASS);
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    int fileno;
    /* make sure the user passed at least one parameter to the program */
    if (argc < 2)
    {
        printf(ERR_USAGE, argv[0]);
        exit(1);
    }

    /* We process each file given in arguments independently */
    for (fileno = 0; fileno < argc; fileno++)
    {
        processFile(argv[fileno]);
    }
    return 0;
}