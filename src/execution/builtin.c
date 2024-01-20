//
// Created by figurantpp on 09/02/23.
//

#include <stdio.h>

#include "builtin.h"

#include "../common/die.h"
#include "gc.h"
#include "state.h"
#include "value.h"


int equals(Value *left, Value *right);

int list_equals(ConsCell *left, ConsCell *right);

long accumulate_minus(ConsCell *arguments);

Value *la1_builtin_list(LA1_State *state, ConsCell *arguments, void *extra) {
    (void) state;
    (void) extra;

    // todo: improve this
    // right now we are creating a new value because
    // there is already an existing to-be-collected value
    // pointing at arguments, but functions can't access it.

    ConsCell *result = la1_cons(arguments->item, arguments->next);

    return la1_cons_into_value(state, result);
}

Value *la1_builtin_cons(LA1_State *state, ConsCell *arguments, void *extra) {
    (void) state;
    (void) extra;

    la1_expect_size(arguments, 2);

    Value *value = arguments->item;
    Value *list = la1_cons_next(arguments)->item;

    la1_expect_type(list, LA1_VALUE_CONS);

    return la1_cons_into_value(
            state, la1_cons(value, list));
}

Value *la1_builtin_first(LA1_State *state, ConsCell *arguments, void *extra) {
    (void) state;
    (void) extra;

    la1_expect_size(arguments, 1);

    Value *list = arguments->item;

    la1_expect_type(list, LA1_VALUE_CONS);

    if (list->content.cons == NULL) {
        return state->nil;
    } else {
        return list->content.cons->item;
    }
}

Value *la1_builtin_rest(LA1_State *state, ConsCell *arguments, void *extra) {
    (void) state;
    (void) extra;

    la1_expect_size(arguments, 1);

    Value *list = arguments->item;

    la1_expect_type(list, LA1_VALUE_CONS);

    if (list->content.cons == NULL) {
        return state->nil;
    } else {
        return list->content.cons->next;
    }
}

Value *la1_builtin_plus(LA1_State *state, ConsCell *arguments, void *extra) {
    (void) extra;

    long result = 0;

    ConsCell *current = arguments;

    while (current != NULL) {
        Value *value = current->item;

        la1_expect_type(value, LA1_VALUE_NUMBER);

        result += value->content.number;

        current = la1_cons_next(current);
    }

    return la1_number_into_value(state, result);
}

Value *la1_builtin_less_than(LA1_State *state, ConsCell *arguments, void *extra) {
    (void) extra;

    la1_expect_size(arguments, 2);

    Value *left = arguments->item;
    Value* right = la1_cons_next(arguments)->item;

    la1_expect_type(left, LA1_VALUE_NUMBER);
    la1_expect_type(right, LA1_VALUE_NUMBER);

    if (left->content.number < right->content.number) {
        return state->true_value;
    } else {
        return state->false_value;
    }
}


Value *la1_builtin_greater_than(LA1_State *state, ConsCell *arguments, void *extra) {
    (void) extra;

    la1_expect_size(arguments, 2);

    Value *left = arguments->item;
    Value* right = la1_cons_next(arguments)->item;

    la1_expect_type(left, LA1_VALUE_NUMBER);
    la1_expect_type(right, LA1_VALUE_NUMBER);

    if (left->content.number > right->content.number) {
        return state->true_value;
    } else {
        return state->false_value;
    }
}


Value *la1_builtin_minus(LA1_State *state, ConsCell *arguments, void *extra) {
    (void) state;
    (void) extra;

    unsigned int size = la1_find_cons_list_size(arguments);

    if (size == 0) {
        la1_die("minus: Expected at least one argument");
    }
    if (size == 1) {
        Value *value = arguments->item;

        la1_expect_type(value, LA1_VALUE_NUMBER);

        return la1_number_into_value(state, -(value->content.number));
    } else {
        long result = accumulate_minus(arguments);

        return la1_number_into_value(state, result);
    }
}

