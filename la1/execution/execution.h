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

#define SPECIAL_FORM_COUNT 5

struct LA1_State;
typedef struct LA1_State LA1_State;

typedef Value* SpecialFormFunction(LA1_State *state, LinkedList *arguments);

typedef struct {
    KnownSymbol symbol;
    SpecialFormFunction *function;
} SpecialFormEntry;

struct LA1_State {
    // target type: char*
    LinkedList *interned_symbols;

    Bindings *global_bindings;

    LinkedList *past_binding_stacks;
    BindingStack *current_binding_stack;

    SpecialFormSymbols special_forms;
    SpecialFormEntry special_form_table[SPECIAL_FORM_COUNT];
};




LA1_State *la1_create_la1_state();

Value *la1_realize_parse_value(LA1_State *state, ParseValue *value);

KnownSymbol la1_intern(LA1_State *state, const char * symbol);

Value *la1_eval(LA1_State *state, Value *value);



#endif //LATTEMPT_EXECUTION_H
