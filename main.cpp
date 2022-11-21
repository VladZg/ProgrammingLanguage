#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "./Assert.h"

double GetG(const char* str); // Считывание всего выражения
double GetE();                // Считывание операций +, -
double GetT();                // Считывание операций *, /
double GetD();                // Считывание операции ^
double GetP();                // Считывание выражений в скобочках (...)
double GetN();                // Считывание неотрицательных целых чисел

const char* S = nullptr;

int main()
{
    FILE* file = fopen("main.txt", "r");

    char s[100] = {};
    fscanf(file, "%[^\n]s", s);

    fprintf(stdout, "значение выражения: %lf\n", GetG(s));

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
