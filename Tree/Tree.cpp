#include "../Config.h"
#include <stdlib.h>
#include <math.h>
#include "../Constants/Constants.h"
#include "../Constants/DefineColourConsts.h"
#include "../Assert.h"
#include "../Stack/Stack.h"
#include "./Tree.h"
#include "../FrontEnd/LexicalAnalyzator.h"

size_t NodeIndex = 0;

void WarningMessage(const char* function_name, const char* fmt_msg, ...)
{
    va_list args;
    va_start(args, fmt_msg);

    fprintf(stderr, KYEL "WARNING!!!" KNRM "(" KBLU "%45s" KNRM "): ", function_name);
    vfprintf(stderr, fmt_msg, args);
    fprintf(stderr, "\n");

    va_end(args);
}

// #define WARNING(fmt_msg, ...) \
{\
    if (sizeof((int[]){__VA_ARGS__})/sizeof(int) > 0)               \
        WarningMessage(__PRETTY_FUNCTION__, #fmt_msg, __VA_ARGS__); \
\
    else  WarningMessage(__PRETTY_FUNCTION__, #fmt_msg);\
}

int NodeVerify(const char* function_name, const Node* node)
{
    if (!node) return 1;

    // if (node->val_type == NODE_NULL_TYPE)
    // {
    //     WarningMessage(function_name, "NODE IS EMPTY");
    //     return 0;
    // }

    if (node->val_type == NODE_NUM_TYPE)
    {
        if (node->left)
        {
            WarningMessage(function_name, "NODE WITH NUMBER HAS LEFT SUBNODE");
            return 0;
        }

        if (node->right)
        {
            WarningMessage(function_name, "NODE WITH NUMBER HAS RIGHT SUBNODE");
            return 0;
        }
    }

    else if (node->val_type == NODE_VAR_TYPE ||
             node->val_type == NODE_FUNC_TYPE  )
    {
        if (!node->value.var.name)
        {
            WarningMessage(function_name, "NODE WITH VARIABLE/FUNCTION TYPE IS EMPTY");
            return 0;
        }

        if (node->left)
        {
            WarningMessage(function_name, "NODE WITH VARIABLE/FUNCTION HAS LEFT SUBNODE");
            return 0;
        }

        if (node->right)
        {
            WarningMessage(function_name, "NODE WITH VARIABLE/FUNCTION HAS RIGHT SUBNODE");
            return 0;
        }
    }

    else if (node->val_type == NODE_OP_TYPE )
    {
        if (node->value.op_val == NOT_OP)
        {
            WarningMessage(function_name, "NODE WITH OPERATOR %d TYPE IS EMPTY", node->value.op_val);
            return 0;
        }

        if (!node->left)
        {
            WarningMessage(function_name, "NODE WITH OPERATOR %d DOESN'T HAVE LEFT SUBNODE", node->value.op_val);
            return 0;
        }

        if (!node->right)
        {
            WarningMessage(function_name, "NODE WITH OPERATOR %d DOESN'T HAVE RIGHT SUBNODE", node->value.op_val);
            return 0;
        }
    }

    else if (node->val_type == NODE_SEP_TYPE )
    {
        if (node->value.sep_val == NOT_SEP)
        {
            WarningMessage(function_name, "NODE WITH SEPARATOR %d TYPE IS EMPTY", node->value.sep_val);
            return 0;
        }

//         if (!node->left)
//         {
//             WarningMessage(function_name, "NODE WITH SEPARATOR %d DOESN'T HAVE LEFT SUBNODE", node->value.sep_val);
//             return 0;
//         }
//
//         if (!node->right)
//         {
//             WarningMessage(function_name, "NODE WITH SEPARATOR %d DOESN'T HAVE RIGHT SUBNODE", node->value.sep_val);
//             return 0;
//         }
    }

    else if (node->val_type == NODE_KEY_TYPE )
    {
        if (node->value.key_val == NOT_KEY)
        {
            WarningMessage(function_name, "NODE WITH KEY WORD %d TYPE IS EMPTY", node->value.key_val);
            return 0;
        }

//         if (!node->left)
//         {
//             WarningMessage(function_name, "NODE WITH KEY WORD %d DOESN'T HAVE LEFT SUBNODE", node->value.key_val);
//             return 0;
//         }
//
//         if (!node->right)
//         {
//             WarningMessage(function_name, "NODE WITH KEY WORD %d DOESN'T HAVE RIGHT SUBNODE", node->value.key_val);
//             return 0;
//         }
    }

    if (node->prev)
    {
        if (node->prev->left != node && node->prev->right != node)
        {
            WarningMessage(function_name, "NODE IS WRONGLY CONNECTED TO IT'S PARENT");
            return 0;
        }
    }

    if (node->left)
    {
        if (node != node->left->prev)
        {
            WarningMessage(function_name, "LEFT SUBNODE'S PREV IS WRONG");
            return 0;
        }

        if (!NodeVerify(function_name, node->left)) return 0;
    }

    if (node->right)
    {
        if (node != node->right->prev)
        {
            WarningMessage(function_name, "RIGHT SUBNODE'S PREV IS WRONG");
            return 0;
        }

        if (!NodeVerify(function_name, node->right)) return 0;
    }

    if ((node->left && node->right) && (node->left == node->right))
    {
        WarningMessage(function_name, "LEFT AND RIGHT SUBNODES ARE THE SAME SUBNODE");
        return 0;
    }

    return 1;
}

int NodeCtor(Node* node, NodeDataType val_type, Value value)
{
    ASSERT(node     != nullptr);
    ASSERT(val_type != NODE_NULL_TYPE);

    if (val_type == NODE_OP_TYPE && value.op_val == NOT_OP)
    {
        WarningMessage(__PRETTY_FUNCTION__, "TRIED TO CONSTRUCT NODE WITH EMPTY OPERATOR");
        return 0;
    }

    if (val_type == NODE_VAR_TYPE && !value.var.name)
    {
        WarningMessage(__PRETTY_FUNCTION__, "TRIED TO CONSTRUCT NODE WITH EMPTY VARIABLE");
        return 0;
    }

    *node = {val_type, value, nullptr, nullptr, nullptr, NodeIndex++};

    return 1;
}

Node* CreateNode(NodeDataType val_type, Value value, Node* left, Node* right)
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    ASSERT(node != nullptr)

    NodeCtor(node, val_type, value);
    NodeConnect(left, right, node);

    return node;
}

Node* CopyNode(const Node* node)
{
    if (node == nullptr) return nullptr;

    Node* left  = CopyNode(node->left);
    Node* right = CopyNode(node->right);

    Value value = {};

    if (node->val_type == NODE_OP_TYPE)
    {
        value.op_val = node->value.op_val;
    }

    if (node->val_type == NODE_NUM_TYPE)
    {
        value.num_val = node->value.num_val;
    }

    if (node->val_type == NODE_KEY_TYPE)
    {
        value.key_val = node->value.key_val;
    }

    if (node->val_type == NODE_SEP_TYPE)
    {
        value.sep_val = node->value.sep_val;
    }

    if (node->val_type == NODE_VAR_TYPE ||
        node->val_type == NODE_FUNC_TYPE  )
    {
        Var var = {};
        value.var = VarCtor(&var, node->value.var.name, node->value.var.value);
    }

    Node* new_node = CreateNode(node->val_type, value, left, right);
    new_node->prev = node->prev;

    return new_node;
}

#define NODE_PTR (*node)

int NodeDtor(Node** node)
{
    ASSERT(node != nullptr);

    VERIFY_NODE(NODE_PTR);

    if (!NODE_PTR)
        return 1;

    if (NODE_PTR->left)  if(!NodeDtor(&(NODE_PTR->left)))  return 0;

    if (NODE_PTR->right) if(!NodeDtor(&(NODE_PTR->right))) return 0;

    if (NODE_PTR->val_type == NODE_NUM_TYPE)
        NODE_PTR->value.num_val = NOT_NUM;

    else if (NODE_PTR->val_type == NODE_OP_TYPE)
        NODE_PTR->value.num_val = NOT_OP;

    else if (NODE_PTR->val_type == NODE_SEP_TYPE)
        NODE_PTR->value.num_val = NOT_SEP;

    else if (NODE_PTR->val_type == NODE_KEY_TYPE)
        NODE_PTR->value.num_val = NOT_KEY;

    else if (NODE_PTR->val_type == NODE_VAR_TYPE)
        VarDtor(&NODE_PTR->value.var);

    else if (NODE_PTR->val_type == NODE_FUNC_TYPE)
        VarDtor(&NODE_PTR->value.var);

    NODE_PTR->val_type = NODE_NULL_TYPE;

    NODE_PTR->left      = nullptr;
    NODE_PTR->right     = nullptr;
    NODE_PTR->prev      = nullptr;
    NodeIndex--;

    free(NODE_PTR);
    *node = nullptr;

    return 1;
}

#undef NODE_PTR

int NodeConnect(Node* left, Node* right, Node* root)
{
    // ASSERT(tree != nullptr);
    ASSERT(root != nullptr);

    // if (!VERIFY_NODE(left))  return 0;
    // if (!VERIFY_NODE(right)) return 0;
    // if (!VERIFY_NODE(root))  return 0;

    if (root)
    {
        root->left = left;

        if (left)
            left->prev  = root;
            // tree->n_nodes++;

        root->right = right;

        if (right)
            right->prev  = root;
            // tree->n_nodes++;
    }

    return VERIFY_NODE(root);
}

Node* ReplaceNode(Node** replacing_node, Node* new_node)
{
    new_node->prev = (*replacing_node)->prev;

    NodeDtor(replacing_node);
    *replacing_node = new_node;

    return *replacing_node;
}

int SwapNodes(Node** node1, Node** node2)
{
    Node* temp_node = *node1;

    *node1 = *node2;
    *node2 = temp_node;

    return 1;
}

size_t TreeDepth(const Node* node)
{
    if (node == nullptr) return 0;

    size_t left_depth  = 0;
    size_t right_depth = 0;

    if (node->left)
        left_depth = TreeDepth(node->left);

    if (node->right)
        right_depth = TreeDepth(node->right);

    // fprintf(stdout, "%ld %ld\n", left_depth, right_depth);

    return 1 + (left_depth > right_depth ? left_depth : right_depth); //max(left_depth, right_depth);
}

size_t TreeNumberOfNodes(const Node* node)
{
    if (!node) return 0;

    return 1 + TreeNumberOfNodes(node->left) + TreeNumberOfNodes(node->right);
}

#define DEF_OP(num, name, val)  \
    if (!strcasecmp(node_val, val)) return OP_##name;  \
    else

Operators IsOperator(const char* node_val)
{
    #include "../Dictionary/Operators.h"

    return NOT_OP;
}

#undef DEF_OP

#define DEF_SEP(num, name, val)  \
    if (!strcasecmp(node_val, val)) return SEP_##name;  \
    else

Separators IsSeparator(const char* node_val)
{
    #include "../Dictionary/Separators.h"

    return NOT_SEP;
}

#undef DEF_SEP

#define DEF_KEY(num, name, val)  \
    if (!strcasecmp(node_val, val)) return KEY_##name;  \
    else

KeyWords IsKeyWord(const char* node_val)
{
    #include "../Dictionary/KeyWords.h"

    return NOT_KEY;
}

#undef DEF_KEY

#define DEF_OP(op_code, op_name, op_lang_name)                           \
    if (code == OP_##op_name) { fprintf(stream, op_lang_name); return; }

void OperatorPrint(Operators code, FILE* stream)
{
    #include "../Dictionary/Operators.h"

    fprintf(stream, "not an operator" );
}

#undef DEF_OP

#define DEF_SEP(sep_code, sep_name, sep_lang_name)                          \
    if (code == SEP_##sep_name) { fprintf(stream, sep_lang_name); return; }

void SeparatorPrint(Separators code, FILE* stream)
{
    #include "../Dictionary/Separators.h"

    fprintf(stream, "not a separator" );
}

#undef DEF_SEP

#define DEF_KEY(key_code, key_name, key_lang_name)                          \
    if (code == KEY_##key_name) { fprintf(stream, key_lang_name); return; }

void KeyWordPrint(KeyWords code, FILE* stream)
{
    #include "../Dictionary/KeyWords.h"

    fprintf(stream, "not a key word" );
}

#undef DEF_KEY

void VarPrint(Var var, FILE* stream)
{
    fprintf(stream, "%s", var.name);
}

void NodeValPrint(const Node* node, FILE* stream)
{
    if (node->val_type == NODE_NUM_TYPE)
    {
        if (node->value.num_val == node->value.num_val) //проверка на nan
            fprintf(stream, "%lg" , node->value.num_val);

        else
        {
            fprintf(stream, "?(inf)?");
            WarningMessage(__PRETTY_FUNCTION__, "NODE DATA IS NAN... WTF?");
        }
    }

    else if (node->val_type == NODE_OP_TYPE)
    {
        OperatorPrint(node->value.op_val, stream);
    }

    else if (node->val_type == NODE_SEP_TYPE)
    {
        SeparatorPrint(node->value.sep_val, stream);
    }

    else if (node->val_type == NODE_KEY_TYPE)
    {
        KeyWordPrint(node->value.key_val, stream);
    }

    else if (node->val_type == NODE_VAR_TYPE)
    {
        VarPrint(node->value.var, stream);
    }

    else
        fprintf(stream, "EMPTY");
}

int IsVarsInTree(const Node* node)
{
    if (!node) return 0;

    VERIFY_NODE(node);

    if (node->val_type == NODE_VAR_TYPE)
        return 1;

    if (node->val_type == NODE_OP_TYPE)
    {
        ASSERT(node->left  != nullptr)
        ASSERT(node->right != nullptr)

        if (IsVarsInTree(node->left) || IsVarsInTree(node->right)) return 1;
    }

    return 0;
}

void TreePreorderPrint(const Node* node, FILE* stream)
{
    ASSERT(node != nullptr);
    ASSERT(stream != nullptr);

    if (!node) return;

    fprintf(stream, "(");

    NodeValPrint(node, stream);

    if (node->left) TreePreorderPrint(node->left, stream);

    if (node->right) TreePreorderPrint(node->right, stream);

    fprintf(stream, ")");
}

void TreeInorderPrint(const Node* node, FILE* stream)
{
    ASSERT(node != nullptr);
    ASSERT(stream != nullptr);

    VERIFY_NODE(node);

    if (!node) return;

    // fprintf(stream, "(");

    if (node->left)
    {
        fprintf(stream, "(");
        TreeInorderPrint(node->left, stream);
    }

    NodeValPrint(node, stream);

    if (node->right)
    {
        TreeInorderPrint(node->right, stream);
        fprintf(stream, ")");
    }

    // fprintf(stream, ")");
}

void TreePostorderPrint(const Node* node, FILE* stream)
{
    ASSERT(node != nullptr);
    ASSERT(stream != nullptr);

    if (!node) return;

    fprintf(stream, "(");

    if (node->left) TreePostorderPrint(node->left, stream);

    if (node->right) TreePostorderPrint(node->right, stream);

    NodeValPrint(node, stream);

    fprintf(stream, ")");
}
