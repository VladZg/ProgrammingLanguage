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
    FILE* programm_file = fopen("./Programms/programm.vlds", "r");
    ASSERT(programm_file != nullptr)

    char programm_code[MAX_PROGRAMM_LENGTH] = {};

    char* programm_code_ptr = programm_code;
    fscanf(programm_file, "%[^EOF]s", programm_code_ptr);

    fclose(programm_file);

    ProgrammTokens* programm_tokens = ProgrammTokensCtor();

    AnalyzeProgrammCode(programm_tokens, programm_code);

    ProgrammTokensDump(programm_tokens);

    // fprintf(stdout, "Выражение: %s\n", programm_code_analyzed);

    Node* programm_tree = GetProgramm(programm_tokens);

    ShowTree(programm_tree, SIMPLE_DUMP_MODE, 1);
    // ShowTree(programm_tree, FULL_FULL_DUMP_MODE, 1);

    // CalculateConstantSubtrees(root);
    // TreePreorderPrint(programm_tree, stdout);
    // fprintf(stdout, "\n");

    FILE* standartized_tree_file = fopen("./FrontEnd/StandartizedTree.txt", "w");
    ASSERT(standartized_tree_file != nullptr)

    WriteTreeInStandartForm(programm_tree, standartized_tree_file);

    fclose(standartized_tree_file);

    // ProcessProgramm(programm_tree);

    ProgrammTokensDtor(&programm_tokens);
    NodeDtor(&programm_tree);

    return 1;
}
