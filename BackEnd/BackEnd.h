#ifndef BACKEND_H
#define BACKEND_H

#include "../Config.h"
#include <stdlib.h>
#include "../Tree/Tree.h"

int ProcessProgramm(Node* node);
int TranslateProgrammToAsm(Node* node, FILE* asm_file);

#endif
