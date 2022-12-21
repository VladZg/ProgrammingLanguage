#include "../Config.h"
#include <stdlib.h>
#include "../Constants/DefineColourConsts.h"
#include "../Assert.h"
#include "../Constants/Constants.h"
#include "../Tree/Tree.h"
#include "../Tree/TreeDump.h"
#include "./LexicalAnalyzator.h"
#include "./SyntaxAnalyzator.h"
#include "./VarTable.h"

#include "./DefineSyntaxAnalyzatorDSL.h"

void ErrorPrint(const char* function_name, const char* error_name, size_t token_cursor, Token* token)
{
    fprintf(stderr, KYEL "%s in the %ld token in \"%s\"!\n" KNRM, error_name, token_cursor, function_name);
}

#define FATAL_ERROR \
    abort();

#define SYNTAX_ERROR { ErrorPrint(__PRETTY_FUNCTION__, "Syntax error", programm_tokens->cursor, CUR_TOKEN); exit(1); }
#define UNKNOWN_NAME_ERROR { fprintf(stderr, KYEL "Unknown name \"%s\"", name_node->value->var->name); ErrorPrint(__PRETTY_FUNCTION__, "", programm_tokens->cursor, CUR_TOKEN); exit(1); }

#define UNINITIALIZED_ERROR                                                                         \
{                                                                                                   \
    fprintf(stderr, KYEL "%s IS UNINITIALIZED!!! DED NE ODOBRYAET... MINUS 10 SOCIAL CREDITS...\n"  \
                    KNRM, initializing_name->value->var->name);                                     \
    ErrorPrint(__PRETTY_FUNCTION__, "", programm_tokens->cursor, CUR_TOKEN);                        \
    FATAL_ERROR                                                                                     \
}


Node* GetProgramm(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* programm_tree = GetProgrammBody(programm_tokens, var_table);

    if (!(VAL_TYPE == TOKEN_END_TYPE))
        SYNTAX_ERROR

    programm_tokens->cursor = 0;

    // ProgrammTokensDump(programm_tokens, var_table);

    return programm_tree;
}

Node* GetStatement(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    return node;
}

Node* GetProgrammBody(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)


    Node* programm_body = nullptr;

    if (VAL_TYPE != TOKEN_END_TYPE)

        programm_body = GetInstructions(programm_tokens, var_table);

    return programm_body;
}

Node* GetAddSub(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = GetMultiplication(programm_tokens, var_table);

    while (VAL_TYPE == TOKEN_OP_TYPE &&
          (VAL_OP == OP_ADD || VAL_OP == OP_SUB))
    {
        Operators op_code = VAL_OP;

        TOKEN_NEXT

        // ProgrammTokensDump(programm_tokens, var_table);

        Node* left_node  = node;
        Node* right_node = GetMultiplication(programm_tokens, var_table);

        if (op_code == OP_ADD)
            ADD(node, left_node, right_node)

        else
            SUB(node, left_node, right_node)
    }

    return node;
}

Node* GetExpression(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* expression = GetAddSub(programm_tokens, var_table);

    if (VAL_TYPE == TOKEN_OP_TYPE)
    {
        Value* value = ValueCtor();

             if (VAL_OP == OP_IS_EE)
            value->op_val = OP_IS_EE;

        else if (VAL_OP == OP_IS_GE)
            value->op_val = OP_IS_GE;

        else if (VAL_OP == OP_IS_BE)
            value->op_val = OP_IS_BE;

        else if (VAL_OP == OP_IS_GT)
            value->op_val = OP_IS_GT;

        else if (VAL_OP == OP_IS_BT)
            value->op_val = OP_IS_BT;

        else if (VAL_OP == OP_IS_NE)
            value->op_val = OP_IS_NE;

        else if (VAL_OP == OP_OR)
            value->op_val = OP_OR;

        else if (VAL_OP == OP_AND)
            value->op_val = OP_AND;

        else SYNTAX_ERROR

        TOKEN_NEXT

        Node* left_expression = expression;
        Node* right_expression = GetAddSub(programm_tokens, var_table);

        expression = CreateNode(NODE_OP_TYPE, value, left_expression, right_expression);
    }

    return expression;
}

