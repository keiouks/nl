#ifndef PRIVATE_NL_H
#define PRIVATE_NL_H
#include "_NL.h"

typedef enum {
    INT_VALUE = 1,
    DOUBLE_VAULE
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

typedef enum {
    EXPRESSION_STATEMENT = 1,
    ASSIGN_STATEMENT,
    PRINT_STATEMENT,
    STATEMENT_TYPE_PLUS
} StatementType;

typedef struct {
    StatementType type;
    union {
        Expression *expression;
        AssignStatement assign;
    } u;
} Statement;

typedef struct StatementList_tag {
    Statement *statement;
    struct StatementList_tag *next;
} StatementList;

struct King_tag {
    Variable *variable;
    StatementList *statement_list;
};

/* create.c */
Expression *nl_alloc_expression(ExpressionType type);
Expression *nl_create_minus_expression(Expression *exp);
Expression *nl_create_binary_expression(ExpressionType type, Expression *left, Expression *right);
Expression *nl_create_variable_expression(char *identifier);
char * nl_create_identifier(char *str);
Statement *nl_create_expression_statement(Expression *expression);
Statement *nl_create_assign_statement(char *identifier, Expression *expression);
Statement *nl_create_print_statement(Expression *expression);
StatementList *nl_add_to_statement_list(StatementList *list, Statement *statement);

/* eval.c */
NL_Value nl_eval_binary_expression(ExpressionType operator, Expression *left, Expression *right);
NL_Value nl_eval_expression(Expression *exp);
void nl_print_value(NL_Value *v);

/* interface.c */
King *nl_get_current_king(void);
Variable *nl_search_global_variable(King *king, char *identifier);
void nl_add_global_variable(King *king, char *identifier, NL_Value *value);

/* execute.c */
void nl_execute_assign_statement(Statement *statement);
void nl_execute_statement_list(StatementList *list);

#endif /* PRIVATE_NL_H */
