#ifndef PRIVATE_NL_H
#define PRIVATE_NL_H
#include "_NL.h"
#include "_MEMORY.h"

typedef struct Expression_tag Expression;

typedef enum {
    INT_EXPRESSION = 1,
    DOUBLE_EXPRESSION,
    ADD_EXPRESSION,
    SUB_EXPRESSION,
    MUL_EXPRESSION,
    DIV_EXPRESSION,
    MOD_EXPRESSION,
    MINUS_EXPRESSION,
    EXPRESSION_TYPE_COUNT_PLUS_1
} ExpressionType;

typedef struct {
    Expression *left;
    Expression *right;
} BinaryExpression;

struct Expression_tag {
    ExpressionType type;
    int line_number;
    union {
        int int_value;
        double double_value;
        BinaryExpression binary_expression;
        Expression *minus_expression;
    } u;
};

typedef enum {
    EXPRESSION_STATEMENT = 1,
    STATEMENT_TYPE_COUNT_PLUS_1
} StatementType;

typedef struct Statement_tag {
    StatementType type;
    int line_number;
    union {
        Expression *expression_s;
    } u;
} Statement;

typedef struct StatementList_tag {
    Statement *statement;
    struct StatementList_tag *next;
} StatementList;

typedef struct {
    StatementList *statement_list;
} Block;

typedef struct ParameterList_tag {
    char *name;
    struct ParameterList_tag *next;
} ParameterList;

typedef enum {
    NL_FUNCTION_DEFINITION = 1,
    NATIVE_FUNCTION_DEFINITION
} FunctionDefinitionType;

typedef struct FunctionDefinition_tag {
    char *name;
    FunctionDefinitionType type;
    union {
        struct {
            ParameterList *parameter;
            Block *block;
        } nl_f;
        struct {
            NL_NativeFunctionProc *proc;
        } native_f;
    }u;
    struct FunctionDefinition_tag *next;
} FunctionDefinition;

typedef struct Variable_tag {
    char *name;
    NL_Value value;
    struct Variable_tag *next;
} Variable;

typedef struct IdentifierList_tag {
    char *name;
    struct IdentifierList_tag *next;
} IdentifierList;

struct NL_Interpreter_tag {
    MEM_Storage interpreter_storage;
    MEM_Storage execute_storage;
    Variable *variable;
    FunctionDefinition *function_list;
    StatementList *statement_list;
    int current_line_number;
};

/* create.c */
IdentifierList *nl_create_identifier(char *identifier);
IdentifierList *nl_chain_identifier(IdentifierList *list, char *identifier);
void nl_print_identifier_list(IdentifierList *list);
Expression *nl_alloc_expression(ExpressionType type);
Expression *nl_create_minus_expression(Expression *operand);
Expression *nl_create_binary_expression(ExpressionType type, Expression *left, Expression *right);

/* eval.c */
NL_Value nl_eval_binary_expression(ExpressionType operator, Expression *left, Expression *right);
NL_Value nl_eval_expression(Expression *exp);
NL_Value nl_eval_minus_expression(Expression *operand);
void nl_print_value(NL_Value *v);

#endif /* PRIVATE_NL_H */
