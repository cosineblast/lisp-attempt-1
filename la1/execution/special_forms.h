//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_SPECIAL_FORMS_H
#define LATTEMPT_SPECIAL_FORMS_H

#include "value.h"

struct LA1_State;

#define LA1_SPECIAL_FORM_X() \
    X(if, IF)                \
    X(lambda, LAMBDA)        \
    X(quote, QUOTE)          \
    X(let, LET)              \
    X(do, DO)                \
    X(def, DEF)

enum {
    SPECIAL_FORM_COUNT = 0
#define X(name, big) +1  // NOLINT(bugprone-macro-parentheses)
    LA1_SPECIAL_FORM_X()
#undef X
};

enum SpecialFormType {
#define X(name, big) LA1_SPECIAL_FORM_##big,
    LA1_SPECIAL_FORM_X()
#undef X
};

#define X(name, big) \
    Value *la1_##name##_special_form(LA1_State *state, LinkedList *arguments);

LA1_SPECIAL_FORM_X()

#undef X

#endif  // LATTEMPT_SPECIAL_FORMS_H
