#include <stdio.h>
#include "parsing/lexer.h"
#include "parsing/parser.h"
#include "execution/execution.h"

void test_lexer() {

    LexerState *state = la1_create_lexer_state();

    while (la1_lex(state) == 0) {

        switch (state->current_token.type) {
            case LEXER_NO_TOKEN:
                printf("none");
                break;

            case LEXER_OPEN_PARENTHESIS_TOKEN:
                printf("open_parenthesis");
                break;

            case LEXER_CLOSING_PARENTHESIS_TOKEN:
                printf("closing_parenthesis");
                break;

            case LEXER_SYMBOL_TOKEN:
                printf("symbol(%s)", state->current_token.value.symbol);
                break;

            case LEXER_NUMBER_TOKEN:
                printf("number(%ld)", state->current_token.value.number);
                break;
        }

        printf("\n");

    }

    la1_free_lexer_state(state);
}

void test_parser() {

    ParseValue *value = la1_parse_value_from_stdin();

    la1_dump_parse_value(value);
}

void test_execution() {


    LA1_State *state = la1_create_la1_state();

    for (;;) {
        ParseValue *parse_value = la1_parse_value_from_stdin();

        Value *value = la1_realize_parse_value(state, parse_value);

        la1_free_parse_value(parse_value);

        la1_display_value(la1_eval(state, value));
    }

}

int main() {
    test_execution();

}
