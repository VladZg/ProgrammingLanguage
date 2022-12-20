#include "../Config.h"
#include <./stdlib.h>
#include "../Assert.h"
#include "../Constants/Constants.h"
#include "./LexicalAnalyzator.h"
#include "./VarTable.h"

Var* CopyVar(Var* var)
{
    ASSERT(var != nullptr)

    return VarCtor(var->name, var->value);
}

VarTable* VarTableCtor()
{
    VarTable* var_table = (VarTable*) calloc(1, sizeof(VarTable));
    ASSERT(var_table != nullptr)

    var_table->capacity = N_VARS_INITIAL;

    var_table->vars = (Var**) calloc(var_table->capacity, sizeof(Var*));
    var_table->n_vars = 0;

    return var_table;
}

int VarTableDtor(VarTable** var_table)
{
    ASSERT(var_table != nullptr)
    ASSERT(*var_table != nullptr)

    while ((*var_table)->n_vars-- > 0)
    {
        if ((*var_table)->vars[(*var_table)->n_vars])
            VarDtor(&(*var_table)->vars[(*var_table)->n_vars]);
    }

    free((*var_table)->vars);
    (*var_table)->vars = nullptr;

    (*var_table)->capacity = 0;

    free(*var_table);
    *var_table = nullptr;

    return 1;
}

int VarTableResize(VarTable* var_table, size_t new_capacity)
{
    ASSERT(var_table != nullptr)

    if (new_capacity > var_table->capacity)
    {
        var_table->vars = (Var**) realloc(var_table->vars, new_capacity * sizeof(Var*));

        var_table->capacity = new_capacity;

        return 1;
    }

    fprintf(stderr, KYEL "Can't resize var_table to a size lower than a number of variables\n" KNRM);

    return 0;
}

int AddVar(VarTable* var_table, Var* var)
{
    ASSERT(var_table != nullptr)
    ASSERT(var != nullptr)

    if (FindVarIndex(var, var_table) != NOT_VAR_INDEX)
    {
        // fprintf(stderr, KYEL "Var table already contains variable with such name\n" KNRM);

        return 0;
    }

    if (var_table->n_vars >= MAX_N_VARS)
    {
        fprintf(stderr, KYEL "Var table contains max. number of vars, new variable can't be added\n" KNRM);

        return 0;
    }

    if (var_table->n_vars + 1 > var_table->capacity)
        VarTableResize(var_table, var_table->n_vars * 2);

    var_table->vars[var_table->n_vars++] = var;

    return 1;
}

int AddNewVar(VarTable* var_table, const char* name, double value)
{
    ASSERT(var_table != nullptr)
    ASSERT(name != nullptr)

    Var* new_var = VarCtor(name, value);
    ASSERT(new_var != nullptr)

    if (AddVar(var_table, new_var)) return 1;

    VarDtor(&new_var);

    return 0;
}

int FindVarIndex(Var* var, VarTable* var_table)
{
    ASSERT(var_table != nullptr)
    ASSERT(var != nullptr)

    size_t var_i = 0;

    while (var_i < var_table->n_vars)
    {
        if (!strcmp(var->name, var_table->vars[var_i]->name))

            return var_i;

        var_i++;
    }

    return NOT_VAR_INDEX;
}

int FindVarIndexByName(const char* var_name, VarTable* var_table)
{
    ASSERT(var_table != nullptr)
    ASSERT(var_name != nullptr)

    Var* var = VarCtor(var_name, VAR_DEAD_VAL);

    int var_index = FindVarIndex(var, var_table);

    VarDtor(&var);

    return var_index;
}

void VarTableDump(VarTable* var_table)
{
    ASSERT(var_table != nullptr)

    fprintf(stdout, KMAG "Var table dump:\n" KNRM
                    "   size = %ld\n", var_table->n_vars);

    for (size_t var_i = 0; var_i < var_table->n_vars; var_i++)
    {
        fprintf(stdout, "   [%ld] %s = %lf\n", var_i, var_table->vars[var_i]->name, var_table->vars[var_i]->value);
    }
}
