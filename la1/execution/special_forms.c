//
// Created by figurantpp on 09/02/23.
//

#include <assert.h>
#include "special_forms.h"
#include "../common/die.h"
#include "../common/alloc.h"

#include "execution.h"

Bindings *load_bindings(LA1_State *state, LinkedList *p_list, unsigned int size);

LinkedList *get_function_parameters(Value *value);

Value *la1_if_special_form(LA1_State *state, LinkedList *arguments) {

    unsigned int size = la1_find_list_size(arguments);

    if (size != 3) {
        la1_die("if: Expected 3 arguments");
    }

    Value *predicate_value = la1_eval(state, arguments->content);

    if (predicate_value == state->nil ||
        predicate_value == state->false_value) {
        return la1_eval(state, arguments->next->next->content);
    } else {
        return la1_eval(state, arguments->next->content);
    }
}


Value *la1_quote_special_form(LA1_State *state, LinkedList *arguments) {

    (void) state;

    la1_expect_size(arguments, 1);

    return arguments->content;
}

Value *la1_let_special_form(LA1_State *state, LinkedList *arguments) {

    unsigned int size = la1_find_list_size(arguments);

    if (size != 2) {
        la1_die("let: Expected two arguments.");
    }

    Value *first_argument = arguments->content;

    if (first_argument->type != LA1_VALUE_LIST) {
        la1_die("let: Expected list bindings.");
    }

    unsigned int first_argument_size = la1_find_list_size(first_argument->content.list);

    if (first_argument_size % 2 != 0) {
        la1_die("let: Expected even number of bindings.");
    }

    Bindings *bindings = load_bindings(state, first_argument->content.list, first_argument_size);

    la1_binding_stack_push(state->binding_stack, bindings);

    Value *result = la1_eval(state, arguments->next->content);

    la1_binding_stack_pop(state->binding_stack);

    return result;
}


Bindings *load_bindings(LA1_State *state, LinkedList *list, unsigned int size) {

    assert(size % 2 == 0);

    Bindings *result = la1_bindings_create_with_capacity(size);

    for (LinkedList *current = list; current != NULL; current = current->next->next) {
        Value *value = current->content;

        la1_expect_type(value, LA1_VALUE_SYMBOL);

        la1_bindings_add(result, value->content.symbol,
                         la1_eval(state, current->next->content));
    }

    return result;
}

Value *la1_nil_special_form(LA1_State *state, LinkedList *arguments) {

    (void) state;
    (void) arguments;

    la1_die("Illegal attempt to call to nil.");
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


static Value *apply_closure_function(LA1_State *state, LinkedList *arguments, void *extra) {

    DataClosure *data_closure = extra;

    return la1_apply_data(state, data_closure, arguments);
}

Value *la1_lambda_special_form(LA1_State *state, LinkedList *lambda_arguments) {

    (void) state;

    la1_expect_size(lambda_arguments, 2);

    DataClosure *data_closure = la1_malloc(sizeof(*data_closure));
    data_closure->parameters = get_function_parameters(lambda_arguments->content);

    la1_bindings_increment_ref(state->binding_stack->list);

    data_closure->environment = state->binding_stack->list;
    data_closure->body_source = lambda_arguments->next->content;

    Closure *result_closure = la1_malloc(sizeof(*data_closure));
    result_closure->extra = data_closure;
    result_closure->function = apply_closure_function;
    result_closure->type = CLOSURE_TYPE_DATA;

    return la1_closure_into_value(state, result_closure);
}


LinkedList *get_function_parameters(Value *value) {

    la1_expect_type(value, LA1_VALUE_LIST);

    LinkedList *parameters = value->content.list;

    if (parameters == NULL) {
        return NULL;
    }

    Value *first_parameter = parameters->content;
    la1_expect_type(first_parameter, LA1_VALUE_SYMBOL);

    LinkedList *result = la1_cons(first_parameter->content.symbol, NULL);
    LinkedList *result_end = result;

    for (LinkedList *current = parameters->next; current != NULL; current = current->next) {
        Value *parameter = current->content;
        la1_expect_type(parameter, LA1_VALUE_SYMBOL);

        result_end->next = la1_cons(parameter->content.symbol, NULL);
        result_end = result_end->next;
    }

    return result;
}

Value *la1_def_special_form(LA1_State *state, LinkedList *arguments) {

    la1_expect_size(arguments, 2);

    Value *target = arguments->content;

    la1_expect_type(target, LA1_VALUE_SYMBOL);

    Value *source = arguments->next->content;

    Value *result = la1_eval(state, source);

    la1_bindings_add(state->global_bindings, target->content.symbol, result);

    return state->nil;
}
