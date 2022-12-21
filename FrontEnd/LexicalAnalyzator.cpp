#include "../Config.h"
#include <stdlib.h>
#include <ctype.h>
#include "../Constants/Constants.h"
#include "../Assert.h"
#include "./LexicalAnalyzator.h"

Var* VarCtor(const char* name, double value)
{
    ASSERT(name != nullptr)

    Var* var = (Var*) calloc(1, sizeof(Var));
    ASSERT(var != nullptr)

    var->name = strdup(name);
    ASSERT(var->name != nullptr)

    var->value = value;

    return var;
}

int VarDtor(Var** var)
{
    ASSERT(var != nullptr)

    if (!(*var))
        return 1;

    if ((*var)->name)
    {
        free((void*) (*var)->name);
        (*var)->name  = nullptr;
    }

    (*var)->value = VAR_DEAD_VAL;

    free((void*) *var);
    *var = nullptr;

    return 1;
}

Token* TokenCtor(TokenDataType val_type, Value value, size_t str_number)
{
    Token* token = (Token*) calloc(1, sizeof(Token));
    ASSERT(token != nullptr);

    token->val_type = val_type;
    token->value    = value   ;
    token->str_number = str_number;

    return token;
}

int TokenDtor(Token** token)
{
    ASSERT(token  != nullptr);

    if(!(*token)) return 1;

    if ((*token)->val_type == TOKEN_KEY_TYPE)
        (*token)->value.key_val = NOT_KEY;

    else if ((*token)->val_type == TOKEN_OP_TYPE )
        (*token)->value.op_val = NOT_OP;

    else if ((*token)->val_type == TOKEN_SEP_TYPE)
        (*token)->value.sep_val = NOT_SEP;

    else if ((*token)->val_type == TOKEN_NAME_TYPE)
        VarDtor(&((*token)->value.var));

    else if ((*token)->val_type == TOKEN_NUM_TYPE)
        (*token)->value.num_val = NOT_NUM;

    (*token)->val_type = TOKEN_NULL_TYPE;
    (*token)->str_number = NOT_STRING_NUMBER;

    free(*token);
    *token = nullptr;

    return 1;
}

ProgrammTokens* ProgrammTokensCtor()
{
    ProgrammTokens* programm_tokens = (ProgrammTokens*) calloc(1, sizeof(ProgrammTokens));
    ASSERT(programm_tokens != nullptr);

    programm_tokens->tokens = (Token**) calloc(MAX_PROGRAMM_LENGTH, sizeof(Token*));
    ASSERT(programm_tokens->tokens != nullptr);

    return programm_tokens;
}

#define CUR_TOKEN (*programm_tokens)->tokens[(*programm_tokens)->size]

int ProgrammTokensDtor(ProgrammTokens** programm_tokens)
{
    ASSERT(programm_tokens  != nullptr);

    // fprintf(stdout, "size at the beggining: %ld\n", programm_tokens->size);

    if ((*programm_tokens)->size != 0)
    {
        for (--(*programm_tokens)->size; (*programm_tokens)->size > 0; (*programm_tokens)->size--)
        {
            TokenDtor(&CUR_TOKEN);
            // fprintf(stdout, "size: %ld\n", programm_tokens->size);
        }

        TokenDtor(&(*programm_tokens)->tokens[0]);
        // fprintf(stdout, "\nsize: %ld\n", programm_tokens->size);
    }

    (*programm_tokens)->cursor = 0;

    free((*programm_tokens)->tokens);
    (*programm_tokens)->tokens = nullptr;

    free((*programm_tokens));
    *programm_tokens = nullptr;

    return 1;
}

#undef CUR_TOKEN

