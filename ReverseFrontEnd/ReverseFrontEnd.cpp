#include "../Config.h"
#include "../Constants/Constants.h"
#include "../Tree/Tree.h"

int Format_prog_shift = 0;

#define DEF_OP(op_code, op_name, op_lang_name, op_tree_name)

#define DEF_KEY(key_code, key_name, key_lang_name, key_tree_name)

#define DEF_SEP(sep_code, sep_name, sep_lang_name, sep_tree_name)

#define TYPE node->val_type

#define IS_OP    TYPE == NODE_OP_TYPE
#define IS_SEP   TYPE == NODE_SEP_TYPE
#define IS_KEY   TYPE == NODE_KEY_TYPE
#define IS_NUM   TYPE == NODE_NUM_TYPE
#define IS_NAME  TYPE == NODE_NAME_TYPE
#define IS_VAR   TYPE == NODE_OP_TYPE
#define IS_PARAM TYPE == NODE_SEP_TYPE
#define IS_ST    TYPE == NODE_KEY_TYPE
#define IS_CALL  TYPE == NODE_NUM_TYPE
#define IS_BLOCK TYPE == NODE_NAME_TYPE

int TranslitProgrammTreeToProgramm(FILE* stream, const Node* node, TranslitModes mode)
{
    if (!node) return 1;

    // FprintfNSymb(stream, ' ', Format_prog_shift);

    if (mode == USUAL_TRANSLIT_MODE)
    {
        if (IS_NUM)
        {
            fprintf(stderr, "%lg ", node->value->num_val);
        }

        else if (IS_VAR)
        {
            fprintf(stderr, "%s", node->value->var->name);
        }

        else if (IS_PARAM)
        {
            TranslitProgrammTreeToProgramm(stream, node->left, mode);

            if (node->right)
            {
                fprintf(stderr, " %s ", SEP_COMMA_STR);
                TranslitProgrammTreeToProgramm(stream, node->right, mode);
            }
        }

        else if (IS_CALL)
        {
            fprintf(stderr, "%s ( ", node->left->value->var->name);
            TranslitProgrammTreeToProgramm(stream, node->left->left, mode);
            fprintf(stderr, " ) ");
        }

        else if (IS_NAME)
        {
            fprintf(stream, "%s ", node->value->var->name);
        }

        else if (IS_FUNC)
        {
            fprintf(stream, "%s ", node->value->var->name);
        }

        // TranslitProgrammTreeToProgramm(node->left , mode);
        // TranslitProgrammTreeToProgramm(node->right, mode);
    }

    // else (mode == USUAL_TRANSLIT_MODE)
    // {
    //     TranslitProgrammTreeToProgramm(node->left , mode);
    //     TranslitProgrammTreeToProgramm(node->right, mode);
    // }

    return 1;
}

#undef DEF_OP
#undef DEF_KEY
#undef DEF_SEP

#undef TYPE

#undef IS_OP
#undef IS_SEP
#undef IS_KEY
#undef IS_NUM
#undef IS_NAME
#undef IS_VAR
#undef IS_PARAM
#undef IS_ST
#undef IS_CALL
#undef IS_BLOCK
