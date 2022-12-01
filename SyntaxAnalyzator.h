#ifndef SYNTAX_ANALYZATOR_H
#define SYNTAX_ANALYZATOR_H

#include "./Config.h"
#include <./stdlib.h>
#include "./Tree.h"

Node*  GetG(const char* str); // Считывание всего выражения
Node*  GetE();                // Считывание операций +, -
Node*  GetT();                // Считывание операций *, /
Node*  GetD();                // Считывание операции ^
Node*  GetP();                // Считывание выражений в скобочках (...)
Node*  GetN();                // Считывание неотрицательных целых чисел

#endif
