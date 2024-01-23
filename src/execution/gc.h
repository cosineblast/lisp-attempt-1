//
// Created by cosineblast on 15/02/23.
//

#ifndef LATTEMPT_GC_H
#define LATTEMPT_GC_H

#include "../common/list.h"
#include "value.h"
#include <stdint.h>

struct LA1_GC;

typedef struct LA1_GC LA1_GC;

LA1_GC *la1_gc_create();
void la1_gc_destroy(LA1_GC *gc);

uint32_t la1_gc_value_count(LA1_GC *gc);



void la1_gc_disable(LA1_GC *gc);
void la1_gc_enable(LA1_GC *gc);

void la1_perform_gc(LA1_State *state);

Value *la1_safe_stack_push(LA1_State *state, Value *value);

void la1_safe_stack_pop(LA1_State *state);

void la1_safe_stack_pop_n(LA1_State *state, uint32_t n);

Value *la1_gc_spawn(LA1_State *state, Value *from_stack);

#endif  // LATTEMPT_GC_H
