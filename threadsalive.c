/*
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <strings.h>
#include <string.h>
#include <ucontext.h>
#include "threadsalive.h"
#include "list.h"
#include "list.c"

//fuck this
/* ***************************** 
     stage 1 library functions
   ***************************** */
static ucontext_t mctx;
static int thread_count=0;

struct node *head;

void ta_libinit(void) {
	//set everything to default values
    return;
}

void ta_create(void (*func)(void *), void *arg) {
	//setting shared stack
	//setting up locks/semaphores
#define STACKSIZE 128000
	thread_count+=1;
	unsigned char *stack = (unsigned char *)malloc(STACKSIZE);
	//struct node *thread=malloc(sizeof(struct node));
	/*thread->next=NULL;
	thread->id=thread_count;
	
	if (head == NULL){
		head=thread;
	}
	else{
		struct node *iterator=head;
		struct node *prev=head;
		iterator=iterator->next;
		
		while (iterator!=NULL){
			prev=iterator;
			iterator=iterator->next;
		}
		prev->next = thread;
		//thread->next=NULL
	}*/
	ucontext_t ctx;
	struct node *thread = list_append(&head, ctx, thread_count);
    assert(stack && thread);
	/* context for thread */
    getcontext(&(thread->ctx));
    /* set up thread's stack */
    thread->ctx.uc_stack.ss_sp   = stack;
    thread->ctx.uc_stack.ss_size = STACKSIZE;
	
    /* set up thread's link: when thread exits, the main thread
     * (context 0) will take over */
    thread->ctx.uc_link = &mctx;
    /* set the thread entry point (function) for thread 1 */
    /* pass 2 argument (int values 1 and 13) to the function */
    makecontext(&(thread->ctx), (void (*)(void))func,1,arg);
	
	//free(stack);
	return;
}

void ta_yield(void) {
	struct node *tmp = head;
	printf("about to push to the back");
	fflush(stdout);
	list_pushtoback(&head);
	printf("Current Thread ID: %d\n", head->id);
	fflush(stdout);
	assert(swapcontext(&(tmp->ctx),&(head->ctx))!=-1); 
	printf("New Thread ID: %d\n", head->id);
	fflush(stdout);

    return;
}

int ta_waitall(void) {
	swapcontext(&mctx, &(head->ctx));
	list_delete(head);
	return 0;

}


/* ***************************** 
     stage 2 library functions
   ***************************** */

void ta_sem_init(tasem_t *sem, int value) {
}

void ta_sem_destroy(tasem_t *sem) {
}

void ta_sem_post(tasem_t *sem) {
}

void ta_sem_wait(tasem_t *sem) {
}

void ta_lock_init(talock_t *mutex) {
}

void ta_lock_destroy(talock_t *mutex) {
}

void ta_lock(talock_t *mutex) {
}

void ta_unlock(talock_t *mutex) {
}


/* ***************************** 
     stage 3 library functions
   ***************************** */

void ta_cond_init(tacond_t *cond) {
}

void ta_cond_destroy(tacond_t *cond) {
}

void ta_wait(talock_t *mutex, tacond_t *cond) {
}

void ta_signal(tacond_t *cond) {
}

