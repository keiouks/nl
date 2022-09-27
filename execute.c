#include "nl.h"

void
nl_execute_assign_statement(King *king, char *identifier, Expression *exp) {
    NL_Value v;
    Variable *var;

    v = nl_eval_expression(exp);
    var = nl_search_global_variable(king, identifier);

    if(var != NULL) {
        var->value = v;
    } else {
        nl_add_global_variable(king, identifier, &v);
    }
}
