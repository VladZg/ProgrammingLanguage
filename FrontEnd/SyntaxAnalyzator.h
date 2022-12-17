#ifndef SYNTAX_ANALYZATOR_H
#define SYNTAX_ANALYZATOR_H

#include "../Config.h"
#include <./stdlib.h>
#include "../Constants.h"
#include "../Tree.h"
#include "./LexicalAnalyzator.h"

void SyntaxErrorPrint(size_t token_cursor, Token* token);

Node*  GetG(ProgrammTokens* programm_tokens, const char* str); // Считывание всего выражения
Node*  GetE(ProgrammTokens* programm_tokens);                  // Считывание операций +, -
Node*  GetT(ProgrammTokens* programm_tokens);                  // Считывание операций *, /
Node*  GetD(ProgrammTokens* programm_tokens);                  // Считывание операции ^
Node*  GetP(ProgrammTokens* programm_tokens);                  // Считывание выражений в скобочках (...)
Node*  GetN(ProgrammTokens* programm_tokens);                  // Считывание неотрицательных целых чисел

#endif
