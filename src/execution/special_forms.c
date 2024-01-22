//
// Created by cosineblast on 09/02/23.
//

#include "special_forms.h"

#include <assert.h>

#include "../common/alloc.h"
#include "../common/die.h"
#include "execution.h"
#include "gc.h"

Bindings *load_bindings(LA1_State *state, ConsCell *p_list,
                        unsigned int size);

LinkedList *get_function_parameters(Value *value);

Value *la1_if_special_form(LA1_State *state, ConsCell *arguments) {

    la1_expect_size(arguments, 3);

    ConsCell *on_true = la1_cons_next(arguments);
    ConsCell *on_false = la1_cons_next(on_true);

    Value *predicate_value = la1_eval(state, arguments->item);

    if (predicate_value == state->nil ||
        predicate_value == state->false_value) {

        return la1_eval(state, on_false->item);
    } else {
        return la1_eval(state, on_true->item);
    }
}

Value *la1_quote_special_form(LA1_State *state, ConsCell *arguments) {
    (void) state;

    la1_expect_size(arguments, 1);

    return arguments->item;
}

Value *la1_let_special_form(LA1_State *state, ConsCell *arguments) {

    la1_expect_size(arguments, 2);

    Value *first_argument = arguments->item;

    if (first_argument->type != LA1_VALUE_CONS) {
        la1_die("let: Expected list bindings.");
    }

    unsigned int first_argument_size =
            la1_find_cons_list_size(first_argument->content.cons);

    if (first_argument_size % 2 != 0) {
        la1_die("let: Expected even number of bindings.");
    }

    Bindings *bindings =
            load_bindings(state, first_argument->content.cons, first_argument_size);

    la1_binding_stack_push(state->binding_stack, bindings);

    Value *result = la1_eval(state, la1_cons_next(arguments)->item);

    la1_binding_stack_pop(state->binding_stack);

    return result;
}

Bindings *load_bindings(LA1_State *state, ConsCell *list, unsigned int size) {
    assert(size % 2 == 0);

    Bindings *result = la1_bindings_create_with_capacity(size);

    unsigned int count = 0;

    for (ConsCell *current = list; current != NULL;
         current = la1_cons_next(la1_cons_next(current))) {
        Value *value = current->item;

        la1_expect_type(value, LA1_VALUE_SYMBOL);

        la1_bindings_add(result, value->content.symbol,
                         la1_eval_push(state, la1_cons_next(current)->item));

        count += 1;
    }

    la1_safe_stack_pop_n(state, count);

    return result;
}

Value *la1_nil_special_form(LA1_State *state, ConsCell *arguments) {
    (void) state;
    (void) arguments;

    la1_die("Illegal attempt to call to nil.");
}

Value *la1_do_special_form(LA1_State *state, ConsCell *arguments) {
    (void)state;
    (void)arguments;

    Value *result = state->nil;

    ConsCell *current = arguments;

    unsigned int count = 0;

    while (current != NULL) {
        result = la1_eval_push(state, current->item);

        current = la1_cons_next(current);

        count += 1;
    }

    la1_safe_stack_pop_n(state, count);

    return result;
}

static Value *apply_closure_function(LA1_State *state, ConsCell *arguments,
                                     void *extra) {
    DataClosure *data_closure = extra;

    return la1_apply_data(state, data_closure, arguments);
}

Value *la1_lambda_special_form(LA1_State *state, ConsCell *lambda_arguments) {
    (void) state;

    la1_expect_size(lambda_arguments, 2);

    DataClosure *data_closure = la1_malloc(sizeof(*data_closure));
    data_closure->parameters =
            get_function_parameters(lambda_arguments->item);

    la1_bindings_increment_ref(state->binding_stack->list);

    data_closure->environment = state->binding_stack->list;
    data_closure->body_source = la1_cons_next(lambda_arguments)->item;

    Closure *result_closure = la1_malloc(sizeof(*data_closure));
    result_closure->extra = data_closure;
    result_closure->is_macro = 0;
    result_closure->function = apply_closure_function;
    result_closure->type = LA1_CLOSURE_DATA;

    return la1_closure_into_value(state, result_closure);
}

LinkedList *get_function_parameters(Value *value) {
    la1_expect_type(value, LA1_VALUE_CONS);

    ConsCell *parameters = value->content.cons;

    if (parameters == NULL) {
        return NULL;
    }

    Value *first_parameter = parameters->item;
    la1_expect_type(first_parameter, LA1_VALUE_SYMBOL);

    LinkedList *result = la1_list(first_parameter->content.symbol, NULL);
    LinkedList *result_end = result;

    for (ConsCell *current = la1_cons_next(parameters); current != NULL;
         current = la1_cons_next(current)) {

        Value *parameter = current->item;
        la1_expect_type(parameter, LA1_VALUE_SYMBOL);

        result_end->next = la1_list(parameter->content.symbol, NULL);
        result_end = result_end->next;
    }

    return result;
}

Value *la1_def_special_form(LA1_State *state, ConsCell *arguments) {
    la1_expect_size(arguments, 2);

    Value *target = arguments->item;

    la1_expect_type(target, LA1_VALUE_SYMBOL);

    Value *source = la1_cons_next(arguments)->item;

    Value *result = la1_eval(state, source);

    la1_bindings_add(state->global_bindings, target->content.symbol, result);

    return state->nil;
}

Value *la1_macro_lambda_special_form(LA1_State *state, ConsCell *arguments) {
    Value *result = la1_lambda_special_form(state, arguments);

    assert(result->type == LA1_VALUE_CLOSURE);

    result->content.closure->is_macro = 1;

    return result;
}
