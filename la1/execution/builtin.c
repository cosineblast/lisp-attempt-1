//
// Created by figurantpp on 09/02/23.
//

#include "builtin.h"

#include "../common/die.h"
#include "execution.h"

int equals(Value *p_value, Value *p_value_1);

int list_equals(LinkedList *p_list, LinkedList *p_list_1);

long accumulate_minus(LinkedList *arguments);

Value *la1_builtin_list(LA1_State *state, LinkedList *arguments, void *extra) {
    (void)state;
    (void)extra;

    return la1_list_into_value(state, arguments);
}

Value *la1_builtin_cons(LA1_State *state, LinkedList *arguments, void *extra) {
    (void)state;
    (void)extra;

    la1_expect_size(arguments, 2);

    Value *list = arguments->next->content;

    la1_expect_type(list, LA1_VALUE_LIST);

    return la1_list_into_value(
        state, la1_cons(arguments->content, list->content.list));
}

Value *la1_builtin_first(LA1_State *state, LinkedList *arguments, void *extra) {
    (void)state;
    (void)extra;

    la1_expect_size(arguments, 1);

    Value *list = arguments->content;

    la1_expect_type(list, LA1_VALUE_LIST);

    if (list->content.list == NULL) {
        return state->nil;
    } else {
        return list->content.list->content;
    }
}

Value *la1_builtin_rest(LA1_State *state, LinkedList *arguments, void *extra) {
    (void)state;
    (void)extra;

    la1_expect_size(arguments, 1);

    Value *list = arguments->content;

    la1_expect_type(list, LA1_VALUE_LIST);

    if (list->content.list == NULL) {
        return state->nil;
    } else {
        return la1_list_into_value(state, list->content.list->next);
    }
}

Value *la1_builtin_plus(LA1_State *state, LinkedList *arguments, void *extra) {
    (void)extra;

    long result = 0;

    LinkedList *current = arguments;

    while (current != NULL) {
        Value *value = current->content;

        la1_expect_type(value, LA1_VALUE_NUMBER);

        result += value->content.number;

        current = current->next;
    }

    return la1_number_into_value(state, result);
}

Value *la1_builtin_minus(LA1_State *state, LinkedList *arguments, void *extra) {
    (void)state;
    (void)extra;

    unsigned int size = la1_find_list_size(arguments);

    if (size == 0) {
        la1_die("minus: Expected at least one argument");
    }
    if (size == 1) {
        Value *value = arguments->content;

        la1_expect_type(value, LA1_VALUE_NUMBER);

        return la1_number_into_value(state, -(value->content.number));
    } else {
        long result = accumulate_minus(arguments);

        return la1_number_into_value(state, result);
    }
}

long accumulate_minus(LinkedList *arguments) {
    long result = ((Value *)arguments->content)->content.number;

    LinkedList *current = arguments->next;

    while (current != NULL) {
        Value *value = current->content;

        la1_expect_type(value, LA1_VALUE_NUMBER);

        result -= value->content.number;

        current = current->next;
    }
    return result;
}

Value *la1_builtin_times(LA1_State *state, LinkedList *arguments, void *extra) {
    (void)state;
    (void)extra;

    long result = 1;

    LinkedList *current = arguments;

    while (current != NULL) {
        Value *value = current->content;

        la1_expect_type(value, LA1_VALUE_NUMBER);

        result *= value->content.number;

        current = current->next;
    }

    return la1_number_into_value(state, result);
}

Value *la1_builtin_equals(LA1_State *state, LinkedList *arguments,
                          void *extra) {
    (void)extra;

    la1_expect_size(arguments, 2);

    Value *left_argument = arguments->content;
    Value *right_argument = arguments->next->content;

    int result = equals(left_argument, right_argument);

    return result ? state->true_value : state->false_value;
}

int equals(Value *left_value, Value *right_value) {
    if (left_value->type != right_value->type) {
        return 0;
    }

    switch (left_value->type) {
        case LA1_VALUE_NUMBER:
            return left_value->content.number == right_value->content.number;

        case LA1_VALUE_LIST:
            return list_equals(left_value->content.list,
                               right_value->content.list);

        case LA1_VALUE_SYMBOL:
            return left_value->content.symbol == right_value->content.symbol;

        case LA1_VALUE_CLOSURE:
            return 0;
    }

    return 0;
}

int list_equals(LinkedList *left, LinkedList *right) {
    if (left == right) {
        return 1;
    }

    if (left == NULL || right == NULL) {
        return 0;
    }

    if (!equals(left->content, right->content)) {
        return 0;
    }

    return list_equals(left->next, right->next);
}

Value *la1_builtin_emptyp(LA1_State *state, LinkedList *arguments,
                          void *extra) {
    (void)extra;

    la1_expect_size(arguments, 1);

    Value *argument = arguments->content;

    int result =
        argument->type == LA1_VALUE_LIST && argument->content.list == NULL;

    return result ? state->true_value : state->false_value;
}
