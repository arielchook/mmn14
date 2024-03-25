
#include <op_parser.h>
#include <reserved.h>
#include <messages.h>
#include <utils.h>
#include <symbols.h>
#include <mcwordtypes.h>
#include <machinecode.h>
#include <unressym.h>

bool handle_immediate(char *op, int lineNumber, mc_word *word)
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

    /* make sure the int value provided can be represented in memory */
    if ((intValue < MIN_VALUE) || (intValue > MAX_VALUE))
    {
        printf(ERR_INT_OUT_OF_BOUNDS, lineNumber, op, MIN_VALUE, MAX_VALUE);
        return false;
    }

    word->type = WT_IMMEDIATE;
    word->contents.immediate.A_R_E = ARE_ABS;
    word->contents.immediate.value = intValue;
    return true;
}

bool handle_fixed_addressing(char *op, int lineNumber, mc_word *word)
{
    char *array_name, *array_index;
    int intValue;
    char *ptr;
    SymbolBlock *sb;

    /* chop the ']' */
    op[strlen(op) - 1] = '\0';

    /* extract the array name and the array index */
    if (((array_name = extractWordSeparator(op, 1, NULL, ARRAY_OPEN_CHAR)) == NULL) ||
        ((array_index = extractWordSeparator(op, 2, NULL, ARRAY_OPEN_CHAR)) == NULL))
    {
        printf(ERR_MALFORMED_ARRAY, lineNumber);
        return false;
    }
    rtrim(array_name);

    /* look up the array name in the symbol list */
    sb = find_symbol(array_name);
    if (sb != NULL)
    {
        /* found symbol - make sure it's a .data or a .string label */
        /* FIXME: could an extern symbol relate to .data/.string? */
        if (sb->type != ST_DATA && sb->type != ST_STRING)
        {
            printf(ERR_OP_IS_NOT_DATA, lineNumber, array_name);
            return false;
        }

        /* store the address of the array label in the word to be written to memory */
        word->contents.fixed_index.array = *(int *)(sb->value);
        word->contents.fixed_index.unresolved = NULL;
    }
    else
    /* symbol not found, mark it as unresolved so it'll be resolved in 2nd pass */
    {
        word->contents.fixed_index.unresolved = array_name;
    }

    /* first word is the array address */
    word->contents.fixed_index.A_R_E_1 = ARE_RELOC; /* relocatable */

    /* now, look at the array_index - try to convert to int */
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

    /* second word is the array index */
    word->type = WT_FIXED_INDEX;
    word->contents.fixed_index.A_R_E_2 = ARE_ABS; /* absolute */
    word->contents.fixed_index.index = intValue;
    return true;
}

bool handle_direct_register(char *op, int lineNumber, mc_word *word)
{
    /* figure out which register it is */
    int reg_num = op[1] - '0';

    word->type = WT_DIRECT_REG;
    word->contents.direct_reg.A_R_E = ARE_ABS;
    /* we use reg_num since we can't tell at this point if it's a source or a dest operand.
    this will be set correctly by the calling funtion */
    word->contents.direct_reg.src = 0;
    word->contents.direct_reg.dest = 0;
    word->contents.direct_reg.regnum = reg_num;
    return true;
}

bool handle_direct_addressing(char *op, int lineNumber, mc_word *word)
{
    SymbolBlock *sb;

    /* in this case the symbol might not have been defined yet*/
    sb = find_symbol(op);
    /* if it's not found we'll add it to the list of symbols that need to be resolved in 2nd pass */
    if (sb == NULL)
    {
        word->type = WT_DIRECT;
        word->contents.direct.A_R_E = ARE_RELOC; /* relocatable */
        word->contents.direct.unresolved = op;   /* unresolved symbol */
    }
    else
    {
        switch (sb->type)
        {
        case ST_DATA:
        case ST_STRING:
        case ST_CODE:
            word->type = WT_DIRECT;
            word->contents.direct.A_R_E = ARE_RELOC;             /* relocatable symbol */
            word->contents.direct.address = *(int *)(sb->value); /* address of label or data */
            word->contents.direct.unresolved = NULL;
            break;
        case ST_EXTERN:
            word->type = WT_DIRECT;
            word->contents.direct.A_R_E = ARE_EXTERN; /* external symbol */
            word->contents.direct.address = 0;        /* FIXME: use strdup when keeping the operands names? */
            word->contents.direct.unresolved = op;    /* this will add the symbol to the unresolved symbol list. even though we */
                                                      /* we found it. this will cause it to be discovered when we go over the */
                                                      /* unresolved symbols list and will allow us to generate the .ext file */
            break;
        case ST_DEFINE:
            printf(ERR_DEFINE_DISALLOWED, lineNumber);
            return false;
            break;
        }
    }
    return true;
}

