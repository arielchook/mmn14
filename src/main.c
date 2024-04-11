/**
 * @file main.c
 * @brief Main function for the assembler program.
 *
 * This file contains the implementation of the main function, which serves as the entry point
 * for the assembler program. The main function checks for input files provided as command-line
 * arguments and processes each file independently. After processing each file, it cleans up
 * the machine code state in preparation for the next file.
 *
 * @authors Ariel Cohen, Jonathan Transky
 */

#include <utils.h>
#include <messages.h>
#include <filemgr.h>
#include <stdlib.h>
#include <machinecode.h>

/**
 * @brief Main function for the assembler program.
 *
 * This function serves as the entry point for the assembler program. It checks for input files
 * provided as command-line arguments and processes each file independently. After processing
 * each file, it cleans up the machine code state in preparation for the next file.
 *
 * @param argc The count of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return Returns 0 on successful completion.
 */
int main(int argc, char *argv[])
{
    int fileno;

    /* Make sure the user passed at least one parameter to the program */
    if (argc < 2)
    {
        printf(ERR_USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }

    /* We process each file given in arguments independently */
    for (fileno = 1; fileno < argc; fileno++)
    {
        processFile(argv[fileno]);
        cleanup_mc_state();
    }
    return 0;
}
