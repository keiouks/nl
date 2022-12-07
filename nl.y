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
    Block *block;
}
%token SEMICOLON ADD SUB MUL DIV LP RP MOD ASSIGN PRINT FUNCTION LC RC RETURN LET
%token <identifier> IDENTIFIER
%token <expression> INT_LITERAL DOUBLE_LITERAL
%type <expression> primary_expression mult_expression add_expression expression
%type <statement> statement expression_statement assign_statement print_statement function_definition_statement return_statement declaration_statement
%type <statement_list> statement_list
%type <block> block
%%
page
    :
    | statement_list
    {
        King *king = nl_get_current_king();
        king->statement_list = $1;
    }
    ;
statement_list
    : statement
    {
        $$ = nl_create_statement_list($1);
    }
    | statement_list statement
    {
        $$ = nl_add_to_statement_list($1, $2);
    }
    ;
statement
    : expression_statement
    | assign_statement
    | print_statement
    | function_definition_statement
    | return_statement
    | declaration_statement
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
declaration_statement
    : LET IDENTIFIER ASSIGN expression SEMICOLON
    {
        $$ = nl_create_declaration_statement($2, $4);
    }
    ;
print_statement
    : PRINT LP expression RP SEMICOLON
    {
        $$ = nl_create_print_statement($3);
    }
    ;
function_definition_statement
    : FUNCTION IDENTIFIER LP RP block 
    {
        $$ = nl_create_function_definition_statement($2, $5);
    }
    ;
return_statement
    : RETURN expression SEMICOLON
    {
        $$ = nl_create_return_statement($2);
    }
    | RETURN SEMICOLON
    {
        $$ = nl_create_return_statement(NULL);
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
    | IDENTIFIER LP RP
    {
        $$ = nl_create_function_call_expression($1);
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
block
    : LC statement_list RC
    {
        $$ = nl_create_block($2);
    }
    | LC RC
    {
        $$ = nl_create_block(NULL);
    }
    ;
%%
int yyerror(char const *str) {
    extern char *yytext;
    fprintf(stderr, "parse error near %s\n", yytext);
    return 0;
}
