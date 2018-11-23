
#include <conf.h>
#include <kernel.h>
#include "lock.h"
#include <proc.h>
#include <q.h>
#include <stdio.h>

#define SETONE  1
#define SETZERO 0
#define SETSIX  6

SYSCALL ldelete(int lockdescriptor) {
STATWORD ps;

struct lentry *lptr;

int pid;
int lock = lockdescriptor / 10000;

disable(ps);

int ret = lock_err(lockdescriptor);

switch (ret) {
  case -SETONE:
        restore(ps);
        return (ret);
        break;
  case -SETSIX:
        restore(ps);
        return (ret);
        break;
}

lptr=&locks[lock];
lptr->lstate = LDELETED;

int emptyIS = lptr->lqhead;
if (nonempty(emptyIS)) {
  /* code */
  int emptyISAgain = lptr->lqhead;
  pid = getfirst(emptyISAgain);
  while (pid != EMPTY ) {
    /* code */
    ready(pid, RESCHNO);
  }
  resched();
}
restore(ps);
return(OK);
}

// /* ldelete.c - ldelete */
//
// #include<conf.h>
// #include<kernel.h>
// #include<proc.h>
// #include<q.h>
// #include<lock.h>
// #include<stdio.h>
//
// SYSCALL ldelete(int lockdescriptor){
// 	STATWORD ps;
// 	struct lentry *lptr;
//
// 	int pid;
// 	int lock=lockdescriptor/LOCKMAXAROUND;
//
// 	disable(ps);
// 	int ret=lock_err(lockdescriptor);
// 	if(ret==SYSERR||ret==DELETED){
// 		restore(ps);
// 		return (ret);
// 	}
//
// 	lptr=&locks[lock];
// 	lptr->lstate=LDELETED;
//
//
// /*awakens all the waiting processes by moving them from the lock queue to the ready list.*/
// 	if(nonempty(lptr->lqhead)){
// 		while((pid=getfirst(lptr->lqhead))!=EMPTY){
// 			proctab[pid].plockret=DELETED;
// 			ready(pid,RESCHNO);
// 		}
// 		resched();
// 	}
// 	restore(ps);
// 	return (OK);
// }
//
