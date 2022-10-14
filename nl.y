%{
#include <stdio.h>
#include <stdlib.h>
#include "nl.h"
int yylex();
int yyerror(char const *str);
%}
%union {
    Expression *expression;
    char *identifier;
}
%token SEMICOLON ADD SUB MUL DIV LP RP MOD ASSIGN PRINT
%token <identifier> IDENTIFIER
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
        nl_eval_expression($1);
    }
    | IDENTIFIER ASSIGN expression SEMICOLON
    {
        King *king = nl_get_current_king();
        nl_execute_assign_statement(king, $1, $3);
    }
    | PRINT LP expression RP SEMICOLON
    {
        NL_Value v = nl_eval_expression($3);
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
    | IDENTIFIER
    {
        $$ = nl_create_variable_expression($1);
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
