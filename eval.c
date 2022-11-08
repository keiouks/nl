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
    v.type = DOUBLE_VALUE;
    v.u.double_value = value;
    return v;
}

static NL_Value
eval_variable_expression(Expression *exp) {
    King *king = nl_get_current_king();
    Variable *var = nl_search_global_variable(king, exp->u.identifier);
    if (var != NULL) {
        return var->value;
    } else {
        printf("[runtime error] This variable[%s] has not been declared.\n", exp->u.identifier);
        exit(1);
    }
}

static NL_Value eval_expression(Expression *exp);

static NL_Value
eval_minus_expression(Expression *exp) {
    NL_Value result = eval_expression(exp->u.minus_expression);
    if (result.type == INT_VALUE) {
        result.u.int_value = -result.u.int_value;
    } else if (result.type == DOUBLE_VALUE) {
        result.u.double_value = -result.u.double_value;
    } else {
        printf("[runtime error] eval minus expression with unexpected value type: %d.\n", result.type);
        exit(1);
    }
    return result;
}

static NL_Value
eval_function_call_expression(Expression *exp) {
    NL_Value result;
    StatementValue sValue;
    char *fun_name = exp->u.identifier;
    FunctionDefinition *fun_def = nl_search_function(fun_name);
    if (!fun_def) {
        printf("[runtime error] eval function call expression, function [%s] is not define.\n", fun_name);
        exit(1);
    }
    sValue = nl_execute_statement_list(fun_def->block->statement_list);
    if (sValue.type == RETURN_STATEMENT_VALUE) {
        result = sValue.return_value;
    } else {
        result.type = NULL_VALUE;
    }
    return result;
}

static NL_Value
eval_expression(Expression *exp) {
    NL_Value v;
    if (!exp) {
        v.type = NULL_VALUE;
        return v;
    }
    switch (exp->type) {
        case INT_EXPRESSION: {
            v = eval_int_expression(exp->u.int_value);
            break;
        }
        case DOUBLE_EXPRESSION: {
            v = eval_double_expression(exp->u.double_value);
            break;
        }
        case VARIABLE_EXPRESSION: {
            v = eval_variable_expression(exp);
            break;
        }
        case MINUS_EXPRESSION: {
            v = eval_minus_expression(exp);
            break;
        }
        case ADD_EXPRESSION:
        case SUB_EXPRESSION:
        case MUL_EXPRESSION:
        case DIV_EXPRESSION:
        case MOD_EXPRESSION: {
            v = nl_eval_binary_expression(exp->type, exp->u.binary_expression.left, exp->u.binary_expression.right);
            break;
        }
        case FUNCTION_CALL_EXPRESSION: {
            v = eval_function_call_expression(exp);
            break;
        }
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
        case VARIABLE_EXPRESSION:
        case MINUS_EXPRESSION:
        case FUNCTION_CALL_EXPRESSION:
        case EXPRESSION_TYPE_PLUS:
        default: {
            printf("[runtime error] eval binary int with unexpected type:%d\n", operator);
            exit(1);
        }
    }
}

static void
eval_binary_double(ExpressionType operator, double left, double right, NL_Value *result) {
    result->type = DOUBLE_VALUE;
    
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
        case VARIABLE_EXPRESSION:
        case MINUS_EXPRESSION:
        case FUNCTION_CALL_EXPRESSION:
        case EXPRESSION_TYPE_PLUS:
        default: {
            printf("[runtime error] eval binary int with unexpected type:%d\n", operator);
            exit(1);
        }
    }
}

NL_Value
nl_eval_binary_expression(ExpressionType type, Expression *left, Expression *right) {
    NL_Value left_val;
    NL_Value right_val;
    NL_Value result;
    left_val = eval_expression(left);
    right_val = eval_expression(right);

    if (left_val.type == INT_VALUE && right_val.type == INT_VALUE) {
        eval_binary_int(type, left_val.u.int_value, right_val.u.int_value, &result);
    } else if (left_val.type == DOUBLE_VALUE && right_val.type == DOUBLE_VALUE) {
        eval_binary_double(type, left_val.u.double_value, right_val.u.double_value, &result);
    } else if (left_val.type == INT_VALUE && right_val.type == DOUBLE_VALUE) {
        left_val.u.double_value = left_val.u.int_value;
        eval_binary_double(type, left_val.u.double_value, right_val.u.double_value, &result);
    } else if (left_val.type == DOUBLE_VALUE && right_val.type == INT_VALUE) {
        right_val.u.double_value = right_val.u.int_value;
        eval_binary_double(type, left_val.u.double_value, right_val.u.double_value, &result);
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
    } else if (v->type == DOUBLE_VALUE) {
        printf("--> %lf\n", v->u.double_value);
    }
}