#define DEF_OP(op_code, op_name, op_lang_name, op_tree_name, op_kryzh_name) \
    if (code == OP_##op_name) fprintf(stdout, op_lang_name);                \
    else

void OperatorDump(Operators code)
{
    if (code == NOT_OP)
        fprintf(stdout, "NOT OPERATOR");

    else
        #include "../Dictionary/Operators.h"

    return;
}

#undef DEF_OP

#define DEF_KEY(key_code, key_name, key_lang_name, key_tree_name, key_kryzh_name) \
    if (code == KEY_##key_name) fprintf(stdout, key_lang_name);                   \
    else

void KeyWordDump(KeyWords code)
{
    if (code == NOT_KEY)
        fprintf(stdout, "NOT KEYWORD");

    else
        #include "../Dictionary/KeyWords.h"

    return;
}

#undef DEF_KEY

#define DEF_SEP(sep_code, sep_name, sep_lang_name, sep_tree_name, sep_kryzh_name) \
    if (code == SEP_##sep_name) fprintf(stdout, #sep_name);                       \
    else

void SeparatorDump(Separators code)
{
    if (code == NOT_SEP)
        fprintf(stdout, "NOT SEPARATOR");

    else
        #include "../Dictionary/Separators.h"

    return;
}

#undef DEF_SEP

#define K_TOKEN programm_tokens->tokens[k]

void ProgrammTokensDump(ProgrammTokens* programm_tokens)
{
    ASSERT(programm_tokens != nullptr)

    fprintf(stdout, KBLU "\n%s:\n" KNRM "       size: %ld cursor:     %ld\n", __PRETTY_FUNCTION__, programm_tokens->size, programm_tokens->cursor);

    for (size_t k = 0; k < programm_tokens->size; k++)
    {
        fprintf(stdout, " [" KMAG "%03ld" KNRM "] token on %ld str is ", k, K_TOKEN->str_number);

        if (K_TOKEN->val_type == TOKEN_NULL_TYPE)
            fprintf(stdout, KYEL "NULL" KNRM);

        else if (K_TOKEN->val_type == TOKEN_KEY_TYPE)
        {
            fprintf(stdout, "KEYWORD   %d " KYEL, K_TOKEN->value.key_val);
            KeyWordDump(K_TOKEN->value.key_val);
            fprintf(stdout, KNRM);
        }

        else if (K_TOKEN->val_type == TOKEN_OP_TYPE)
        {
            fprintf(stdout, "OPERATOR  %d " KCYN, K_TOKEN->value.op_val);
            OperatorDump(K_TOKEN->value.op_val);
            fprintf(stdout, KNRM);
        }

        else if (K_TOKEN->val_type == TOKEN_SEP_TYPE)
        {
            fprintf(stdout, "SEPARATOR %d " KGRN, K_TOKEN->value.sep_val);
            SeparatorDump(K_TOKEN->value.sep_val);
            fprintf(stdout, KNRM);
        }

        else if (K_TOKEN->val_type == TOKEN_NAME_TYPE)
        {
            fprintf(stdout, "NAME        " KMAG "%s" KNRM " = " KRED "%lg" KNRM " %s", K_TOKEN->value.var->name, K_TOKEN->value.var->value, (K_TOKEN->value.var->value == VAR_DEAD_VAL ? KBLU "(x.x.DEAD.x.x)" KNRM : ""));
        }

        else if (K_TOKEN->val_type == TOKEN_NUM_TYPE)
        {
            fprintf(stdout, "NUMBER      " KRED "%lg" KNRM, K_TOKEN->value.num_val);
        }

        else if (K_TOKEN->val_type == TOKEN_END_TYPE)
        {
            fprintf(stdout, "END SYMBOL  " KYEL "EOF" KNRM);
        }

        fprintf(stdout, "\n");
    }

    fprintf(stdout, "\n");
}

#undef K_TOKEN

int SkipSpaces(char* str, size_t* i_letter, size_t* str_n)
{
    ASSERT(str != nullptr);

    size_t i_letter_old = *i_letter;

    char* str_ptr = str;

    // fprintf(stderr, "до: %s\n", *str);

    while (isspace(*str_ptr))
    {
        if (*str_ptr == '\n') (*str_n)++;

        // fprintf(stdout, "skip\n");
        (*i_letter)++;
        str_ptr++;
    }

    // fprintf(stderr, "после: %s\n", *str);

    if (*i_letter > i_letter_old) return 1;

    return 0;
}

double CheckForNum(char* programm_code, size_t* i_letter)
{
    ASSERT(i_letter != nullptr)

    if (!(*programm_code)) return NOT_NUM;

    char* programm_code_ptr = programm_code;
    double num_val = 0;

    size_t i_letter_old = *i_letter;

    while (isdigit(*programm_code_ptr))
    {
        num_val = num_val * 10 + (*programm_code_ptr - '0');
        programm_code_ptr++;
        (*i_letter)++;
    }

    if (*i_letter > i_letter_old)
        return num_val;

    return NOT_NUM;
}

char* CheckForVar(char* programm_code, size_t* i_letter)
{
    ASSERT(i_letter      != nullptr)
    ASSERT(programm_code != nullptr)

    if (!(*programm_code)) return nullptr;

    char* var_name = strtok(programm_code, " \n"); //0123456789+-=/^(){}[]&%@!$_\\|~`'\"<>?.,#№;");

    // fprintf(stdout, "%s %d\n", lexema_name, key_val);

    int shift = strlen(var_name) + 1;
    *i_letter += shift;
    // programm_code_ptr += shift;

    return var_name;
}

#define DEF_OP(op_code, op_name, op_lang_name, op_tree_name, op_kryzh_name)                 \
                                                                                            \
    if (OP_##op_name != NOT_OP &&                                                           \
       (!strcasecmp(lexema_name, op_lang_name) || !strcasecmp(lexema_name, op_kryzh_name))) \
        op_val = OP_##op_name;                                                              \
                                                                                            \
    else

Operators CheckForOperator(char* programm_code, size_t* i_letter)
{
    ASSERT(i_letter      != nullptr)
    ASSERT(programm_code != nullptr)

    if (!(*programm_code)) return NOT_OP;

    // char* programm_code_ptr = programm_code;

    Operators op_val = NOT_OP;

    // fprintf(stdout, "1: %s\n", programm_code_ptr);
    // fprintf(stdout, "до: %ld\n", *i_letter);

    char* lexema_name = strtok(programm_code, " \n"); //0123456789(){}[]&%@!$_\\|~`'\"<>?.,#№");

    #include "../Dictionary/Operators.h"

    op_val == NOT_OP;

    // fprintf(stdout, "%s %d\n", lexema_name, op_val);

    if (op_val != NOT_OP)
    {
        int shift = strlen(lexema_name) + 1;
        *i_letter += shift;
        // programm_code_ptr += shift;
    }

    // fprintf(stdout, "lexema: %s\n", lexema_name);
    // fprintf(stdout, "2: %s\n", programm_code_ptr);

    return op_val;
}

#undef DEF_OP

#define DEF_KEY(key_code, key_name, key_lang_name, key_tree_name, key_kryzh_name)             \
                                                                                              \
    if (KEY_##key_name != NOT_KEY &&                                                          \
       (!strcasecmp(lexema_name, key_lang_name) || !strcasecmp(lexema_name, key_kryzh_name))) \
        key_val = KEY_##key_name;                                                             \
                                                                                              \
    else

KeyWords CheckForKeyWord(char* programm_code, size_t* i_letter)
{
    ASSERT(i_letter      != nullptr)
    ASSERT(programm_code != nullptr)

    if (!(*programm_code)) return NOT_KEY;

    KeyWords key_val = NOT_KEY;

    char* lexema_name = strtok(programm_code, " \n"); //0123456789+-=/^(){}[]&%@!$_\\|~`'\"<>?.,#№;");

    #include "../Dictionary/KeyWords.h"

    key_val == NOT_KEY;

    // fprintf(stdout, "%s %d\n", lexema_name, key_val);

    if (key_val != NOT_KEY)
    {
        int shift = strlen(lexema_name) + 1;
        *i_letter += shift;
        // programm_code_ptr += shift;
    }

    return key_val;
}

#undef DEF_KEY

#define DEF_SEP(sep_code, sep_name, sep_lang_name, sep_tree_name, sep_kryzh_name)             \
                                                                                              \
    if (SEP_##sep_name != NOT_SEP &&                                                          \
       (!strcasecmp(lexema_name, sep_lang_name) || !strcasecmp(lexema_name, sep_kryzh_name))) \
        sep_val = SEP_##sep_name;                                                             \
                                                                                              \
    else

Separators CheckForSeparator(char* programm_code, size_t* i_letter)
{
    ASSERT(i_letter      != nullptr)
    ASSERT(programm_code != nullptr)

    if (!(*programm_code)) return NOT_SEP;

    Separators sep_val = NOT_SEP;

    // char* next_str = strpbrk(programm_code, " \0\n0123456789+-=/^&%@!$_\\|~`'\"<>?#№abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    // if (next_str) fprintf(stderr, "tut: %c\n", *next_str);

    char* lexema_name = strtok(programm_code, " \n"); //0123456789+-=/^&%@!$_\\|~`'\"<>?#№abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

    // fprintf(stdout, ".%s.\n", lexema_name);

    #include "../Dictionary/Separators.h"

    sep_val == NOT_SEP;

    // fprintf(stdout, "%s %d\n", lexema_name, sep_val);

    // fprintf(stdout, "%d\n", sep_val);

    if (sep_val != NOT_SEP)
    {
        // fprintf(stderr, "HERE\n");
        int shift = strlen(lexema_name) + 1;
        *i_letter += shift;
    }

    return sep_val;
}

#undef DEF_SEP

#define DEF_COMM(comm_beggining_str)                                           \
{                                                                              \
    int comment_begginig_length = strlen(comm_beggining_str);                  \
                                                                               \
    char* comment_beggining = strndup(programm_code, comment_begginig_length); \
                                                                               \
    if (!strcmp(comm_beggining_str, comment_beggining))                        \
    {                                                                          \
        char* comment = strtok(programm_code, "\n");                           \
                                                                               \
        int shift = strlen(comment) + 1;                                       \
        *i_letter += shift;                                                    \
    }                                                                          \
                                                                               \
    free((void*) comment_beggining);                                           \
}

int CheckForComment(char* programm_code, size_t* i_letter)
{
    ASSERT(programm_code != nullptr)

    #include "../Dictionary/CommentBeginners.h"

    return 1;
}

#undef DEF_COMM

int CheckForEnd(char* programm_code, size_t* i_letter)
{
    ASSERT(programm_code != nullptr)

    if (!(*programm_code))
    {
        (*i_letter)++;
        return 1;
    }

    return 0;
}

#define CUR_TOKEN_NEXT programm_tokens->tokens[programm_tokens->size++]

#define DEF_COMM(comm_beggining_str) !strcmp(comm_beggining_str, strndup(&programm_code_copy[i_letter], strlen(comm_beggining_str))) ||

ProgrammTokens* AnalyzeProgrammCode(ProgrammTokens* programm_tokens, const char* programm_code)
{
    ASSERT(programm_tokens != nullptr);
    ASSERT(programm_code   != nullptr);

    char* programm_code_copy = strdup(programm_code);
    ASSERT(programm_code_copy != nullptr)

    char* programm_code_copy_old = programm_code_copy;
    size_t max_programm_length = strlen(programm_code_copy);

    size_t i_letter = 0;
    size_t str_n = 1;
    Value token_value = {};

    while (programm_code_copy[i_letter] != '\0' && i_letter < max_programm_length)
    {
//         char* comm1 = nullptr;
//         char* comm2 = nullptr;
//         char* comm3 = nullptr;
//
//         while (!strcmp("P.S",   (comm1 = strndup(&programm_code_copy[i_letter], strlen("P.S"  )))) ||
//                !strcmp("P.P.S", (comm2 = strndup(&programm_code_copy[i_letter], strlen("P.P.S")))) ||
//                !strcmp("\\\\",  (comm3 = strndup(&programm_code_copy[i_letter], strlen("\\\\" )))) ||
//                isspace(programm_code_copy[i_letter]))
//         {
            SkipSpaces(&programm_code_copy[i_letter], &i_letter, &str_n);
            CheckForComment(&programm_code_copy[i_letter], &i_letter);
            SkipSpaces(&programm_code_copy[i_letter], &i_letter, &str_n);

        //     if (comm1) free((void*) comm1);
        //     if (comm2) free((void*) comm2);
        //     if (comm3) free((void*) comm3);
        // }

        token_value.num_val = CheckForNum(&programm_code_copy[i_letter], &i_letter);

        if (token_value.num_val != NOT_NUM)
        {
            CUR_TOKEN_NEXT = TokenCtor(TOKEN_NUM_TYPE, token_value, str_n);
            continue;
        }

        token_value.op_val = CheckForOperator(&programm_code_copy[i_letter], &i_letter);

        if (token_value.op_val != NOT_OP)
        {
            CUR_TOKEN_NEXT = TokenCtor(TOKEN_OP_TYPE, token_value, str_n);
            continue;
        }

        token_value.key_val = CheckForKeyWord(&programm_code_copy[i_letter], &i_letter);

        if (token_value.key_val != NOT_KEY)
        {
            CUR_TOKEN_NEXT = TokenCtor(TOKEN_KEY_TYPE, token_value, str_n);
            continue;
        }

        token_value.sep_val = CheckForSeparator(&programm_code_copy[i_letter], &i_letter);

        if (token_value.sep_val != NOT_SEP)
        {
            CUR_TOKEN_NEXT = TokenCtor(TOKEN_SEP_TYPE, token_value, str_n);
            continue;
        }

        char* var_name = CheckForVar(&programm_code_copy[i_letter], &i_letter);

        if (var_name != nullptr)
        {
            token_value.var = VarCtor(var_name, VAR_DEAD_VAL);
            CUR_TOKEN_NEXT = TokenCtor(TOKEN_NAME_TYPE, token_value, str_n);
            continue;
        }

        break;
    }

    if (CheckForEnd(&programm_code_copy[i_letter], &i_letter))
    {
        CUR_TOKEN_NEXT = TokenCtor(TOKEN_END_TYPE, token_value, str_n);
    }

    programm_tokens->tokens = (Token**) realloc(programm_tokens->tokens, programm_tokens->size * sizeof(Token*));
    ASSERT(programm_tokens->tokens != nullptr)

    // if (programm_tokens->size != 0) programm_tokens->cursor = 1;

    fprintf(stdout, "size: %ld\n", programm_tokens->size);

    free((void*) programm_code_copy_old);

    return programm_tokens;
}

#undef DEF_COMM

#undef CUR_TOKEN_NEXT
