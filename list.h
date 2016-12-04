#ifndef LIST_H
#define LIST_H
#include <stdint.h>
#include "util.h"
#include <stdlib.h>
#include <string.h>
/*
 * That file include two Data Structure
 * Double link list
 * Use "create_list" to CREATE an empty list
 * Use "list_entry" to enter the real struct you want
 * Offer three kinds of foreach-iterator to Manipulate the list:
 *          // Step from head to tail(forward) or tail to head(backward)
 *          for_each_list_forward, for_each_list_backward
 *          // Step from a Specific position to tail(forward) or head(backward)
 *          for_each_list_mid_forward, for_each_list_mid_backward
 *          // Step from a Specific position to a Specific destination
 *          for_each_list_subset_forward, for_each_list_subset_backward
 * Offer four ways to Manipulate the list:
 *          list_add_after, list_add_before, list_add_head, remove_from_list
 * Use For Real
 */

struct list{
    struct list * prev;
    struct list * next;
};
typedef struct list * list_t;


#define LIST_INIT(list_head) \
    do { \
        (list_head)->prev = (list_head);\
        (list_head)->next = (list_head);\
    }while(0)

/*
 * Create an empty list
 */
#define create_list(type, member) ({\
    type * tmp = calloc(1, sizeof(type));\
    LIST_INIT(&(tmp->member));tmp;\
    })

/*
 * Use That For real object address return, which is contain the struct list object
 */
#define list_entry(list_head, type, member) ({\
        (type *)( (char *)list_head - offsetof(type, member));})

#define list_empty(list_head) \
    (list_head->prev == list_head && list_head->next == list_head)

#define _for_each_list(_list, _man, pos) \
    for (_man = (_list)->pos; _man != _list; _man = (_man)->pos)

/*
 * Use That when you get the head of the list
 * Iterate From Begin to the end
 */
#define for_each_list_forward(list_head, man) \
    _for_each_list(list_head, man, next)

/*
 * Iterate From end to begin
 */
#define for_each_list_backward(list_head, man) \
    _for_each_list(list_head, man, prev)

/*
 * @start_pos Start position
 * @end_pos   End position
 * @man       Manipulate storage, use it to iterate the list
 * @dir       Direction of the iteration
 */
#define _for_each_list_subset(start_pos, end_pos, man, dir)\
    for(man = start_pos; man != end_pos; man = man->dir)

/*
 * Iterate From the position,between head and tail, to the end
 * 从中间向前遍历
 * @mid_pos position between head and tail
 */
#define for_each_list_mid_forward(mid_pos, list_head, man)\
    _for_each_list_subset(mid_pos, list_head, man, next)
/*
 * Iterate From the position,between head and tail, to the start
 * 从中间向后遍历
 * @mid_pos position between head and tail
 */
#define for_each_list_mid_backward(mid_pos, list_head, man)\
    _for_each_list_subset(mid_pos, list_head, man, prev)

/*
 * 从头至尾遍历某个子集
 */
#define for_each_list_subset_forward(start_pos, end_pos, man)\
    _for_each_list_subset(start_pos, end_pos, man, next)

/*
 * 从尾至头遍历某个子集
 */
#define for_each_list_subset_backward(start_pos, end_pos, man)\
    _for_each_list_subset(end_pos, start_pos, man, prev)

/*
 * Add _add_node to the _add_pos list
 */
#define _list_add_inner(_add_pos, _add_node) \
    ({do {\
        struct list * tmp = _add_pos;\
        struct list * tmp_s = _add_node;\
        tmp_s->next = tmp->next;\
        tmp_s->prev = tmp;\
        tmp->next->prev = tmp_s;\
        tmp->next = tmp_s;\
    } while(0);_add_node;})

/*
 * Add add_node after the add_pos
 */
static inline struct list * list_add_after(struct list * add_pos, struct list * add_node) {
    return _list_add_inner(add_pos, add_node);
}

/*
 * Add add_node before the add_pos
 */
static inline struct list * list_add_before(struct list * add_pos, struct list * add_node) {
    return _list_add_inner(add_pos->prev, add_node);
}

/*
 * Add add_node to the front
 * You must get the list head
 */
static inline struct list * list_add_head(struct list * list_head, struct list * add_node) {
    return _list_add_inner(list_head, add_node);
}

/*
 * Add add_node to the tail
 */
static inline void list_add_tail(struct list * list_head, struct list * add_node) {

}

/*
 * fast remove from the list,
 * But the memry free should be notice.
 */
static inline struct list * remove_from_list(struct list * del_node) {
    if(unlikely(list_empty(del_node)))
        return NULL;
    struct list * before = del_node->prev;
    struct list * after  = del_node->next;
    before->next = after;
    after->prev  = before;
    del_node->next = del_node->prev = NULL;
    return del_node;
}

/*
 * Duplicate an exist list
 */
#define DUP_LIST(list_head, type, member) ({\
    type* new_list = calloc(1, sizeof(type)); \
    LIST_INIT(&(new_list->member));\
    type * head = new_list; \
    do {\
        struct list * man = NULL; \
        for_each_list_forward(list_head, man){\
            const type * tmp =  list_entry(man, type, member);\
            type * new_tmp = malloc(sizeof(type)); \
            memmove(new_tmp, tmp, sizeof(type)); \
            list_add_after(&(new_list->member), &(new_tmp->member));\
            new_list = new_tmp; \
        }\
    }while(0);head;})

#endif // LIST_H

