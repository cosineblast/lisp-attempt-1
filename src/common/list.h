//
// Created by cosineblast on 09/02/23.
//

#ifndef LATTEMPT_LIST_H
#define LATTEMPT_LIST_H

struct LinkedList {
    void *item;
    struct LinkedList *next;
};

typedef struct LinkedList LinkedList;

LinkedList *la1_list(void *content, LinkedList *next);

// todo: adopt usage of this.
LinkedList *la1_empty_list();

unsigned int la1_find_list_size(LinkedList *list);

#endif  // LATTEMPT_LIST_H
