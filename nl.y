%{
#include <stdio.h>
#include <stdlib.h>
#include "nl.h"
int yylex();
int yyerror(char const *str);
%}
%union {
    Expression *expression;
}
%token SEMICOLON ADD SUB MUL DIV LP RP MOD
%token <expression> INT_LITERAL DOUBLE_LITERAL
%type <expression> primary_expression mult_expression add_expression expression
%%
statement_list
    : statement
    | statement_list statement
    ;
statement
    : expression SEMICOLON
    {
        NL_Value v = nl_eval_expression($1);
        nl_print_value(&v);
    }
    ;
expression
    : add_expression
    ;
add_expression
    : mult_expression
    | add_expression ADD mult_expression
    {
        $$ = nl_create_binary_expression(ADD_EXPRESSION, $1, $3);
    }
    | add_expression SUB mult_expression
    {
        $$ = nl_create_binary_expression(SUB_EXPRESSION, $1, $3);
    }
    ;
mult_expression
    : primary_expression
    | mult_expression MUL primary_expression
    {
        $$ = nl_create_binary_expression(MUL_EXPRESSION, $1, $3);
    }
    | mult_expression DIV primary_expression
    {
        $$ = nl_create_binary_expression(DIV_EXPRESSION, $1, $3);
    }
    | mult_expression MOD primary_expression
    {
        $$ = nl_create_binary_expression(MOD_EXPRESSION, $1, $3);
    }
    ;
primary_expression
    : SUB primary_expression
    {
        $$ = nl_create_minus_expression($2);
    }
    | LP expression RP
    {
        $$ = $2;
    }
    | INT_LITERAL
    | DOUBLE_LITERAL
    ;
%%
int yyerror(char const *str) {
    extern char *yytext;
    fprintf(stderr, "parse error near %s\n", yytext);
    return 0;
}
