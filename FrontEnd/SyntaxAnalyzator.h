#ifndef SYNTAX_ANALYZATOR_H
#define SYNTAX_ANALYZATOR_H

#include "../Config.h"
#include <./stdlib.h>
#include "../Constants/Constants.h"
#include "../Tree/Tree.h"
#include "./LexicalAnalyzator.h"
#include "./VarTable.h"

void ErrorPrint(const char* function_name, const char* error_name, size_t token_cursor, Token* token);

Node* GetProgramm             (ProgrammTokens* programm_tokens, VarTable* var_table); // Считывание всего выражения
// Node* GetProgrammBody         (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetStatement            (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetProgrammBody         (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetInstructions         (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetKeyInstruction       (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetInstruction          (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetInitialization       (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetAssignment           (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetExpression           (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetAddSub               (ProgrammTokens* programm_tokens, VarTable* var_table); // Считывание операций +, -
Node* GetMultiplication       (ProgrammTokens* programm_tokens, VarTable* var_table); // Считывание операций *, /
Node* GetDegree               (ProgrammTokens* programm_tokens, VarTable* var_table); // Считывание операции ^
Node* GetPrimaryExpression    (ProgrammTokens* programm_tokens, VarTable* var_table); // Считывание выражений в скобочках (...)
Node* GetIfElseInstructionBody(ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetWhileInstruction     (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetIfElseInstruction    (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetIOInstruction        (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetFunctionBlock        (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetBlock                (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetName                 (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetFunctionDefinition   (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetFunctionInitParam    (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetFunctionInitParams   (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetFunctionCallParam    (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetFunctionCallParams   (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetFunctionCall         (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetReturnInstruction    (ProgrammTokens* programm_tokens, VarTable* var_table);
Node* GetNumber               (ProgrammTokens* programm_tokens, VarTable* var_table);
// void CheckForSemiColon        (ProgrammTokens* programm_tokens, VarTable* var_table);

int IsReturnStatementInFunction(const Node* node);


#endif
