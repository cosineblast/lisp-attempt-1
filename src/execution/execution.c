//
// Created by cosineblast on 09/02/23.
//

#include "execution.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../common/alloc.h"
#include "../common/die.h"
#include "builtin.h"
#include "data_stack.h"
#include "gc.h"
#include "special_forms.h"

static void push_special_forms(LA1_State *p_state);

static int try_eval_special_form(LA1_State *state, ConsCell *list,
                                 Value **result);

static Value *eval_list(LA1_State *state, Value *list_value);

static Value *eval_symbol(LA1_State *p_state, KnownSymbol symbol);

static void initialize_prelude(LA1_State *p_state);

static Value *evaluate_children(LA1_State *state, Value *list_value);

static Value *closure_for(LA1_State *state, ClosureFunction *function);

static int lookup_variable(LA1_State *state, KnownSymbol symbol,
                           Value **result);

static Value *apply(LA1_State *state, Value *call);

static Bindings *bind_arguments(LinkedList *parameters, ConsCell *arguments);

static void restore_binding_stack(LA1_State *state);

static void set_binding_stack(LA1_State *state, LinkedList *new_list);

static bool try_expand_macro(LA1_State *state, ConsCell *input, Value **output);

LA1_State *la1_create_la1_state() {
    LA1_State *state = la1_malloc(sizeof(*state));

    state->binding_stack = la1_binding_stack_create();
    state->global_bindings = la1_bindings_create();
    state->interned_symbols = la1_empty_list();
    state->past_stacks = NULL;
    state->data_stack = la1_data_stack_create();

    la1_gc_init(&state->gc);

    push_special_forms(state);

    initialize_prelude(state);

    la1_gc_enable(&state->gc);

    return state;
}

void la1_destroy_state(LA1_State *state) {
    la1_perform_gc(state);

    la1_gc_kill_all_values(&state->gc);

    assert(state->past_stacks == NULL);

    LinkedList *node = state->past_stacks;

    while (node != NULL) {
        la1_binding_stack_free(node->item);

        LinkedList *next = node->next;
        la1_binding_stack_free(node->item);
        free(node);
        node = next;
    }

    node = state->interned_symbols;

    while (node != NULL) {
        LinkedList *next = node->next;
        free(node->item);
        free(node);
        node = next;
    }

    la1_bindings_free(state->global_bindings);

    la1_binding_stack_free(state->binding_stack);

    la1_data_stack_free(state->data_stack);

    free(state);
}

