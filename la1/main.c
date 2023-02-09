#include <stdio.h>
#include "parsing/lexer.h"

int main() {

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



    return 0;
}
