//
// Created by figurantpp on 15/02/23.
//

#include "gc.h"

#include <assert.h>
#include <stdlib.h>

#include "../common/alloc.h"
#include "state.h"

#define GC_THRESHOLD 100

void la1_gc_init(LA1_GC *gc) {
    gc->safe_stack = NULL;
    gc->values_since_last_gc = 0;
    gc->gc_values = NULL;
    gc->gc_value_count = 0;
}

static void mark(LA1_State *state);
static void sweep(LA1_State *state);
static void mark_value(Value *value);
static void mark_bindings(Bindings *bindings);

static void free_value(Value *value);
static void free_nodes(LinkedList *nodes);
void la1_perform_gc(LA1_State *state) {
    mark(state);
    sweep(state);
}

static void mark(LA1_State *state) {
    mark_value(state->true_value);
    mark_value(state->false_value);
    mark_value(state->nil);

    mark_bindings(state->global_bindings);

    for (LinkedList *current = state->binding_stack->list; current != NULL;
         current = current->next) {
        mark_bindings(current->item);
    }

    for (LinkedList *current = state->gc.safe_stack; current != NULL;
         current = current->next) {
        mark_value(current->item);
    }
}

static void mark_value(Value *value) {
    assert(value != NULL);

    value->gc_tag = 1;

    if (value->type == LA1_VALUE_CONS) {
        ConsCell *cell = value->content.cons;

        if (cell != NULL) {
            mark_value(cell->item);
            mark_value(cell->next);
        }
    } else if (value->type == LA1_VALUE_CLOSURE) {
        Closure *closure = value->content.closure;

        if (closure->type == LA1_CLOSURE_DATA) {
            DataClosure *data_closure = closure->extra;
            mark_value(data_closure->body_source);
        }
    }
}

static void mark_bindings(Bindings *bindings) {
    for (size_t i = 0; i < bindings->size; i++) {
        mark_value(bindings->content[i].value);
    }
}

static void sweep(LA1_State *state) {
    LinkedList *previous = NULL;
    LinkedList *current = state->gc.gc_values;
    LinkedList *next = NULL;

    while (current != NULL) {
        Value *value = current->item;
        next = current->next;

        if (value->gc_tag) {
            value->gc_tag = 0;

            previous = current;
        } else {
            free_value(value);

            if (previous == NULL)
                state->gc.gc_values = current->next;
            else
                previous->next = current->next;

            free(current);
        }

        current = next;
    }
}

static void free_value(Value *value) {
    if (value->type == LA1_VALUE_CONS) {
        ConsCell *cell = value->content.cons;

        free(cell);
    }

    if (value->type == LA1_VALUE_CLOSURE) {
        Closure *closure = value->content.closure;

        if (closure->type == LA1_CLOSURE_DATA) {
            DataClosure *data_closure = closure->extra;
            la1_bindings_decrement_ref(data_closure->environment);
            free_nodes(data_closure->parameters);
            free(data_closure);
        }
    }

    free(value);
}
static void free_nodes(LinkedList *nodes) {
    LinkedList *current = nodes;

    while (current != NULL) {
        LinkedList *next = current->next;

        free(current);

        current = next;
    }
}

Value *la1_safe_stack_push(LA1_State *state, Value *value) {
    state->gc.safe_stack = la1_list(value, state->gc.safe_stack);

    return value;
}

void la1_safe_stack_pop(LA1_State *state) {
    LinkedList *node = state->gc.safe_stack;

    state->gc.safe_stack = state->gc.safe_stack->next;

    free(node);
}

void la1_safe_stack_pop_n(LA1_State *state, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        la1_safe_stack_pop(state);
    }
}

Value *la1_gc_spawn(LA1_State *state, Value *from_stack) {
    if (state->gc.values_since_last_gc >= GC_THRESHOLD) {
        la1_perform_gc(state);
        state->gc.values_since_last_gc = 1;
    }

    Value *result = la1_malloc(sizeof(*result));
    *result = *from_stack;
    result->gc_tag = 0;

    state->gc.values_since_last_gc += 1;
    state->gc.gc_value_count += 1;
    state->gc.gc_values = la1_list(result, state->gc.gc_values);

    return result;
}
