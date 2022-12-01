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
#include "./ReadAndWriteFunctions.h"
#include "./DiffDSL.h"

#include "./LexicalAnalyzator.h"
#include "./SyntaxAnalyzator.h"

int main()
{
    FILE* file = fopen("main.txt", "r");

    char programm_code[100] = {};

    char* programm_code_ptr = programm_code;
    fscanf(file, "%[^EOF]s", programm_code_ptr);

    fclose(file);

    char programm_code_analyzed[100] = {};

    AnalyzeProgrammCode(programm_code, programm_code_analyzed);

    fprintf(stdout, "Выражение: %s\n", programm_code_analyzed);

    Node* root = GetG(programm_code_analyzed);

    ShowTree(root, SIMPLE_DUMP_MODE, 0);

    CalculateConstantSubtrees(root);
    // TreeInorderPrint(root, stdout);

    ShowTree(root, SIMPLE_DUMP_MODE, 0);

    fprintf(stdout, "Значение выражения: %lf\n", root->num_val);

    NodeDtor(&root);

    return 1;
}
