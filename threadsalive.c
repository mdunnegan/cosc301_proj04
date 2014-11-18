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

	return;
}

void ta_yield(void) {
	if(head->next!=NULL){
		struct node *tmp = head;
		list_pushtoback(&head);
		assert(swapcontext(&(tmp->ctx),&(head->ctx))!=-1); 
	}

  	return;
}

int ta_waitall(void) {
	struct node *iterator = head;
	if (iterator==NULL){
		return -1;
	}
	for (int i=0; i<thread_count;i++){
		if (i>=thread_count){
			list_delete(head);
			return -1;
		}
		swapcontext(&mctx,&(iterator->ctx));
		if(iterator->next==NULL){
			continue;
				
		}
		else{
			iterator=iterator->next;
		}	
	}
	list_delete(head);
	return 0;

}

/* ***************************** 
     stage 2 library functions
   ***************************** */

void ta_sem_init(tasem_t *sem, int value) {
    sem -> val = value;
    sem -> list_head = malloc(sizeof(struct node));
}

void ta_sem_destroy(tasem_t *sem) {
	list_delete(sem->list_head);

}

void ta_sem_post(tasem_t *sem) {
    (sem -> val)++; 
	if (sem->val>0 && sem->list_head->next!=NULL){
		ta_yield();
	}
}

void ta_sem_wait(tasem_t *sem) {
	while (sem->val==0){
		ta_yield();
		//swapcontext(&sem->list_head->ctx,&(sem->list_head->next->ctx));
	}
	if (sem->val>0){
    	(sem -> val)--;
	}
}

void ta_lock_init(talock_t *mutex) {
	mutex->lock = malloc(sizeof(tasem_t));
	mutex->lock->val = 0;
	mutex->lock->list_head = malloc(sizeof(struct node));
	
}

void ta_lock_destroy(talock_t *mutex) {
	list_delete(mutex->lock->list_head);
	free(mutex->lock);
}

void ta_lock(talock_t *mutex) {
	if (mutex->lock->val == 1){
		ta_yield();
	}
	mutex->lock->val=1;

}

void ta_unlock(talock_t *mutex) {
	mutex->lock->val=0;
	//ta_sem_post(mutex->lock);
	
}


/* ***************************** 
     stage 3 library functions
   ***************************** */
talock_t *lock;

void ta_cond_init(tacond_t *cond) {
	cond->queue_head = malloc(sizeof(struct node));
}

void ta_cond_destroy(tacond_t *cond) {
	list_delete(cond->queue_head);
}

void ta_wait(talock_t *mutex, tacond_t *cond) {
	lock=mutex;
	list_append(&cond->queue_head, cond->queue_head->ctx,1);
	ta_yield();
}

void ta_signal(tacond_t *cond) {
	if (cond->queue_head != NULL){
		ta_lock(lock);
	}
}

