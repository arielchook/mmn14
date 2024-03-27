#include <reserved.h>
#include <string.h>

const char *directives[] = {"mcr", "endmcr", ".define", ";", ".data", ".string", ".entry", ".extern"};

const instruction_props instruction_table[] = {
    {"mov", 0, 2, AR_ALL, (AR_DRCT | AR_FXIND | AR_DRREG)},
    {"cmp", 1, 2, AR_ALL, AR_ALL},
    {"add", 2, 2, AR_ALL, (AR_DRCT | AR_FXIND | AR_DRREG)},
    {"sub", 3, 2, AR_ALL, (AR_DRCT | AR_FXIND | AR_DRREG)},
    {"not", 4, 1, AR_NA, (AR_DRCT | AR_FXIND | AR_DRREG)},
    {"clr", 5, 1, AR_NA, (AR_DRCT | AR_FXIND | AR_DRREG)},
    {"lea", 6, 2, (AR_IMDT | AR_DRCT), (AR_DRCT | AR_FXIND | AR_DRREG)},
    {"inc", 7, 1, AR_NA, (AR_DRCT | AR_FXIND | AR_DRREG)},
    {"dec", 8, 1, AR_NA, (AR_DRCT | AR_FXIND | AR_DRREG)},
    {"jmp", 9, 1, AR_NA, (AR_DRCT | AR_DRREG)},
    {"bne", 10, 1, AR_NA, (AR_DRCT | AR_DRREG)},
    {"red", 11, 1, (AR_DRCT | AR_FXIND | AR_DRREG)},
    {"prn", 12, 1, AR_NA, AR_ALL},
    {"jsr", 13, 1, AR_NA, (AR_DRCT | AR_DRREG)},
    {"rts", 14, 0, AR_NA, AR_NA},
    {"hlt", 15, 0, AR_NA, AR_NA}};

/**
 * @brief Get the properties for a given command
 *
 * @param cmd
 * @return const instruction_props* corresponding to the cmd provided
 */
const instruction_props *get_instruction_props(char *cmd)
{
    int i;
    long num_words;
    if (cmd == NULL)
        return NULL;

    /* array size */
    num_words = sizeof(instruction_table) / sizeof(instruction_table[0]);

    /* look up the command in the array of available commands and return its properties if found */
    for (i = 0; i < num_words; i++)
    {
        if (strcmp(instruction_table[i].instruction, cmd) == 0)
            return &instruction_table[i];
    }

    /* no such command - return NULL */
    return NULL;
}

/**
 * @brief Returns whether op is a valid register name. Valid names are "r0" to "r7". Case sensitive.
 *
 * @param op operand string
 * @return true valid register name
 * @return false not a register name
 */
bool is_register(char *op)
{
    if (op == NULL)
        return false;
    /* register string is exactly 2 characters with the first one being 'r' */
    if (strlen(op) != 2 || op[0] != 'r')
        return false;
    /* second character is 0 to 7 */
    if ((op[1] < '0') || (op[1] > '7'))
        return false;
    return true;
}

/**
 * @brief Find out whether string given in parameter is a reserved word. A reserved word could be a one of the
 * supported assembly commands, a directive (e.g. .define, .extern etc..) or a name of a register (r0..r7).
 *
 * @param cmd string to check
 * @return true cmd is a reserved word
 * @return false cmd is not a reserved word
 */
bool is_reserved_word(char *cmd)
{
    int i, num_words;
    if (cmd == NULL)
        return false;

    /* check list of supported assembly instructions */
    if (get_instruction_props(cmd) != NULL)
        return true;

    /* check register names */
    if (is_register(cmd))
        return true;

    /* check list of directives */
    num_words = sizeof(directives) / sizeof(directives[0]);
    for (i = 0; i < num_words; i++)
    {
        if (strcmp(directives[i], cmd) == 0)
            return true;
    }
    return false;
}