//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_BUILTIN_H
#define LATTEMPT_BUILTIN_H

#include "value.h"

#define LA1_BUILTIN_FUNCTION_X() \
    X(list, "list")              \
    X(cons, "cons")              \
    X(first, "first")            \
    X(rest, "rest")              \
    X(plus, "+")                 \
    X(minus, "-")                \
    X(times, "*")                \
    X(div, "div")                \
    X(mod, "mod")                \
    X(equals, "=")               \
    X(emptyp, "empty?")          \
    X(gc, "gc")                  \
    X(prn, "prn")

#define X(name, symbol) \
    Value *la1_builtin_##name(LA1_State *state, ConsCell *arguments, void *);

LA1_BUILTIN_FUNCTION_X()

#undef X

#endif  // LATTEMPT_BUILTIN_H
