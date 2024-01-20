//
// Created by figurantpp on 09/02/23.
//

#include "binding.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "../common/alloc.h"
#include "../execution/value.h"

Bindings *la1_bindings_create() {
    Bindings *bindings = la1_malloc(sizeof(*bindings));
    bindings->content = NULL;
    bindings->size = 0;
    bindings->capacity = 0;
    bindings->reference_count = 1;
    return bindings;
}

void la1_bindings_increment_ref(LinkedList *node) {
    if (node != NULL) {
        Bindings *binding = node->item;

        binding->reference_count += 1;
    }
}

void la1_bindings_decrement_ref(LinkedList *node) {
    if (node != NULL) {
        Bindings *binding = node->item;

        assert(binding->reference_count >= 1);

        if (binding->reference_count == 1) {
            LinkedList *next = node->next;

            free(binding->content);
            free(binding);
            free(node);
            la1_bindings_decrement_ref(next);
        } else {
            binding->reference_count -= 1;
        }
    }
}

Bindings *la1_bindings_create_with_capacity(size_t capacity) {
    Bindings *bindings = la1_malloc(sizeof(*bindings));
    bindings->content = la1_malloc(capacity * sizeof(Binding));
    bindings->size = 0;
    bindings->capacity = capacity;
    bindings->reference_count = 1;
    return bindings;
}

BindingStack *la1_binding_stack_create() {
    BindingStack *stack = la1_malloc(sizeof(*stack));
    stack->list = NULL;
    return stack;
}

#define DEFAULT_CAPACITY 4

void la1_bindings_add(Bindings *bindings, KnownSymbol key, Value *value) {
    assert(bindings && key && value);

    if (bindings->size >= bindings->capacity) {
        size_t new_capacity =
                bindings->capacity == 0 ? DEFAULT_CAPACITY : 2 * bindings->capacity;

        bindings->content =
                la1_realloc(bindings->content, new_capacity * sizeof(Binding));

        bindings->capacity = new_capacity;
    }

    bindings->content[bindings->size].symbol = key;
    bindings->content[bindings->size].value = value;

    bindings->size += 1;
}

int la1_bindings_lookup(Bindings *bindings, KnownSymbol key, Value **result) {
    assert(bindings && key && result);

    for (size_t i = 0; i < bindings->size; i++) {
        if (bindings->content[i].symbol == key) {
            *result = bindings->content[i].value;
            return 1;
        }
    }

    return 0;
}

void la1_binding_stack_push(BindingStack *stack, Bindings *bindings) {
    stack->list = la1_list(bindings, stack->list);
}

int la1_binding_stack_lookup(BindingStack *stack, KnownSymbol key,
                             Value **result) {
    LinkedList *current = stack->list;

    while (current != NULL) {
        Bindings *bindings = current->item;

        if (la1_bindings_lookup(bindings, key, result)) {
            return 1;
        }

        current = current->next;
    }

    return 0;
}

void la1_binding_stack_pop(BindingStack *stack) {
    LinkedList *head = stack->list;
    LinkedList *next = head->next;

    la1_bindings_increment_ref(next);
    la1_bindings_decrement_ref(head);

    stack->list = next;
}
void la1_bindings_free(Bindings *bindings) {
    free(bindings->content);
    free(bindings);
}

void la1_binding_stack_free(BindingStack *stack) {
    LinkedList *node = stack->list;

    while (node != NULL) {
        LinkedList *next = node->next;

        la1_bindings_free(node->item);

        free(node);
        node = next;
    }

    free(stack);
}

void la1_bindings_overwrite_values(Bindings *bindings, ConsCell *list) {
    assert(bindings && list);

    size_t i = 0;

    assert(la1_find_cons_list_size(list) == bindings->size);

    for (ConsCell *current = list; current != NULL;
         current = la1_cons_next(current)) {
        Value *value = current->item;

        bindings->content[i].value = value;

        i += 1;
    }
}
