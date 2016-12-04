#ifndef STACK_H
#define STACK_H
#include "list.h"

/*
 * That is a stack data structure
 * list_head                        list_head->prev
 * stack_end                        stack_top
 *     |                               |
 *     V                               V
 *   [S1]<==>[S2]<==>[S3]<==>[S4]<...>[Sn]
 *    ^                                ^
 *    |================================|
 * Offer three ways to Create stack: create_stack, create_stack_from_list
 *                                   create_stack_from_list_ex
 * Offer three Manipulation : stack_push, stack_pop, stack_top
 * Just put the "stack_elem" into your own struct ,and it will work
 * Example:
 * struct your_struct {
 *         int value;
 *         struct stack_elem node; // That is Important.
 * };
 * struct stack * stack_yours = create_stack ();
 * struct your_struct * something = create_your_struct();
 * stack_push (stack_yours, &(something->node));
 * struct your_struct * something_shadow = stack_pop (stack_yours);
 * // If you Wanna to destroy it , free it by your self.
 * free(something_shadow);
 */

typedef struct list stack_attr;

struct stack {
    stack_attr * stack_top;
    stack_attr * stack_end;
    uint32_t     stack_size;
};
#define _STACK_INIT(_stack_obj, _inner_list_head) \
    _stack_obj->stack_top  = _inner_list_head;\
    _stack_obj->stack_end  = _inner_list_head;\
    _stack_obj->stack_size = 0;

static inline uint32_t get_stack_size(const struct stack * stack_obj) {
    return stack_obj->stack_size;
}

/*
 * Create an empty stack
 */
static inline struct stack * create_stack(void) {
    struct stack * ret = calloc(1, sizeof(struct stack));
    assert(NULL != ret);
    if (unlikely(NULL == ret))
        return NULL;
    list_t inner = calloc (1, sizeof(struct list));
    assert(NULL != inner);
    LIST_INIT(inner);
    if (unlikely(NULL == inner))
        return NULL;
    _STACK_INIT(ret, inner);
    return ret;
}

/*
 * Destroy the stack include the element
 */
static inline void destroy_stack(struct stack * dst) {
    struct list * begin = dst->stack_end;
    struct list * manager = NULL;
    for_each_list_backward(begin, manager) {
        free(remove_from_list (manager));
    }
    free(begin);
    free(dst);
}

/*
 * Build a stack from a exist list WITHOUT duplicate it.
 * WARNING:
 * it acts like the move in C++, So if it works after the call, that means
 * everything(read/write) you do for the "list_head" directly is forbidden!
 */
static inline struct stack * create_stack_from_list(struct list * list_head) {
    struct stack * ret = calloc(1, sizeof(struct stack));
    assert(NULL != ret);
    if (unlikely(NULL == ret))
        return ret;
    int count = 0;
    struct list * unused = NULL;
    for_each_list_backward(list_head, unused) {
        count++;
    }
    ret->stack_size = count;
    ret->stack_end  = list_head;
    ret->stack_top  = list_head->prev;
    return ret;
}

/*
 * Build a stack from a exist list WITH duplicate it
 * You would not care about the list exist,
 * it will duplicate the list you put into the function.
 */
#define create_stack_from_list_ex(list_head, type, member) ({\
    type * ret = NULL;\
    ret = DUP_LIST(list_head, type, member);\
    create_stack_from_list(&ret->member);})

typedef struct list stack_elem;

/*
 * Check for stack empty
 */
#define stack_empty(stack_obj) ({stack_obj->stack_top == stack_obj->stack_end;})

static inline void stack_push(struct stack * stack_obj, stack_elem * elem) {
    list_t head = stack_obj->stack_top;
    stack_obj->stack_top = list_add_after (head, elem);
    stack_obj->stack_size += 1;
}

static inline stack_elem * _remove_from_stack(stack_elem * remove) {
    return remove_from_list(remove);
}

static inline stack_elem * stack_pop(struct stack * stack_obj){
    if (stack_empty(stack_obj)) {
        return NULL;
    }
    stack_elem * elem = stack_obj->stack_top;
    stack_obj->stack_top = stack_obj->stack_top->prev;
    _remove_from_stack (elem);
    stack_obj->stack_size -= 1;
    return elem;
}

static inline const stack_elem * stack_top(struct stack * stack_obj) {
    return stack_obj->stack_top;
}

#endif // STACK_H

