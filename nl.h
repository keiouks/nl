#ifndef PRIVATE_NL_H
#define PRIVATE_NL_H

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

/* eval.c */
NL_Value nl_eval_binary_expression(ExpressionType operator, Expression *left, Expression *right);
NL_Value nl_eval_expression(Expression *exp);
void nl_print_value(NL_Value *v);

#endif /* PRIVATE_NL_H */
