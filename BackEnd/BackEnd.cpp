#include "../Config.h"
#include <stdlib.h>
#include <cstring>
#include <stdarg.h>
#include <math.h>
#include <sys/stat.h>
#include "../Constants/Constants.h"
#include "../Constants/DefineColourConsts.h"
#include "../Assert.h"
#include "../Tree/Tree.h"
#include "../Tree/TreeDump.h"
#include "../FrontEnd/LexicalAnalyzator.h"
#include "../FrontEnd/SyntaxAnalyzator.h"
#include "../FrontEnd/VarTable.h"
#include "./BackEnd.h"
#include "../Stack/Stack.h"

int  PrintInAsm(FILE* asm_file, const char* fmt_text, ...)
{
    va_list args;
    va_start(args, fmt_text);

    vfprintf(asm_file, fmt_text, args);

    va_end(args);

    return 1;
}

#define ASM_PRINT(...) PrintInAsm(asm_file, __VA_ARGS__);

#define ASM_COMMENT_PRINT(asm_cmd_str, ...)                                         \
{                                                                                   \
    FprintfNSymb(asm_file, ' ', (size_t) ASM_COMMENT_SHIFT - strlen(asm_cmd_str));  \
    ASM_PRINT(__VA_ARGS__)                                                          \
    ASM_PRINT("\n")                                                                 \
}

#define ASM_CMD_PRINT(asm_cmd_str, ...) \
    sprintf(asm_cmd_str, __VA_ARGS__);  \
    ASM_PRINT(asm_cmd_str)

#define VARS_REG "vars_start"

int ProcessProgramm(const Node* programm_tree)
{
    FILE* asm_file = fopen("./Processor/Source.asm", "w");
    ASSERT(asm_file != nullptr)

    TranslateProgrammTreeToAsm(programm_tree, asm_file);

    fclose(asm_file);

    system("cd ./Processor                    ;"
           "make -f Run clean                 ;"
           "make -f Run filename=./Source.asm ;"
           "cd ../                            ;");
   ;
    return 1;
}

int TranslateProgrammTreeToAsm(const Node* programm_tree, FILE* asm_file)
{
    ASSERT(asm_file != nullptr)

    TranslateProgrammBodyToAsm(programm_tree, asm_file);

    char asm_cmd_str[MAX_ASM_CMD_LENGTH] = {};
    ASM_CMD_PRINT(asm_cmd_str, "HLT")
    ASM_COMMENT_PRINT(asm_cmd_str, "\\\\ end of the programm")

    return 1;
}

size_t N_statement = 1;

