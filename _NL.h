#ifndef PUBLIC_NL_H
#define PUBLIC_NL_H

typedef struct NL_Interpreter_tag NL_Interpreter;

NL_Interpreter *NL_create_interpreter(void);

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

typedef NL_Value NL_NativeFunctionProc(NL_Interpreter *interpreter, int arg_count, NL_Value *args);
#endif /* PUBLIC_NL_H */
