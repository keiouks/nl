#include "nl.h"
#include <stdlib.h>
#include <string.h>

StatementValue
nl_execute_expression_statement(Statement *statement, Scope *scope) {
    StatementValue sValue;
    sValue.type = NORMAL_STATEMENT_VALUE;
    nl_eval_expression(statement->u.expression, scope);
    return sValue;
}

StatementValue
nl_execute_assign_statement(Statement *statement, Scope *scope) {
    StatementValue sValue;
    NL_Value val;
    Variable *var;
    Expression *exp = statement->u.assign.expression;
    char *identifier = statement->u.assign.identifier;

    /* 找到变量 */
    var = nl_search_variable(scope, identifier);
    if (var == NULL) {
        printf("[runtime error] assign to not exist variable [%s].\n", identifier);
        exit(1);
    }

    sValue.type = NORMAL_STATEMENT_VALUE;
    /* 表达式求值 */
    val = nl_eval_expression(exp, scope);
    /* 结果赋值 */
    var->value = val;
    return sValue;
}

StatementValue
nl_execute_declaration_statement(Statement *statement, Scope *scope) {
    StatementValue sValue;
    NL_Value val;
    Expression *exp = statement->u.declaration.expression;
    char *identifier = statement->u.declaration.identifier;

    sValue.type = NORMAL_STATEMENT_VALUE;

    val = nl_eval_expression(exp, scope);
    nl_add_variable(scope, identifier, &val);
    return sValue;
}

StatementValue
nl_execute_print_statement(Statement *statement, Scope *scope) {
    StatementValue sValue;
    NL_Value v = nl_eval_expression(statement->u.expression, scope);
    sValue.type = NORMAL_STATEMENT_VALUE;
    nl_print_value(&v);
    return sValue;
}

StatementValue
nl_execute_function_definition_statement(Statement *statement, Scope *scope) {
    StatementValue sValue;
    FunctionDefinition *newFun = NULL;
    FunctionDefinition *temp = NULL;
    char *identifier = statement->u.functionDefinition.identifier;
    Block *block = statement->u.functionDefinition.block;
    /* 先找，只找同一个scope */
    for(temp = scope->function_list; temp; temp = temp->next) {
        if(!strcmp(temp->name, identifier)) {
            newFun = temp;
            break;
        }
    }

    sValue.type = NORMAL_STATEMENT_VALUE;
    /* 已有同名函数 */
    if (newFun != NULL) {
        printf("[runtime error] execute function definition statement while function [%s] is exist.\n", identifier);
        exit(1);
    }
    newFun = malloc(sizeof(FunctionDefinition));
    newFun->block = block;
    newFun->name = identifier;
    newFun->scope = scope;
    newFun->next = scope->function_list;
    scope->function_list = newFun;

    return sValue;
}

StatementValue
nl_execute_return_statement(Statement *statement, Scope *scope) {
    StatementValue sValue;
    sValue.type = RETURN_STATEMENT_VALUE;
    if (!statement->u.expression) {
        sValue.return_value.type = NULL_VALUE;
        return sValue;
    }
    sValue.return_value = nl_eval_expression(statement->u.expression, scope);
    return sValue;
}

StatementValue
nl_execute_statement(Statement *statement, Scope *scope) {
    StatementValue sValue;
    switch(statement->type) {
        case EXPRESSION_STATEMENT: {
            sValue = nl_execute_expression_statement(statement, scope);
            break;
        }
        case ASSIGN_STATEMENT: {
            sValue = nl_execute_assign_statement(statement, scope);
            break;
        }
        case DECLARATION_STATEMENT: {
            sValue = nl_execute_declaration_statement(statement, scope);
            break;
        }
        case PRINT_STATEMENT: {
            sValue = nl_execute_print_statement(statement, scope);
            break;
        }
        case FUNCTION_DEFINITION_STATEMENT: {
            sValue = nl_execute_function_definition_statement(statement, scope);
            break;
        }
        case RETURN_STATEMENT: {
            sValue = nl_execute_return_statement(statement, scope);
            break;
        }
        case STATEMENT_TYPE_PLUS:
        default: {
            printf("[runtime error] execute statement with unexpected type [%d].\n", statement->type);
            exit(1);
        }
    }
    return sValue;
}

StatementValue
nl_execute_statement_list(StatementList *list, Scope *scope) {
    StatementList *now;
    StatementValue sValue;
    sValue.type = NORMAL_STATEMENT_VALUE;
    if (list == NULL) {
        return sValue;
    }
    for (now = list; now && (sValue.type == NORMAL_STATEMENT_VALUE); now = now->next) {
        sValue = nl_execute_statement(now->statement, scope);
    }
    return sValue;
}
