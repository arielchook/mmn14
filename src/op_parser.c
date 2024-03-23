#include <op_parser.h>
#include <reserved.h>
#include <messages.h>
#include <utils.h>
#include <symbols.h>
#include <mcwordtypes.h>
#include <machinecode.h>
#include <unressym.h>

bool handle_immediate(char *op, int lineNumber, mc_word *word, uint8_t *addressing_type)
{
    int intValue;
    char *ptr;
    SymbolBlock *sb;

    op++; /* skip the # */
    /* try to convert to int */
    intValue = strtol(op, &ptr, 10);

    /* this means it's a string. check if it's a name of a constant */
    if (*ptr != '\0')
    {
        sb = find_symbol(op);
        if (sb == NULL || sb->type != ST_DEFINE)
        {
            printf(ERR_CANT_FIND_DEFINE, lineNumber, op);
            return false;
        }

        /* at this point we found a constant with that name. use its value */
        intValue = *(int *)(sb->value);
    }
    *addressing_type = 0; /*immediate*/

    word->type = WT_IMMEDIATE;
    word->immediate.A_R_E = 0;
    word->immediate.value = intValue; /* TODO: check overflow */
    return true;
}

bool handle_fixed_addressing(char *op, int lineNumber, mc_word *word, uint8_t *addressing_type)
{
    char *array_name, *array_index;
    int array_name_address, intValue;
    char *ptr;
    SymbolBlock *sb;

    /* chop the ']' */
    op[strlen(op) - 1] = '\0';

    /* extract the array name and the array index */
    if (((array_name = extractWordSeparator(op, 1, NULL, '[')) == NULL) ||
        ((array_index = extractWordSeparator(op, 2, NULL, '[')) == NULL))
    {
        printf(ERR_MALFORMED_ARRAY, lineNumber);
        return false;
    }
    rtrim(array_name);

    /* look up the array name in the symbol list */
    sb = find_symbol(array_name);
    if (sb == NULL || sb->type != ST_DEFINE)
    {
        printf(ERR_CANT_FIND_DEFINE, lineNumber, array_name);
        return false;
    }
    array_name_address = *(int *)(sb->value);

    /* try to convert to int */
    intValue = strtol(array_index, &ptr, 10);

    /* this means it's a string. check if it's a name of a constant */
    if (*ptr != '\0')
    {
        sb = find_symbol(array_index);
        if (sb == NULL || sb->type != ST_DEFINE)
        {
            printf(ERR_CANT_FIND_DEFINE, lineNumber, array_index);
            return false;
        }

        /* at this point we found a constant with that name. use its value */
        intValue = *(int *)(sb->value);
    }
    /* array index must be non negative */
    if (intValue < 0)
    {
        printf(ERR_ARR_IND_NEGATIVE, lineNumber);
        return false;
    }
    /* TODO: check for out bounds? */

    *addressing_type = 2; /*fixed index*/

    /* first word is the array address */
    /* second word is the array index */
    word->type = WT_FIXED_INDEX;
    /* FIXME: could this label be external? it's a data label */
    word->fixed_index.A_R_E_1 = 2; /* 0b10 - no binary literals in ansi c.. */
    word->fixed_index.array = array_name_address;
    word->fixed_index.A_R_E_2 = 0; /*0b00*/
    word->fixed_index.index = intValue;
    return true;
}

bool handle_direct_register(char *op, int lineNumber, mc_word *word, uint8_t *addressing_type)
{
    int reg_num = op[1] - '0';

    *addressing_type = 3; /*direct register*/

    word->type = WT_DIRECT_REG;
    word->direct_reg.regnum = reg_num;
    return true;
}

bool handle_direct_addressing(char *op, int lineNumber, mc_word *word, uint8_t *addressing_type)
{
    SymbolBlock *sb;

    /* in this case the symbol might not have been defined yet*/
    sb = find_symbol(op);
    /* if it's not found we'll add it to the list of symbols that need to be resolved in 2nd pass */
    if (sb == NULL)
    {
        append_unersolved_symbol(op, IC + 1); /* FIXME: is this the correct address? */
    }
    else
    {
        switch (sb->type)
        {
        case ST_DATA:
        case ST_STRING:
        case ST_CODE:
            *addressing_type = 1; /*direct */

            word->type = WT_DIRECT;
            word->direct.A_R_E = 2;                     /* 0b10 - internal relocatable symbol */
            word->direct.address = *(int *)(sb->value); /* address of define */
            break;
        case ST_EXTERN:
            *addressing_type = 1; /*direct */

            word->type = WT_DIRECT;
            word->direct.A_R_E = 1;   /*0b01 - external symbol */
            word->direct.address = 0; /* FIXME: what should we put here? */
            break;
        case ST_DEFINE:
            printf(ERR_DEFINE_DISALLOWED, lineNumber);
            return false;
            break;
        case ST_ENTRY:
            /* TODO: handle .entry */
            return false;
            break;
        }
    }
    return true;
}

