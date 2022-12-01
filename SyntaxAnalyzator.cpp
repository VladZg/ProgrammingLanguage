#include "./Config.h"
#include <stdlib.h>
#include "./Stack/Assert.h"
#include "./Tree.h"
#include "./SyntaxAnalyzator.h"

const char* S   = nullptr;

Node* GetG(const char* str)
{
    ASSERT(str != nullptr);

    S = str;

    Node* node = GetE();

    ASSERT(*S == '\0');

    return node;
}

Node* GetE()
{
    Node* node = GetT();

    while (*S == '+' || *S == '-')
    {
        char op = *S;

        S++;

        Node* left_node  = node;
        Node* right_node = GetT();

        if (op == '+')
            node = ADD(left_node, right_node);

        else
            node = SUB(left_node, right_node);
    }

    return node;
}

Node* GetT()
{
    Node* node = GetD();

    while (*S == '*' || *S == '/')
    {
        char op = *S;

        S++;

        Node* left_node  = node;
        Node* right_node = GetD();

        if (op == '*')
            node = MUL(left_node, right_node);

        else
            node = DIV(left_node, right_node);
    }

    return node;
}

Node* GetD()
{
    Node* node = GetP();

    while (*S == '^')
    {
        char op = *S;

        S++;

        Node* left_node  = node;
        Node* right_node = GetP();

        node = DEG(left_node, right_node);
    }

    return node;
}

Node* GetP()
{
    Node* node = nullptr;

    if (*S == '(')
    {
        S++;

        node = GetE();

        ASSERT(*S == ')');

        S++;
    }

    else
        node = GetN();

    return node;
}

Node* GetN()
{
    double val = 0;

    const char* s_old = S;

    while ('0' <= *S && *S <= '9')
    {
        val = val * 10 + (*S - '0');
        S++;
    }

    ASSERT(S > s_old);

    // fprintf(stderr, "%lf\n", val);

    return CREATE_NUM(val);
}
