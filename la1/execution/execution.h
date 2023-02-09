//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_EXECUTION_H
#define LATTEMPT_EXECUTION_H

#include "../common/list.h"
#include "value.h"
#include "../parsing/parse_value.h"

enum SpecialFormType {
    LA1_SPECIAL_FORM_IF = 0,
    LA1_SPECIAL_FORM_QUOTE = 1,
    LA1_SPECIAL_FORM_NIL = 2,
    LA1_SPECIAL_FORM_LAMBDA = 3,
    LA1_SPECIAL_FORM_LET = 4,
    LA1_SPECIAL_FORM_DO = 5,
};


#define SPECIAL_FORM_COUNT 6

struct LA1_State;
typedef struct LA1_State LA1_State;

typedef Value *SpecialFormFunction(LA1_State *state, LinkedList *arguments);

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

    SpecialFormEntry special_form_table[SPECIAL_FORM_COUNT];

    Value *nil;
};




LA1_State *la1_create_la1_state();

Value *la1_realize_parse_value(LA1_State *state, ParseValue *value);

KnownSymbol la1_intern(LA1_State *state, const char * symbol);

Value *la1_eval(LA1_State *state, Value *value);



#endif //LATTEMPT_EXECUTION_H