bool process_operand(char *op, uint8_t address_rules, int lineNumber, mc_word *word, uint8_t *addressing_type)
{
    /* make sure it's not empty */
    ltrim(op);
    if (strlen(op) == 0)
    {
        printf(ERR_EMPTY_OPERAND, lineNumber);
    }
    rtrim(op);

    /* 1. immediate addressing */
    if (startsWith(op, "#"))
    {
        /* is immediate addressing allowed for this operand? */
        if (!(address_rules & AR_IMDT))
        {
            printf(ERR_IMDT_DISALLOWED, lineNumber);
            return false;
        }
        if (!handle_immediate(op, lineNumber, word, addressing_type))
            return false;

        return true;
    }

    /* fixed index addressing */
    if (endsWith(op, "]"))
    {
        /* is fixed index addressing allowed for this operand? */
        if (!(address_rules & AR_FXIND))
        {
            printf(ERR_FXIND_DISALLOWED, lineNumber);
            return false;
        }

        if (!handle_fixed_addressing(op, lineNumber, word, addressing_type))
            return false;

        return true;
    }

    /* direct register addressing */
    if (is_register(op))
    {
        /* is register addressing allowed for this operand? */
        if (!(address_rules & AR_DRREG))
        {
            printf(ERR_DRREG_DISALLOWED, lineNumber);
            return false;
        }
        if (!handle_direct_register(op, lineNumber, word, addressing_type))
            return false;

        return true;
    }

    /* if none of the above it means it's direct addressing */

    /* is direct addressing allowed for this operand? */
    if (!(address_rules & AR_DRCT))
    {
        printf(ERR_DIRECT_DISALLOWED, lineNumber);
        return false;
    }

    if (!handle_direct_addressing(op, lineNumber, word, addressing_type))
        return false;

    return true;
}

bool parse_operands(char *stmt, int lineNumber, const instruction_props *props)
{
    mc_word word1st, *wordSrc = NULL, *wordDest = NULL;

    /* get the first operand, second operand and 3rd operand if such */
    char *op_src = extractWordSeparator(stmt, 1, NULL, OP_SEPARATOR);
    char *op_dest = extractWordSeparator(stmt, 2, NULL, OP_SEPARATOR);
    char *more_ops = extractWordSeparator(stmt, 3, NULL, OP_SEPARATOR);

    /* if any of the following applies we have more operands than we should:
    1. more than 2 operands or
    2. required number of operands for the command is 1 and there are 2 or
    3. required number of operands for the command is 0 and there is more than 0 */
    if ((more_ops != NULL) || ((op_dest != NULL) && (props->num_operands < 2)) || ((op_src != NULL) && (props->num_operands < 1)))
    {
        printf(ERR_NUM_OPERANDS, lineNumber, props->instruction);
        return false;
    }

    /* we need to prepare the first word of the instruction and then depending on the command, operands and addressing
    we need to prepare the source operand word and dest operand word for memory encoding */

    /* first word is the instruction word. it will always exist */
    word1st.type = WT_INSTRUCTION;
    word1st.instruction.A_R_E = 0;
    word1st.instruction.opcode = props->opcode;

    /* is there a source operand ? */
    if (op_src != NULL)
    {
        wordSrc = safe_malloc(sizeof(mc_word));
        if (!process_operand(op_src, props->op_src_addr_rules, lineNumber, wordSrc, &word1st.instruction.src_addressing))
            return false;
    }

    if (op_dest != NULL)
    {
        wordDest = safe_malloc(sizeof(mc_word));

        if (!process_operand(op_dest, props->op_dest_addr_rules, lineNumber, wordDest, &word1st.instruction.dest_addressing))
            return false;
    }

    /* now that we've parsed everything, time to write it to memory */
    /* first word is the instruction word. always gets written */
    write_code_word(word1st);

    /* only if there's a source operand */
    if (wordSrc != NULL)
    {
        write_code_word(wordSrc);
        free(wordSrc);
    }
    /* only if there's a dest operand */
    if (wordDest != NULL)
    {
        write_code_word(wordDest);
        free(wordDest);
    }
    return true;
}