#ifndef SYNTAX_ANALYZATOR_H
#define SYNTAX_ANALYZATOR_H

#include "../Config.h"
#include <./stdlib.h>
#include "../Constants/Constants.h"
#include "../Tree/Tree.h"
#include "./LexicalAnalyzator.h"

void SyntaxErrorPrint(size_t token_cursor, Token* token);

Node*  GetProgramm         (ProgrammTokens* programm_tokens); // Считывание всего выражения
Node*  GetExpression       (ProgrammTokens* programm_tokens); // Считывание операций +, -
Node*  GetMultiplication   (ProgrammTokens* programm_tokens); // Считывание операций *, /
Node*  GetDegree           (ProgrammTokens* programm_tokens); // Считывание операции ^
Node*  GetPrimaryExpression(ProgrammTokens* programm_tokens); // Считывание выражений в скобочках (...)
Node*  GetW                (ProgrammTokens* programm_tokens); // Считывание неотрицательных целых чисел
char*  GetVariable         (const char**    expression_str );
Node*  GetNumber           (ProgrammTokens* programm_tokens);

#endif
