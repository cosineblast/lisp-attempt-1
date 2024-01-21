//
// Created by cosineblast on 09/02/23.
//

#include "parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int parse_value(LexerState *state, ParseValue **output);

int parse_list(LexerState *state, LinkedList **output);

int parse_number(LexerState *state, long *output);

int parse_symbol(LexerState *state, char **output);

int la1_parse_value_from_stdin(ParseValue **output) {
    LexerState *state = la1_create_lexer_state();

    la1_lex(state);

    if (!parse_value(state, output)) {
        la1_free_lexer_state(state);
        return 0;
    }

    la1_free_lexer_state(state);
    return 1;
}

static int parse_list_into_value(LexerState *state, ParseValue **output) {
    LinkedList *list;

    if (parse_list(state, &list)) {
        *output = la1_list_into_parse_value(list);
        return 1;
    }

    return 0;
}

static void parse_number_into_value(LexerState *state, ParseValue **output) {
    long value;

    parse_number(state, &value);

    *output = la1_number_into_parse_value(value);
}

static void parse_symbol_into_value(LexerState *state, ParseValue **output) {
    char *value;

    parse_symbol(state, &value);

    *output = la1_symbol_into_parse_value(value);
}

int parse_value(LexerState *state, ParseValue **output) {
    if (state->current_token.type == LEXER_NO_TOKEN) {
        return 0;
    }

    assert(state->current_token.type != LEXER_NO_TOKEN);

    switch (state->current_token.type) {
        case '(':
            if (!parse_list_into_value(state, output)) return 0;
            break;

        case LEXER_NUMBER_TOKEN:
            parse_number_into_value(state, output);
            break;

        case LEXER_SYMBOL_TOKEN:
            parse_symbol_into_value(state, output);
            break;

        default: {
            fprintf(stderr, "Unexpected token\n.");
            abort();
        }
    }

    return 1;
}

int parse_symbol(LexerState *state, char **output) {
    assert(state->current_token.type == LEXER_SYMBOL_TOKEN);

    *output = state->current_token.value.symbol;

    return 1;
}

int parse_number(LexerState *state, long *output) {
    assert(state->current_token.type == LEXER_NUMBER_TOKEN);

    *output = state->current_token.value.number;

    return 1;
}

int parse_list(LexerState *state, LinkedList **output) {
    la1_lex(state);

    if (state->current_token.type == ')') {
        *output = NULL;
        return 1;
    }

    ParseValue *value;
    LinkedList *next;

    if (!parse_value(state, &value)) {
        return 0;
    }

    if (!parse_list(state, &next)) {
        return 0;
    }

    *output = la1_list(value, next);

    return 1;
}
