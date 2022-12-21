#ifndef BACKEND_H
#define BACKEND_H

#include "../Config.h"
#include <stdlib.h>
#include "../Tree/Tree.h"
#include "../Stack/Stack.h"

int ProcessProgramm(const Node* node);

int TranslateProgrammTreeToAsm(const Node* node, FILE* asm_file);
int TranslateProgrammBodyToAsm(const Node* programm_tree, FILE* asm_file);
int TranslateExpressionToAsm(const Node* programm_tree, FILE* asm_file);
int TranslateComprasionToAsm(const Node* instruction, FILE* asm_file);
int TranslateVarInitializationToAsm(const Node* instruction, FILE* asm_file);
int TranslateFunctionInitializationToAsm(const Node* instruction, FILE* asm_file);
int TranslateFunctionCallToAsm(const Node* call, FILE* asm_file);
int TranslateAssignmentToAsm(const Node* assignment, FILE* asm_file);
int TranslateIfElseToAsm(const Node* assignment, FILE* asm_file, Stack* stack_of_calls);
int TranslateWhileToAsm(const Node* instruction, FILE* asm_file, Stack* stack_of_calls);
int TranslateIOInAsm(const Node* instruction, FILE* asm_file);
int TranslateIntputToAsm(const Node* param, FILE* asm_file);
int TranslateFunctionCallParamsToAsm(const Node* param, FILE* asm_file);
int TranslateOutputToAsm(const Node* param, FILE* asm_file);
int TranslateFunctionInitParamsToAsm(const Node* param, FILE* asm_file);

int  PrintInAsm(FILE* asm_file, const char* fmt_text, ...);

#endif
