//
// Created by figurantpp on 09/02/23.
//

#include "execution.h"
#include "../common/alloc.h"
#include "special_forms.h"
#include "../common/die.h"

#include <assert.h>
#include <string.h>

void push_special_forms(LA1_State *p_state);


int try_eval_special_form(LA1_State *state, LinkedList *list, Value **result);

Value *eval_list(LA1_State *p_state, LinkedList *p_list);

Value *eval_symbol(LA1_State *p_state, KnownSymbol symbol);

void initialize_prelude(LA1_State *p_state);

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

void initialize_prelude(LA1_State *state) {

    la1_bindings_add(
            state->global_bindings,
            la1_intern(state, "n"),
            la1_number_into_value(10)
    );

}

void push_special_forms(LA1_State *state) {

    assert(state);

    SpecialFormSymbols *symbols = &state->special_forms;

    symbols->if_symbol = la1_intern(state, "if");
    symbols->lambda_symbol = la1_intern(state, "lambda");
    symbols->quote_symbol = la1_intern(state, "quote");
    symbols->let_symbol = la1_intern(state, "let");
    symbols->nil_symbol = la1_intern(state, "nil");

    symbols->nil = la1_symbol_into_value(symbols->nil_symbol);

    SpecialFormEntry table[SPECIAL_FORM_COUNT] = {
            {symbols->if_symbol,     la1_if_special_form},
            {symbols->lambda_symbol, la1_lambda_special_form},
            {symbols->quote_symbol,  la1_quote_special_form},
            {symbols->let_symbol,    la1_let_special_form},
            {symbols->nil_symbol,    la1_nil_special_form},
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

        case LA1_VALUE_FUNCTION:
            die("not implemented");
    }

    die("Not implemented");

}

Value *eval_symbol(LA1_State *state, KnownSymbol symbol) {

    if (symbol == state->special_forms.nil_symbol) {
        return state->special_forms.nil;
    }

    Value *result;

    if (la1_binding_stack_lookup(state->current_binding_stack, symbol, &result)) {
        return result;
    }

    if (la1_bindings_lookup(state->global_bindings, symbol, &result)) {
        return result;
    }

    die("Symbol not found.");
}

Value *eval_list(LA1_State *state, LinkedList *list) {

    Value *first_element = list->content;

    if (first_element->type != LA1_VALUE_SYMBOL) {
        die("Cannot call given value");
    }

    Value *result;

    if (try_eval_special_form(state, list, &result)) {
        return result;
    } else {
        die("Function calls are not implemented");
    }
}

int try_eval_special_form(LA1_State *state, LinkedList *list, Value **result) {

    assert(state && list && result);
    Value *first_value = list->content;
    assert(first_value->type == LA1_VALUE_SYMBOL);

    KnownSymbol symbol = first_value->content.symbol;

    for (int i = 0; i < SPECIAL_FORM_COUNT; i++) {
        if (state->special_form_table[i].symbol == symbol) {

            *result = state->special_form_table[i].function(state, list->next);
            return 1;
        }
    }

    return 0;
}
