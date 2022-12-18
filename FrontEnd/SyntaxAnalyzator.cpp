#include "../Config.h"
#include <stdlib.h>
#include "../Constants/DefineColourConsts.h"
#include "../Assert.h"
#include "../Constants/Constants.h"
#include "../Tree/Tree.h"
#include "../Tree/TreeDump.h"
#include "./LexicalAnalyzator.h"
#include "./SyntaxAnalyzator.h"

#define CUR_TOKEN programm_tokens->tokens[programm_tokens->cursor]
#define VAL_TYPE CUR_TOKEN->val_type
#define VAL_NUM  CUR_TOKEN->value.num_val
#define VAL_OP   CUR_TOKEN->value.op_val
#define VAL_KEY  CUR_TOKEN->value.key_val
#define VAL_SEP  CUR_TOKEN->value.sep_val
#define VAL_VAR  CUR_TOKEN->value.var

#define CREATE_OP( op_code, left, right )                    \
    {                                                        \
        Value value = {};                                    \
        value.op_val = op_code;                              \
                                                             \
        node = CreateNode(NODE_OP_TYPE, value, left, right); \
    }

#define ADD( left, right ) CREATE_OP(OP_ADD,  left, right)
#define SUB( left, right ) CREATE_OP(OP_SUB,  left, right)
#define MUL( left, right ) CREATE_OP(OP_MUL,  left, right)
#define DIV( left, right ) CREATE_OP(OP_DIV,  left, right)
#define DEG( left, right ) CREATE_OP(OP_DEG,  left, right)
#define SQRT(left, right ) CREATE_OP(OP_SQRT, left, right)
#define SIN( left, right ) CREATE_OP(OP_SIN,  left, right)
#define COS( left, right ) CREATE_OP(OP_COS,  left, right)
#define LN(  left, right ) CREATE_OP(OP_LN,   left, right)
#define EQ(  left, right ) CREATE_OP(OP_EQ,   left, right)

void SyntaxErrorPrint(size_t token_cursor, Token* token)
{
    fprintf(stderr, KYEL "Syntax error on the %ld token: !\n" KNRM, token_cursor);
    exit(1);
}

Node* GetProgramm(ProgrammTokens* programm_tokens)
{
    Node* node = GetExpression(programm_tokens);

    if (!(VAL_TYPE == TOKEN_END_TYPE))
        SyntaxErrorPrint(programm_tokens->cursor, CUR_TOKEN);

    programm_tokens->cursor = 0;

    // ProgrammTokensDump(programm_tokens);

    return node;
}

Node* GetExpression(ProgrammTokens* programm_tokens)
{
    Node* node = GetMultiplication(programm_tokens);

    while (VAL_TYPE == TOKEN_OP_TYPE &&
          (VAL_OP == OP_ADD || VAL_OP == OP_SUB))
    {
        Operators op_code = VAL_OP;

        programm_tokens->cursor++;

        // ProgrammTokensDump(programm_tokens);

        Node* left_node  = node;
        Node* right_node = GetMultiplication(programm_tokens);

        if (op_code == OP_ADD)
            ADD(left_node, right_node)

        else
            SUB(left_node, right_node)
    }

    return node;
}

Node* GetMultiplication(ProgrammTokens* programm_tokens)
{
    Node* node = GetDegree(programm_tokens);

    while (VAL_TYPE == TOKEN_OP_TYPE &&
          (VAL_OP == OP_MUL || VAL_OP == OP_DIV))
    {
        Operators op_code = VAL_OP;

        programm_tokens->cursor++;

        // ProgrammTokensDump(programm_tokens);

        Node* left_node  = node;
        Node* right_node = GetDegree(programm_tokens);

        if (op_code == OP_MUL)
            MUL(left_node, right_node)

        else
            DIV(left_node, right_node)
    }

    return node;
}

Node* GetDegree(ProgrammTokens* programm_tokens)
{
    Node* node = GetPrimaryExpression(programm_tokens);

    // fprintf(stderr, );
    // ProgrammTokensDump(programm_tokens);

    while (VAL_TYPE == TOKEN_OP_TYPE &&
           VAL_OP == OP_DEG            )
    {
        programm_tokens->cursor++;

        // ProgrammTokensDump(programm_tokens);

        Node* left_node  = node;
        Node* right_node = GetPrimaryExpression(programm_tokens);

        DEG(left_node, right_node)
    }

    return node;
}

