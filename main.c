#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "stack.h"

struct test {
    struct list node;
    int value;
};

void test_env(void) {
    struct test * head = NULL;
    head = calloc (1, sizeof(struct test));
    LIST_INIT(&(head->node));
    struct test * tmphead = head;
    for (int i = 1 ; i < 3; ++i) {
        struct test * tmp = calloc (1, sizeof(struct test));
        tmp->value = i;
        list_add_after (&tmphead->node, &tmp->node);
        //remove_from_list (&tmp->node);
    }
    struct list * print;
    for_each_list_backward(&tmphead->node, print) {
        struct test * tmp = list_entry(print, struct test, node);
        fprintf(stderr, "%p:%d\t ", &tmp->node, tmp->value);
    }
}

void test_stack() {
    struct stack * tmp_stack = create_stack ();
    for (int i = 1 ; i < 3; ++i) {
        struct test * tmp = calloc (1, sizeof(struct test));
        tmp->value = i+1;
        stack_push (tmp_stack, &tmp->node);
        struct test * print = list_entry(stack_pop(tmp_stack), struct test, node);
        //fprintf(stderr, "%p:%d\t ", &print->node, print->value);
        free(print);
    }
    // list create stack
    struct test * head = create_list(struct test, node);
    //head = calloc (1, sizeof(struct test));
    //LIST_INIT(&(head->node));
    struct test * tmphead = head;
    for (int i = 1 ; i < 4; ++i) {
        struct test * tmp = calloc (1, sizeof(struct test));
        tmp->value = i;
        list_add_after (&tmphead->node, &tmp->node);
        //remove_from_list (&tmp->node);
    }

    tmp_stack = create_stack_from_list_ex(&tmphead->node, struct test, node);
    int count = tmp_stack->stack_size;
    for(int i = 0; i < count; ++i) {
        struct test * print = list_entry(stack_pop(tmp_stack), struct test, node);
        fprintf(stderr, "%p:%d\t ", &print->node, print->value);
    }
}

int main(void)
{
    test_stack ();
    return 0;
}