Node* GetMultiplication(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = GetDegree(programm_tokens, var_table);

    while (VAL_TYPE == TOKEN_OP_TYPE &&
          (VAL_OP == OP_MUL || VAL_OP == OP_DIV))
    {
        Operators op_code = VAL_OP;

        TOKEN_NEXT

        // ProgrammTokensDump(programm_tokens, var_table);

        Node* left_node  = node;
        Node* right_node = GetDegree(programm_tokens, var_table);

        if (op_code == OP_MUL)
            MUL(node, left_node, right_node)

        else
            DIV(node, left_node, right_node)
    }

    return node;
}

Node* GetDegree(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = GetPrimaryExpression(programm_tokens, var_table);

    // fprintf(stderr, );
    // ProgrammTokensDump(programm_tokens, var_table);

    while (VAL_TYPE == TOKEN_OP_TYPE &&
           VAL_OP == OP_DEG            )
    {
        TOKEN_NEXT

        // ProgrammTokensDump(programm_tokens, var_table);

        Node* left_node  = node;
        Node* right_node = GetPrimaryExpression(programm_tokens, var_table);

        DEG(node, left_node, right_node)
    }

    return node;
}

Node* GetPrimaryExpression(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* primary_expression = nullptr;

    if (VAL_TYPE == TOKEN_SEP_TYPE      &&
        VAL_SEP  == SEP_L_ROUND_BRACKET   )
    {
        TOKEN_NEXT

        primary_expression = GetExpression(programm_tokens, var_table);

        if (VAL_TYPE != TOKEN_SEP_TYPE     ||
            VAL_SEP != SEP_R_ROUND_BRACKET   )

            SYNTAX_ERROR

        TOKEN_NEXT
    }

    else if (VAL_TYPE == TOKEN_NUM_TYPE)
    {
        primary_expression = GetNumber(programm_tokens, var_table);
    }

    else if (VAL_TYPE == TOKEN_NAME_TYPE)
    {
        if (NEXT_TOKEN->val_type == TOKEN_SEP_TYPE          &&
            NEXT_TOKEN->value.sep_val == SEP_L_ROUND_BRACKET  )
        {
            primary_expression = GetFunctionCall(programm_tokens, var_table);
        }

        else
        {
            primary_expression = GetName(programm_tokens, var_table);
        }
    }

    else if (VAL_TYPE == TOKEN_OP_TYPE &&
            (VAL_OP == OP_SIN          ||
             VAL_OP == OP_COS          ||
             VAL_OP == OP_SQRT         ||
             VAL_OP == OP_TAN            ))
    {
        Value* function_value = ValueCtor();
        function_value->op_val = VAL_OP;

        TOKEN_NEXT

        CheckForLBracket

        Node* function_param = GetFunctionCallParam(programm_tokens, var_table);

        CheckForRBracket

        primary_expression = CreateNode(NODE_OP_TYPE, function_value, function_param, nullptr);
    }

    else
        SYNTAX_ERROR

    // ShowTree(node, SIMPLE_DUMP_MODE, 1);

    return primary_expression;
}

Node* GetInstructions(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* instruction = GetInstruction(programm_tokens, var_table);
    Node* next_instructions = nullptr;

    // ShowTree(instruction, SIMPLE_DUMP_MODE, 1);

    if (VAL_TYPE == TOKEN_NAME_TYPE ||
        VAL_TYPE == TOKEN_KEY_TYPE  ||
        VAL_TYPE == TOKEN_OP_TYPE     )
    {
        next_instructions = GetInstructions(programm_tokens, var_table);
    }

    return CreateNode(NODE_ST_TYPE, nullptr, instruction, next_instructions);
}

// void CheckForSemiColon(ProgrammTokens* programm_tokens, VarTable* var_table)
// {
//     ASSERT(programm_tokens != nullptr)
//
//     if (VAL_TYPE == TOKEN_SEP_TYPE &&
//         VAL_SEP == SEP_SEMI_COLON    )
//     {
//         TOKEN_NEXT
//     }
//
//     else SYNTAX_ERROR
// }

