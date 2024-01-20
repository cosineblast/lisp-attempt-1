//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_BINDING_H
#define LATTEMPT_BINDING_H

#include <stddef.h>

#include "../common/list.h"

struct Value;

typedef struct Value Value;
typedef struct ConsCell ConsCell;

typedef void *KnownSymbol;

struct Binding {
    KnownSymbol symbol;
    Value *value;
};

typedef struct Binding Binding;

struct Bindings {
    Binding *content;
    size_t size;
    size_t capacity;
    unsigned int reference_count;
};

typedef struct Bindings Bindings;

Bindings *la1_bindings_create();

void la1_bindings_free(Bindings *bindings);

Bindings *la1_bindings_create_with_capacity(size_t capacity);

void la1_bindings_add(Bindings *bindings, KnownSymbol key, Value *value);

int la1_bindings_lookup(Bindings *bindings, KnownSymbol key, Value **result);

void la1_bindings_increment_ref(LinkedList *node);

void la1_bindings_decrement_ref(LinkedList *node);

typedef struct {
    LinkedList *list;
} BindingStack;

BindingStack *la1_binding_stack_create();

void la1_binding_stack_free(BindingStack *stack);

void la1_binding_stack_push(BindingStack *stack, Bindings *bindings);

void la1_binding_stack_pop(BindingStack *stack);

int la1_binding_stack_lookup(BindingStack *stack, KnownSymbol key,
                             Value **result);

void la1_bindings_overwrite_values(Bindings *bindings, ConsCell *list);

#endif  // LATTEMPT_BINDING_H
