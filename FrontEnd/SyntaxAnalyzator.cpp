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

void SyntaxErrorPrint(const char* function_name, size_t token_cursor, Token* token)
{
    fprintf(stderr, KYEL "Syntax error in the %ld token in \"%s\"!\n" KNRM, token_cursor, function_name);
    exit(1);
}

#define ERROR SyntaxErrorPrint(__PRETTY_FUNCTION__, programm_tokens->cursor, CUR_TOKEN);

Node* GetProgramm(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* programm_tree = GetProgrammBody(programm_tokens);

    if (!(VAL_TYPE == TOKEN_END_TYPE))
        ERROR

    programm_tokens->cursor = 0;

    // ProgrammTokensDump(programm_tokens);

    return programm_tree;
}

Node* GetStatement(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    return node;
}

Node* GetProgrammBody(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* programm_body = GetInstructions(programm_tokens);

    return programm_body;
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

    Node* primary_expression = nullptr;

    if (VAL_TYPE == TOKEN_SEP_TYPE      &&
        VAL_SEP  == SEP_L_ROUND_BRACKET   )
    {
        // HERE(1)

        TOKEN_NEXT

        primary_expression = GetExpression(programm_tokens);

        if (VAL_TYPE != TOKEN_SEP_TYPE     ||
            VAL_SEP != SEP_R_ROUND_BRACKET   )

            ERROR

        TOKEN_NEXT
    }

    else if (VAL_TYPE == TOKEN_NUM_TYPE)
    {
        primary_expression = GetNumber(programm_tokens);
    }

    else if (VAL_TYPE == TOKEN_NAME_TYPE)
    {
        Node* name = GetName(programm_tokens);

        if (VAL_TYPE == TOKEN_SEP_TYPE)
        {
            if (VAL_SEP == SEP_SEMI_COLON)
            {
                primary_expression = name;
            }

            else if (VAL_SEP == SEP_L_ROUND_BRACKET)
            {
                TOKEN_NEXT

                Node* function_call_params = nullptr; // GetFunctionCallParams(programm_tokens);

                Value* value = ValueCtor();
                value->var   = VarCtor(name->value->var->name, NOT_NUM);

                NodeDtor(&name);

                Node* function_name_node = CreateNode(NODE_NAME_TYPE, value, function_call_params, nullptr);

                primary_expression = CreateNode(NODE_CALL_TYPE, nullptr, function_name_node, nullptr);

                if (VAL_SEP == SEP_R_ROUND_BRACKET)
                {
                    TOKEN_NEXT
                }

                else ERROR
            }
        }

        else
        {
            primary_expression = name;
        }
    }

    else
        ERROR

    // ShowTree(node, SIMPLE_DUMP_MODE, 1);

    return primary_expression;
}

Node* GetInstructions(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* instruction = GetInstruction(programm_tokens);
    Node* next_instructions = nullptr;

    if (VAL_TYPE == TOKEN_NAME_TYPE ||
        VAL_TYPE == TOKEN_KEY_TYPE  &&
        VAL_KEY  != KEY_RETURN        )
    {
        next_instructions = GetInstructions(programm_tokens);
    }

    return CreateNode(NODE_ST_TYPE, nullptr, instruction, next_instructions);
}

Node* GetInstruction(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* instruction = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE)
    {
        if (VAL_KEY == KEY_INIT)
        {
            instruction = GetInitialization(programm_tokens);
        }

        if (VAL_KEY == KEY_IF)
        {
            instruction = GetIfElseInstruction(programm_tokens);
        }

        if (VAL_KEY == KEY_WHILE)
        {
            instruction = GetWhileInstruction(programm_tokens);
        }
    }

    else if (VAL_TYPE == TOKEN_NAME_TYPE)
    {
        if (NEXT_TOKEN->val_type     == TOKEN_OP_TYPE &&
            NEXT_TOKEN->value.op_val == OP_EQ           )
        {
            instruction = GetAssignment(programm_tokens);
        }

        // else if (NEXT_TOKEN->val_type == TOKEN_SEP_TYPE &&
        //          NEXT_TOKEN->value.sep_val == SEP_L_ROUND_BRACKET)
        // {
        //     instruction = GetFunctionCall(programm_tokens);
        // }
    }

    else
        return instruction;

    return instruction;
}

