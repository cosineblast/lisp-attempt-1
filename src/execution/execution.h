//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_EXECUTION_H
#define LATTEMPT_EXECUTION_H

#include "../common/list.h"
#include "../parsing/parse_value.h"
#include "gc.h"
#include "state.h"
#include "value.h"

LA1_State *la1_create_la1_state();
void la1_destroy_state(LA1_State *state);

KnownSymbol la1_intern(LA1_State *state, const char *symbol);

Value *la1_eval_push(LA1_State *state, Value *value);

Value *la1_eval(LA1_State *state, Value *value);

Value *la1_apply_data(LA1_State *state, DataClosure *closure,
                      ConsCell *arguments);

#endif  // LATTEMPT_EXECUTION_H
