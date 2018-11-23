

#include <kernel.h>
#include "lock.h"
#include <proc.h>
#include <q.h>
#include <stdio.h>

#define SETONE  1
#define SETSIX  6
#define SETZERO 0
SYSCALL lock(int ldes1, int type, int priority) {
  STATWORD ps;

  Bool needToWait = 0;

  int lock = ldes1 / 10000;

  struct lentry *lptr;
  struct pentry *pptr;

  int index, lmaxPrio;

  disable(ps);

  lptr = &locks[lock];
  int ret = lock_err(ldes1);

  switch (ret) {
    case -SETONE:
          restore(ps);
          return (ret);
    case -SETSIX:
          restore(ps);
          return (ret);
  }

  int checkNreader = lptr->nreaders;
  int checkNWriters = lptr->nwriters;

  if (checkNreader == SETZERO && checkNWriters != SETZERO) {
    /* code */
    needToWait = 1;
  } else if (checkNreader == SETZERO && checkNWriters == SETZERO && type == READ) {
    /* code */
    int tail = lptr->lqtail;
    lmaxPrio = q[tail].qprev;
    while (priority<q[lmaxPrio].qkey) {
      /* code */
      int checkType = q[lmaxPrio].qtype;
      if (checkType == WRITE) {
        /* code */
        needToWait = 1;
        break;
      }
      lmaxPrio = q[lmaxPrio].qprev;
    }
  }

  pptr=&proctab[currpid];
	pptr->plockret=1;

	if(!needwait){
    type==READ?lptr->nreaders++:lptr->nwriters++;
		lptr->pidheld[currpid]=SETONE;
		pptr->lockheld[lock]=SETONE;
		newpinh(currpid);
	}
	else{

    pptr->pstate=PRLOCK;
    pptr->lockid=ldes1/10000;
    insert(currpid,lptr->lqhead,priority);

    q[currpid].qtype=type;
    q[currpid].qtime=ctr1000;

    pptr->plockret=1;

    /* update lprio in locks */
    newlprio(lock);

    /* update the pinh of the low priority process holding the lock */

    struct lentry * helplptr=&locks[lock];
    while (index < NPROC) {
      /* code */
      int checkPID = helplptr->pidheld[index];
      if (checkPID == SETONE) {
        /* code */
        newpinh(index)
      }
      index = index + 1;
    }
    resched();
	}
	restore(ps);
	return (pptr->plockret);

}

int lock_err(int ldes){
	int lock=ldes/10000;
	int lockard=ldes-lock*10000;
	register struct lentry *lptr=&locks[lock];
  int checkState = lptr->lstate;
	if(isBadLock(lock) || checkState==LFREE || lockard!=lockaround){
		return -1;
	}
	else if(lptr->lstate==LDELETED){
		return -6;
	}
	return 1;
}
/* update the pinh of the low priority process holding the lock */

void newpinh(int pid){
	int pmaxprio = -SETONE;
  int index    = SETZERO;
	int priocompare,tmppid;
	register struct lentry *lptr;
	register struct pentry *pptr=&proctab[pid];

  while (index < NUMLOCKS) {
    /* code */
    int checkLock = proctab[pid].lockheld[index];
    if (checkLock == SETONE) {
      /* code */
      lptr = &locks[index];
      int setPrio = lptr->lprio;
      if (pmaxprio<setPrio) {
        /* code */
        pmaxprio=setPrio
      }
    }
    index = index + SETONE;
  }
	proctab[pid].pinh=(pptr->pprio>pmaxprio)?0:pmaxprio;
}

/*
 indicating the maximum scheduling priority among all the processes
waiting in the lock's wait queue.
*/
void newlprio(int lock){
	int maxprio=-SETONE;
  int priocompare;
	struct lentry *tmplptr=&locks[lock];
	int curlockid=q[tmplptr->lqtail].qprev;
	while((q[tmplptr->lqtail].qprev))!=tmplptr->lqhead){
		priocompare=(proctab[q[tmplptr->lqtail].qprev].pinh==SETZERO?proctab[q[tmplptr->lqtail].qprev].pprio:proctab[q[tmplptr->lqtail].qprev].pinh);
		if(priocompare>maxprio){
			maxprio=priocompare;
		}
		q[tmplptr->lqtail].qprev=q[q[tmplptr->lqtail].qprev].qprev;
	}
	tmplptr->lprio=maxprio;
}
