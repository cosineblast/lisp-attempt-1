//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_BUILTIN_H
#define LATTEMPT_BUILTIN_H

#include "value.h"

#define LA1_BUILTIN_FUNCTION_X() \
    X(list, "list") \
    X(cons, "cons") \
    X(plus, "+") \



#define X(name, symbol) \
    Value *la1_builtin_##name(LA1_State *state, LinkedList *arguments, void *);

LA1_BUILTIN_FUNCTION_X()

#undef X


#endif //LATTEMPT_BUILTIN_H
