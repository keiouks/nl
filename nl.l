%{
#include <stdio.h>
#include "nl.h"
#include "y.tab.h"

int yywrap(void) {
    return 1;
}
%}
%%
"+" return ADD;
"-" return SUB;
"*" return MUL;
"/" return DIV;
"%" return MOD;
"(" return LP;
")" return RP;
"\n" return CR;
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
. {
    printf("lexical error with unexpected charactor %s\n", yytext);
    exit(1);
}
%%
