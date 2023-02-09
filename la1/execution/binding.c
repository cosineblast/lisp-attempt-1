//
// Created by figurantpp on 09/02/23.
//

#include <stddef.h>
#include "binding.h"

BindingStack *la1_binding_stack_create() {
    return NULL;
}

void la1_bindings_add(Bindings *bindings, KnownSymbol key, Value *value) {

}

int la1_bindings_lookup(Bindings *bindings, KnownSymbol key, Value **result) {
    return 0;
}

void la1_binding_stack_add(BindingStack *stack, Bindings *bindings) {

}

int la1_binding_stack_lookup(BindingStack *stack, KnownSymbol key, Value **result) {
    return 0;
}

Bindings *la1_bindings_create() {
    return NULL;
}