Node* GetPrimaryExpression(ProgrammTokens* programm_tokens)
{
    Node* node = nullptr;

    if (VAL_TYPE == TOKEN_SEP_TYPE &&
        VAL_SEP  == SEP_L_ROUND_BRACKET)
    {
        // HERE(1)

        programm_tokens->cursor++;

        node = GetExpression(programm_tokens);

        // TreeInorderPrint(node, stderr);

        // fprintf(stderr, "\n%d\n", VAL_SEP);

        if (VAL_TYPE != TOKEN_SEP_TYPE || VAL_SEP != SEP_R_ROUND_BRACKET)
            SyntaxErrorPrint(programm_tokens->cursor, CUR_TOKEN);

        // ProgrammTokensDump(programm_tokens);

        programm_tokens->cursor++;
    }

    else if (VAL_TYPE == TOKEN_NUM_TYPE)
        node = GetNumber(programm_tokens);

    // else
    //     node = GetW(programm_tokens);

    // ShowTree(node, SIMPLE_DUMP_MODE, 1);

    return node;
}

// Node* GetW(ProgrammTokens* programm_tokens)
// {
//     Node* node = nullptr;
//     char* name = GetV(expression_str);
//
//     if (**expression_str == '(')
//     {
//         (*expression_str)++;
//
//         Node* arg_node = GetE(expression_str, params);
//
//         // fprintf(stdout, "function name: %s\n", name);
//
//              if (!strcasecmp(name, "exp"   )) node = EXP   (arg_node);
//         else if (!strcasecmp(name, "sqrt"  )) node = SQRT  (arg_node);
//         else if (!strcasecmp(name, "ln"    )) node = LN    (arg_node);
//         else if (!strcasecmp(name, "sin"   )) node = SIN   (arg_node);
//         else if (!strcasecmp(name, "cos"   )) node = COS   (arg_node);
//         else if (!strcasecmp(name, "tg"    )) node = TG    (arg_node);
//         else if (!strcasecmp(name, "ctg"   )) node = CTG   (arg_node);
//         else if (!strcasecmp(name, "ch"    )) node = CH    (arg_node);
//         else if (!strcasecmp(name, "sh"    )) node = SH    (arg_node);
//         else if (!strcasecmp(name, "arcsin")) node = ARCSIN(arg_node);
//         else if (!strcasecmp(name, "arccos")) node = ARCCOS(arg_node);
//         else if (!strcasecmp(name, "arctg" )) node = ARCTG (arg_node);
//         else if (!strcasecmp(name, "arcctg")) node = ARCCTG(arg_node);
//
//         free((void*) name);
//
//         ASSERT(**expression_str == ')')
//
//         (*expression_str)++;
//     }
//
//     else
//     {
//         if (FindVarIndex(name, params->vars, params->n_vars) == NO_VAR_NAME)
//         {
//             char*  new_var_name = (char*) calloc(MAX_VAR_NAME_LEN, sizeof(char));
//             ASSERT(new_var_name != nullptr);
//
//             memcpy(new_var_name, name, MAX_VAR_NAME_LEN);
//             AddVar(new_var_name, DEFAULT_UNDEFINED_VAR_VALUE, params);
//         }
//
//         node = CREATE_VAR(name);
//     }
//
//     return node;
// }
//
// char* GetVariable(const char** expression_str)
// {
//     char* name = (char*) calloc(MAX_VAR_NAME_LEN, sizeof(char));
//
//     int i = 0;
//
//     while (('A' <= **expression_str && **expression_str <= 'Z') ||
//            ('a' <= **expression_str && **expression_str <= 'z')   )
//     {
//         name[i++] = **expression_str;
//
//         (*expression_str)++;
//     }
//
//     return name;
// }

Node* GetWhile(ProgrammTokens* programm_tokens)
{
    Node* node = nullptr;

    return node;
}

Node* GetCondition(ProgrammTokens* programm_tokens)
{
    Node* node = nullptr;

    return node;
}

Node* GetNumber(ProgrammTokens* programm_tokens)
{
    Node* node = nullptr;

    // ProgrammTokensDump(programm_tokens);

    if (VAL_TYPE == TOKEN_NUM_TYPE)
    {
        Value value = {};
        value.num_val = VAL_NUM;

        node = CreateNode(NODE_NUM_TYPE, value, nullptr, nullptr);
    }

    // else if (CUR_TOKEN->val_type == NODE_VAR_TYPE)
    //     node = CREATE_VAR(VAL_VAR.name);

    else SyntaxErrorPrint(programm_tokens->cursor, CUR_TOKEN);

    programm_tokens->cursor++;

    return node;;
}

#undef CUR_TOKEN
#undef NUM_VAL
#undef OP_VAL
#undef KEY_VAL
#undef SEP_VAL
#undef VAR_VAL

#undef CREATE_OP

#undef ADD
#undef SUB
#undef MUL
#undef DIV
#undef DEG
#undef SQRT
#undef SIN
#undef COS
#undef LN
#undef EQ
