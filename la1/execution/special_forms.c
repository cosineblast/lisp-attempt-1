//
// Created by figurantpp on 09/02/23.
//

#include <assert.h>
#include "special_forms.h"
#include "../common/die.h"

Bindings *load_bindings(LA1_State *state, LinkedList *p_list, unsigned int size);

Value *la1_if_special_form(LA1_State *state, LinkedList *arguments) {

    unsigned int size = la1_find_list_size(arguments);

    if (size != 3) {
        die("if: Expected 3 arguments");
    }

    Value *predicate_value = la1_eval(state, arguments->content);

    if (predicate_value == state->nil) {
        return la1_eval(state, arguments->next->next->content);
    } else {
        return la1_eval(state, arguments->next->content);
    }
}

Value *la1_lambda_special_form(LA1_State *state, LinkedList *arguments) {

    (void) state;

    unsigned int size = la1_find_list_size(arguments);

    if (size != 3) {
        die("lambda: Expected 3 arguments");
    }

    die("Lambda is not implemented");
}

Value *la1_quote_special_form(LA1_State *state, LinkedList *arguments) {

    (void) state;

    unsigned int size = la1_find_list_size(arguments);

    if (size != 1) {
        die("quote: Expected 1 arguments.");
    }

    return arguments->content;
}

Value *la1_let_special_form(LA1_State *state, LinkedList *arguments) {

    unsigned int size = la1_find_list_size(arguments);

    if (size != 2) {
        die("let: Expected two arguments.");
    }

    Value *first_argument = arguments->content;

    if (first_argument->type != LA1_VALUE_LIST) {
        die("let: Expected list bindings.");
    }

    unsigned int first_argument_size = la1_find_list_size(first_argument->content.list);

    if (first_argument_size % 2 != 0) {
        die("let: Expected even number of bindings.");
    }

    Bindings *bindings = load_bindings(state, first_argument->content.list, first_argument_size);

    la1_binding_stack_push(state->current_binding_stack, bindings);

    Value *result = la1_eval(state, arguments->next->content);

    la1_binding_stack_pop(state->current_binding_stack);

    return result;
}

void expect_type(Value *value, ValueType type) {
    if (value->type != type) {
        die("Unexpected type");
    }
}

Bindings *load_bindings(LA1_State *state, LinkedList *list, unsigned int size) {

    assert(size % 2 == 0);

    Bindings *result = la1_bindings_create_with_capacity(size);

    for (LinkedList *current = list; current != NULL; current = current->next->next) {
        Value *value = current->content;

        expect_type(value, LA1_VALUE_SYMBOL);

        la1_bindings_add(result, value->content.symbol,
                         la1_eval(state, current->next->content));
    }

    return result;
}

Value *la1_nil_special_form(LA1_State *state, LinkedList *arguments) {

    (void) state;
    (void) arguments;

    die("Illegal attempt to call to nil.");
}

Value *la1_do_special_form(LA1_State *state, LinkedList *arguments) {

    (void) state;
    (void) arguments;

    Value *result = state->nil;

    LinkedList *current = arguments;

    while (current != NULL) {

        result = la1_eval(state, current->content);

        current = current->next;
    }

    return result;
}
