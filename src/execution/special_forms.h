//
// Created by cosineblast on 09/02/23.
//

#ifndef LATTEMPT_SPECIAL_FORMS_H
#define LATTEMPT_SPECIAL_FORMS_H

#include "value.h"

struct LA1_State;

#define LA1_SPECIAL_FORM_X()                      \
    X("if", if, IF)                               \
    X("lambda", lambda, LAMBDA)                   \
    X("macro-lambda", macro_lambda, MACRO_LAMBDA) \
    X("quote", quote, QUOTE)                      \
    X("let", let, LET)                            \
    X("do", do, DO)                               \
    X("def", def, DEF)                            \
    X("loop", loop, LOOP)

enum {
    SPECIAL_FORM_COUNT = 0
#define X(symbol, name, big) +1  // NOLINT(bugprone-macro-parentheses)
    LA1_SPECIAL_FORM_X()
#undef X
};

enum SpecialFormType {
#define X(symbol, name, big) LA1_SPECIAL_FORM_##big,
    LA1_SPECIAL_FORM_X()
#undef X
};

#define X(symbol, name, big) \
    Value *la1_##name##_special_form(LA1_State *state, ConsCell *arguments);

LA1_SPECIAL_FORM_X()

#undef X

typedef Value *SpecialFormFunction(LA1_State *state, ConsCell *arguments);

#endif  // LATTEMPT_SPECIAL_FORMS_H
