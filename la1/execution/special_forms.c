//
// Created by figurantpp on 09/02/23.
//

#include "special_forms.h"
#include "../common/die.h"

Value *la1_if_special_form(LA1_State *state, LinkedList *arguments) {

    unsigned int size = la1_find_list_size(arguments);

    if (size != 3) {
        die("if: Expected 3 arguments");
    }

    Value *predicate_value = la1_eval(state, arguments->content);

    if (predicate_value == state->special_forms.nil) {
        return la1_eval(state, arguments->next->next->content);
    }
    else {
        return la1_eval(state, arguments->next->content);
    }
}

Value *la1_lambda_special_form(LA1_State *state, LinkedList *arguments) {

    unsigned int size = la1_find_list_size(arguments);

    if (size != 3) {
        die("lambda: Expected 3 arguments");
    }

    die("Lambda is not implemented");
}

Value *la1_quote_special_form(LA1_State *state, LinkedList *arguments) {

    (void) state;

    unsigned int size = la1_find_list_size(arguments);

    if (size != 1) {
        die("quote: Expected 1 arguments.");
    }

    return arguments->content;
}

Value *la1_let_special_form(LA1_State *state, LinkedList *arguments) {
    die("let is not implemented");
}

Value *la1_nil_special_form(LA1_State *state, LinkedList *arguments) {
    die("Illegal attempt to call to nil.");
}
