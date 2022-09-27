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

struct King_tag {
    Variable *variable;
};

typedef struct Expression_tag Expression;

typedef enum {
    INT_EXPRESSION = 1,
    DOUBLE_EXPRESSION,
    ADD_EXPRESSION,
    SUB_EXPRESSION,
    MUL_EXPRESSION,
    DIV_EXPRESSION,
    MOD_EXPRESSION,
    EXPRESSION_TYPE_PLUS
} ExpressionType;

struct Expression_tag {
    ExpressionType type;
    union {
        int int_value;
        double double_value;
    } u;
};

typedef enum {
    EXPRESSION_STATEMENT = 1,
    STATEMENT_TYPE_PLUS
} StatementType;

/* create.c */
Expression *nl_alloc_expression(ExpressionType type);
Expression *nl_create_minus_expression(Expression *exp);
Expression *nl_create_binary_expression(ExpressionType type, Expression *left, Expression *right);
Expression *nl_create_variable_expression(char *identifier);
char * nl_create_identifier(char *str);

/* eval.c */
NL_Value nl_eval_binary_expression(ExpressionType operator, Expression *left, Expression *right);
NL_Value nl_eval_expression(Expression *exp);
NL_Value nl_eval_variable(King *king, char *identifier);
void nl_print_value(NL_Value *v);

/* interface.c */
King *nl_get_current_king(void);
Variable *nl_search_global_variable(King *king, char *identifier);
void nl_add_global_variable(King *king, char *identifier, NL_Value *value);

/* execute.c */
void nl_execute_assign_statement(King *king, char *identifier, Expression *exp);

#endif /* PRIVATE_NL_H */
