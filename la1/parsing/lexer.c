//
// Created by figurantpp on 09/02/23.
//

#include "lexer.h"
#include "../common/alloc.h"
#include "../common/die.h"
#include <stdlib.h>

#include <stdio.h>
#include <ctype.h>

#define OK 0
#define END 1

static int is_space(int current);

static int lex_ws();


int lex_number(LexerState *state);

int lex_symbol(LexerState *state);

LexerState *la1_create_lexer_state() {

    LexerState *state = la1_malloc(sizeof(*state));
    state->current_token.type = LEXER_NO_TOKEN;

    return state;
}

static int lex_ws() {
    int current = getchar();

    while (is_space((char) current)) {
        current = getchar();
    }
    return current;
}

static int is_space(int character) {
    return character == ' ' || character == '\n' || character == '\t';
}

static int is_symbol_char(int character) {
    return !is_space(character)
           && character != '('
           && character != ')';
}

static int is_digit(int character) {
    return isdigit(character);
}

int la1_lex(LexerState *state) {

    int current = lex_ws();

    if (current == EOF) {
        state->current_token.type = LEXER_NO_TOKEN;
        return END;
    }

    if (current == '(' || current == ')') {
        state->current_token.type = current;
        return OK;
    }

    ungetc(current, stdin);

    if (is_digit(current)) {
        return lex_number(state);
    }

    if (is_symbol_char(current)) {
        return lex_symbol(state);

    }

    fprintf(stderr, "Unrecognized token character %c\n", current);
    die("Sytnax Error");
}


int lex_number(LexerState *state) {

    // 2^63 -1 has 19 digits, + 1 for \0

    long result = 0;

    scanf("%ld", &result);


    state->current_token.type = LEXER_NUMBER_TOKEN;
    state->current_token.value.number = result;

    return OK;
}

int lex_symbol(LexerState *state) {

    state->current_token.type = LEXER_SYMBOL_TOKEN;

    state->current_token.value.symbol = la1_malloc(LEXER_TOKEN_SYMBOL_LIMIT + 1);

    int index = 0;

    int current = getchar();

    while (is_symbol_char(current)) {

        if (index >= LEXER_TOKEN_SYMBOL_LIMIT) {
            fprintf(stderr, "Symbol is too big.\n");
            abort();
        }

        state->current_token.value.symbol[index] = (char) current;

        index += 1;

        current = getchar();
    }

    state->current_token.value.symbol[index] = 0;

    ungetc(current, stdin);

    return OK;
}


void la1_free_lexer_state(LexerState *state) {

    free(state);

}
