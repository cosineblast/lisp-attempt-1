//
// Created by figurantpp on 09/02/23.
//

#include "execution.h"
#include "../common/alloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void push_special_forms(LA1_State *p_state);

LinkedList *realize_list(LA1_State *state, LinkedList *p_list);

long realize_number(LA1_State *state, long number);

Value *realize_symbol_or_nil(LA1_State *state, char *input_symbol);

int try_eval_special_form(LinkedList *list, Value **value);

LA1_State *la1_create_la1_state() {

    LA1_State *state = la1_malloc(sizeof(*state));

    state->current_binding_stack = la1_binding_stack_create();
    state->global_bindings = la1_bindings_create();
    state->interned_symbols = la1_empty_list();
    state->past_binding_stacks = la1_empty_list();

    push_special_forms(state);

    return state;
}

void push_special_forms(LA1_State *state) {

    assert(state);

    state->special_forms.if_symbol = la1_intern(state, "if");
    state->special_forms.lambda_symbol = la1_intern(state, "lambda");
    state->special_forms.quote_symbol = la1_intern(state, "quote");
    state->special_forms.let_symbol = la1_intern(state, "let");
    state->special_forms.nil_symbol = la1_intern(state, "nil");

    state->special_forms.nil = la1_symbol_into_value(state->special_forms.nil_symbol);
}

Value *la1_realize_parse_value(LA1_State *state, ParseValue *value) {

    switch (value->type) {

        case PARSE_VALUE_LIST:
            return la1_list_into_value(realize_list(state, value->content.list));

        case PARSE_VALUE_NUMBER:
            return la1_number_into_value(realize_number(state, value->content.number));

        case PARSE_VALUE_SYMBOL:
            return realize_symbol_or_nil(state, value->content.symbol);
    }

    abort();
}


long realize_number(LA1_State *state, long number) {
    return number;
}

LinkedList *realize_list(LA1_State *state, LinkedList *list) {

    if (list == NULL) {
        return NULL;
    } else {
        return la1_cons(
                la1_realize_parse_value(state, list->content),
                realize_list(state, list->next)

        );
    }
}

Value *realize_symbol_or_nil(LA1_State *state, char *input_symbol) {

    KnownSymbol symbol = la1_intern(state, input_symbol);

    if (symbol == state->special_forms.nil_symbol) {
        return state->special_forms.nil;
    } else {
        return la1_symbol_into_value(symbol);
    }
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

    if (value->type != LA1_VALUE_LIST || value->content.list == NULL) {
        return value;
    }

    LinkedList *list  = value->content.list;

    Value *first_element = list->content;

    if (first_element->type != LA1_VALUE_SYMBOL) {
        fprintf(stderr, "Cannot call value .\n");
        abort();
    }

    Value *result;

    if (try_eval_special_form(list, &result)) {
        return result;
    }
    else {
        fprintf(stderr, "Not implemented\n");
        abort();
    }
}

int try_eval_special_form(LinkedList *list, Value **value) {
    return 0;
}
