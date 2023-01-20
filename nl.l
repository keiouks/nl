%{
#include <stdio.h>
#include "nl.h"
#include "y.tab.h"

int yywrap(void) {
    return 1;
}

void increace_line_number(void) {
    nl_get_current_king()->line_number++;
}
%}
%%
"let" return LET;
"print" return PRINT;
"function" return FUNCTION;
"return" return RETURN;
";" return SEMICOLON;
"+" return ADD;
"-" return SUB;
"*" return MUL;
"/" return DIV;
"%" return MOD;
"(" return LP;
")" return RP;
"{" return LC;
"}" return RC;
"=" return ASSIGN;
[A-Za-z_][A-Za-z_0-9]* {
    yylval.identifier = nl_create_identifier(yytext);
    return IDENTIFIER;
}
([1-9][0-9]*)|"0" {
    Expression *expression = nl_alloc_expression(INT_EXPRESSION);
    sscanf(yytext, "%d", &expression->u.int_value);
    yylval.expression = expression;
    return INT_LITERAL;
}
[0-9]+\.[0-9]+ {
    Expression *expression = nl_alloc_expression(DOUBLE_EXPRESSION);
    sscanf(yytext, "%lf", &expression->u.double_value);
    yylval.expression = expression;
    return DOUBLE_LITERAL;
}
[ \t] ;
"\n" {increace_line_number();};
. {
    printf("lexical error with unexpected charactor %s, in line [%d]\n", yytext, nl_get_current_king()->line_number);
    exit(1);
}
%%
