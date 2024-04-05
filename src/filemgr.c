#include <utils.h>
#include <messages.h>
#include <string.h>
#include <precomp.h>
#include <firstpass.h>
#include <secondpass.h>
#include <machinecode.h>
#include <entries.h>
#include <externs.h>
#include <filemgr.h>

bool dump_object_file(FILE *f)
{
    if (f == NULL)
        f = stdout;
    fprintf(f, "%u %u\n", getIC(), getDC());
    /* TODO: write contents of object file from code section+data section */
    return true;
}

bool processFile(char *fname)
{
    FILE *asFile, *amFile;
    FILE *entFile, *extFile, *obFile;
    char fnameWext[FILENAME_MAX];

    /* add .as extension and try to open the input file */
    sprintf(fnameWext, "%s%s", fname, AS_EXTENSION);
    if ((asFile = fopen(fnameWext, "r")) == NULL)
    {
        printf(ERR_FILE_CANT_BE_READ, fnameWext);
        return false;
    }

    /* add .am extension and try to open the macro output file.
    no need to allocate a new file name since it's the same length */
    sprintf(fnameWext, "%s%s", fname, AM_EXTENSION);
    if ((amFile = fopen(fnameWext, "w")) == NULL)
    {
        printf(ERR_FILE_CANT_BE_WRITTEN, fnameWext);
        return false;
    }

    printf(MSG_PROCESSING_FILE, fname);
    printf(MSG_PRECOMPILATION, fname, AS_EXTENSION, fname, AM_EXTENSION);

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

    printf(MSG_DONE);

    /* resets the state for the compiler */
    reset_mc_state();

    printf(MSG_FIRST_PASS, fnameWext);

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

    printf(MSG_DONE);

    printf(MSG_SECOND_PASS, fnameWext);
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
    printf(MSG_DONE);

    /* write entries file only if we have entries in the entry table */
    if (!entries_is_empty())
    {
        sprintf(fnameWext, "%s%s", fname, ENT_EXTENSION);
        printf(MSG_ENTRIES_FILE, fnameWext);
        if ((entFile = fopen(fnameWext, "w")) == NULL)
        {
            printf(ERR_FILE_CANT_BE_WRITTEN, fnameWext);
            return false;
        }
        entries_dump(entFile);
        if (fclose(entFile) != 0)
        {
            printf(ERR_CLOSING_FILE, fname, ENT_EXTENSION);
            return false;
        }
        printf(MSG_DONE);
    }

    /* write externs file only if we have symbols in the externs */
    if (!externs_is_empty())
    {
        sprintf(fnameWext, "%s%s", fname, EXT_EXTENSION);

        printf(MSG_EXTERNS_FILE, fnameWext);
        if ((extFile = fopen(fnameWext, "w")) == NULL)
        {
            printf(ERR_FILE_CANT_BE_WRITTEN, fnameWext);
            return false;
        }
        externs_dump(entFile);
        if (fclose(extFile) != 0)
        {
            printf(ERR_CLOSING_FILE, fname, EXT_EXTENSION);
            return false;
        }
        printf(MSG_DONE);
    }

    /* write object file */
    sprintf(fnameWext, "%s%s", fname, OB_EXTENSION);
    printf(MSG_OBJECT_FILE, fnameWext);

    if ((obFile = fopen(fnameWext, "w")) == NULL)
    {
        printf(ERR_FILE_CANT_BE_WRITTEN, fnameWext);
        return false;
    }
    dump_object_file(obFile);
    if (fclose(obFile) != 0)
    {
        printf(ERR_CLOSING_FILE, fname, OB_EXTENSION);
        return false;
    }

    printf(MSG_DONE);
    return true;
}
