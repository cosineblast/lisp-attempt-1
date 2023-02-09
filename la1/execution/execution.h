//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_EXECUTION_H
#define LATTEMPT_EXECUTION_H

#include "../common/list.h"
#include "value.h"
#include "../parsing/parse_value.h"

typedef struct {
    KnownSymbol if_symbol;
    KnownSymbol quote_symbol;
    KnownSymbol nil_symbol;
    KnownSymbol lambda_symbol;
    KnownSymbol let_symbol;
    Value *nil;
}SpecialFormSymbols ;

struct LA1_State {
    // target type: char*
    LinkedList *interned_symbols;

    Bindings *global_bindings;

    LinkedList *past_binding_stacks;
    BindingStack *current_binding_stack;

    SpecialFormSymbols special_forms;
};

typedef struct LA1_State LA1_State;

LA1_State *la1_create_la1_state();

Value *la1_realize_parse_value(LA1_State *state, ParseValue *value);

KnownSymbol la1_intern(LA1_State *state, const char * symbol);



#endif //LATTEMPT_EXECUTION_H
