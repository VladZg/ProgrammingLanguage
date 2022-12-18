#include "./Config.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include "./Constants/Constants.h"
#include "./Constants/DefineColourConsts.h"
#include "./Assert.h"
#include "./Stack/Stack.h"
#include "./Tree/Tree.h"
#include "./Tree/TreeDump.h"
// #include "./ReadAndWriteFunctions.h"
#include "./Tree/TreeDSL.h"

#include "./FrontEnd/LexicalAnalyzator.h"
#include "./FrontEnd/SyntaxAnalyzator.h"
#include "./BackEnd/BackEnd.h"

int main()
{
    FILE* file = fopen("./Programms/programm.vlds", "r");

    char programm_code[MAX_PROGRAMM_LENGTH] = {};

    char* programm_code_ptr = programm_code;
    fscanf(file, "%[^EOF]s", programm_code_ptr);

    // fprintf(stdout, "%s", programm_code_ptr);

    fclose(file);

    ProgrammTokens* programm_tokens = ProgrammTokensCtor();

    AnalyzeProgrammCode(programm_tokens, programm_code);

    ProgrammTokensDump(programm_tokens);

    // fprintf(stdout, "Выражение: %s\n", programm_code_analyzed);

    Node* root = GetProgramm(programm_tokens);

//     Value value1 = {};
//     value1.num_val = 99.87;
//
//     Value value2 = {};
//     value2.sep_val = SEP_SEMI_COLON;
//
//     Value value3 = {};
//     value3.op_val = OP_ADD;
//
//     Node* node_l = CreateNode(NODE_NUM_TYPE, value1, nullptr, nullptr);
//     Node* node_r = CreateNode(NODE_SEP_TYPE, value2, nullptr, nullptr);
//     Node* root = CreateNode(NODE_OP_TYPE, value3, node_l, node_r);

    ShowTree(root, SIMPLE_DUMP_MODE, 0);
    ShowTree(root, FULL_FULL_DUMP_MODE, 1);

    // CalculateConstantSubtrees(root);
    // TreeInorderPrint(root, stdout);
    // fprintf(stdout, "\n");

    // ShowTree(root, FULL_FULL_DUMP_MODE, 1);

    // fprintf(stdout, "Значение выражения: %lf\n", root->num_val);

    // ProgrammTokensDump(&programm_tokens);
    // fprintf(stdout, "\n");

    // ProcessProgramm(root);

    // fprintf(stderr, "%f %f %d\n", (float) 5, (float) 2, (int) pow(5, 2));

    ProgrammTokensDtor(&programm_tokens);
    NodeDtor(&root);

    return 1;
}
