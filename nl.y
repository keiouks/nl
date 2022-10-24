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
    Statement *statement;
    StatementList *statement_list;
}
%token SEMICOLON ADD SUB MUL DIV LP RP MOD ASSIGN PRINT
%token <identifier> IDENTIFIER
%token <expression> INT_LITERAL DOUBLE_LITERAL
%type <expression> primary_expression mult_expression add_expression expression
%type <statement> statement expression_statement assign_statement print_statement
%type <statement_list> statement_list
%%
statement_list
    : statement
    {
        King *king = nl_get_current_king();
        king->statement_list = nl_add_to_statement_list(king->statement_list, $1);
        $$ = king->statement_list;
    }
    | statement_list statement
    {
        King *king = nl_get_current_king();
        king->statement_list = nl_add_to_statement_list($1, $2);
        $$ = king->statement_list;
    }
    ;
statement
    : expression_statement
    | assign_statement
    | print_statement
    ;
expression_statement
    : expression SEMICOLON
    {
        $$ = nl_create_expression_statement($1);
    }
    ;
assign_statement
    : IDENTIFIER ASSIGN expression SEMICOLON
    {
        $$ = nl_create_assign_statement($1, $3);
    }
    ;
print_statement
    : PRINT LP expression RP SEMICOLON
    {
        $$ = nl_create_print_statement($3);
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
