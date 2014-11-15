#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "threadsalive.h"

struct node* list_append(struct node **head, ucontext_t c, int i){
        struct node *new = malloc(sizeof(struct node));
        new->ctx = c;
		new->id = i;
        if (*head==NULL){
                new->next = NULL;
                *head = new;
                return new;
        }

		struct node *iterator = *head;
		struct node *prev = *head;
		iterator = iterator->next;
	
		while(iterator != NULL) {
			prev = iterator;
			iterator = iterator->next;
		}
	
		prev->next = new;
		new->next = iterator;

		return new;
}
void list_remove(struct node *head){
		if (head!=NULL){
			struct node *tmp = head;
			head=head->next;
			free(tmp);
		}
}
void list_pushtoback(struct node **head){
		if (head==NULL){
			printf("Error what did you do...\n");
		}
		struct node *end =*head;
		struct node *iterator=*head;
		struct node *prev=*head;
		while (iterator!=NULL){
			prev=iterator;
			iterator=iterator->next;
		}
		prev->next=end; //tack on the head of the queue to the end.
		end->next=NULL;

		//pop head from front...
		*head=(*head)->next;

}
		
		
		
void list_print(struct node *list) {
        //cycle through each element and print it out...
		printf("should print list...\n");
        while (list != NULL) {
                printf("%i\n", list->id);
                list = list->next;
        }
}

void list_delete(struct node *list) {
        //cycle through each element and delete it.
        while (list != NULL) {
                struct node *tmp = list;
                list = list->next;
                free(tmp);
        }
}
