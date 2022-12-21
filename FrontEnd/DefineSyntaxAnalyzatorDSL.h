#ifndef _DEFINE_SYNTAX_ANALYZATOR_DSL_H_
#define _DEFINE_SYNTAX_ANALYZATOR_DSL_H_

#define CUR_TOKEN  programm_tokens->tokens[programm_tokens->cursor]
#define NEXT_TOKEN programm_tokens->tokens[programm_tokens->cursor + 1]
#define PREV_TOKEN programm_tokens->tokens[programm_tokens->cursor - 1]
#define TOKEN_NEXT programm_tokens->cursor++;

#define VAL_TYPE CUR_TOKEN->val_type
#define VAL_NUM  CUR_TOKEN->value.num_val
#define VAL_OP   CUR_TOKEN->value.op_val
#define VAL_KEY  CUR_TOKEN->value.key_val
#define VAL_SEP  CUR_TOKEN->value.sep_val
#define VAL_VAR  CUR_TOKEN->value.var

#define CREATE_EMPTY CreateNode(NODE_NULL_TYPE, ValueCtor(), nullptr, nullptr)

#define CREATE_OP( node, op_code, left, right )              \
    {                                                        \
        Value* value = ValueCtor();                          \
        value->op_val = op_code;                             \
                                                             \
        node = CreateNode(NODE_OP_TYPE, value, left, right); \
    }

#define ADD( node, left, right ) CREATE_OP(node, OP_ADD,  left, right)
#define SUB( node, left, right ) CREATE_OP(node, OP_SUB,  left, right)
#define MUL( node, left, right ) CREATE_OP(node, OP_MUL,  left, right)
#define DIV( node, left, right ) CREATE_OP(node, OP_DIV,  left, right)
#define DEG( node, left, right ) CREATE_OP(node, OP_DEG,  left, right)
#define SQRT(node, left, right ) CREATE_OP(node, OP_SQRT, left, right)
#define SIN( node, left, right ) CREATE_OP(node, OP_SIN,  left, right)
#define COS( node, left, right ) CREATE_OP(node, OP_COS,  left, right)
#define LN(  node, left, right ) CREATE_OP(node, OP_LN,   left, right)
#define EQ(  node, left, right ) CREATE_OP(node, OP_EQ,   left, right)

#define CREATE_NAME( node, var_name )                               \
    {                                                               \
        Value* value = ValueCtor();                                 \
        value->var   = VarCtor(var_name, NOT_NUM);                  \
                                                                    \
        node = CreateNode(NODE_NAME_TYPE, value, nullptr, nullptr); \
    }

#define CheckForLBracket                    \
    if (VAL_TYPE == TOKEN_SEP_TYPE     &&   \
        VAL_SEP  == SEP_L_ROUND_BRACKET  )  \
    {                                       \
        TOKEN_NEXT                          \
    }                                       \
                                            \
    else SYNTAX_ERROR

#define CheckForRBracket                    \
    if (VAL_TYPE == TOKEN_SEP_TYPE     &&   \
        VAL_SEP  == SEP_R_ROUND_BRACKET  )  \
    {                                       \
        TOKEN_NEXT                          \
    }                                       \
                                            \
    else SYNTAX_ERROR


#define CheckForSemiColon                   \
    if (VAL_TYPE == TOKEN_SEP_TYPE &&       \
         VAL_SEP  == SEP_SEMI_COLON  )      \
    {                                       \
        TOKEN_NEXT                          \
    }                                       \
                                            \
    else SYNTAX_ERROR


#endif
