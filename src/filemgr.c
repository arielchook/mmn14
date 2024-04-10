/**
 * @file filemgr.c
 * @brief Assembly Source File Compilation Process.
 *
 * Implements the complete process of compiling an assembly source file (.as) through
 * precompilation, first pass, and second pass stages, generating intermediate and final
 * output files such as macro-expanded source (.am), entries (.ent), externals (.ext), and
 * object code (.ob). Each stage involves specific processing tasks:
 *
 * - Precompilation: Expands macros and generates a macro-expanded source file.
 * - First Pass: Analyzes the source to build symbol tables and determine memory layout.
 * - Second Pass: Finalizes the machine code by resolving symbol references and generating
 *   the object code.
 *
 * The process involves extensive error checking and reporting, ensuring the source file
 * is correctly processed or providing informative error messages if issues are encountered.
 * The function relies on various utility and handling functions defined in included headers,
 * orchestrating the compilation process and managing output file generation and memory cleanup.
 *
 * @authors Ariel Cohen, Jonathan Transky
 */

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
#include <obwriter.h>

/**
 * @brief Processes an assembly source file through various compilation stages.
 *
 * The function performs precompilation, first pass, and second pass on the given file.
 * It generates intermediate files (.am, .ent, .ext, .ob) throughout the process.
 *
 * @param fname The base name of the file to process, without any extension.
 * @return True if the processing was successful, false if any errors were encountered.
 */
bool processFile(char *fname)
{
    FILE *asFile, *amFile;
    FILE *entFile, *extFile, *obFile;
    char fnameWext[FILENAME_MAX];

    /* Add .as extension and try to open the input file */
    sprintf(fnameWext, "%s%s", fname, AS_EXTENSION);
    if ((asFile = fopen(fnameWext, "r")) == NULL)
    {
        printf(ERR_FILE_CANT_BE_READ, fnameWext);
        return false;
    }

    /* Add .am extension and try to open the macro output file.
       No need to allocate a new file name since it's the same length */
    sprintf(fnameWext, "%s%s", fname, AM_EXTENSION);
    if ((amFile = fopen(fnameWext, "w")) == NULL)
    {
        printf(ERR_FILE_CANT_BE_WRITTEN, fnameWext);
        return false;
    }

    printf(MSG_PROCESSING_FILE, fname);
    printf(MSG_PRECOMPILATION, fname, AS_EXTENSION, fname, AM_EXTENSION);

    /* Run precompiler to generate .am file from .as file */
    if (!precompile(asFile, amFile))
    {
        printf(ERR_FOUND_IN_PRECOMP);
        return false;
    }

    /* We don't need access to the .as file anymore - close file */
    if (fclose(asFile) != 0)
    {
        printf(ERR_CLOSING_FILE, fname, AS_EXTENSION);
        return false;
    }

    /* Close the .am file and reopen it for reading */
    if (fclose(amFile) != 0)
    {
        printf(ERR_CLOSING_FILE, fname, AM_EXTENSION);
        return false;
    }

    printf(MSG_DONE);

    /* Resets the state for the compiler */
    reset_mc_state();

    printf(MSG_FIRST_PASS, fnameWext);

    if ((amFile = fopen(fnameWext, "r")) == NULL)
    {
        printf(ERR_FILE_CANT_BE_READ, fnameWext);
        return false;
    }

    /* Run first pass on the .am file */
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

    /* Done with input files */
    if (fclose(amFile) != 0)
    {
        printf(ERR_CLOSING_FILE, fname, AM_EXTENSION);
        return false;
    }
    printf(MSG_DONE);

    /* Write entries file only if we have entries in the entry table */
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

    /* Write externs file only if we have symbols in the externs */
    if (!externs_is_empty())
    {
        sprintf(fnameWext, "%s%s", fname, EXT_EXTENSION);

        printf(MSG_EXTERNS_FILE, fnameWext);
        if ((extFile = fopen(fnameWext, "w")) == NULL)
        {
            printf(ERR_FILE_CANT_BE_WRITTEN, fnameWext);
            return false;
        }
        externs_dump(extFile);
        if (fclose(extFile) != 0)
        {
            printf(ERR_CLOSING_FILE, fname, EXT_EXTENSION);
            return false;
        }
        printf(MSG_DONE);
    }

    /* Write object file */
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
