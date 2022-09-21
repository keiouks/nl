#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nl.h"

IdentifierList *
nl_create_identifier(char *identifier) {
    IdentifierList *i_node;
    int len = strlen(identifier);
    char *name = malloc(len + 1);
    strcpy(name, identifier);
    name[len] = '\0';
    i_node = malloc(sizeof(IdentifierList));

    i_node->name = name;
    i_node->next = NULL;

    return i_node;
}

IdentifierList *
nl_chain_identifier(IdentifierList *list, char *identifier) {
    IdentifierList *pos;
    for (pos = list; pos && pos->next; pos = pos->next)
        ;
        
    pos->next = nl_create_identifier(identifier);
    return list;
}

void
nl_print_identifier_list(IdentifierList *list) {
    IdentifierList *pos;
    for (pos = list; pos; pos = pos->next) {
        printf("-->%s", pos->name);
    }
    printf(";\n");
}

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
nl_create_minus_expression(Expression *operand) {
    if (operand->type == INT_EXPRESSION || operand->type == DOUBLE_EXPRESSION) {
        NL_Value v;
        v = nl_eval_minus_expression(operand);
        *operand = convert_value_to_expression(&v);
        return operand;
    } else {
        Expression *exp;
        exp = nl_alloc_expression(MINUS_EXPRESSION);
        exp->u.minus_expression = operand;
        return exp;
    }
}

Expression *
nl_create_binary_expression(ExpressionType type, Expression *left, Expression *right) {
    if ((left->type == INT_EXPRESSION || left->type == DOUBLE_EXPRESSION)
        && (right->type == INT_EXPRESSION || right->type == DOUBLE_EXPRESSION)) {
        NL_Value v;
        v = nl_eval_binary_expression(type, left, right);

        *left = convert_value_to_expression(&v);
        return left;
    } else {
        Expression *exp;
        exp = nl_alloc_expression(type);
        exp->u.binary_expression.left = left;
        exp->u.binary_expression.right = right;
        return exp;
    }
}
