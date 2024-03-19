#include <stdio.h>
#include <messages.h>
#include <utils.h>
#include <string.h>
#include <precomp.h>
#include <firstpass.h>
#include <secondpass.h>

bool processFile(char *fname)
{
    FILE *asFile, *amFile;
    char *fnameWext = (char *)safe_malloc(strlen(fname) + 4);
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

    if (!preprocess(asFile, amFile))
    {
        printf(ERR_FOUND_IN_PRECOMP);
        return false;
    }
    /* we don't need the .as file anymore */
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
    /* make sure the user passed at least one parameter to the program */
    if (argc < 2)
    {
        printf(ERR_USAGE, argv[0]);
        exit(1);
    }

    /* We process each file given in arguments independently */
    while (--argc)
    {
        processFile(argv[argc]);
    }
    return 0;
}