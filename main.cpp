#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "./Assert.h"

char* SkipSpaces(char* str);
int AnalyzeProgrammCode(char* programm_code, char* programm_code_analyzed);

double GetG(const char* str); // Считывание всего выражения
double GetE();                // Считывание операций +, -
double GetT();                // Считывание операций *, /
double GetD();                // Считывание операции ^
double GetP();                // Считывание выражений в скобочках (...)
double GetN();                // Считывание неотрицательных целых чисел

const char* S   = nullptr;

int main()
{
    FILE* file = fopen("main.txt", "r");

    char programm_code[100] = {};

    char* programm_code_ptr = programm_code;
    fscanf(file, "%[^EOF]s", programm_code_ptr);

    fclose(file);

    char programm_code_analyzed[100] = {};

    AnalyzeProgrammCode(programm_code, programm_code_analyzed);

    fprintf(stdout, "Выражение: %s\n", programm_code_analyzed);
    fprintf(stdout, "значение выражения: %lf\n", GetG(programm_code_analyzed));

    return 1;
}

char* SkipSpaces(char* str)
{
    while ((*str == ' ') || (*str == '\n'))
    {
        // fprintf(stdout, "skip\n");
        str++;
    }

    return str;
}

int AnalyzeProgrammCode(char* programm_code, char* programm_code_analyzed)
{
    size_t i = 0;

    while (*programm_code)
    {
        if ((*programm_code == ' ') || (*programm_code == '\n'))
        {
            programm_code = SkipSpaces(programm_code);
            // fprintf(stdout, "HERE");
        }

        programm_code_analyzed[i++] = *(programm_code++);

        // fprintf(stdout, ". %c\n", *(programm_code++));
    }

    return 1;
}

double GetG(const char* str)
{
    ASSERT(str != nullptr);

    S = str;

    double val = GetE();

    ASSERT(*S == '\0');

    return val;
}

double GetE()
{
    double val = GetT();

    while (*S == '+' || *S == '-')
    {
        char op = *S;

        S++;

        double val2 = GetT();

        if (op == '+')
            val += val2;

        else
            val -= val2;
    }

    return val;
}

double GetT()
{
    double val = GetD();

    while (*S == '*' || *S == '/')
    {
        char op = *S;

        S++;

        double val2 = GetD();

        if (op == '*')
            val *= val2;

        else
            val /= val2;
    }

    return val;
}

double GetD()
{
    double val = GetP();

    while (*S == '^')
    {
        char op = *S;

        S++;

        double val2 = GetP();

        val = pow(val, val2);
    }

    return val;
}

double GetP()
{
    double val = 0;

    if (*S == '(')
    {
        S++;

        val = GetE();

        ASSERT(*S == ')');

        S++;
    }

    else
        val = GetN();

    return val;
}

double GetN()
{
    double val = 0;

    const char* s_old = S;

    while ('0' <= *S && *S <= '9')
    {
        val = val * 10 + (*S - '0');
        S++;
    }

    ASSERT(S > s_old);

    return val;
}