Node* GetInstruction(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* instruction = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE)
    {
        if (VAL_KEY == KEY_INIT)
        {
            instruction = GetInitialization(programm_tokens, var_table);
        }

        else if (VAL_KEY == KEY_IF)
        {
            instruction = GetIfElseInstruction(programm_tokens, var_table);
        }

        else if (VAL_KEY == KEY_WHILE)
        {
            instruction = GetWhileInstruction(programm_tokens, var_table);
        }

        else if (VAL_KEY == KEY_SCANF  ||
                 VAL_KEY == KEY_PRINTF   )
        {
            instruction = GetIOInstruction(programm_tokens, var_table);
        }

        else if (VAL_KEY == KEY_RETURN)
        {
            instruction = GetReturnInstruction(programm_tokens, var_table);
        }

        else if (VAL_KEY == KEY_EXIT)
        {
            TOKEN_NEXT

            CheckForSemiColon

            Value* value = ValueCtor();
            value->key_val = KEY_EXIT;
            instruction = CreateNode(NODE_KEY_TYPE, value, nullptr, nullptr);
        }

        else SYNTAX_ERROR
    }

    else if (VAL_TYPE == TOKEN_NAME_TYPE)
    {
        if (NEXT_TOKEN->val_type     == TOKEN_OP_TYPE &&
            NEXT_TOKEN->value.op_val == OP_EQ           )
        {
            instruction = GetAssignment(programm_tokens, var_table);
        }

        else if (NEXT_TOKEN->val_type == TOKEN_SEP_TYPE          &&
                 NEXT_TOKEN->value.sep_val == SEP_L_ROUND_BRACKET  )
        {
            instruction = GetExpression(programm_tokens, var_table);

            CheckForSemiColon
        }
    }

    else
    {
        instruction = GetExpression(programm_tokens, var_table);

        if (instruction)
        {
            CheckForSemiColon
        }
    }

    return instruction;
}

Node* GetKeyInstruction(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* key_instruction = nullptr;

    return key_instruction;
}

Node* GetInitialization(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* initialization = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE &&
        VAL_KEY == KEY_INIT          )
    {
        TOKEN_NEXT

        Node* initializing_name = nullptr;

        if (VAL_TYPE == TOKEN_NAME_TYPE)

           initializing_name = GetName(programm_tokens, var_table);

        else SYNTAX_ERROR

        if (VAL_TYPE == TOKEN_OP_TYPE &&
            VAL_OP   == OP_EQ           )
        {
            TOKEN_NEXT

            Node* var_value = GetExpression(programm_tokens, var_table);

            initialization = CreateNode(NODE_VAR_TYPE, nullptr, initializing_name, var_value);

            CheckForSemiColon
        }

        else if (VAL_TYPE == TOKEN_SEP_TYPE      &&
                 VAL_SEP  == SEP_L_ROUND_BRACKET   )
        {
            TOKEN_NEXT

            Node* function_init_params = GetFunctionInitParams(programm_tokens, var_table);

            Value* function_name_value = ValueCtor();
            function_name_value->var = VarCtor(initializing_name->value->var->name, NOT_NUM);

            NodeDtor(&initializing_name);

            TOKEN_NEXT

            Node* function_body = GetFunctionBlock(programm_tokens, var_table);

            Node* return_type = nullptr;

            Value* value = ValueCtor();

            value->key_val = KEY_VOID;

            if (IsReturnStatementInFunction(function_body))
            {
                value->key_val = KEY_NE_VOID;
            }

            return_type = CreateNode(NODE_KEY_TYPE, value, nullptr, nullptr);

            Node* function_name_node = CreateNode(NODE_NAME_TYPE, function_name_value, function_init_params, return_type);

            initialization = CreateNode(NODE_FUNC_TYPE, nullptr, function_name_node, function_body);

            // ShowTree(initialization, SIMPLE_DUMP_MODE, 1);
        }

        else UNINITIALIZED_ERROR
    }

    else SYNTAX_ERROR

    return initialization;
}