Node* GetKeyInstruction(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* key_instruction = nullptr;

    return key_instruction;
}

Node* GetInitialization(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* initialization = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE &&
        VAL_KEY == KEY_INIT          )
    {
        TOKEN_NEXT

        Node* initializing_name = nullptr;

        if (VAL_TYPE == TOKEN_NAME_TYPE)

           initializing_name = GetName(programm_tokens);

        else ERROR

        if (VAL_TYPE == TOKEN_OP_TYPE &&
            VAL_OP   == OP_EQ           )
        {
            TOKEN_NEXT

            Node* var_value = GetExpression(programm_tokens);

            initialization = CreateNode(NODE_VAR_TYPE, nullptr, initializing_name, var_value);

            if (VAL_TYPE == TOKEN_SEP_TYPE &&
                VAL_SEP  == SEP_SEMI_COLON   )
            {
                TOKEN_NEXT
            }

            else ERROR
        }

        else if (VAL_TYPE == TOKEN_SEP_TYPE      &&
                 VAL_SEP  == SEP_L_ROUND_BRACKET   )
        {
            TOKEN_NEXT

            Node* function_init_params = GetFunctionInitParams(programm_tokens);

            Value* function_name_value = ValueCtor();
            function_name_value->var = VarCtor(initializing_name->value->var->name, NOT_NUM);

            NodeDtor(&initializing_name);

            TOKEN_NEXT

            Node* function_body = GetFunctionBlock(programm_tokens);

            Node* return_type = nullptr;

            Value* value = ValueCtor();

            if (!function_body->right)
            {
                value->key_val = KEY_VOID;

                Node* function_body_temp = function_body;

                function_body = CopyNode(function_body->left);

                NodeDtor(&function_body_temp);
            }

            else
                value->key_val = KEY_NE_VOID;

            return_type = CreateNode(NODE_KEY_TYPE, value, nullptr, nullptr);

            Node* function_name_node = CreateNode(NODE_NAME_TYPE, function_name_value, function_init_params, return_type);

            initialization = CreateNode(NODE_FUNC_TYPE, nullptr, function_name_node, function_body);
        }

        else ERROR
    }

    else ERROR

    return initialization;
}


Node* GetFunctionInitParam(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* function_init_param = nullptr;
    Node* param_name = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE &&
        VAL_KEY == KEY_INIT          )
    {
        TOKEN_NEXT

        if (VAL_TYPE == TOKEN_NAME_TYPE)

            param_name = GetName(programm_tokens);

        else ERROR

        Node* param_value = nullptr;

        if (VAL_TYPE == TOKEN_OP_TYPE &&
            VAL_OP   == OP_EQ           )
        {
            TOKEN_NEXT

            if (VAL_TYPE != TOKEN_SEP_TYPE ||
                VAL_SEP  != SEP_COMMA        )
            {
                param_value = GetExpression(programm_tokens);
            }

            else ERROR
        }

        else if (VAL_TYPE != TOKEN_SEP_TYPE     ||
                 VAL_SEP != SEP_R_ROUND_BRACKET &&
                 VAL_SEP != SEP_COMMA             )
        {
            ERROR
        }

        function_init_param = CreateNode(NODE_VAR_TYPE, nullptr, param_name, param_value);
    }

    return function_init_param;
}

Node* GetFunctionInitParams(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* function_init_params = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE &&
        VAL_KEY  == KEY_INIT         )
    {
        Node* function_init_param = GetFunctionInitParam(programm_tokens);
        Node* function_init_next_params = nullptr;

        if (VAL_TYPE == TOKEN_SEP_TYPE &&
            VAL_SEP  == SEP_COMMA        )
        {
            TOKEN_NEXT

            function_init_next_params = GetFunctionInitParams(programm_tokens);
        }

        else if (VAL_TYPE != TOKEN_SEP_TYPE     ||
                 VAL_SEP != SEP_R_ROUND_BRACKET   )
        {
            ERROR
        }

        function_init_params = CreateNode(NODE_PARAM_TYPE, nullptr, function_init_param, function_init_next_params);
    }

    return function_init_params;
}

