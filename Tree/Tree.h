#ifndef TREE_H
#define TREE_H

#include "../Config.h"
#include <stdlib.h>
#include <stdio.h>
#include "../Constants/Constants.h"

struct Node
{
    NodeDataType val_type = NODE_NULL_TYPE;

    Value* value;

    Node*  prev;
    Node*  left;
    Node*  right;

    size_t index;
};

struct Tree
{
    Node*  root;
    size_t n_nodes;
};

struct ExpressionVar
{
    char*  name;
    double value;
};

void WarningMessage(const char* function_name, const char* fmt_msg, ...);

Value* ValueCtor();
int ValueDtor(Value** value, NodeDataType val_type);

int NodeVerify (const char* function_name, const Node* node);
int NodeCtor   (Node* node, NodeDataType val_type, Value* value);
int NodeDtor   (Node** node);
int NodeConnect(Node* left, Node* right, Node* root);

Node* CreateNode(NodeDataType val_type, Value* value, Node* left, Node* right);
Node* CopyNode(const Node* node);
Node* CalculateConstantSubtrees(Node* node);
Node* DestroyNeutralTreeElements(Node* node);
Node* SimplifyTree(Node** node);
int IsVarsInTree(const Node* node);

size_t TreeDepth(const Node* node);
size_t TreeNumberOfNodes(const Node* node);

Operators IsOperator(const char* node_val);
Separators IsSeparator(const char* node_val);
KeyWords IsKeyWord(const char* node_val);

void OperatorPrint(Operators code, FILE* stream);
void SeparatorPrint(Separators code, FILE* stream);
void KeyWordPrint(KeyWords code, FILE* stream);
void VarPrint(Var* var, FILE* stream);
void NodeValPrint(const Node* node, FILE* stream);

void TreePreorderPrint (const Node* node, FILE* stream);
void TreeInorderPrint  (const Node* node, FILE* stream);
void TreePostorderPrint(const Node* node, FILE* stream);

void FprintfNSymb(FILE* stream, char symb, size_t n_symb);
int NodeSdandartPrint(const Node* node, FILE* stream);
int WriteTreeInStandartForm(const Node* node, FILE* stream);

// int  IsObjectExist(const Node* node, const char* obj_name);
// int  CountSubNodes(Node* node, int* counter);

#ifndef NDEBUG

#define VERIFY_TREE( tree_ptr )     \
    if (!TreeVerify(tree_ptr)) return 0;

#define VERIFY_NODE( node_ptr ) NodeVerify(__PRETTY_FUNCTION__, node_ptr)


#else

#define VERIFY_TREE( tree_ptr )
#define VERIFY_NODE( node_ptr )

#endif

#include "./TreeDSL.h"

#endif
