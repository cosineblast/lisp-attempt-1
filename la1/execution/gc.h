//
// Created by figurantpp on 15/02/23.
//

#ifndef LATTEMPT_GC_H
#define LATTEMPT_GC_H

#include "../common/list.h"
#include "value.h"

struct LA1_GC {
    LinkedList *gc_values;
    int gc_value_count;
    int values_since_last_gc;
    int enabled;
    LinkedList *safe_stack;
};

typedef struct LA1_GC LA1_GC;

void la1_gc_init(LA1_GC *gc);

void la1_gc_disable(LA1_GC *gc);
void la1_gc_enable(LA1_GC *gc);

void la1_perform_gc(LA1_State *state);

Value *la1_safe_stack_push(LA1_State *state, Value *value);

void la1_safe_stack_pop(LA1_State *state);

void la1_safe_stack_pop_n(LA1_State *state, unsigned int n);

Value *la1_gc_spawn(LA1_State *state, Value *from_stack);

#endif  // LATTEMPT_GC_H