static void push_special_forms(LA1_State *state) {
    assert(state);

    SpecialFormEntry table[SPECIAL_FORM_COUNT] = {

#define X(symbol, name, big) \
    [LA1_SPECIAL_FORM_##     \
        big] = {la1_intern(state, symbol), la1_##name##_special_form},
        LA1_SPECIAL_FORM_X()
#undef X
    };

    assert(sizeof(table) == sizeof(state->special_form_table));

    memcpy(state->special_form_table, table, sizeof(table));

    state->nil = la1_symbol_into_value(state, la1_intern(state, "nil"));
    state->true_value = la1_symbol_into_value(state, la1_intern(state, "true"));
    state->false_value =
        la1_symbol_into_value(state, la1_intern(state, "false"));
}

static void initialize_prelude(LA1_State *state) {
    struct {
        KnownSymbol symbol;
        ClosureFunction *function;
    } builtin_functions[] = {
#define X(name, symbol) {la1_intern(state, symbol), la1_builtin_##name},
        LA1_BUILTIN_FUNCTION_X()
#undef X
    };

    for (int i = 0;
         i < sizeof(builtin_functions) / sizeof(builtin_functions[0]); i++) {
        la1_bindings_add(state->global_bindings, builtin_functions[i].symbol,
                         closure_for(state, builtin_functions[i].function));
    }

    la1_bindings_add(state->global_bindings, state->nil->content.symbol,
                     state->nil);

    la1_bindings_add(state->global_bindings, state->true_value->content.symbol,
                     state->true_value);

    la1_bindings_add(state->global_bindings, state->false_value->content.symbol,
                     state->false_value);
}

static Value *closure_for(LA1_State *state, ClosureFunction *function) {
    return la1_closure_into_value(state, la1_create_c_closure(function, NULL));
}

KnownSymbol la1_intern(LA1_State *state, const char *symbol) {
    LinkedList *current = state->interned_symbols;

    while (current != NULL) {
        if (strcmp(symbol, current->item) == 0) {
            return current->item;
        }

        current = current->next;
    }

    state->interned_symbols = la1_list(strdup(symbol), state->interned_symbols);

    return state->interned_symbols->item;
}

Value *la1_eval_push(LA1_State *state, Value *value) {
    return la1_safe_stack_push(state, la1_eval(state, value));
}

Value *la1_eval(LA1_State *state, Value *value) {
    assert(state && value);

    switch (value->type) {
        case LA1_VALUE_SYMBOL:
            return eval_symbol(state, value->content.symbol);

        case LA1_VALUE_NUMBER:
            return value;

        case LA1_VALUE_CONS:
            return eval_list(state, value);

        default:
            la1_die("Illegal value in source code.");
    }
}

static Value *eval_symbol(LA1_State *state, KnownSymbol symbol) {
    if (symbol == state->nil->content.symbol) {
        return state->nil;
    }

    Value *result;

    if (lookup_variable(state, symbol, &result)) {
        return result;
    }

    la1_die_format("Symbol %s not found.\n", (char *)symbol);
}

static int lookup_variable(LA1_State *state, KnownSymbol symbol,
                           Value **result) {
    if (la1_binding_stack_lookup(state->binding_stack, symbol, result)) {
        return 1;
    }

    if (la1_bindings_lookup(state->global_bindings, symbol, result)) {
        return 1;
    }

    return 0;
}

static Value *eval_list(LA1_State *state, Value *list_value) {
    assert(list_value->type == LA1_VALUE_CONS);

    ConsCell *list = list_value->content.cons;

    if (list == NULL) {
        return la1_cons_into_value(state, NULL);
    }

    Value *first_element = list->item;

    if (first_element->type == LA1_VALUE_NUMBER) {
        la1_die("Cannot call number");
    }

    Value *result;

    if (try_eval_special_form(state, list, &result)) {
        return result;
    } else if (try_expand_macro(state, list, &result)) {
        return la1_eval(state, result);
    } else {
        Value *evaluated = evaluate_children(state, list_value);

        return apply(state, evaluated);
    }
}

static bool try_expand_macro(LA1_State *state, ConsCell *input,
                             Value **output) {
    assert(input != NULL);

    Value *first_value = input->item;

    if (first_value->type == LA1_VALUE_SYMBOL) {
        KnownSymbol symbol = first_value->content.symbol;

        Value *value;

        if (la1_bindings_lookup(state->global_bindings, symbol, &value) &&
            value->type == LA1_VALUE_CLOSURE &&
            value->content.closure->is_macro) {
            Closure *closure = value->content.closure;

            la1_safe_stack_push(state, input->next);

            *output =
                closure->function(state, la1_cons_next(input), closure->extra);

            la1_safe_stack_pop(state);

            return true;
        }
    }

    return false;
}

int try_eval_special_form(LA1_State *state, ConsCell *list, Value **result) {
    assert(state && list && result);
    Value *first_value = list->item;

    if (first_value->type != LA1_VALUE_SYMBOL) {
        return 0;
    }

    KnownSymbol symbol = first_value->content.symbol;

    for (int i = 0; i < SPECIAL_FORM_COUNT; i++) {
        if (state->special_form_table[i].symbol == symbol) {
            la1_safe_stack_push(state, list->next);

            *result = state->special_form_table[i].function(
                state, la1_cons_next(list));

            la1_safe_stack_pop(state);

            return 1;
        }
    }

    return 0;
}

Value *evaluate_children(LA1_State *state, Value *list_value) {
    // todo: make this stupid thing conform to the 35 max
    //  line function rule.

    if (list_value == NULL) {
        return NULL;
    }

    la1_safe_stack_push(state, list_value);

    ConsCell *list = list_value->content.cons;

    Value *empty_list = la1_cons_into_value(state, NULL);

    la1_safe_stack_push(state, empty_list);

    ConsCell *result = la1_cons(la1_eval_push(state, list->item), empty_list);

    ConsCell *current_argument = la1_cons_next(list);
    ConsCell *result_end = result;

    unsigned int count = 1;

    while (current_argument != NULL) {
        result_end->next = la1_cons_into_value(
            state,
            la1_cons(la1_eval_push(state, current_argument->item), empty_list));

        la1_safe_stack_push(state, result_end->next);

        result_end = la1_cons_next(result_end);
        current_argument = la1_cons_next(current_argument);
        count += 1;
    }

    Value *result_value = la1_cons_into_value(state, result);

    // what? why 2 * count?
    // if you look at the code, you'll see there are two pushes before the loop
    // runs, and two pushes on every iteration of the loop, and because count
    // starts at one, we do two pushes.
    la1_safe_stack_pop_n(state, 2 * count);

    la1_safe_stack_pop(state);

    return result_value;
}

Value *apply(LA1_State *state, Value *call_value) {
    assert(call_value->type == LA1_VALUE_CONS);

    ConsCell *call = call_value->content.cons;

    Value *target_value = call->item;

    if (target_value->type != LA1_VALUE_CLOSURE) {
        la1_die("Cannot apply non-closure value.");
    }

    Closure *closure = target_value->content.closure;

    la1_safe_stack_push(state, call_value);

    Value *result =
        closure->function(state, la1_cons_next(call), closure->extra);

    la1_safe_stack_pop(state);

    return result;
}

Value *la1_apply_data(LA1_State *state, DataClosure *closure,
                      ConsCell *arguments) {
    la1_expect_size(arguments, la1_find_list_size(closure->parameters));

    Bindings *bindings = bind_arguments(closure->parameters, arguments);

    set_binding_stack(state, closure->environment);

    la1_binding_stack_push(state->binding_stack, bindings);

    Value *result = la1_eval(state, closure->body_source);

    la1_binding_stack_pop(state->binding_stack);

    assert(state->binding_stack->list == closure->environment);

    restore_binding_stack(state);

    return result;
}

static Bindings *bind_arguments(LinkedList *parameters, ConsCell *arguments) {
    unsigned int count = la1_find_list_size(parameters);

    Bindings *bindings = la1_bindings_create_with_capacity(count);

    LinkedList *parameter = parameters;
    ConsCell *argument = arguments;

    while (parameter != NULL) {
        assert(argument != NULL);

        la1_bindings_add(bindings, parameter->item, argument->item);

        parameter = parameter->next;
        argument = la1_cons_next(argument);
    }

    return bindings;
}

static void set_binding_stack(LA1_State *state, LinkedList *new_list) {
    state->past_stacks =
        la1_list(state->binding_stack->list, state->past_stacks);
    state->binding_stack->list = new_list;
}

static void restore_binding_stack(LA1_State *state) {
    LinkedList *current = state->past_stacks;

    state->binding_stack->list = current->item;
    state->past_stacks = current->next;

    free(current);
}