Node* GetFunctionInitParam(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* function_init_param = nullptr;
    Node* param_name = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE &&
        VAL_KEY == KEY_INIT          )
    {
        TOKEN_NEXT

        if (VAL_TYPE == TOKEN_NAME_TYPE)

            param_name = GetName(programm_tokens, var_table);

        else SYNTAX_ERROR

        Node* param_value = nullptr;

        if (VAL_TYPE == TOKEN_OP_TYPE &&
            VAL_OP   == OP_EQ           )
        {
            TOKEN_NEXT

            if (VAL_TYPE != TOKEN_SEP_TYPE ||
                VAL_SEP  != SEP_COMMA        )
            {
                param_value = GetExpression(programm_tokens, var_table);
            }

            else SYNTAX_ERROR
        }

        else if (VAL_TYPE != TOKEN_SEP_TYPE     ||
                 VAL_SEP != SEP_R_ROUND_BRACKET &&
                 VAL_SEP != SEP_COMMA             )
        {
            SYNTAX_ERROR
        }

        function_init_param = CreateNode(NODE_VAR_TYPE, nullptr, param_name, param_value);
    }

    return function_init_param;
}

Node* GetFunctionInitParams(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* function_init_params = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE &&
        VAL_KEY  == KEY_INIT         )
    {
        Node* function_init_param = GetFunctionInitParam(programm_tokens, var_table);
        Node* function_init_next_params = nullptr;

        if (VAL_TYPE == TOKEN_SEP_TYPE &&
            VAL_SEP  == SEP_COMMA        )
        {
            TOKEN_NEXT

            function_init_next_params = GetFunctionInitParams(programm_tokens, var_table);
        }

        else if (VAL_TYPE != TOKEN_SEP_TYPE     ||
                 VAL_SEP != SEP_R_ROUND_BRACKET   )
        {
            SYNTAX_ERROR
        }

        function_init_params = CreateNode(NODE_PARAM_TYPE, nullptr, function_init_param, function_init_next_params);
    }

    return function_init_params;
}

#define NEXT_NEXT_TOKEN programm_tokens->tokens[programm_tokens->cursor + 2]

Node* GetFunctionCallParam(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* param = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE                 &&
        VAL_KEY  == KEY_INIT                       &&
        NEXT_TOKEN->val_type == TOKEN_NAME_TYPE      )
    {
        if (NEXT_NEXT_TOKEN->val_type == TOKEN_OP_TYPE &&
            NEXT_NEXT_TOKEN->value.op_val == OP_EQ        )
        {
            param = GetFunctionInitParam(programm_tokens, var_table);
        }

        else
        {
            fprintf(stderr, KYEL "UNINITIALIZED VARIABLE!!! DED NE ODOBRYAET... MINUS 10 SOCIAL CREDITS...\n" KNRM);
            ErrorPrint(__PRETTY_FUNCTION__, "", programm_tokens->cursor, CUR_TOKEN);
            abort();
        }
    }

    else
    {
        param = GetExpression(programm_tokens, var_table);
    }

    if (VAL_TYPE != TOKEN_SEP_TYPE     ||
        VAL_SEP != SEP_R_ROUND_BRACKET &&
        VAL_SEP != SEP_COMMA             )
    {
        SYNTAX_ERROR
    }

    return param;
}

Node* GetFunctionCallParams(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* function_call_params = nullptr;

    if (VAL_TYPE != TOKEN_SEP_TYPE     &&
        VAL_SEP  != SEP_R_ROUND_BRACKET  )
    {
        Node* function_call_param = GetFunctionCallParam(programm_tokens, var_table);
        Node* function_call_next_params = nullptr;

        if (VAL_TYPE == TOKEN_SEP_TYPE &&
            VAL_SEP  == SEP_COMMA        )
        {
            TOKEN_NEXT

            function_call_next_params = GetFunctionCallParams(programm_tokens, var_table);
        }

        else if (VAL_TYPE != TOKEN_SEP_TYPE     ||
                VAL_SEP != SEP_R_ROUND_BRACKET   )
        {
            SYNTAX_ERROR
        }

        function_call_params = CreateNode(NODE_PARAM_TYPE, nullptr, function_call_param, function_call_next_params);
    }

    return function_call_params;
}

