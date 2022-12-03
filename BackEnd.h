#ifndef BACKEND_H
#define BACKEND_H

#include "./Config.h"
#include <stdlib.h>
#include <cstring>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Tree.h"

int ProcessProgramm(Node* node);
int TranslateProgrammToAsm(Node* node, FILE* asm_file);

#endif
