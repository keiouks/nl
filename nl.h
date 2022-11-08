#ifndef PRIVATE_NL_H
#define PRIVATE_NL_H
#include "_NL.h"

typedef enum {
    INT_VALUE = 1,
    DOUBLE_VALUE,
    NULL_VALUE
} ValueType;

typedef struct {
    ValueType type;
    union {
        int int_value;
        double double_value;
    } u;
} NL_Value;

typedef struct Variable_tag {
    char *name;
    NL_Value value;
    struct Variable_tag *next;    
} Variable;

typedef struct Expression_tag Expression;

typedef enum {
    INT_EXPRESSION = 1,
    DOUBLE_EXPRESSION,
    ADD_EXPRESSION,
    SUB_EXPRESSION,
    MUL_EXPRESSION,
    DIV_EXPRESSION,
    MOD_EXPRESSION,
    VARIABLE_EXPRESSION,
    MINUS_EXPRESSION,
    FUNCTION_CALL_EXPRESSION,
    EXPRESSION_TYPE_PLUS
} ExpressionType;

typedef struct {
    Expression *left;
    Expression *right;
} BinaryExpression;

struct Expression_tag {
    ExpressionType type;
    union {
        int int_value;
        double double_value;
        char *identifier;
        Expression *minus_expression;
        BinaryExpression binary_expression;
    } u;
};

typedef struct {
    char *identifier;
    Expression *expression;
} AssignStatement;

typedef struct Block_tag Block;

typedef struct {
    char *identifier;
    Block *block;
} FunctionDefinitionStatement;

typedef enum {
    EXPRESSION_STATEMENT = 1,
    ASSIGN_STATEMENT,
    PRINT_STATEMENT,
    FUNCTION_DEFINITION_STATEMENT,
    RETURN_STATEMENT,
    STATEMENT_TYPE_PLUS
} StatementType;

typedef struct {
    StatementType type;
    union {
        Expression *expression;
        AssignStatement assign;
        FunctionDefinitionStatement functionDefinition;
    } u;
} Statement;

typedef struct StatementList_tag {
    Statement *statement;
    struct StatementList_tag *next;
} StatementList;

struct Block_tag {
    StatementList *statement_list;
};


typedef struct FunctionDefinition_tag {
    char *name;
    Block *block;
    struct FunctionDefinition_tag *next;
} FunctionDefinition;

typedef enum {
    NORMAL_STATEMENT_VALUE = 1,
    RETURN_STATEMENT_VALUE,
    STATEMENT_VALUE_PLUS
} StatementValueType;

typedef struct {
    StatementValueType type;
    NL_Value return_value;
} StatementValue;

struct King_tag {
    Variable *variable;
    StatementList *statement_list;
    FunctionDefinition *function_list;
};

/* create.c */
Expression *nl_alloc_expression(ExpressionType type);
Expression *nl_create_minus_expression(Expression *exp);
Expression *nl_create_binary_expression(ExpressionType type, Expression *left, Expression *right);
Expression *nl_create_variable_expression(char *identifier);
Expression *nl_create_function_call_expression(char *identifier);
char * nl_create_identifier(char *str);
Statement *nl_create_expression_statement(Expression *expression);
Statement *nl_create_assign_statement(char *identifier, Expression *expression);
Statement *nl_create_print_statement(Expression *expression);
StatementList *nl_create_statement_list(Statement *statement);
StatementList *nl_add_to_statement_list(StatementList *list, Statement *statement);
Block *nl_create_block(StatementList *statement_list);
Statement *nl_create_function_definition_statement(char *identifier, Block *block);
Statement *nl_create_return_statement(Expression *expression);

/* eval.c */
NL_Value nl_eval_binary_expression(ExpressionType type, Expression *left, Expression *right);
NL_Value nl_eval_expression(Expression *exp);
void nl_print_value(NL_Value *v);

/* interface.c */
King *nl_get_current_king(void);
Variable *nl_search_global_variable(King *king, char *identifier);
void nl_add_global_variable(King *king, char *identifier, NL_Value *value);
FunctionDefinition *nl_search_function(char *name);

/* execute.c */
StatementValue nl_execute_assign_statement(Statement *statement);
StatementValue nl_execute_statement_list(StatementList *list);

#endif /* PRIVATE_NL_H */
