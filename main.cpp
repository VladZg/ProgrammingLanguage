#include "./Config.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Stack/Stack.h"
#include "./Tree.h"
#include "./TreeDump.h"
// #include "./ReadAndWriteFunctions.h"
#include "./DiffDSL.h"

#include "./FrontEnd/LexicalAnalyzator.h"
#include "./FrontEnd/SyntaxAnalyzator.h"
#include "./BackEnd.h"

int main()
{
    FILE* file = fopen("./programm.vlds", "r");

    char programm_code[MAX_PROGRAMM_LENGTH] = {};

    char* programm_code_ptr = programm_code;
    fscanf(file, "%[^EOF]s", programm_code_ptr);

    // fprintf(stdout, "%s", programm_code_ptr);

    fclose(file);

    ProgrammTokens* programm_tokens = ProgrammTokensCtor();

    AnalyzeProgrammCode(programm_tokens, programm_code);

    ProgrammTokensDump(programm_tokens);

    // fprintf(stdout, "Выражение: %s\n", programm_code_analyzed);

    // Node* root = GetG(&programm_tokens, programm_code_analyzed);

    // ShowTree(root, SIMPLE_DUMP_MODE, 1);

    // CalculateConstantSubtrees(root);
    // TreeInorderPrint(root, stdout);

    // ShowTree(root, FULL_FULL_DUMP_MODE, 1);

    // fprintf(stdout, "Значение выражения: %lf\n", root->num_val);

    // ProgrammTokensDump(&programm_tokens);
    // fprintf(stdout, "\n");

    // ProcessProgramm(root);

    // fprintf(stderr, "%f %f %d\n", (float) 5, (float) 2, (int) pow(5, 2));

    ProgrammTokensDtor(&programm_tokens);
    // NodeDtor(&root);

    return 1;
}
