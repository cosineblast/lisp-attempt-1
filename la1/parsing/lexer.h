//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_LEXER_H
#define LATTEMPT_LEXER_H

#define LEXER_TOKEN_SYMBOL_LIMIT 256

enum LexerTokenType {
    LEXER_NO_TOKEN  = 0,
    LEXER_OPEN_PARENTHESIS_TOKEN = '(',
    LEXER_CLOSING_PARENTHESIS_TOKEN = ')',
    LEXER_SYMBOL_TOKEN = 300,
    LEXER_NUMBER_TOKEN = 301,
};

typedef enum LexerTokenType LexerTokenType;


struct LexerToken {
    LexerTokenType type;

    union LexerTokenValue {
        long number;
        char *symbol;
    } value;
};

typedef struct LexerToken LexerToken;

struct LexerState {
    LexerToken current_token;
};

typedef struct LexerState LexerState;

/**
 * Initializes LexerState from stdin.
 */
LexerState *la1_create_lexer_state();

/**
 * Tries to tokenize a symbol from the current state.
 * Returns 0 if ok, nonzero if error or EOF.
 */
int la1_lex(LexerState *state);

/**
 * Frees lexer state.
 */
void la1_free_lexer_state(LexerState *state);



#endif //LATTEMPT_LEXER_H
