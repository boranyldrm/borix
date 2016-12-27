//
// Created by boran on 07.11.2016.
//

#ifndef BORIX_LIST_H
#define BORIX_LIST_H

#include "types.h"

typedef struct LIST {
    struct LIST *next;
    struct LIST *prev;
} list;

#define LIST_HEAD_INIT(name) { &(name), &(name); }

#define LIST_HEAD(name) \
		struct list_head name = LIST_HEAD_INIT(name)

void init_list_head (list *head);

void list_add (list *head, list *entry);

void list_del (list *del_ptr);

bool list_is_empty (const list *head);


#define list_entry(ptr, type, member) \
		(type*) ((char*) ptr - (char*) &((type*)0)->member)

#define list_first_entry(head, type, member) \
		list_entry((head)->next, type, member)

#define list_for_each(p, head) \
		for (p = (head)->next; p != (head); p = p->next)

#define list_for_each_safe(p, n, head) \
		for (p = (head)->next, n = p->next; p != (head); p = n, n = n->next)

#define list_for_each_entry(p, head, member)				\
		for (p = list_entry((head)->next, typeof(*p), member);		\
			 &p->member != (head);					\
			 p = list_entry(p->member.next, typeof(*p), member))	\

#endif //BORIX_LIST_H
