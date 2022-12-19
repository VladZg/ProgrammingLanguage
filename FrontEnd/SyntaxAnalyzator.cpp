#include "../Config.h"
#include <stdlib.h>
#include "../Constants/DefineColourConsts.h"
#include "../Assert.h"
#include "../Constants/Constants.h"
#include "../Tree/Tree.h"
#include "../Tree/TreeDump.h"
#include "./LexicalAnalyzator.h"
#include "./SyntaxAnalyzator.h"

#include "./DefineSyntaxAnalyzatorDSL.h"

void SyntaxErrorPrint(size_t token_cursor, Token* token)
{
    fprintf(stderr, KYEL "Syntax error on the %ld token: !\n" KNRM, token_cursor);
    exit(1);
}

Node* GetProgramm(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* programm_body = GetProgrammBody(programm_tokens);

    Node* programm_tree = CreateNode(NODE_ST_TYPE, nullptr, programm_body, nullptr);

    if (!(VAL_TYPE == TOKEN_END_TYPE))
        SyntaxErrorPrint(programm_tokens->cursor, CUR_TOKEN);

    programm_tokens->cursor = 0;

    // ProgrammTokensDump(programm_tokens);

    return programm_tree;
}

Node* GetProgrammBody(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* programm_body = GetAssignment(programm_tokens);

    // TOKEN_NEXT

    return programm_body;
}

Node* GetStatement(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    return node;
}

Node* GetBody(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = GetAssignment(programm_tokens);

    return node;
}

Node* GetExpression(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = GetMultiplication(programm_tokens);

    while (VAL_TYPE == TOKEN_OP_TYPE &&
          (VAL_OP == OP_ADD || VAL_OP == OP_SUB))
    {
        Operators op_code = VAL_OP;

        TOKEN_NEXT

        // ProgrammTokensDump(programm_tokens);

        Node* left_node  = node;
        Node* right_node = GetMultiplication(programm_tokens);

        if (op_code == OP_ADD)
            ADD(node, left_node, right_node)

        else
            SUB(node, left_node, right_node)
    }

    return node;
}

Node* GetMultiplication(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = GetDegree(programm_tokens);

    while (VAL_TYPE == TOKEN_OP_TYPE &&
          (VAL_OP == OP_MUL || VAL_OP == OP_DIV))
    {
        Operators op_code = VAL_OP;

        TOKEN_NEXT

        // ProgrammTokensDump(programm_tokens);

        Node* left_node  = node;
        Node* right_node = GetDegree(programm_tokens);

        if (op_code == OP_MUL)
            MUL(node, left_node, right_node)

        else
            DIV(node, left_node, right_node)
    }

    return node;
}

Node* GetDegree(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = GetPrimaryExpression(programm_tokens);

    // fprintf(stderr, );
    // ProgrammTokensDump(programm_tokens);

    while (VAL_TYPE == TOKEN_OP_TYPE &&
           VAL_OP == OP_DEG            )
    {
        TOKEN_NEXT

        // ProgrammTokensDump(programm_tokens);

        Node* left_node  = node;
        Node* right_node = GetPrimaryExpression(programm_tokens);

        DEG(node, left_node, right_node)
    }

    return node;
}

Node* GetPrimaryExpression(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    if (VAL_TYPE == TOKEN_SEP_TYPE &&
        VAL_SEP  == SEP_L_ROUND_BRACKET)
    {
        // HERE(1)

        TOKEN_NEXT

        node = GetExpression(programm_tokens);

        // TreeInorderPrint(node, stderr);

        // fprintf(stderr, "\n%d\n", VAL_SEP);

        if (VAL_TYPE != TOKEN_SEP_TYPE || VAL_SEP != SEP_R_ROUND_BRACKET)
            SyntaxErrorPrint(programm_tokens->cursor, CUR_TOKEN);

        // ProgrammTokensDump(programm_tokens);

        TOKEN_NEXT
    }

    else if (VAL_TYPE == TOKEN_NUM_TYPE)
        node = GetNumber(programm_tokens);

    else if (VAL_TYPE == TOKEN_NAME_TYPE)
        node = GetName(programm_tokens);

    // else
        // node = GetW(programm_tokens);

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

Node* GetAssignment(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    if (VAL_TYPE == TOKEN_NAME_TYPE)
    {
        Node* var_name_node = GetName(programm_tokens);

        if (VAL_TYPE == TOKEN_OP_TYPE &&
            VAL_OP   == OP_EQ           )
        {
            TOKEN_NEXT
        }

        else
            SyntaxErrorPrint(programm_tokens->cursor, CUR_TOKEN);

        Node* var_value_node = GetExpression(programm_tokens);

        if (VAL_TYPE == TOKEN_SEP_TYPE &&
            VAL_SEP  == SEP_SEMI_COLON   )
        {
            TOKEN_NEXT
        }

        else
            SyntaxErrorPrint(programm_tokens->cursor, CUR_TOKEN);

        EQ(node, var_name_node, var_value_node)
    }

    return node;
}

Node* GetFunctionDefinition(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    return node;
}

// Node* GetFunctionCall(ProgrammTokens* programm_tokens)
// {
//     ASSERT(programm_tokens != nullptr)
//
//     Node* node = nullptr;
//
//
//
//     return node;
// }

Node* GetName(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    if (VAL_TYPE == TOKEN_NAME_TYPE)
    {
        CREATE_NAME(node, VAL_VAR->name)

        TOKEN_NEXT

        // ProgrammTokensDump(programm_tokens);
    }

    return node;
}

Node* GetWhile(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE &&
        VAL_KEY  == KEY_WHILE        )
    {
        Value* value = ValueCtor();
        value->key_val = KEY_WHILE;

        Node* condition_node  = GetExpression(programm_tokens);
        // ASSERT(condition_node != nullptr)

        Node* while_body_node = GetBlock(programm_tokens);
        // ASSERT(while_body_node != nullptr)

        node = CreateNode(NODE_KEY_TYPE, value, condition_node, while_body_node);
    }

    return node;
}

Node* GetIfElseCondition(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    return node;
}

Node* GetBlock(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    if (VAL_TYPE == TOKEN_SEP_TYPE        &&
       (VAL_SEP  == SEP_L_ROUND_BRACKET ) ||
       (VAL_SEP  == SEP_L_FIGURE_BRACKET) ||
       (VAL_SEP  == SEP_L_SQUARE_BRACKET)   )
    {
        // node = Get


        if (VAL_TYPE == TOKEN_SEP_TYPE       &&
           (VAL_SEP  == SEP_R_ROUND_BRACKET  ||
            VAL_SEP  == SEP_R_FIGURE_BRACKET ||
            VAL_SEP  == SEP_R_SQUARE_BRACKET   ))
        {
            TOKEN_NEXT
        }

        else
            SyntaxErrorPrint(programm_tokens->cursor, CUR_TOKEN);
    }

    return node;
}

Node* GetNumber(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    // ProgrammTokensDump(programm_tokens);

    if (VAL_TYPE == TOKEN_NUM_TYPE)
    {
        Value* value = ValueCtor();
        value->num_val = VAL_NUM;

        node = CreateNode(NODE_NUM_TYPE, value, nullptr, nullptr);
    }

    // else if (CUR_TOKEN->val_type == NODE_VAR_TYPE)
    //     node = CREATE_VAR(VAL_VAR.name);

    else SyntaxErrorPrint(programm_tokens->cursor, CUR_TOKEN);

    TOKEN_NEXT

    return node;;
}

#include "./UndefSyntaxAnalyzatorDSL"