Node* GetAssignment(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* assignment = nullptr;

    if (VAL_TYPE == TOKEN_NAME_TYPE)
    {
        Node* var_name_node = GetName(programm_tokens, var_table);

        if (VAL_TYPE == TOKEN_OP_TYPE &&
            VAL_OP   == OP_EQ           )
        {
            TOKEN_NEXT
        }

        else SYNTAX_ERROR

        Node* var_value_node = GetExpression(programm_tokens, var_table);

        EQ(assignment, var_name_node, var_value_node)
    }

    CheckForSemiColon

    return assignment;
}

Node* GetFunctionDefinition(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    return node;
}

Node* GetFunctionCall(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* function_name = GetName(programm_tokens, var_table);
    Node* function_call = nullptr;

    if (VAL_TYPE == TOKEN_SEP_TYPE &&
        VAL_SEP == SEP_L_ROUND_BRACKET)
    {
        TOKEN_NEXT

        Node* function_call_params = GetFunctionCallParams(programm_tokens, var_table);

        Value* value = ValueCtor();
        value->var   = VarCtor(function_name->value->var->name, NOT_NUM);

        NodeDtor(&function_name);

        Node* function_name_node = CreateNode(NODE_NAME_TYPE, value, function_call_params, nullptr);

        function_call = CreateNode(NODE_CALL_TYPE, nullptr, function_name_node, nullptr);

        if (VAL_TYPE == TOKEN_SEP_TYPE    &&
            VAL_SEP == SEP_R_ROUND_BRACKET  )
        {
            TOKEN_NEXT
        }

        else SYNTAX_ERROR
    }

    return function_call;
}

Node* GetReturnInstruction(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* return_instruction = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE &&
        VAL_KEY == KEY_RETURN        )
    {
        TOKEN_NEXT
    }

    else SYNTAX_ERROR

    Node* returning_expression = GetExpression(programm_tokens, var_table);

    Value* value = ValueCtor();
    value->key_val = KEY_RETURN;

    return_instruction = CreateNode(NODE_KEY_TYPE, value, returning_expression, nullptr);

    CheckForSemiColon

    return return_instruction;
}

Node* GetName(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* name_node = nullptr;

    if (VAL_TYPE == TOKEN_NAME_TYPE)
    {
        CREATE_NAME(name_node, VAL_VAR->name)

        if (programm_tokens->cursor > 0            &&
            PREV_TOKEN->val_type == TOKEN_KEY_TYPE &&
            PREV_TOKEN->value.key_val == KEY_INIT    )
        {
            AddNewVar(var_table, name_node->value->var->name, name_node->value->var->value);
        }

        else if (FindVarIndexByName(name_node->value->var->name, var_table) == NOT_VAR_INDEX)
        {
            UNKNOWN_NAME_ERROR

            VarTableDump(var_table);
        }

        name_node->value->var->value = (double) FindVarIndexByName(name_node->value->var->name, var_table);

        TOKEN_NEXT

        // ProgrammTokensDump(programm_tokens, var_table);
    }

    return name_node;
}

