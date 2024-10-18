#ifndef __LIST_H__
#define __LIST_H__
#include<stdio.h>

struct list_head {
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
    // TODO: some code goes here
}

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

#define container_of(ptr, type, member) ({                  \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})

#define offsetof(type, member) ((size_t) &((type *)0)->member)

#define list_empty(head) (/* TODO: some code goes here */)

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	// TODO: some code goes here
}

#define list_for_each(pos, head)
// TODO: some code goes here

#endif
