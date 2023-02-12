//
// Created by figurantpp on 09/02/23.
//

#include <stdlib.h>

#include "execution.h"

LinkedList *realize_list(LA1_State *state, LinkedList *p_list);

Value *realize_symbol(LA1_State *state, char *input_symbol);

Value *la1_realize_parse_value(LA1_State *state, ParseValue *value) {
    switch (value->type) {
        case PARSE_VALUE_LIST:
            return la1_list_into_value(
                state, realize_list(state, value->content.list));

        case PARSE_VALUE_NUMBER:
            return la1_number_into_value(state, value->content.number);

        case PARSE_VALUE_SYMBOL:
            return realize_symbol(state, value->content.symbol);
    }

    abort();
}

LinkedList *realize_list(LA1_State *state, LinkedList *list) {
    if (list == NULL) {
        return NULL;
    } else {
        return la1_cons(la1_realize_parse_value(state, list->content),
                        realize_list(state, list->next)

        );
    }
}

Value *realize_symbol(LA1_State *state, char *input_symbol) {
    KnownSymbol symbol = la1_intern(state, input_symbol);

    if (symbol == state->nil->content.symbol) {
        return state->nil;
    } else if (symbol == state->false_value) {
        return state->false_value;
    } else if (symbol == state->true_value) {
        return state->true_value;
    } else {
        return la1_symbol_into_value(state, symbol);
    }
}
