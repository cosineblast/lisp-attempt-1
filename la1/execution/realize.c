//
// Created by figurantpp on 09/02/23.
//

#include "execution.h"

#include <stdlib.h>

LinkedList *realize_list(LA1_State *state, LinkedList *p_list);

Value *realize_symbol_or_nil(LA1_State *state, char *input_symbol);

Value *la1_realize_parse_value(LA1_State *state, ParseValue *value) {

    switch (value->type) {

        case PARSE_VALUE_LIST:
            return la1_list_into_value(realize_list(state, value->content.list));

        case PARSE_VALUE_NUMBER:
            return la1_number_into_value(value->content.number);

        case PARSE_VALUE_SYMBOL:
            return realize_symbol_or_nil(state, value->content.symbol);
    }

    abort();
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