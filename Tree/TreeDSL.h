#ifndef TREE_DSL_H
#define TREE_DSL_H

#define CN CopyNode(node)
#define CL CopyNode(node->left)
#define CR CopyNode(node->right)

// #define CREATE_NUM(num)                CreateNode(NUM_TYPE, {}, nullptr, nullptr)
// #define CREATE_OP(op_val, left, right) CreateNode(OP_TYPE,  {}, left   , right  )
// #define CREATE_VAR(var_name)           CreateNode(VAR_TYPE, {}, nullptr, nullptr)
//
// #define CREATE_NUM(num)                CreateNode(NUM_TYPE, {}, nullptr, nullptr)
// #define CREATE_OP(op_val, left, right) CreateNode(OP_TYPE,  {}, left   , right  )
// #define CREATE_VAR(var_name)           CreateNode(VAR_TYPE, {}, nullptr, nullptr)

// #define ADD( left, right) CREATE_OP(OP_ADD , left         , right)
// #define SUB( left, right) CREATE_OP(OP_SUB , left         , right)
// #define MUL( left, right) CREATE_OP(OP_MUL , left         , right)
// #define DIV( left, right) CREATE_OP(OP_DIV , left         , right)
// #define DEG( left, right) CREATE_OP(OP_DEG , left         , right)
// #define SQRT(      right) CREATE_OP(OP_SQRT, CREATE_NUM(0), right)
// #define LN(        right) CREATE_OP(OP_LN  , CREATE_NUM(0), right)
// #define SIN(       right) CREATE_OP(OP_SIN , CREATE_NUM(0), right)
// #define COS(       right) CREATE_OP(OP_COS , CREATE_NUM(0), right)

#endif
