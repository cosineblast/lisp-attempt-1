//
// Created by cosineblast on 16/02/23.
//

#ifndef LATTEMPT_STATE_H
#define LATTEMPT_STATE_H

#include "data_stack.h"
#include "gc.h"
#include "special_forms.h"

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

    LA1_DataStack *data_stack;

    LA1_GC gc;

    SpecialFormEntry special_form_table[SPECIAL_FORM_COUNT];

    Value *nil;
    Value *true_value;
    Value *false_value;
};
#endif  // LATTEMPT_STATE_H