Node* GetAssignment(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* assignment = nullptr;

    if (VAL_TYPE == TOKEN_NAME_TYPE)
    {
        Node* var_name_node = GetName(programm_tokens);

        if (VAL_TYPE == TOKEN_OP_TYPE &&
            VAL_OP   == OP_EQ           )
        {
            TOKEN_NEXT
        }

        else ERROR

        Node* var_value_node = GetExpression(programm_tokens);

        EQ(assignment, var_name_node, var_value_node)
    }

    if (VAL_TYPE == TOKEN_SEP_TYPE &&
        VAL_SEP  == SEP_SEMI_COLON   )
    {
        TOKEN_NEXT
    }

    else ERROR

    return assignment;
}

Node* GetFunctionDefinition(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    return node;
}

Node* GetFunctionCall(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    return node;
}

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

Node* GetWhileInstruction(ProgrammTokens* programm_tokens)
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

Node* GetIfElseInstruction(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* condition = nullptr;
    Node* instruction_body = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE &&
        VAL_KEY == KEY_IF            )
    {

    }

    else ERROR

    Value* value = ValueCtor();
    value->key_val = KEY_IF;

    Node* instruction = CreateNode(NODE_KEY_TYPE, value, condition, instruction_body);

    return instruction;
}

Node* GetFunctionBlock(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* function_block = nullptr;

    if (VAL_TYPE == TOKEN_SEP_TYPE       &&
       (VAL_SEP  == SEP_L_ROUND_BRACKET  ||
        VAL_SEP  == SEP_L_FIGURE_BRACKET ||
        VAL_SEP  == SEP_L_SQUARE_BRACKET   ))
    {
        TOKEN_NEXT

        Node* function_block_without_return = GetInstructions(programm_tokens);
        Node* return_statement = nullptr;

        if (VAL_TYPE == TOKEN_KEY_TYPE &&
            VAL_KEY  == KEY_RETURN       )
        {
            TOKEN_NEXT

            Node* return_expression = GetExpression(programm_tokens);

            if (VAL_TYPE == TOKEN_SEP_TYPE &&
                VAL_SEP  == SEP_SEMI_COLON   )
            {
                Value* value = ValueCtor();
                value->key_val = KEY_RETURN;

                return_statement = CreateNode(NODE_KEY_TYPE, value, return_expression, nullptr);

                TOKEN_NEXT
            }

            else ERROR
        }

        if (VAL_TYPE == TOKEN_SEP_TYPE       &&
           (VAL_SEP  == SEP_R_ROUND_BRACKET  ||
            VAL_SEP  == SEP_R_FIGURE_BRACKET ||
            VAL_SEP  == SEP_R_SQUARE_BRACKET   ))
        {
            TOKEN_NEXT
        }

        else ERROR

        function_block = CreateNode(NODE_ST_TYPE, nullptr, function_block_without_return, return_statement);
    }

    else ERROR

    return function_block;
}

Node* GetBlock(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    Node* block = nullptr;

    if (VAL_TYPE == TOKEN_SEP_TYPE       &&
       (VAL_SEP  == SEP_L_ROUND_BRACKET  ||
        VAL_SEP  == SEP_L_FIGURE_BRACKET ||
        VAL_SEP  == SEP_L_SQUARE_BRACKET   ))
    {
        TOKEN_NEXT

        block = GetInstructions(programm_tokens);

        if (VAL_TYPE == TOKEN_SEP_TYPE       &&
           (VAL_SEP  == SEP_R_ROUND_BRACKET  ||
            VAL_SEP  == SEP_R_FIGURE_BRACKET ||
            VAL_SEP  == SEP_R_SQUARE_BRACKET   ))
        {
            TOKEN_NEXT
        }

        else ERROR
    }

    return block;
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

    else ERROR

    TOKEN_NEXT

    return node;;
}

#include "./UndefSyntaxAnalyzatorDSL"