long accumulate_minus(ConsCell *arguments) {
    long result = ((Value *) arguments->item)->content.number;

    ConsCell *current = la1_cons_next(arguments);

    while (current != NULL) {
        Value *value = current->item;

        la1_expect_type(value, LA1_VALUE_NUMBER);

        result -= value->content.number;

        current = la1_cons_next(current);
    }
    return result;
}

Value *la1_builtin_times(LA1_State *state, ConsCell *arguments, void *extra) {
    (void) state;
    (void) extra;

    long result = 1;

    ConsCell *current = arguments;

    while (current != NULL) {
        Value *value = current->item;

        la1_expect_type(value, LA1_VALUE_NUMBER);

        result *= value->content.number;

        current = la1_cons_next(current);
    }

    return la1_number_into_value(state, result);
}

Value *la1_builtin_equals(LA1_State *state, ConsCell *arguments,
                          void *extra) {
    (void) extra;

    la1_expect_size(arguments, 2);

    Value *left_argument = arguments->item;
    Value *right_argument = la1_cons_next(arguments)->item;

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

        case LA1_VALUE_CONS:
            return list_equals(left_value->content.cons,
                               right_value->content.cons);

        case LA1_VALUE_SYMBOL:
            return left_value->content.symbol == right_value->content.symbol;

        case LA1_VALUE_CLOSURE:
            return 0;

        case LA1_VALUE_REITERATION:
            return 0;
    }

    return 0;
}

int list_equals(ConsCell *left, ConsCell *right) {
    if (left == right) {
        return 1;
    }

    if (left == NULL || right == NULL) {
        return 0;
    }

    if (!equals(left->item, right->item)) {
        return 0;
    }

    return list_equals(la1_cons_next(left), la1_cons_next(right));
}

Value *la1_builtin_emptyp(LA1_State *state, ConsCell *arguments,
                          void *extra) {
    (void) extra;

    la1_expect_size(arguments, 1);

    Value *argument = arguments->item;

    int result =
        argument->type == LA1_VALUE_CONS && argument->content.cons == NULL;

    return result ? state->true_value : state->false_value;
}

Value *la1_builtin_gc(LA1_State *state, ConsCell *arguments, void *extra) {
    (void)extra;

    la1_expect_size(arguments, 0);

    la1_perform_gc(state);

    return la1_number_into_value(state, (long)state->gc.gc_value_count);
}

Value *la1_builtin_div(LA1_State *state, ConsCell *arguments, void *extra) {
    (void)extra;

    la1_expect_size(arguments, 2);

    Value *first = arguments->item;
    Value *next = la1_cons_next(arguments)->item;

    la1_expect_type(first, LA1_VALUE_NUMBER);
    la1_expect_type(next, LA1_VALUE_NUMBER);

    long result = first->content.number / next->content.number;

    return la1_number_into_value(state, result);
}

Value *la1_builtin_mod(LA1_State *state, ConsCell *arguments, void *extra) {
    (void)extra;

    la1_expect_size(arguments, 2);

    Value *first = arguments->item;
    Value *next = la1_cons_next(arguments)->item;

    la1_expect_type(first, LA1_VALUE_NUMBER);
    la1_expect_type(next, LA1_VALUE_NUMBER);

    long result = first->content.number % next->content.number;

    return la1_number_into_value(state, result);
}

Value *la1_builtin_prn(LA1_State *state, ConsCell *arguments, void *extra) {
    (void)extra;

    la1_expect_size(arguments, 1);

    Value *value = arguments->item;

    la1_display_value(value);
    printf("\n");

    return state->nil;
}


Value *la1_builtin_repeat(LA1_State *state, ConsCell *arguments, void *extra) {
    (void) state;
    (void) extra;

    // why do we copy the arguments node?
    // see: la1_builtin_list

    ConsCell *content = la1_cons(arguments->item, arguments->next);

    return la1_cons_into_reiteration_value(state, content);
}
