#include <filemgr.h>
#include <messages.h>
#include <utils.h>
#include <string.h>
#include <precomp.h>
#include <firstpass.h>
#include <secondpass.h>
#include <machinecode.h>
#include <entries.h>
#include <externs.h>

bool dump_object_file(FILE *f)
{
    if (f == NULL)
        f = stdout;
    fprintf(f, "%u %u\n", IC, DC);

    return true;
}

bool processFile(char *fname)
{
    FILE *asFile, *amFile;
    FILE *entFile, *extFile, *obFile;
    char fnameWext[FILENAME_MAX];

    /* add .as extension and try to open the input file */
    sprintf(fnameWext, "%s%s", AS_EXTENSION, fname);
    if ((asFile = fopen(fnameWext, "r")) == NULL)
    {
        printf(ERR_FILE_CANT_BE_READ, fnameWext);
        return false;
    }

    /* add .am extension and try to open the macro output file.
    no need to allocate a new file name since it's the same length */
    sprintf(fnameWext, "%s%s", AM_EXTENSION, fname);
    if ((amFile = fopen(fnameWext, "w")) == NULL)
    {
        printf(ERR_FILE_CANT_BE_WRITTEN, fnameWext);
        return false;
    }

    /* resets the state for the compiler */
    reset_mc_state();

    /* run precompiler to generate .am file from .as file */
    if (!precompile(asFile, amFile))
    {
        printf(ERR_FOUND_IN_PRECOMP);
        return false;
    }

    /* we don't need access to the .as file anymore - close file */
    if (fclose(asFile) != 0)
    {
        printf(ERR_CLOSING_FILE, fname, AS_EXTENSION);
        return false;
    }

    /* close the .am file and reopen it for for reading */
    if (fclose(amFile) != 0)
    {
        printf(ERR_CLOSING_FILE, fname, AM_EXTENSION);
        return false;
    }
    if ((amFile = fopen(fnameWext, "r")) == NULL)
    {
        printf(ERR_FILE_CANT_BE_READ, fnameWext);
        return false;
    }

    /* run first pass on the .am file */
    if (!firstPass(amFile))
    {
        printf(ERR_FOUND_IN_FIRSTPASS);

        if (fclose(amFile) != 0)
        {
            printf(ERR_CLOSING_FILE, fname, AM_EXTENSION);
        }

        return false;
    }

    /* Rewind the file pointer back to the beginning of the amFile for the second pass */
    if (fseek(amFile, 0, SEEK_SET) != 0)
    {
        printf(ERR_FSEEK_FILE, fname, AM_EXTENSION);
        if (fclose(amFile) != 0)
        {
            printf(ERR_CLOSING_FILE, fname, AM_EXTENSION);
        }
        return false;
    }

    if (!secondPass(amFile))
    {
        printf(ERR_FOUND_IN_SECONDPASS);

        if (fclose(amFile) != 0)
        {
            printf(ERR_CLOSING_FILE, fname, AM_EXTENSION);
        }
        return false;
    }

    /* done with input files */
    if (fclose(amFile) != 0)
    {
        printf(ERR_CLOSING_FILE, fname, AM_EXTENSION);
        return false;
    }

    /* write entries file only if we have entries in the entry table */
    if (!entries_is_empty())
    {
        sprintf(fnameWext, "%s%s", ENT_EXTENSION, fname);
        if ((entFile = fopen(fnameWext, "r")) == NULL)
        {
            printf(ERR_FILE_CANT_BE_READ, fnameWext);
            return false;
        }
        entries_dump(entFile);
        if (fclose(entFile) != 0)
        {
            printf(ERR_CLOSING_FILE, fname, ENT_EXTENSION);
            return false;
        }
    }

    /* write externs file only if we have symbols in the externs */
    if (!externs_is_empty())
    {
        sprintf(fnameWext, "%s%s", EXT_EXTENSION, fname);
        if ((extFile = fopen(fnameWext, "r")) == NULL)
        {
            printf(ERR_FILE_CANT_BE_READ, fnameWext);
            return false;
        }
        externs_dump(entFile);
        if (fclose(extFile) != 0)
        {
            printf(ERR_CLOSING_FILE, fname, EXT_EXTENSION);
            return false;
        }
    }

    /* write object file */
    sprintf(fnameWext, "%s%s", OB_EXTENSION, fname);
    if ((obFile = fopen(fnameWext, "r")) == NULL)
    {
        printf(ERR_FILE_CANT_BE_READ, fnameWext);
        return false;
    }
    dump_object_file(obFile);
    if (fclose(obFile) != 0)
    {
        printf(ERR_CLOSING_FILE, fname, OB_EXTENSION);
        return false;
    }

    return true;
}
