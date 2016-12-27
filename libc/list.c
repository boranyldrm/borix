//
// Created by boran on 27.12.2016.
//

#include "../include/list.h"

void init_list_head (list *head) {
    head->next = head;
    head->prev = head;
}

void list_add (list *head, list *entry) {
    entry->next = head;
    entry->prev = head;
    if (head->next) {
        head->next->prev = entry;
        head->next = entry;
    }
}

void list_del (list *del_ptr) {
    if(del_ptr->prev && del_ptr->next) {
        del_ptr->next->prev = del_ptr->prev;
        del_ptr->prev->next = del_ptr->next;
        del_ptr->next = NULL;
        del_ptr->prev = NULL;
    }
}

bool list_is_empty (const list *head) {
    return (bool) (head->next == head);
}
