//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_BINDING_H
#define LATTEMPT_BINDING_H

#include "../common/list.h"

struct Value;

typedef struct Value Value;

typedef char * KnownSymbol;

struct Binding {
    KnownSymbol symbol;
    Value *value;
};

typedef struct Binding Binding;

struct Bindings {
    LinkedList list;
};

typedef struct Bindings Bindings;

Bindings *la1_bindings_create();
void la1_bindings_add(Bindings *bindings, KnownSymbol key, Value *value);
int la1_bindings_lookup(Bindings *bindings, KnownSymbol key, Value **result);

typedef struct {
    LinkedList list;
} BindingStack;

BindingStack *la1_binding_stack_create();
void la1_binding_stack_add(BindingStack *stack, Bindings *bindings);
int la1_binding_stack_lookup(BindingStack *stack, KnownSymbol key, Value **result);

#endif //LATTEMPT_BINDING_H