int TranslateProgrammBodyToAsm(const Node* programm_tree, FILE* asm_file)
{
    ASSERT(asm_file != nullptr)

    ASM_PRINT("                               \\\\ <---- ") // %ld statement, N_statement

    if (programm_tree->left->val_type == NODE_VAR_TYPE)
    {
        ASM_PRINT("var initialization started here\n")
        TranslateVarInitializationToAsm(programm_tree->left, asm_file);
        ASM_PRINT("                               \\\\ <---- var initialization finished here\n\n")
    }

    else if (programm_tree->left->val_type == NODE_OP_TYPE)
    {
        if (programm_tree->left->value->op_val == OP_EQ)
        {
            ASM_PRINT("assignment started here\n")
            TranslateAssignmentToAsm(programm_tree->left, asm_file);
            ASM_PRINT("                               \\\\ <---- assignment finished here\n\n")
        }

        // else if ()
    }

    else if (programm_tree->left->val_type == NODE_FUNC_TYPE)
    {
        ASM_PRINT("function initialization started here\n")
        TranslateFunctionInitializationToAsm(programm_tree->left, asm_file);
        ASM_PRINT("                               \\\\ <---- function initialization finished here\n\n")
    }

    else if (programm_tree->left->val_type == NODE_CALL_TYPE)
    {
        ASM_PRINT("function call started here\n")
        TranslateFunctionCallToAsm(programm_tree->left, asm_file);
        ASM_PRINT("                               \\\\ <---- function call finished here\n\n")
    }

    else if (programm_tree->left->val_type == NODE_KEY_TYPE)
    {
        if (programm_tree->left->value->key_val == KEY_IF)
        {
            ASM_PRINT("if construction started here\n")

            Stack stack_of_conditional_calls = {};
            StackCtor(stack_of_conditional_calls);

            TranslateIfElseToAsm(programm_tree->left, asm_file, &stack_of_conditional_calls);
            ASM_PRINT("                               \\\\ <---- if construction finished here\n\n")

            StackDtor(&stack_of_conditional_calls);
        }

        else if (programm_tree->left->value->key_val == KEY_WHILE)
        {
            ASM_PRINT("while started here\n")

            Stack stack_of_while_calls = {};
            StackCtor(stack_of_while_calls);

            TranslateWhileToAsm(programm_tree->left, asm_file, &stack_of_while_calls);
            ASM_PRINT("                               \\\\ <---- while finished here\n\n")

            StackDtor(&stack_of_while_calls);
        }

        else if (programm_tree->left->value->key_val == KEY_SCANF ||
                 programm_tree->left->value->key_val == KEY_PRINTF  )
        {
            ASM_PRINT("input/output started here\n")
            TranslateIOInAsm(programm_tree->left, asm_file);
            ASM_PRINT("                               \\\\ <---- input/output finished here\n\n")
        }

        else if (programm_tree->left->value->key_val == KEY_RETURN)
        {
            ASM_PRINT("function return statement starts here\n")
            TranslateExpressionToAsm(programm_tree->left->left, asm_file);

            char asm_func_ret_cmd_str[MAX_ASM_CMD_LENGTH] = {};
            ASM_CMD_PRINT(asm_func_ret_cmd_str, "RET")
            ASM_COMMENT_PRINT(asm_func_ret_cmd_str, "\\\\<--- function return statement")
        }

        else if (programm_tree->left->value->key_val == KEY_EXIT)
        {
            ASM_PRINT("exit case here\n")
            char asm_func_exit_cmd_str[MAX_ASM_CMD_LENGTH] = {};
            ASM_CMD_PRINT(asm_func_exit_cmd_str, "EXIT")
            ASM_COMMENT_PRINT(asm_func_exit_cmd_str, "\\\\<--- programm exit statement")
        }
    }

    else ASM_PRINT("empty block\n")

    // ASM_PRINT("                               \\\\ <---- finished here\n\n") //%ld statement , N_statement++)

    if (programm_tree->right)
        TranslateProgrammBodyToAsm(programm_tree->right, asm_file);

    return 0;
}

