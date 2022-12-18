#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdlib.h>
#include <cstring>

enum Constants
{
    MAX_NODE_VAL_LEN    = 10,
    MAX_PROGRAMM_LENGTH = 500,
};

#define DEF_KEY(key_code, key_name, key_lang_name) \
    KEY_##key_name = key_code,

enum KeyWords
{
    NOT_KEY = 0,
    #include "../Dictionary/KeyWords.h"
};

#undef DEF_KEY

#define DEF_SEP(sep_code, sep_name, sep_lang_name) \
    SEP_##sep_name = sep_code,

enum Separators
{
    NOT_SEP = 0,
    #include "../Dictionary/Separators.h"
};

#undef DEF_SEP

#define DEF_OP(op_code, op_name, op_lang_name) \
    OP_##op_name = op_code,

enum Operators
{
    NOT_OP = 0,
    #include "../Dictionary/Operators.h"
};

#undef DEF_OP

enum Functions
{
    NOT_FUNC = 0,
};

typedef double Var_val;

struct Var
{
    char*   name ;
    Var_val value;
};

enum VarConstants
{
    VAR_DEAD_VAL = 193726,
};

union Value
{
    KeyWords   key_val;
    Operators  op_val ;
    Separators sep_val;
    Var        var    ;
    double     num_val;
};

enum TokenDataType
{
    TOKEN_NULL_TYPE = 0,

    TOKEN_KEY_TYPE     ,
    TOKEN_OP_TYPE      ,
    TOKEN_SEP_TYPE     ,
    TOKEN_VAR_TYPE     ,
    TOKEN_NUM_TYPE     ,

    TOKEN_END_TYPE     ,
};

enum TokenConstants
{
    NOT_NUM = 872364,
};

enum OpTexPrintModes
{
    OP_TEX_PREPRINT_ONE_ARG ,
    OP_TEX_PREPRINT_TWO_ARGS,
    OP_TEX_INPRINT          ,
};

enum NodeDataType
{
    NODE_NULL_TYPE = 0,

    NODE_NUM_TYPE     ,
    NODE_VAR_TYPE     ,
    NODE_OP_TYPE      ,
    NODE_KEY_TYPE     ,
    NODE_SEP_TYPE     ,
    NODE_FUNC_TYPE    ,
};

enum TreeDumpModes
{
    SIMPLE_DUMP_MODE   ,
    FULL_FULL_DUMP_MODE,
};

// #define TEXT_FOR_PRONOUNCING_FILENAME  "./TextForPronouncing.txt"
// #define DATABASE_DUMP_HTML_FILENAME    "./DumpInfo/DatabaseDump.html"
// #define TEXT_FOR_DOT_DUMP_FILENAME     "./DumpInfo/TextForDatabaseDump.dot"
// #define DATABASE_DUMP_PICTURE_FILENAME "DatabaseDump.svg"
// #define DATABASE_DUMP_PICTURE_FOLDER   "./DumpInfo/"

#endif