Node* GetWhileInstruction(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* condition = nullptr;
    Node* instruction_body = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE &&
        VAL_KEY == KEY_WHILE         )
    {
        TOKEN_NEXT

        if (VAL_TYPE == TOKEN_SEP_TYPE     &&
            VAL_SEP == SEP_L_ROUND_BRACKET   )
        {
            TOKEN_NEXT

            condition = GetExpression(programm_tokens, var_table);
            // ShowTree(condition, SIMPLE_DUMP_MODE, 1);
        }

        else SYNTAX_ERROR

        if (VAL_TYPE == TOKEN_SEP_TYPE     &&
            VAL_SEP == SEP_R_ROUND_BRACKET   )
        {
            TOKEN_NEXT
        }

        else SYNTAX_ERROR

        if (VAL_TYPE == TOKEN_SEP_TYPE)
        {
            if (VAL_SEP == SEP_L_FIGURE_BRACKET)
            {
                instruction_body = GetBlock(programm_tokens, var_table);
            }

            else if (VAL_SEP == SEP_SEMI_COLON)
            {
                instruction_body = nullptr;
            }
        }

        else
        {
            instruction_body = GetInstruction(programm_tokens, var_table);
        }
    }

    else SYNTAX_ERROR

    Value* while_value = ValueCtor();
    while_value->key_val = KEY_WHILE;

    Node* instruction = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE &&
        VAL_KEY == KEY_ELSE           )
    {
        TOKEN_NEXT

        Node* else_instruction_body = GetIfElseInstructionBody(programm_tokens, var_table);

        Value* else_block_value = ValueCtor();
        else_block_value->key_val = KEY_ELSE;

        Node* else_block = CreateNode(NODE_KEY_TYPE, else_block_value, instruction_body, else_instruction_body);

        instruction = CreateNode(NODE_KEY_TYPE, while_value, condition, else_block);
    }

    else
    {
        instruction = CreateNode(NODE_KEY_TYPE, while_value, condition, instruction_body);
    }

    return instruction;
}

Node* GetIfElseInstructionBody(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* instruction_body = nullptr;

    if (VAL_TYPE == TOKEN_SEP_TYPE)
    {
        if (VAL_SEP == SEP_L_FIGURE_BRACKET)
        {
            instruction_body = GetBlock(programm_tokens, var_table);
        }

        else if (VAL_SEP == SEP_SEMI_COLON)
        {
            instruction_body = nullptr;

            TOKEN_NEXT
        }

        else SYNTAX_ERROR

    }

    else
    {
        instruction_body = GetInstruction(programm_tokens, var_table);
    }

    return instruction_body;
}

Node* GetIfElseInstruction(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* condition = nullptr;
    Node* if_instruction_body = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE &&
        VAL_KEY == KEY_IF            )
    {
        TOKEN_NEXT

        if (VAL_TYPE == TOKEN_SEP_TYPE     &&
            VAL_SEP == SEP_L_ROUND_BRACKET   )
        {
            TOKEN_NEXT

            condition = GetExpression(programm_tokens, var_table);
            // ShowTree(condition, SIMPLE_DUMP_MODE, 1);
        }

        else SYNTAX_ERROR

        if (VAL_TYPE == TOKEN_SEP_TYPE     &&
            VAL_SEP == SEP_R_ROUND_BRACKET   )
        {
            TOKEN_NEXT
        }

        else SYNTAX_ERROR

        if_instruction_body = GetIfElseInstructionBody(programm_tokens, var_table);
    }

    else SYNTAX_ERROR

    Node* instruction = nullptr;

    Value* value = ValueCtor();
    value->key_val = KEY_IF;

    if (VAL_TYPE == TOKEN_KEY_TYPE &&
        VAL_KEY == KEY_ELSE           )
    {
        TOKEN_NEXT

        Node* else_instruction_body = GetIfElseInstructionBody(programm_tokens, var_table);

        Value* else_block_value = ValueCtor();
        else_block_value->key_val = KEY_ELSE;

        Node* else_block = CreateNode(NODE_KEY_TYPE, else_block_value, if_instruction_body, else_instruction_body);

        instruction = CreateNode(NODE_KEY_TYPE, value, condition, else_block);
    }

    else
    {
        instruction = CreateNode(NODE_KEY_TYPE, value, condition, if_instruction_body);
    }

    return instruction;
}