#define DEF_OP(op_code, op_name, op_lang_name, op_tree_name, op_kryzh_name) \
    case OP_##op_name:                                                      \
            {                                                               \
                char asm_cmd_str[MAX_ASM_CMD_LENGTH] = {};                  \
                ASM_CMD_PRINT(asm_cmd_str, "%s", #op_name)                  \
                ASM_COMMENT_PRINT(asm_cmd_str, "\\\\ %sING",                \
                                                #op_name)                   \
                break;                                                      \
            }

#define IS_COMPRASION(expression) (expression->value->op_val == OP_IS_EE ||  \
                                   expression->value->op_val == OP_IS_NE ||  \
                                   expression->value->op_val == OP_IS_GE ||  \
                                   expression->value->op_val == OP_IS_BE ||  \
                                   expression->value->op_val == OP_IS_GT ||  \
                                   expression->value->op_val == OP_IS_BT ||  \
                                   expression->value->op_val == OP_OR    ||  \
                                   expression->value->op_val == OP_AND     )

int TranslateExpressionToAsm(const Node* expression, FILE* asm_file)
{
    ASSERT(asm_file != nullptr)

    if (!expression) return 0;

    if (expression->val_type == NODE_CALL_TYPE)// &&  // нужна проверка на возвращаемое значение
             //expression->left->left == nullptr        ) //считывание только функций без параметров
    {
        // ASM_PRINT("\n\nHERE\n\n")
        TranslateFunctionCallToAsm(expression, asm_file);
        // ASM_PRINT("\n\nHERE\n\n")

        return 1;
    }

    if (expression->left)
        TranslateExpressionToAsm(expression->left , asm_file);

    if (expression->right)
        TranslateExpressionToAsm(expression->right, asm_file);

    if (expression->val_type == NODE_NUM_TYPE)
    {
        // programm_tree->value->num_val += 0.5;

        double num_val = expression->value->num_val;

        char asm_cmd_str[MAX_ASM_CMD_LENGTH] = {};
        ASM_CMD_PRINT(asm_cmd_str, "PUSH %d", (int) num_val)

        if (abs((int) num_val - num_val) > 0.001)
        {
            ASM_COMMENT_PRINT(asm_cmd_str, "\\\\ rounded up to an integer (%lg -> %d)",
                                            num_val, (int) num_val)
        }

        else ASM_PRINT("\n")
    }

    else if (expression->val_type == NODE_OP_TYPE)
    {
        // if (IS_COMPRASION                               &&
        //     expression->prev->val_type == NODE_KEY_TYPE &&
        //    (expression->prev->value->key_val == KEY_IF  ||
        //     expression->prev->value->key_val == KEY_ELSE  ))
        // {
        //     TranslateComprasionToAsm(expression, asm_file);
        // }

        if (!IS_COMPRASION(expression))
        {
            switch (expression->value->op_val)
            {
                #include "../Dictionary/Operators.h"
            }
        }
    }

    else if (expression->val_type == NODE_NAME_TYPE      &&
             expression->prev->val_type != NODE_CALL_TYPE  )
    {
        char asm_cmd_str[MAX_ASM_CMD_LENGTH] = {};
        ASM_CMD_PRINT(asm_cmd_str, "PUSH [%d + %s]", (int) expression->value->var->value, VARS_REG)
        ASM_COMMENT_PRINT(asm_cmd_str, "\\\\ var %s", expression->value->var->name)
    }

    return 0;
}

#define VAL_OP expression->value->op_val
#define UNDEF_COMPARATOR {fprintf(stderr, KYEL "UNDEFINED COMPARATOR!!!\n" KNRM); exit(1);}

int TranslateComprasionToAsm(const Node* expression, FILE* asm_file)
{
    ASSERT(asm_file != nullptr)

    if (!expression || !expression->left || !expression->right) return 0;

    TranslateExpressionToAsm(expression->left , asm_file);
    TranslateExpressionToAsm(expression->right, asm_file);

    if (IS_COMPRASION(expression))
    {
            if (VAL_OP == OP_IS_EE)
        {
            ASM_PRINT("JE")
        }

        else if (VAL_OP == OP_IS_GE)
        {
            ASM_PRINT("JAE");
        }

        else if (VAL_OP == OP_IS_BE)
        {
            ASM_PRINT("JBE");
        }

        else if (VAL_OP == OP_IS_GT)
        {
            ASM_PRINT("JA");
        }

        else if (VAL_OP == OP_IS_BT)
        {
            ASM_PRINT("JB");
        }

        else if (VAL_OP == OP_IS_NE)
        {
            ASM_PRINT("JNE")
        }

        else if (VAL_OP == OP_OR)
        {
            UNDEF_COMPARATOR
        }

        else if (VAL_OP == OP_AND)
        {
            UNDEF_COMPARATOR
        }
    }

    else
    {
        ASM_PRINT("PUSH 0\n")
        ASM_PRINT("JNE");
    }

    return 1;
}

#undef UNDEF_COMPARATOR
#undef VAL_OP
#undef IS_COMPRASION
#undef DEF_OP

int TranslateVarInitializationToAsm(const Node* instruction, FILE* asm_file)
{
    ASSERT(asm_file != nullptr)

    if (!instruction) return 0;

    char asm_cmd_str[MAX_ASM_CMD_LENGTH] = {};

    if (instruction->val_type == NODE_VAR_TYPE)
    {
        ASM_PRINT("                               \\\\ <-- var %s initialization started\n", instruction->left->value->var->name)

        TranslateExpressionToAsm(instruction->right, asm_file);

        ASM_CMD_PRINT(asm_cmd_str, "POP [%d + %s]", (int) instruction->left->value->var->value, VARS_REG)
        ASM_COMMENT_PRINT(asm_cmd_str, "\\\\ <-- var %s initialized", instruction->left->value->var->name)

        return 1;
    }

    return 0;
}

int N_function_def = 0;

int TranslateFunctionInitializationToAsm(const Node* instruction, FILE* asm_file)
{
    ASSERT(asm_file != nullptr)

    if (!instruction || instruction->val_type != NODE_FUNC_TYPE) return 0;

    ASM_PRINT("JMP :skip_func_%s_def\n", instruction->left->value->var->name)

    char asm_func_init_cmd_str[MAX_ASM_CMD_LENGTH] = {};

    ASM_CMD_PRINT(asm_func_init_cmd_str, "func_%s:", instruction->left->value->var->name)
    ASM_COMMENT_PRINT(asm_func_init_cmd_str, "\\\\ <-- function %s initialization", instruction->left->value->var->name)

    if (instruction->left->left)
    {
        TranslateFunctionInitParamsToAsm(instruction->left->left, asm_file);
    }

    TranslateProgrammBodyToAsm(instruction->right, asm_file);

//     char asm_func_ret_cmd_str[MAX_ASM_CMD_LENGTH] = {};
//
//     ASM_CMD_PRINT(asm_func_ret_cmd_str, "RET")
    // ASM_COMMENT_PRINT(asm_func_ret_cmd_str, "\\\\ <-- function %s return statement", instruction->left->value->var->name)

    ASM_PRINT("\nskip_func_%s_def:\n", instruction->left->value->var->name)

    return 1;
}

int TranslateFunctionCallToAsm(const Node* call, FILE* asm_file)
{
    ASSERT(asm_file != nullptr)

    if (!call                            ||
        call->val_type != NODE_CALL_TYPE   )
        return 0;

    // ASM_PRINT("\n\nHERE\n\n")

    if (call->left->left)
    {
        TranslateFunctionCallParamsToAsm(call->left->left, asm_file);
    }

    // ASM_PRINT("\n\nHERE\n\n")

    char asm_func_call_cmd_str[MAX_ASM_CMD_LENGTH] = {};
    ASM_CMD_PRINT(asm_func_call_cmd_str, "CALL :func_%s", call->left->value->var->name)
    ASM_COMMENT_PRINT(asm_func_call_cmd_str, "\\\\ <--- function %s call", call->left->value->var->name)

    return 1;
}

int TranslateAssignmentToAsm(const Node* assignment, FILE* asm_file)
{
    ASSERT(asm_file != nullptr)

    if (!assignment                          ||
        assignment->val_type != NODE_OP_TYPE ||
        assignment->value->op_val != OP_EQ     )
        return 0;

    TranslateExpressionToAsm(assignment->right, asm_file);

    char asm_cmd_str[MAX_ASM_CMD_LENGTH] = {};

    ASM_CMD_PRINT(asm_cmd_str, "POP [%d + %s]", (int) assignment->left->value->var->value, VARS_REG)
    ASM_COMMENT_PRINT(asm_cmd_str, "\\\\ var's %s value changed", assignment->left->value->var->name)

    return 1;
}

int N_conditional_construction = 0;
int N_while_construction = 0;
// int N_else_construction = 0;

int TranslateIfElseToAsm(const Node* instruction, FILE* asm_file, Stack* stack_of_calls)
{
    ASSERT(asm_file != nullptr)

    if (!instruction) return 0;

    TranslateComprasionToAsm(instruction->left, asm_file);

    ASM_PRINT(" :if_block_%p\n", instruction)

    // StackPush(stack_of_calls, (Elem_t) ++N_conditional_construction);

    int endif_num = 0;

    if (instruction->right)
    {
        if (instruction->right->val_type != NODE_KEY_TYPE ||
            instruction->right->value->key_val != KEY_ELSE  )
        {
            ASM_PRINT("JMP :endif_%p\n\n", instruction)

            char asm_if_block_cmd_str[MAX_ASM_CMD_LENGTH] = {};
            ASM_CMD_PRINT(asm_if_block_cmd_str, "if_block_%p:", instruction)

            ASM_COMMENT_PRINT(asm_if_block_cmd_str, "\\\\ <-- the beginning ob if block")
            TranslateProgrammBodyToAsm(instruction->right, asm_file);
            ASM_PRINT("JMP :endif_%p\n\n", instruction)
        }

        else
        {
            ASM_PRINT("JMP :else_block_%p\n\n", instruction)

            // ShowTree(instruction->right->left, SIMPLE_DUMP_MODE, 1);

            char asm_if_block_cmd_str[MAX_ASM_CMD_LENGTH] = {};
            ASM_CMD_PRINT(asm_if_block_cmd_str, "if_block_%p:", instruction)
            ASM_COMMENT_PRINT(asm_if_block_cmd_str, "\\\\ <-- the beginning ob if block")
            TranslateProgrammBodyToAsm(instruction->right->left, asm_file);
            ASM_PRINT("JMP :endif_%p\n\n", instruction)

            char asm_else_block_cmd_str[MAX_ASM_CMD_LENGTH] = {};
            ASM_CMD_PRINT(asm_else_block_cmd_str, "else_block_%p:", instruction)
            ASM_COMMENT_PRINT(asm_else_block_cmd_str, "\\\\ <-- the beginning ob else block")
            TranslateProgrammBodyToAsm(instruction->right->right, asm_file);
            ASM_PRINT("JMP :endif_%p\n\n", instruction)
        }
    }

    else
    {
        char asm_if_block_cmd_str[MAX_ASM_CMD_LENGTH] = {};
        ASM_CMD_PRINT(asm_if_block_cmd_str, "if_block_%p:", instruction)
        ASM_COMMENT_PRINT(asm_if_block_cmd_str, "\\\\ <-- the beginning ob if block")
        ASM_PRINT("JMP :endif_%p\n\n", instruction)
    }

    // endif_num = (int) StackPop(stack_of_calls);

    char asm_end_of_if_block_cmd_str[MAX_ASM_CMD_LENGTH] = {};
    ASM_CMD_PRINT(asm_end_of_if_block_cmd_str,"endif_%p:", instruction)
    ASM_COMMENT_PRINT(asm_end_of_if_block_cmd_str, "\\\\ <-- continuation after conditional block")

    return 1;
}

int TranslateWhileToAsm(const Node* instruction, FILE* asm_file, Stack* stack_of_calls)
{
    ASSERT(asm_file != nullptr)

    if (!instruction) return 0;

    // StackPush(stack_of_calls, (Elem_t) ++N_while_construction);

    ASM_PRINT("while_%p:\n", instruction)

    if (!TranslateComprasionToAsm(instruction->left, asm_file)) ASM_PRINT("JMP")

    ASM_PRINT(" :while_block_%p\n", instruction)
    ASM_PRINT("JMP :while_else_%p\n\n", instruction)

    char asm_while_block_cmd_str[MAX_ASM_CMD_LENGTH] = {};
    ASM_CMD_PRINT(asm_while_block_cmd_str, "while_block_%p:", instruction)
    ASM_COMMENT_PRINT(asm_while_block_cmd_str, "\\\\ <-- the beginning ob while block")

    if (instruction->right)
    {
        if (instruction->right->val_type != NODE_KEY_TYPE ||
            instruction->right->value->key_val != KEY_ELSE  )
        {
            TranslateProgrammBodyToAsm(instruction->right, asm_file);
        }

        else
        {
            TranslateProgrammBodyToAsm(instruction->right->left, asm_file);
        }
    }

    // int while_else_num = (int) StackPop(stack_of_calls);

    ASM_PRINT("JMP :while_%p\n\n", instruction)

    char asm_while_else_block_cmd_str[MAX_ASM_CMD_LENGTH] = {};
    ASM_CMD_PRINT(asm_while_else_block_cmd_str,"while_else_%p:", instruction)
    ASM_COMMENT_PRINT(asm_while_else_block_cmd_str, "\\\\ <-- while else block")

    if (instruction->right                            &&
        instruction->right->val_type == NODE_KEY_TYPE &&
        instruction->right->value->key_val == KEY_ELSE  )
    {
        TranslateProgrammBodyToAsm(instruction->right->right, asm_file);
    }

    ASM_PRINT("JMP :end_while_%p\n\n", instruction)

    char asm_end_of_while_cmd_str[MAX_ASM_CMD_LENGTH] = {};
    ASM_CMD_PRINT(asm_end_of_while_cmd_str,"end_while_%p:", instruction)
    ASM_COMMENT_PRINT(asm_end_of_while_cmd_str, "\\\\ <-- continuation after while block")

    return 1;
}

int TranslateIOInAsm(const Node* instruction, FILE* asm_file)
{
    ASSERT(asm_file != nullptr)

    if (!instruction) return 0;

    if (instruction->value->key_val == KEY_PRINTF)

        TranslateOutputToAsm(instruction->left, asm_file);

    else if (instruction->value->key_val == KEY_SCANF)

        TranslateIntputToAsm(instruction->left, asm_file);

    return 1;
}

int TranslateIntputToAsm(const Node* param, FILE* asm_file)
{
    ASSERT(asm_file != nullptr)

    if (!param || param->val_type != NODE_PARAM_TYPE) return 1;

    Node* name_node = nullptr;

    if (param->left->val_type == NODE_VAR_TYPE)
    {
        TranslateVarInitializationToAsm(param->left, asm_file);
        name_node = param->left->left;
    }

    else if (param->left->val_type == NODE_NAME_TYPE)
    {
        name_node = param->left;
    }

    else return 0;

    ASM_PRINT("PIN\n");

    char asm_print_param_cmd_str[MAX_ASM_CMD_LENGTH] = {};
    ASM_CMD_PRINT(asm_print_param_cmd_str,"POP [%d + %s]", (int) name_node->value->var->value, VARS_REG)
    ASM_COMMENT_PRINT(asm_print_param_cmd_str, "\\\\ <-- standart scanf in var %s", name_node->value->var->name)

    TranslateIntputToAsm(param->right, asm_file);

    return 1;
}

int TranslateOutputToAsm(const Node* param, FILE* asm_file)
{
    ASSERT(asm_file != nullptr)

    if (!param || param->val_type != NODE_PARAM_TYPE) return 1;

    TranslateExpressionToAsm(param->left, asm_file);

    char asm_print_param_cmd_str[MAX_ASM_CMD_LENGTH] = {};
    ASM_CMD_PRINT(asm_print_param_cmd_str,"OUT")
    ASM_COMMENT_PRINT(asm_print_param_cmd_str, "\\\\ <-- standart printf\n")

    TranslateOutputToAsm(param->right, asm_file);

    return 1;
}

int TranslateFunctionCallParamsToAsm(const Node* param, FILE* asm_file)
{
    ASSERT(asm_file != nullptr)

    if (!param || param->val_type != NODE_PARAM_TYPE) return 1;

    TranslateExpressionToAsm(param->left, asm_file);

    if (param->right)
    {
        TranslateFunctionCallParamsToAsm(param->right, asm_file);
    }

    ASM_PRINT("                               \\\\ <--- initializing function params\n")

    return 1;
}

int TranslateFunctionInitParamsToAsm(const Node* param, FILE* asm_file)
{
    ASSERT(asm_file != nullptr)

    if (!param || param->val_type != NODE_PARAM_TYPE) return 1;

    Node* name_node = nullptr;

    if (param->left->val_type == NODE_VAR_TYPE)
    {
        name_node = param->left->left;

        char asm_print_param_cmd_str[MAX_ASM_CMD_LENGTH] = {};
        ASM_CMD_PRINT(asm_print_param_cmd_str,"POP [%d + %s]", (int) name_node->value->var->value, VARS_REG)
        ASM_COMMENT_PRINT(asm_print_param_cmd_str, "\\\\ <-- initialization of intern variable %s", name_node->value->var->name)
    }

    else return 0;

    if (param->right)
    {
        TranslateFunctionInitParamsToAsm(param->right, asm_file);
    }

    return 1;
}


#undef VARS_REG
