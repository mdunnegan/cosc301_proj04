#ifndef __LIST_H__
#define __LIST_H__
#include <ucontext.h>


struct node {
	ucontext_t ctx;
	struct node *next;
	int id;
} node;

// Our node class has 3 functions, an add that places an item in
// the correct location to keep list sorted, a list destroy, and
// a list print
struct node* list_append(struct node **head, ucontext_t c, int i);
void list_remove(struct node *head);
void list_pushtoback(struct node**head);
void list_print(struct node *head);
void list_delete(struct node *head);

#endif // __LIST_H__
