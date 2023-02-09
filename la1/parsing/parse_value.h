//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_PARSE_VALUE_H
#define LATTEMPT_PARSE_VALUE_H

#include "../common/list.h"

union ParseValue {
    int type;

    struct {
        int type;
        LinkedList *value;
    } list;

    struct {
        int type;
        long value;
    } number;

    struct {
        int type;
        char *value;
    } symbol;
};

typedef union ParseValue ParseValue;

#endif //LATTEMPT_PARSE_VALUE_H