Node* GetIOInstruction(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* instruction = nullptr;

    if (VAL_TYPE == TOKEN_KEY_TYPE)
    {
        Value* value = ValueCtor();

        if (VAL_KEY == KEY_PRINTF)

            value->key_val = KEY_PRINTF;

        else if (VAL_KEY == KEY_SCANF)

            value->key_val = KEY_SCANF;

        else SYNTAX_ERROR

        TOKEN_NEXT

        if (VAL_TYPE != TOKEN_SEP_TYPE    ||
            VAL_SEP != SEP_L_ROUND_BRACKET  )
        {
            SYNTAX_ERROR
        }

        TOKEN_NEXT

        Node* IO_params = GetFunctionCallParams(programm_tokens, var_table);

        if (VAL_TYPE != TOKEN_SEP_TYPE    ||
            VAL_SEP != SEP_R_ROUND_BRACKET  )
        {
            SYNTAX_ERROR
        }

        TOKEN_NEXT

        CheckForSemiColon

        instruction = CreateNode(NODE_KEY_TYPE, value, IO_params, nullptr);
    }

    else SYNTAX_ERROR

    return instruction;
}

Node* GetFunctionBlock(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* function_block = nullptr;

    if (VAL_TYPE == TOKEN_SEP_TYPE       &&
       (VAL_SEP  == SEP_L_ROUND_BRACKET  ||
        VAL_SEP  == SEP_L_FIGURE_BRACKET ||
        VAL_SEP  == SEP_L_SQUARE_BRACKET   ))
    {
        TOKEN_NEXT

        Node* function_block = GetInstructions(programm_tokens, var_table);

        // ShowTree(function_block, SIMPLE_DUMP_MODE, 1);

        if (VAL_TYPE == TOKEN_SEP_TYPE       &&
           (VAL_SEP  == SEP_R_ROUND_BRACKET  ||
            VAL_SEP  == SEP_R_FIGURE_BRACKET ||
            VAL_SEP  == SEP_R_SQUARE_BRACKET   ))
        {
            TOKEN_NEXT
        }

        else SYNTAX_ERROR

        return function_block;

        // function_block = CreateNode(NODE_ST_TYPE, nullptr, function_block, nullptr);
    }

    return function_block;
}

Node* GetBlock(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* block = nullptr;

    if (VAL_TYPE == TOKEN_SEP_TYPE       &&
       (VAL_SEP  == SEP_L_ROUND_BRACKET  ||
        VAL_SEP  == SEP_L_FIGURE_BRACKET ||
        VAL_SEP  == SEP_L_SQUARE_BRACKET   ))
    {
        TOKEN_NEXT

        block = GetInstructions(programm_tokens, var_table);

        if (VAL_TYPE == TOKEN_SEP_TYPE       &&
           (VAL_SEP  == SEP_R_ROUND_BRACKET  ||
            VAL_SEP  == SEP_R_FIGURE_BRACKET ||
            VAL_SEP  == SEP_R_SQUARE_BRACKET   ))
        {
            TOKEN_NEXT
        }

        else SYNTAX_ERROR
    }

    return block;
}

Node* GetNumber(ProgrammTokens* programm_tokens, VarTable* var_table)
{
    ASSERT(programm_tokens != nullptr)

    Node* node = nullptr;

    // ProgrammTokensDump(programm_tokens, var_table);

    if (VAL_TYPE == TOKEN_NUM_TYPE)
    {
        Value* value = ValueCtor();
        value->num_val = VAL_NUM;

        node = CreateNode(NODE_NUM_TYPE, value, nullptr, nullptr);
    }

    // else if (CUR_TOKEN->val_type == NODE_VAR_TYPE)
    //     node = CREATE_VAR(VAL_VAR.name);

    else SYNTAX_ERROR

    TOKEN_NEXT

    return node;;
}

int IsReturnStatementInFunction(const Node* node)
{
    if (!node) return 0;

    if (node->val_type == NODE_KEY_TYPE &&
        node->value->key_val == KEY_RETURN)
        return 1;

    if (IsReturnStatementInFunction(node->left) ||
        IsReturnStatementInFunction(node->right)  )
        return 1;

    return 0;
}

#undef SYNTAX_ERROR
#undef UNKNOWN_NAME_ERROR
#undef UNINITIALIZED_ERROR

#include "./UndefSyntaxAnalyzatorDSL"
