#include "../Config.h"
#include <stdlib.h>
#include "../DefineColourConsts.h"
#include "../Stack/Assert.h"
#include "../Constants.h"
#include "../Tree.h"
#include "./LexicalAnalyzator.h"
#include "./SyntaxAnalyzator.h"

#define CUR_TOKEN programm_tokens->tokens[programm_tokens->cursor]

const char* S   = nullptr;

void SyntaxErrorPrint(size_t token_cursor, Token* token)
{
    fprintf(stderr, KYEL "Syntax error on the %ld token: !\n" KNRM, token_cursor);
    exit(1);
}

Node* GetG(ProgrammTokens* programm_tokens, const char* str)
{
//     ASSERT(str != nullptr);
//
//     S = str;
//
//     // ProgrammTokensDump(programm_tokens);
//
    Node* node = GetE(programm_tokens);
//
//     ASSERT(*S == '\0');

    if (!(CUR_TOKEN->val_type == OP_TYPE) ||
        !(CUR_TOKEN->op_val   == OP_END )   )
        SyntaxErrorPrint(programm_tokens->cursor, CUR_TOKEN);

    programm_tokens->cursor = 0;

    return node;
}

Node* GetE(ProgrammTokens* programm_tokens)
{
    // ProgrammTokensDump(programm_tokens);

//     Node* node = GetN(programm_tokens);
//
//     while (*S == '+' || *S == '-')
//     {
//         char op = *S;
//
//         S++;

//         Node* left_node  = node;
//         Node* right_node = GetN(programm_tokens);
//
//         if (op == '+')
//             node = ADD(left_node, right_node);
//
//         else
//             node = SUB(left_node, right_node);
    // }

    Node* node = GetT(programm_tokens);

    while (CUR_TOKEN->val_type == OP_TYPE &&
          (CUR_TOKEN->op_val == OP_ADD || CUR_TOKEN->op_val == OP_SUB))
    {
        int op_val = CUR_TOKEN->op_val;

        programm_tokens->cursor++;

        // ProgrammTokensDump(programm_tokens);

        Node* left_node  = node;
        Node* right_node = GetT(programm_tokens);

        if (op_val == OP_ADD)
            node = ADD(left_node, right_node);

        else
            node = SUB(left_node, right_node);
    }

    return node;
}

Node* GetT(ProgrammTokens* programm_tokens)
{
    // Node* node = GetD(programm_tokens);

//     while (*S == '*' || *S == '/')
//     {
//         char op = *S;
//
//         S++;
//
//         Node* left_node  = node;
//         Node* right_node = GetD(programm_tokens);
//
//         if (op == '*')
//             node = MUL(left_node, right_node);
//
//         else
//             node = DIV(left_node, right_node);
//     }


    Node* node = GetD(programm_tokens);

    while (CUR_TOKEN->val_type == OP_TYPE &&
          (CUR_TOKEN->op_val == OP_MUL || CUR_TOKEN->op_val == OP_DIV))
    {
        int op_val = CUR_TOKEN->op_val;

        programm_tokens->cursor++;

        // ProgrammTokensDump(programm_tokens);

        Node* left_node  = node;
        Node* right_node = GetD(programm_tokens);

        if (op_val == OP_MUL)
            node = MUL(left_node, right_node);

        else
            node = DIV(left_node, right_node);
    }

    return node;
}

Node* GetD(ProgrammTokens* programm_tokens)
{
//     Node* node = GetP(programm_tokens);
//
//     while (*S == '^')
//     {
//         char op = *S;
//
//         S++;
//
//         Node* left_node  = node;
//         Node* right_node = GetP(programm_tokens);
//
//         node = DEG(left_node, right_node);
//     }
//
//     return node;

    Node* node = GetP(programm_tokens);

    while (CUR_TOKEN->val_type == OP_TYPE && CUR_TOKEN->op_val == OP_DEG)
    {
        programm_tokens->cursor++;

        // ProgrammTokensDump(programm_tokens);

        Node* left_node  = node;
        Node* right_node = GetP(programm_tokens);

        node = DEG(left_node, right_node);
    }

    return node;
}

Node* GetP(ProgrammTokens* programm_tokens)
{
    Node* node = nullptr;

//     if (*S == '(')
//     {
//         S++;
//
//         node = GetE(programm_tokens);
//
//         ASSERT(*S == ')');
//
//         S++;
//     }
//
//     else
//         node = GetN(programm_tokens);

    if (CUR_TOKEN->val_type == OP_TYPE && CUR_TOKEN->op_val == SEP_L_ROUND_BRACKET)
    {
        programm_tokens->cursor++;

        node = GetE(programm_tokens);

        if (CUR_TOKEN->val_type != OP_TYPE || CUR_TOKEN->op_val != SEP_R_ROUND_BRACKET)
            SyntaxErrorPrint(programm_tokens->cursor, CUR_TOKEN);

        programm_tokens->cursor++;
    }

    else
        node = GetN(programm_tokens);

    return node;
}

Node* GetN(ProgrammTokens* programm_tokens)
{
//     double val = 0;
//
//     const char* s_old = S;
//
//     while ('0' <= *S && *S <= '9')
//     {
//         val = val * 10 + (*S - '0');
//         S++;
//     }
//
//     ASSERT(S > s_old);

    // fprintf(stderr, "%lf\n", val);

    Node* node = nullptr;

    // ProgrammTokensDump(programm_tokens);

    if (CUR_TOKEN->val_type == NUM_TYPE)
        node = CREATE_NUM(CUR_TOKEN->num_val);

    else if (CUR_TOKEN->val_type == VAR_TYPE)
        node = CREATE_VAR(CUR_TOKEN->var_val);

    else SyntaxErrorPrint(programm_tokens->cursor, CUR_TOKEN);

    programm_tokens->cursor++;

    return node;

    // return CREATE_NUM(val);
}

#undef CUR_TOKEN
