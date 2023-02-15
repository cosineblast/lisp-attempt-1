//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_EXECUTION_H
#define LATTEMPT_EXECUTION_H

#include "../common/list.h"
#include "../parsing/parse_value.h"
#include "gc.h"
#include "special_forms.h"
#include "value.h"

typedef Value *SpecialFormFunction(LA1_State *state, ConsCell *arguments);

typedef struct {
    KnownSymbol symbol;
    SpecialFormFunction *function;
} SpecialFormEntry;

struct LA1_State {
    // target type: KnownSymbol
    LinkedList *interned_symbols;

    Bindings *global_bindings;

    LinkedList *past_stacks;

    BindingStack *binding_stack;

    LA1_GC gc;

    SpecialFormEntry special_form_table[SPECIAL_FORM_COUNT];

    Value *nil;
    Value *true_value;
    Value *false_value;
};

LA1_State *la1_create_la1_state();

Value *la1_realize_parse_value(LA1_State *state, ParseValue *value);

KnownSymbol la1_intern(LA1_State *state, const char *symbol);

Value *la1_eval(LA1_State *state, Value *value);

Value *la1_eval_push(LA1_State *state, Value *value);

Value *la1_apply_data(LA1_State *state, DataClosure *closure,
                      ConsCell *arguments);

#endif  // LATTEMPT_EXECUTION_H
