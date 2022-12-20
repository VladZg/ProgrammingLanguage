#ifndef SYNTAX_ANALYZATOR_H
#define SYNTAX_ANALYZATOR_H

#include "../Config.h"
#include <./stdlib.h>
#include "../Constants/Constants.h"
#include "../Tree/Tree.h"
#include "./LexicalAnalyzator.h"

void SyntaxErrorPrint(const char* function_name, size_t token_cursor, Token* token);

Node* GetProgramm          (ProgrammTokens* programm_tokens); // Считывание всего выражения
// Node* GetProgrammBody      (ProgrammTokens* programm_tokens);
Node* GetStatement         (ProgrammTokens* programm_tokens);
Node* GetProgrammBody      (ProgrammTokens* programm_tokens);
Node* GetInstructions      (ProgrammTokens* programm_tokens);
Node* GetKeyInstruction    (ProgrammTokens* programm_tokens);
Node* GetInstruction       (ProgrammTokens* programm_tokens);
Node* GetInitialization    (ProgrammTokens* programm_tokens);
Node* GetAssignment        (ProgrammTokens* programm_tokens);
Node* GetExpression        (ProgrammTokens* programm_tokens); // Считывание операций +, -
Node* GetMultiplication    (ProgrammTokens* programm_tokens); // Считывание операций *, /
Node* GetDegree            (ProgrammTokens* programm_tokens); // Считывание операции ^
Node* GetPrimaryExpression (ProgrammTokens* programm_tokens); // Считывание выражений в скобочках (...)
Node* GetW                 (ProgrammTokens* programm_tokens); // Считывание неотрицательных целых чисел
Node* GetWhileInstruction  (ProgrammTokens* programm_tokens);
Node* GetIfElseInstruction (ProgrammTokens* programm_tokens);
Node* GetFunctionBlock     (ProgrammTokens* programm_tokens);
Node* GetBlock             (ProgrammTokens* programm_tokens);
Node* GetName              (ProgrammTokens* programm_tokens);
Node* GetFunctionDefinition(ProgrammTokens* programm_tokens);
Node* GetFunctionInitParam (ProgrammTokens* programm_tokens);
Node* GetFunctionInitParams(ProgrammTokens* programm_tokens);
Node* GetFunctionCall      (ProgrammTokens* programm_tokens);
Node* GetNumber            (ProgrammTokens* programm_tokens);

#endif
