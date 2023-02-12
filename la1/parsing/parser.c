//
// Created by figurantpp on 09/02/23.
//

#include "parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

ParseValue *parse_value(LexerState *pState);

LinkedList *parse_list(LexerState *p_state);

long parse_number(LexerState *p_state);

char *parse_symbol(LexerState *p_state);

ParseValue *la1_parse_value_from_stdin() {
    LexerState *state = la1_create_lexer_state();

    la1_lex(state);

    ParseValue *result = parse_value(state);

    la1_free_lexer_state(state);

    return result;
}

ParseValue *parse_value(LexerState *state) {
    if (state->current_token.type == LEXER_NO_TOKEN) {
        fprintf(stderr, "EOF\n");
        exit(0);
    }

    assert(state->current_token.type != LEXER_NO_TOKEN);

    switch (state->current_token.type) {
        case '(':
            return la1_list_into_parse_value(parse_list(state));

        case LEXER_NUMBER_TOKEN:
            return la1_number_into_parse_value(parse_number(state));

        case LEXER_SYMBOL_TOKEN:
            return la1_symbol_into_parse_value(parse_symbol(state));

        default:
            fprintf(stderr, "Unexpected token\n.");
            abort();
    }
}

char *parse_symbol(LexerState *state) {
    assert(state->current_token.type == LEXER_SYMBOL_TOKEN);

    return state->current_token.value.symbol;
}

long parse_number(LexerState *state) {
    assert(state->current_token.type == LEXER_NUMBER_TOKEN);

    return state->current_token.value.number;
}

LinkedList *parse_list(LexerState *state) {
    la1_lex(state);

    if (state->current_token.type == ')') {
        return NULL;
    }

    ParseValue *value = parse_value(state);

    return la1_cons(value, parse_list(state));
}
