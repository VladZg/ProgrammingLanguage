#include "./Config.h"
#include <stdlib.h>
#include <cstring>
#include <sys/stat.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Tree.h"
#include "./LexicalAnalyzator.h"
#include "./BackEnd.h"

int ProcessProgramm(Node* node)
{
    ASSERT(node != nullptr);

    FILE* asm_file = fopen("./Processor/Source.txt", "w");

    TranslateProgrammToAsm(node, asm_file);

    fprintf(asm_file, "out\n"
                      "hlt\n");

    fclose(asm_file);

    system("cd ./Processor  ;"
           "./asm Source.txt;"
           "./cpu           ;"
           "cd ..           ;");

    return 1;
}

int TranslateProgrammToAsm(Node* node, FILE* asm_file)
{
    if (!node) return 1;

    if (node->left)
        TranslateProgrammToAsm(node->left , asm_file);

    if (node->right)
        TranslateProgrammToAsm(node->right, asm_file);

    if (node->val_type == NUM_TYPE)
    {
        fprintf(asm_file, "push %d\n", (int) node->num_val);
        return 1;
    }

    if (node->val_type == OP_TYPE)
    {
        switch (node->op_val)
        {
            case OP_ADD:
            {
                fprintf(asm_file, "add      \\\\adding\n\n");
                break;
            }

            case OP_SUB:
            {
                fprintf(asm_file, "sub      \\\\subbing\n\n");
                break;
            }

            case OP_MUL:
            {
                fprintf(asm_file, "mul      \\\\mulling\n\n");
                break;
            }

            case OP_DIV:
            {
                fprintf(asm_file, "div      \\\\divving\n\n");
                break;
            }

            case OP_DEG:
            {
                fprintf(asm_file, "deg      \\\\powwing\n\n");
                break;
            }
        }

        return 1;
    }

    return 0;
}
