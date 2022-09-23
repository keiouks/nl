#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nl.h"

Expression *
nl_alloc_expression(ExpressionType type) {
    Expression *exp;
    exp = malloc(sizeof(Expression));
    exp->type = type;

    return exp;
}

static Expression
convert_value_to_expression(NL_Value *v) {
    Expression exp;

    if (v->type == INT_VALUE) {
        exp.type = INT_EXPRESSION;
        exp.u.int_value = v->u.int_value;
    } else if (v->type == DOUBLE_VAULE) {
        exp.type = DOUBLE_EXPRESSION;
        exp.u.double_value = v->u.double_value;
    } else {
        printf("[runtime error] convert value with unexpected type:%d\n", v->type);
        exit(1);
    }
    return exp;
}

Expression *
nl_create_binary_expression(ExpressionType type, Expression *left, Expression *right) {
    NL_Value v;
    v = nl_eval_binary_expression(type, left, right);

    *left = convert_value_to_expression(&v);
    return left;
}

Expression *
nl_create_minus_expression(Expression *exp) {
    if (exp->type == INT_EXPRESSION) {
        exp->u.int_value = -exp->u.int_value;
    } else if (exp->type == DOUBLE_EXPRESSION) {
        exp->u.double_value = -exp->u.double_value;
    }
    return exp;
}
