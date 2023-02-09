//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_LIST_H
#define LATTEMPT_LIST_H

struct LinkedList {
    void *content;
    struct LinkedList *next;
};

typedef struct LinkedList LinkedList;

#endif //LATTEMPT_LIST_H
