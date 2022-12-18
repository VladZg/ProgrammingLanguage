#include "../Config.h"
#include <stdlib.h>
#include <cstring>
#include <sys/stat.h>
#include "../Constants/Constants.h"
#include "../Constants/DefineColourConsts.h"
#include "../Assert.h"
#include "../Tree/Tree.h"
#include "../FrontEnd/LexicalAnalyzator.h"
#include "./BackEnd.h"

int ProcessProgramm(Node* node)
{
    ASSERT(node != nullptr);

    FILE* asm_file = fopen("./Programms/Source.txt", "w");

    TranslateProgrammToAsm(node, asm_file);

    fprintf(asm_file, "out\n"
                      "hlt\n");

    fclose(asm_file);

    system("cd ./Processor                      ;"
           "./asm ../Programms/Source.txt       ;"
           "./cpu ../Programms/Source_output.asm;"
           "cd ../                              ;");

    return 1;
}

#define DEF_OP(op_code, op_name, op_lang_name)                  \
    case OP_##op_name:                                          \
            {                                                   \
                fprintf(asm_file, "%s      \\\\%sING\n\n",      \
                                  #op_name, #op_name);          \
                break;                                          \
            }

int TranslateProgrammToAsm(Node* node, FILE* asm_file)
{
    if (!node) return 1;

    if (node->left)
        TranslateProgrammToAsm(node->left , asm_file);

    if (node->right)
        TranslateProgrammToAsm(node->right, asm_file);

    if (node->val_type == NODE_NUM_TYPE)
    {
        fprintf(asm_file, "push %d\n", (int) node->value.num_val);
        return 1;
    }

    if (node->val_type == NODE_OP_TYPE)
    {
        switch (node->value.op_val)
        {
            #include "../Dictionary/Operators.h"
        }

        return 1;
    }

    return 0;
}

#undef DEF_OP
