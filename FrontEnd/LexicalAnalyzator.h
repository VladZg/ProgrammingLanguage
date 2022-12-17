#ifndef LEXICAL_ANALYZATOR_H
#define LEXICAL_ANALYZATOR_H

#include "../Config.h"
#include <./stdlib.h>
#include "../Constants.h"
// #include "./FrontEndConstants.h"

typedef double Var_val;

struct Var
{
    char*   name ;
    Var_val value;
};

union TokenValue
{
    KeyWords   key_val;
    Operators  op_val ;
    Separators sep_val;
    Var        var    ;
    double     num_val;
};

struct Token
{
    TokenDataType val_type;
    TokenValue    value   ;
};

struct ProgrammTokens
{
    Token** tokens;
    size_t  size  ;
    size_t  cursor;
};

Var VarCtor(Var* var, const char* name, double value);
int VarDtor(Var* var);

Token* TokenCtor(TokenDataType val_type, TokenValue value);
int    TokenDtor(Token** token);

ProgrammTokens* ProgrammTokensCtor();
int  ProgrammTokensDtor(ProgrammTokens** programm_tokens);

void OperatorDump (Operators  code);
void KeyWordDump  (KeyWords   code);
void SeparatorDump(Separators code);
void ProgrammTokensDump(ProgrammTokens*  programm_tokens);

int SkipSpaces(char* str, size_t* i_letter);

double     CheckForNum      (char* programm_code, size_t* i_letter);
char*      CheckForVar      (char* programm_code, size_t* i_letter);
Operators  CheckForOperator (char* programm_code, size_t* i_letter);
KeyWords   CheckForKeyWord  (char* programm_code, size_t* i_letter);
Separators CheckForSeparator(char* programm_code, size_t* i_letter);
int        CheckForEnd      (char* programm_code, size_t* i_letter);

ProgrammTokens* AnalyzeProgrammCode(ProgrammTokens* programm_tokens, const char* programm_code);

#endif
