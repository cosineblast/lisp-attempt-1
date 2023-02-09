//
// Created by figurantpp on 09/02/23.
//

#include "execution.h"
#include "../common/alloc.h"
#include "special_forms.h"
#include "../common/die.h"
#include "builtin.h"

#include <assert.h>
#include <string.h>

void push_special_forms(LA1_State *p_state);


int try_eval_special_form(LA1_State *state, LinkedList *list, Value **result);

Value *eval_list(LA1_State *p_state, LinkedList *p_list);

Value *eval_symbol(LA1_State *p_state, KnownSymbol symbol);

void initialize_prelude(LA1_State *p_state);

Value *eval_function_call(LA1_State *state, LinkedList *arguments);

Value *apply_symbol(LA1_State *state, const LinkedList *arguments, KnownSymbol symbol);

LinkedList *evaluate_children(LA1_State *state, struct LinkedList *list);

LA1_State *la1_create_la1_state() {

    LA1_State *state = la1_malloc(sizeof(*state));

    state->current_binding_stack = la1_binding_stack_create();
    state->global_bindings = la1_bindings_create();
    state->interned_symbols = la1_empty_list();
    state->past_binding_stacks = la1_empty_list();

    push_special_forms(state);

    initialize_prelude(state);

    return state;
}

static Value *closure_for(ClosureFunction *function) {
    return la1_closure_into_value(la1_create_closure(function, NULL));
}

void initialize_prelude(LA1_State *state) {

    struct {
        KnownSymbol symbol;
        ClosureFunction *function;
    } builtin_functions[]
            = {
                    {la1_intern(state, "cons"), la1_builtin_cons},
                    {la1_intern(state, "list"), la1_builtin_list},
                    {la1_intern(state, "+"),    la1_builtin_plus}
            };

    for (int i = 0; i < sizeof(builtin_functions) / sizeof(builtin_functions[0]); i++) {

        la1_bindings_add(
                state->global_bindings,
                builtin_functions[i].symbol,
                closure_for(builtin_functions[i].function)
        );
    }


}

void push_special_forms(LA1_State *state) {

    assert(state);

    state->nil = la1_symbol_into_value(la1_intern(state, "nil"));

    SpecialFormEntry table[SPECIAL_FORM_COUNT] = {
            [LA1_SPECIAL_FORM_IF] = {la1_intern(state, "if"), la1_if_special_form},
            [LA1_SPECIAL_FORM_LAMBDA] = {la1_intern(state, "lambda"), la1_lambda_special_form},
            [LA1_SPECIAL_FORM_QUOTE] = {la1_intern(state, "quote"), la1_quote_special_form},
            [LA1_SPECIAL_FORM_LET] = {la1_intern(state, "let"), la1_let_special_form},
            [LA1_SPECIAL_FORM_DO] = {la1_intern(state, "do"), la1_do_special_form},
            [LA1_SPECIAL_FORM_NIL] = {la1_intern(state, "nil"), la1_nil_special_form},
    };

    assert(sizeof(table) == sizeof(state->special_form_table));

    memcpy(state->special_form_table, table, sizeof(table));
}


KnownSymbol la1_intern(LA1_State *state, const char *symbol) {

    LinkedList *current = state->interned_symbols;

    while (current != NULL) {
        if (strcmp(symbol, current->content) == 0) {
            return current->content;
        }

        current = current->next;
    }

    state->interned_symbols = la1_cons(strdup(symbol), state->interned_symbols);

    return state->interned_symbols->content;
}

Value *la1_eval(LA1_State *state, Value *value) {

    assert(state && value);

    switch (value->type) {
        case LA1_VALUE_NUMBER:
            return value;

        case LA1_VALUE_LIST:
            return eval_list(state, value->content.list);

        case LA1_VALUE_SYMBOL:
            return eval_symbol(state, value->content.symbol);

        case LA1_VALUE_CLOSURE:
            die("not implemented");
    }

    die("Not implemented");

}

static int lookup_variable(LA1_State *state, KnownSymbol symbol, Value **result) {

    if (la1_binding_stack_lookup(state->current_binding_stack, symbol, result)) {
        return 1;
    }

    if (la1_bindings_lookup(state->global_bindings, symbol, result)) {
        return 1;
    }

    return 0;
}

Value *eval_symbol(LA1_State *state, KnownSymbol symbol) {

    if (symbol == state->nil->content.symbol) {
        return state->nil;
    }

    Value *result;

    if (lookup_variable(state, symbol, &result)) {
        return result;
    }

    die("Symbol not found.");
}

Value *apply(LA1_State *state, LinkedList *arguments);

Value *eval_list(LA1_State *state, LinkedList *list) {

    if (list == NULL) {
        return la1_list_into_value(NULL);
    }

    Value *first_element = list->content;

    if (first_element->type == LA1_VALUE_NUMBER) {
        die("Cannot call number");
    }

    Value *result;

    if (try_eval_special_form(state, list, &result)) {
        return result;
    } else {
        return apply(state, evaluate_children(state, list));
    }
}


LinkedList *evaluate_children(LA1_State *state, struct LinkedList *list) {

    if (list == NULL) {
        return NULL;
    }

    LinkedList *result = la1_cons(la1_eval(state, list->content), NULL);

    LinkedList *current_argument = list->next;
    LinkedList *result_end = result;

    while (current_argument != NULL) {

        result_end->next = la1_cons(la1_eval(state, current_argument->content), NULL);
        result_end = result_end->next;

        current_argument = current_argument->next;
    }

    return result;
}

Value *apply(LA1_State *state, LinkedList *call) {

    Value *target_value = call->content;

    if (target_value->type != LA1_VALUE_CLOSURE) {
        die("Cannot apply non-closure value.");
    }

    Closure *closure = target_value->content.closure;

    return closure->function(state, call->next, closure->extra);
}

Value *apply_symbol(LA1_State *state, const LinkedList *arguments, KnownSymbol symbol) {
    Value *result;

    if (lookup_variable(state, symbol, &result)) {
        if (result->type != LA1_VALUE_CLOSURE) {
            die("Tried to call uncallable type");
        }

        Closure *closure = result->content.closure;

        return closure->function(state, arguments->next, closure->extra);
    } else {
        die("No closure associated with variable");
    }
}

Value *la1_apply_data(LA1_State *state, DataClosure *closure, LinkedList *arguments) {
    die("Apply not implemented");
}

int try_eval_special_form(LA1_State *state, LinkedList *list, Value **result) {

    assert(state && list && result);
    Value *first_value = list->content;

    if (first_value->type != LA1_VALUE_SYMBOL) {
        return 0;
    }

    KnownSymbol symbol = first_value->content.symbol;

    for (int i = 0; i < SPECIAL_FORM_COUNT; i++) {
        if (state->special_form_table[i].symbol == symbol) {

            *result = state->special_form_table[i].function(state, list->next);
            return 1;
        }
    }

    return 0;
}
