#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nl.h"

Expression *
nl_alloc_expression(ExpressionType type) {
    Expression *exp;
    exp = malloc(sizeof(Expression));
    exp->type = type;
    exp->line_number = nl_get_current_king()->line_number;
    return exp;
}

Expression *
nl_create_variable_expression(char *identifier) {
    Expression *exp;
    exp = nl_alloc_expression(VARIABLE_EXPRESSION);
    exp->u.identifier = identifier;
    return exp;
}

Expression *
nl_create_binary_expression(ExpressionType type, Expression *left, Expression *right) {
    Expression *exp;
    exp = nl_alloc_expression(type);
    exp->u.binary_expression.left = left;
    exp->u.binary_expression.right = right;
    return exp;
    
}

Expression *
nl_create_minus_expression(Expression *exp) {
    Expression *result;
    if (exp->type == INT_EXPRESSION) {
        exp->u.int_value = -exp->u.int_value;
        result = exp;
    } else if (exp->type == DOUBLE_EXPRESSION) {
        exp->u.double_value = -exp->u.double_value;
        result = exp;
    } else {
        result = nl_alloc_expression(MINUS_EXPRESSION);
        result->u.minus_expression = exp;
    }
    return result;
}

Expression *
nl_create_function_call_expression(char *identifier) {
    Expression *result = nl_alloc_expression(FUNCTION_CALL_EXPRESSION);
    result->u.identifier = identifier;
    return result;
}

char *
nl_create_identifier(char *str) {
    char *new_str;
    new_str = malloc(strlen(str) + 1);
    strcpy(new_str, str);
    return new_str;
}

Statement *
malloc_statement(StatementType type) {
    Statement *statement = malloc(sizeof(Statement));
    statement->type = type;
    statement->line_number = nl_get_current_king()->line_number;
    return statement;
}

Statement *
nl_create_expression_statement(Expression *expression) {
    Statement *statement = malloc_statement(EXPRESSION_STATEMENT);
    statement->u.expression = expression;
    return statement;
}

Statement *
nl_create_assign_statement(char *identifier, Expression *expression) {
    Statement *statement = malloc_statement(ASSIGN_STATEMENT);
    statement->u.assign.identifier = identifier;
    statement->u.assign.expression = expression;
    return statement;
}

Statement *
nl_create_declaration_statement(char *identifier, Expression *expression) {
    Statement *statement = malloc_statement(DECLARATION_STATEMENT);
    statement->u.declaration.identifier = identifier;
    statement->u.declaration.expression = expression;
    return statement;
}

Statement *
nl_create_print_statement(Expression *expression) {
    Statement *statement = malloc_statement(PRINT_STATEMENT);
    statement->u.expression = expression;
    return statement;
}

Statement *
nl_create_function_definition_statement(char *identifier, Block *block) {
    Statement *statement = malloc_statement(FUNCTION_DEFINITION_STATEMENT);
    statement->u.functionDefinition.identifier = identifier;
    statement->u.functionDefinition.block = block;
    return statement;
}

Statement *
nl_create_return_statement(Expression *expression) {
    Statement *statement = malloc_statement(RETURN_STATEMENT);
    statement->u.expression = expression;
    return statement;
}

StatementList *
nl_create_statement_list(Statement *statement) {
    StatementList *statement_list = malloc(sizeof(StatementList));
    statement_list->statement = statement;
    statement_list->next = NULL;
    return statement_list;
}

StatementList *
nl_add_to_statement_list(StatementList *list, Statement *statement) {
    StatementList *now;
    if (list == NULL) {
        return nl_create_statement_list(statement);
    }
    for (now = list; now->next; now = now->next)
        ;
    
    now->next = nl_create_statement_list(statement);

    return list;
}

Block *
nl_create_block(StatementList *statement_list) {
    Block *block = malloc(sizeof(Block));
    block->statement_list = statement_list;
    return block;
}
