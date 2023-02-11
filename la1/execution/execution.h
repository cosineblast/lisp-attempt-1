//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_EXECUTION_H
#define LATTEMPT_EXECUTION_H

#include "../common/list.h"
#include "value.h"
#include "../parsing/parse_value.h"

#include "special_forms.h"



struct LA1_State;
typedef struct LA1_State LA1_State;

typedef Value *SpecialFormFunction(LA1_State *state, LinkedList *arguments);

typedef struct {
    KnownSymbol symbol;
    SpecialFormFunction *function;
} SpecialFormEntry;

struct LA1_State {
    // target type: KnownSymbol
    LinkedList *interned_symbols;

    Bindings *global_bindings;

    LinkedList *past_stacks;

    LinkedList *gc_values;

    BindingStack *binding_stack;

    SpecialFormEntry special_form_table[SPECIAL_FORM_COUNT];

    Value *nil;
    Value *true_value;
    Value *false_value;
};


LA1_State *la1_create_la1_state();

Value *la1_realize_parse_value(LA1_State *state, ParseValue *value);

KnownSymbol la1_intern(LA1_State *state, const char *symbol);

Value *la1_eval(LA1_State *state, Value *value);

Value *la1_apply_data(LA1_State *state, DataClosure *closure, LinkedList *arguments);


#endif //LATTEMPT_EXECUTION_H
