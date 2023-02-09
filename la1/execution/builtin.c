//
// Created by figurantpp on 09/02/23.
//

#include "builtin.h"
#include "../common/die.h"

Value *la1_builtin_list(LA1_State *state, LinkedList *arguments, void *extra) {
    return la1_list_into_value(arguments);
}

Value *la1_builtin_cons(LA1_State *state, LinkedList *arguments, void *extra) {

    la1_expect_size(arguments, 2);

    Value *list = arguments->next->content;

    la1_expect_type(list, LA1_VALUE_LIST);

    return la1_list_into_value(la1_cons(arguments->content, list->content.list));
}

Value *la1_builtin_plus(LA1_State *state, LinkedList *arguments, void *extra) {

    long result = 0;

    LinkedList *current = arguments;

    while (current != NULL) {

        Value *value = current->content;

        la1_expect_type(value, LA1_VALUE_NUMBER);

        result += value->content.number;

        current = current->next;
    }

    return la1_number_into_value(result);
}
