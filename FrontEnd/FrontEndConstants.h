#ifndef _FRONTEND_CONSTANTS_H_
#define _FRONTEND_CONSTANTS_H_


#define DEF_KEY(key_code, key_name, key_lang_name) \
    KEY_##key_name = key_code,

enum KeyWords
{
    NOT_KEY = 0,
    #include "../Dictionary/KeyWords.h"
}

#undef DEF_KEY


#define DEF_SEP(sep_code, sep_name, sep_lang_name) \
    KEY_##sep_name = sep_code,

enum Separators
{
    NOT_SEP = 0,
    #include "../Dictionary/Separators.h"
}

#undef DEF_SEP


#define DEF_OP(op_code, op_name, op_lang_name) \
    KEY_##op_name = op_code,

enum Operators
{
    NOT_OP = 0,
    #include "../Dictionary/Operators.h"
}

#undef DEF_OP


enum TokenDataType
{
    NULL_TYPE = 0,
    OP_TYPE      ,
    SEP_TYPE     ,
    KEY_TYPE     ,
    NUM_TYPE     ,
    VAR_TYPE     ,
}


#endif
