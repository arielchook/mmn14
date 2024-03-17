#ifndef MESSAGES_H
#define MESSAGES_H


#define ERR_USAGE "Usage: %s <file1> [<file2> .. <fileN>]\nWhere file1..fileN are .as files\n"
#define ERR_FILE_CANT_BE_READ "%s: file could not be opened for reading\n"
#define ERR_FILE_CANT_BE_WRITTEN "%s: cannot open file for writing\n"
#define ERR_MEM_ALLOC_FAILED "Memory allocation failed.\n"


/* Error messages for the pre-processing step */
#define PP_ERR_NO_NESTED_MACROS "Precomp:%d:Nested macros are not allowed\n"
#define PP_ERR_EXTRA_CHARS "Precomp:%d:Extraneous characters after macro name\n"
#define PP_ERR_INVALID_MACRO_NAME "Precomp:%d:invalid macro name specified\n"
#define PP_ERR_RESERVED_WORD "Precomp:%d:Cannot use reserved word as a macro name(%s)\n"
#define PP_ERR_EXTRA_ENDMCR "Precomp:%d:Extraneous characters after endmcr command\n"
#define PP_ERR_ENDMCR_MISLOCATION "Precomp:%d:endmcr encountered but not inside macro definition\n"

/* Error messages for definition step */

#define ERR_NO_NESTED_DEFINITIONS "Definition:%d: Nested definitions are not allowed\n"
#define ERR_INVALID_DEFINITION_NAME "Definition:%d: invalid definition name specified\n"
#define ERR_RESERVED_WORD "Definition:%d: Cannot use reserved words as a definition name\n"
#define ERR_INVALID_DEFINE "Definition:%d: Invalid number of arguments in definition statement\n"
#define ERR_DEF_NOT_STRING "Definition:%d: Definition name is not a string\n"

#endif