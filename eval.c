#include "nl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static NL_Value
eval_int_expression(int value) {
    NL_Value v;
    v.type = INT_VALUE;
    v.u.int_value = value;
    return v;
}

static NL_Value
eval_double_expression(double value) {
    NL_Value v;
    v.type = DOUBLE_VAULE;
    v.u.double_value = value;
    return v;
}

static NL_Value
eval_expression(Expression *exp) {
    NL_Value v;
    switch (exp->type) {
        case INT_EXPRESSION: {
            v = eval_int_expression(exp->u.int_value);
            break;
        }
        case DOUBLE_EXPRESSION: {
            v = eval_double_expression(exp->u.double_value);
            break;
        }
        case ADD_EXPRESSION:
        case SUB_EXPRESSION:
        case MUL_EXPRESSION:
        case DIV_EXPRESSION:
        case MOD_EXPRESSION:
        case EXPRESSION_TYPE_PLUS:
        default: {
            printf("[runtime error] eval expression with unexpected type:%d\n", exp->type);
            exit(1);
        }
    }
    return v;
}

static void
eval_binary_int(ExpressionType operator, int left, int right, NL_Value *result) {
    result->type = INT_VALUE;
    
    switch (operator) {
        case ADD_EXPRESSION: {
            result->u.int_value = left + right;
            break;
        }
        case SUB_EXPRESSION: {
            result->u.int_value = left - right;
            break;
        }
        case MUL_EXPRESSION: {
            result->u.int_value = left * right;
            break;
        }
        case DIV_EXPRESSION: {
            result->u.int_value = left / right;
            break;
        }
        case MOD_EXPRESSION: {
            result->u.int_value = left % right;
            break;
        }
        case INT_EXPRESSION:
        case DOUBLE_EXPRESSION:
        case EXPRESSION_TYPE_PLUS:
        default: {
            printf("[runtime error] eval binary int with unexpected type:%d\n", operator);
            exit(1);
        }
    }
}

static void
eval_binary_double(ExpressionType operator, double left, double right, NL_Value *result) {
    result->type = DOUBLE_VAULE;
    
    switch (operator) {
        case ADD_EXPRESSION: {
            result->u.double_value = left + right;
            break;
        }
        case SUB_EXPRESSION: {
            result->u.double_value = left - right;
            break;
        }
        case MUL_EXPRESSION: {
            result->u.double_value = left * right;
            break;
        }
        case DIV_EXPRESSION: {
            result->u.double_value = left / right;
            break;
        }
        case MOD_EXPRESSION: {
            result->u.double_value = fmod(left, right);
            break;
        }
        case INT_EXPRESSION:
        case DOUBLE_EXPRESSION:
        case EXPRESSION_TYPE_PLUS:
        default: {
            printf("[runtime error] eval binary int with unexpected type:%d\n", operator);
            exit(1);
        }
    }
}

NL_Value
nl_eval_binary_expression(ExpressionType operator, Expression *left, Expression *right) {
    NL_Value left_val;
    NL_Value right_val;
    NL_Value result;
    left_val = eval_expression(left);
    right_val = eval_expression(right);

    if (left_val.type == INT_VALUE && right_val.type == INT_VALUE) {
        eval_binary_int(operator, left_val.u.int_value, right_val.u.int_value, &result);
    } else if (left_val.type == DOUBLE_VAULE && right_val.type == DOUBLE_VAULE) {
        eval_binary_double(operator, left_val.u.double_value, right_val.u.double_value, &result);
    } else if (left_val.type == INT_VALUE && right_val.type == DOUBLE_VAULE) {
        left_val.u.double_value = left_val.u.int_value;
        eval_binary_double(operator, left_val.u.double_value, right_val.u.double_value, &result);
    } else if (left_val.type == DOUBLE_VAULE && right_val.type == INT_VALUE) {
        right_val.u.double_value = right_val.u.int_value;
        eval_binary_double(operator, left_val.u.double_value, right_val.u.double_value, &result);
    } else {
        printf("[runtime error] eval binary expression with unexpected type, left:%d, right:%d\n", left_val.type, right_val.type);
        exit(1);
    }

    return result;
}

NL_Value
nl_eval_expression(Expression *exp) {
    return eval_expression(exp);
}

void
nl_print_value(NL_Value *v) {
    if (v->type == INT_VALUE) {
        printf("--> %d\n", v->u.int_value);
    } else if (v->type == DOUBLE_VAULE) {
        printf("--> %lf\n", v->u.double_value);
    }
}
