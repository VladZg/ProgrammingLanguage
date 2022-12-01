#ifndef LEXICAL_ANALYZATOR_H
#define LEXICAL_ANALYZATOR_H

#include "./Config.h"
#include <./stdlib.h>

char* SkipSpaces(char* str);
int AnalyzeProgrammCode(char* programm_code, char* programm_code_analyzed);

#endif
