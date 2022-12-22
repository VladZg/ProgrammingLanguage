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
#include "./FrontEnd/VarTable.h"
#include "./BackEnd/BackEnd.h"

struct Programm
{
    FILE* file;
    ProgrammTokens* tokens;
    Node* tree;
    VarTable* var_table;
};

int main()
{
//     VarTable* var_table = VarTableCtor();
//
//     char s[10] = {};
//
//     for (int i = 40; i < 80; i++)
//     {
//         sprintf(s, "%c", (char) i);
//         AddNewVar(var_table, s, (double) i * 3.33);
//     }
//
//     VarTableDump(var_table);
//
//     VarTableDtor(&var_table);

    Programm programm = {};

    programm.var_table = VarTableCtor();

    programm.file = fopen("./Programms/square.vlds", "r");
    ASSERT(programm.file != nullptr)

    char programm_code[MAX_PROGRAMM_LENGTH] = {};

    char* programm_code_ptr = programm_code;
    fscanf(programm.file, "%[^EOF]s", programm_code_ptr);

    fclose(programm.file);

    programm.tokens = ProgrammTokensCtor();

    AnalyzeProgrammCode(programm.tokens, programm_code);

    ProgrammTokensDump(programm.tokens);

    programm.tree = GetProgramm(programm.tokens, programm.var_table);

    // ShowTree(programm.tree, SIMPLE_DUMP_MODE, 1);
    // ShowTree(programm.tree, FULL_FULL_DUMP_MODE, 1);

    // CalculateConstantSubtrees(root);
    // TreePreorderPrint(programm_tree, stdout);
    // fprintf(stdout, "\n");

    FILE* standartized_tree_file = fopen("./FrontEnd/StandartizedTree.txt", "w");
    ASSERT(standartized_tree_file != nullptr)

    WriteTreeInStandartForm(programm.tree, standartized_tree_file);

    fclose(standartized_tree_file);

    ProcessProgramm(programm.tree);

    ProgrammTokensDtor(&programm.tokens);
    VarTableDtor(&programm.var_table);
    NodeDtor(&programm.tree);

    return 1;
}
