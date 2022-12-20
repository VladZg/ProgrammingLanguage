#ifndef _VAR_TABLE_H_
#define _VAR_TABLE_H_

#include "../Config.h"
#include <./stdlib.h>
#include "../Constants/Constants.h"
#include "./LexicalAnalyzator.h"

struct VarTable
{
    Var** vars;
    size_t n_vars;
    size_t capacity;
};

Var* CopyVar(Var* var);
VarTable* VarTableCtor();
int VarTableDtor(VarTable** var_table);
int VarTableResize(VarTable* var_table, size_t new_capacity);
int AddVar(VarTable* var_table, Var* var);
int AddNewVar(VarTable* var_table, const char* name, double value);
int FindVarIndex(Var* var, VarTable* var_table);
int FindVarIndexByName(const char* var_name, VarTable* var_table);
void VarTableDump(VarTable* var_table);

#endif
