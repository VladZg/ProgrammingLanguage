#ifndef _REVERSE_FRONT_END_H
#define _REVERSE_FRONT_END_H

#include "../Config.h"
#include "../Constants/Constants.h"
#include "../Tree/Tree.h"

int TranslitProgrammTreeToProgramm(FILE* stream, const Node* node, TranslitModes mode);

#endif