bool process_operand(char *op, uint8_t address_rules, int lineNumber, mc_word *word)
{
    /* make sure it's not empty */
    ltrim(op);
    if (strlen(op) == 0)
    {
        printf(ERR_EMPTY_OPERAND, lineNumber);
    }
    rtrim(op);
    printf("|%s", op);
    /* 1. immediate addressing */
    if (startsWith(op, IMMEDIATE_VALUE_PREFIX))
    {
        /* is immediate addressing allowed for this operand? */
        if (!(address_rules & AR_IMDT))
        {
            printf(ERR_IMDT_DISALLOWED, lineNumber);
            return false;
        }
        return handle_immediate(op, lineNumber, word);
    }

    /* fixed index addressing */
    if (endsWith(op, ARRAY_CLOSE_CHAR))
    {
        /* is fixed index addressing allowed for this operand? */
        if (!(address_rules & AR_FXIND))
        {
            printf(ERR_FXIND_DISALLOWED, lineNumber);
            return false;
        }

        return handle_fixed_addressing(op, lineNumber, word);
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
        return handle_direct_register(op, lineNumber, word);
    }

    /* if none of the above it means it's direct addressing */

    /* is direct addressing allowed for this operand? */
    if (!(address_rules & AR_DRCT))
    {
        printf(ERR_DIRECT_DISALLOWED, lineNumber);
        return false;
    }

    return handle_direct_addressing(op, lineNumber, word);
}

bool parse_operands(char *stmt, int lineNumber, const instruction_props *props)
{
    bool success = true;
    mc_word *word1st = NULL, *word_src = NULL, *word_dest = NULL;
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
        success = false;
    }

    if (success)
    {
        /* if we have only 1 operand, it's the dest operand */
        if (props->num_operands == 1)
        {
            op_dest = op_src;
            op_src = NULL;
        }

        /* prepare the first word of the instruction and then depending on the command, operands and addressing
        we need to prepare the source operand word and dest operand word for memory encoding */

        /* first word is the instruction word. it will always exist */
        word1st = safe_malloc(sizeof(mc_word));
        word1st->type = WT_INSTRUCTION;
        word1st->contents.instruction.A_R_E = ARE_ABS;
        word1st->contents.instruction.opcode = props->opcode;
    }

    /* is there a source operand ? */
    if (success && op_src != NULL)
    {
        word_src = safe_malloc(sizeof(mc_word));
        success = process_operand(op_src, props->op_src_addr_rules, lineNumber, word_src);
        if (success)
        {
            /* update the source operand addressing type */
            word1st->contents.instruction.src_addressing = word_src->type;

            /* if op is direct reg need to store the register number in source reg */
            if (word_src->type == WT_DIRECT_REG)
            {
                word_src->contents.direct_reg.src = word_src->contents.direct_reg.regnum;
            }
        }
    }
    if (success && op_dest != NULL)
    {
        word_dest = safe_malloc(sizeof(mc_word));
        success = process_operand(op_dest, props->op_dest_addr_rules, lineNumber, word_dest);
        if (success)
        {
            /* update the dest operand addressing type */
            word1st->contents.instruction.dest_addressing = word_dest->type;
            /* if op is direct reg need to store the register number in source reg */
            /* FIXME: what if src and dest are both regs? */
            if (word_dest->type == WT_DIRECT_REG)
            {
                word_src->contents.direct_reg.dest = word_src->contents.direct_reg.regnum;
            }
        }
    }
    printf("\n");
    if (success)
    {
        /* now that we've parsed everything, time to write it to memory */
        /* first word is the instruction word. always gets written */
        write_code_word(word1st);

        /* there's a special case where we have both source and dest operands
        and they are both registers */
        if ((word_src != NULL) && (word_src->type == WT_DIRECT_REG) && (word_dest != NULL && (word_dest->type == WT_DIRECT_REG)))
        {
            /* we store both registers in one word and so eliminate the word_src */
            word_dest->contents.direct_reg.src = word_src->contents.direct_reg.src;
            word_src = NULL;
        }
        /* only if there's a source operand */
        if (word_src != NULL)
        {
            write_code_word(word_src);
        }
        /* only if there's a dest operand */
        if (word_dest != NULL)
        {
            write_code_word(word_dest);
        }
    }

    /* cleanup */
    free_if_not_null(word1st);
    free_if_not_null(word_src);
    free_if_not_null(word_dest);

    return success;
}