//
//
// #include <kernel.h>
// #include "lock.h"
// #include <proc.h>
// #include <q.h>
// #include <stdio.h>
//
// #define SETONE  1
// #define SETSIX  6
// #define SETZERO 0
// SYSCALL lock(int ldes1, int type, int priority) {
//   STATWORD ps;
//
//   Bool needToWait = 0;
//
//   int lock = ldes1 / 10000;
//
//   struct lentry *lptr;
//   struct pentry *pptr;
//
//   int index, lmaxPrio;
//
//   disable(ps);
//
//   lptr = &locks[lock];
//   int ret = lock_err(ldes1);
//
//   // switch (ret) {
//   //   case -SETONE:
//   //         restore(ps);
//   //         return (ret);
//   //         break;
//   //   case -SETSIX:
//   //         restore(ps);
//   //         return (ret);
//   //         break;
//   // }
//   if(ret==-1||ret==-6){
// 		restore(ps);
// 		return (ret);
// 	}
//
//   int checkNreader = lptr->nreaders;
//   int checkNWriters = lptr->nwriters;
//
//   if (checkNreader == SETZERO && checkNWriters != SETZERO) {
//     /* code */
//     needToWait = 1;
//   } else if (checkNreader != SETZERO && checkNWriters == SETZERO && type == WRITE) {
//     /* code */
//     needToWait = 1;
//   }
//   else if (checkNreader != SETZERO && checkNWriters == SETZERO && type == READ) {
//     /* code */
//     int tail = lptr->lqtail;
//     lmaxPrio = q[tail].qprev;
//     while (priority<q[lmaxPrio].qkey) {
//       /* code */
//       int checkType = q[lmaxPrio].qtype;
//       if (checkType == WRITE) {
//         /* code */
//         needToWait = 1;
//         break;
//       }
//       lmaxPrio = q[lmaxPrio].qprev;
//     }
//   }
//
//   pptr=&proctab[currpid];
// 	pptr->plockret=1;
//
// 	if(!needToWait){
//     type==READ?lptr->nreaders++:lptr->nwriters++;
// 		lptr->pidheld[currpid]=SETONE;
// 		pptr->lockheld[lock]=SETONE;
// 		newpinh(currpid);
// 	}
// 	else{
//
//     pptr->pstate=PRLOCK;
//     pptr->lockid=ldes1/10000;
//     insert(currpid,lptr->lqhead,priority);
//
//     q[currpid].qtype=type;
//     q[currpid].qtime=ctr1000;
//
//     pptr->plockret=1;
//
//     /* update lprio in locks */
//     newlprio(lock);
//
//     /* update the pinh of the low priority process holding the lock */
//
//     struct lentry * helplptr=&locks[lock];
//     while (index < NPROC) {
//       /* code */
//       int checkPID = helplptr->pidheld[index];
//       if (checkPID == SETONE) {
//         /* code */
//         newpinh(index);
//       }
//       index = index + 1;
//     }
//     resched();
// 	}
// 	restore(ps);
// 	return (pptr->plockret);
//
// }
//
// int lock_err(int ldes){
// 	int lock=ldes/10000;
// 	int lockard=ldes-lock*10000;
// 	register struct lentry *lptr=&locks[lock];
//   int checkState = lptr->lstate;
// 	if(isbadlock(lock) || checkState==LFREE || lockard!=lockaround){
// 		return -1;
// 	}
// 	else if(lptr->lstate==LDELETED){
// 		return -6;
// 	}
// 	return 1;
// }
// /* update the pinh of the low priority process holding the lock */
//
// void newpinh(int pid){
// 	int pmaxprio = -SETONE;
//   int index    = SETZERO;
// 	int priocompare,tmppid;
// 	register struct lentry *lptr;
// 	register struct pentry *pptr=&proctab[pid];
//
//   while (index < NLOCKS) {
//     /* code */
//     int checkLock = proctab[pid].lockheld[index];
//     if (checkLock == SETONE) {
//       /* code */
//       lptr = &locks[index];
//       int setPrio = lptr->lprio;
//       if (pmaxprio<setPrio) {
//         /* code */
//         pmaxprio=setPrio;
//       }
//     }
//     index = index + SETONE;
//   }
// 	proctab[pid].pinh=(pptr->pprio>pmaxprio)?0:pmaxprio;
// }
//
// /*
//  indicating the maximum scheduling priority among all the processes
// waiting in the lock's wait queue.
// */
// void newlprio(int lock){
// 	int maxprio=-SETONE;
//   int priocompare;
// 	struct lentry *tmplptr=&locks[lock];
// 	int curlockid=q[tmplptr->lqtail].qprev;
// 	while((q[tmplptr->lqtail].qprev)!=tmplptr->lqhead){
// 		priocompare=(proctab[q[tmplptr->lqtail].qprev].pinh==SETZERO?proctab[q[tmplptr->lqtail].qprev].pprio:proctab[q[tmplptr->lqtail].qprev].pinh);
// 		if(priocompare>maxprio){
// 			maxprio=priocompare;
// 		}
// 		q[tmplptr->lqtail].qprev=q[q[tmplptr->lqtail].qprev].qprev;
// 	}
// 	tmplptr->lprio=maxprio;
// }

// /* lock.c - lock*/
//
// #include<kernel.h>
// #include<proc.h>
// #include<q.h>
// #include<lock.h>
// #include<stdio.h>
//
//
// SYSCALL lock(int ldes1, int type, int priority){
// 	STATWORD ps;
//
// 	/* help me check lock if this process need wait*/
// 	Bool needwait=FALSE;
//
// 	int lock=ldes1/LOCKMAXAROUND;
//
// 	struct lentry *lptr;
// 	struct pentry *pptr;
//
// 	int i,lmaxprio;
//
// 	disable(ps);
//
// 	lptr=&locks[lock];
// 	/* lock is invalid or not created.  */
// 	int ret=lock_err(ldes1);;
// 	if(ret==SYSERR||ret==DELETED){
// 		restore(ps);
// 		return (ret);
// 	}
//
// 	if(lptr->nreaders==0&&lptr->nwriters!=0){
// 		needwait=TRUE;
// 		/* write lock here */
// 	}
// 	else if(lptr->nreaders!=0&&lptr->nwriters==0 && type==WRITE){
// 		needwait=TRUE;
// 		/* read lock now but requested by write*/
// 	}
// 	else if(lptr->nreaders!=0&&lptr->nwriters==0 && type==READ){
// 		lmaxprio=q[lptr->lqtail].qprev;
// 		/* any higher priority writer process waiting for the lock*/
// 		while(priority<q[lmaxprio].qkey){
// 			if(q[lmaxprio].qtype==WRITE){
// 				needwait=TRUE;
// 				break;
// 			}
// 			lmaxprio=q[lmaxprio].qprev;
// 		}
// 	}
// 	pptr=&proctab[currpid];
// 	pptr->plockret=OK;
// 	if(needwait){
// 		pptr->pstate=PRLOCK;
// 		pptr->lockid=ldes1/LOCKMAXAROUND;
// 		insert(currpid,lptr->lqhead,priority);
//
// 		q[currpid].qtype=type;
// 		q[currpid].qtime=ctr1000;
//
// 		pptr->plockret=OK;
//
// 		/* update lprio in locks */
// 		newlprio(lock);
//
// 		/* update the pinh of the low priority process holding the lock */
//
// 		struct lentry * helplptr=&locks[lock];
// 		for(i=0;i<NPROC;++i){
// 			if(helplptr->pidheld[i]==1){
// 				newpinh(i);
// 			}
// 		}
// 		resched();
// 	}
// 	else{
// 		type==READ?lptr->nreaders++:lptr->nwriters++;
// 		lptr->pidheld[currpid]=1;
// 		pptr->lockheld[lock]=1;
// 		newpinh(currpid);
// 	}
// 	restore(ps);
// 	return (pptr->plockret);
//
// }
//
// 	/* lock is invalid or not created.  */
// int lock_err(int ldes){
// 	int lock=ldes/LOCKMAXAROUND;
// 	int lockard=ldes-lock*LOCKMAXAROUND;
// 	register struct lentry *lptr=&locks[lock];
// 	if(isbadlock(lock) || lptr->lstate==LFREE || lockard!=lockaround){
// 		return SYSERR;
// 	}
// 	else if(lptr->lstate==LDELETED){
// 		return DELETED;
// 	}
// 	return OK;
// }
// /* update the pinh of the low priority process holding the lock */
//
// void newpinh(int pid){
// 	int i,pmaxprio=-1;
// 	int priocompare,tmppid;
// 	register struct lentry *lptr;
// 	register struct pentry *pptr=&proctab[pid];
// 	for(i=0;i<NLOCKS;++i){
// 		if(proctab[pid].lockheld[i]==1){
// 			lptr=&locks[i];
// 			if(pmaxprio<lptr->lprio){
// 				pmaxprio=lptr->lprio;
// 			}
// 		}
//
// 	}
// 	proctab[pid].pinh=(pptr->pprio>pmaxprio)?0:pmaxprio;
// }
//
// /*
//  indicating the maximum scheduling priority among all the processes
// waiting in the lock's wait queue.
// */
// void newlprio(int lock){
// 	int maxprio=-1,priocompare;
// 	struct lentry *tmplptr=&locks[lock];
// 	int curlockid=q[tmplptr->lqtail].qprev;
// 	while(curlockid!=tmplptr->lqhead){
// 		priocompare=(proctab[curlockid].pinh==0?proctab[curlockid].pprio:proctab[curlockid].pinh);
// 		if(priocompare>maxprio){
// 			maxprio=priocompare;
// 		}
// 		curlockid=q[curlockid].qprev;
// 	}
// 	tmplptr->lprio=maxprio;
// }
//


/* lock.c - lock*/

#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<lock.h>
#include<stdio.h>

#define SETONE  1
#define SETZERO 0


SYSCALL lock(int ldes1, int type, int priority){
	STATWORD ps;

	/* help me check lock if this process need wait*/
	Bool needwait=0;

	int lock=ldes1/10000;

	struct lentry *lptr;
	struct pentry *pptr;

	int i;
  int lmaxprio;

	disable(ps);

	lptr=&locks[lock];
	/* lock is invalid or not created.  */
	int ret=lock_err(ldes1);;
	if(ret==-1||ret==-6){
		restore(ps);
		return (ret);
	}

  int checkNReaders = lptr->nreaders;
  int checkNWriters = lptr->nwriters;
	if(checkNReaders == SETZERO &&
     checkNWriters !=SETZERO){
		needwait=1;
		/* write lock here */
	}
	else if(checkNReaders != SETZERO &&
          checkNWriters == SETZERO &&
          type  ==(SETONE + SETONE)   ){
		needwait=1;
		/* read lock now but requested by write*/
	}
	else if(checkNReaders != SETZERO &&
          checkNWriters == SETZERO &&
          type==1){
		lmaxprio=q[lptr->lqtail].qprev;
		/* any higher priority writer process waiting for the lock*/
		while(priority<q[lmaxprio].qkey){
			if(q[lmaxprio].qtype==(SETONE + SETONE)){
				needwait=1;
				break;
			}
			lmaxprio=q[lmaxprio].qprev;
		}
	}
	pptr=&proctab[currpid];
	pptr->plockret=1;
	if(!needwait){
    type==1?lptr->nreaders++:lptr->nwriters++;
		lptr->pidheld[currpid]=SETONE;
		pptr->lockheld[lock]=SETONE;
    int pidToPass = currpid;
		newpinh(pidToPass);
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
    i = SETZERO;
    while (i < NPROC) {
      /* code */
      int checkPIDHeld = helplptr->pidheld[i];
      if(checkPIDHeld == SETONE){
				newpinh(i);
      }
       i = i + SETONE;
    }
		resched();

	}
	restore(ps);
  int returnValue = pptr->plockret;
	return (returnValue);

}

	/* lock is invalid or not created.  */
int lock_err(int ldes){
	int lock=ldes/10000;
	int lockard=ldes-lock*10000;
	register struct lentry *lptr=&locks[lock];
  int checkBadLock = isbadlock(lock);
  int checkLockState = lptr->lstate ;
	if( checkBadLock || checkLockState == LFREE || lockard!=lockaround) {
		return -1;
	}
	else if(checkLockState==LDELETED){
		return -6;
	}
	return 1;
}
/* update the pinh of the low priority process holding the lock */

void newpinh(int pid){
	int i,pmaxprio=-SETONE;
	int priocompare,tmppid;
	register struct lentry *lptr;
	register struct pentry *pptr=&proctab[pid];
	for(i=SETZERO;i<NLOCKS;++i){
		if(proctab[pid].lockheld[i]==SETONE){
			lptr=&locks[i];
			if(pmaxprio<lptr->lprio){
				pmaxprio=lptr->lprio;
			}
		}

	}
	proctab[pid].pinh=(pptr->pprio>pmaxprio)?SETZERO:pmaxprio;
}

/*
 indicating the maximum scheduling priority among all the processes
waiting in the lock's wait queue.
*/
void newlprio(int lock){
	int maxprio=-SETONE,priocompare;
	struct lentry *tmplptr=&locks[lock];
	int curlockid=q[tmplptr->lqtail].qprev;
	while(curlockid!=tmplptr->lqhead){
		priocompare=(proctab[curlockid].pinh==SETZERO?proctab[curlockid].pprio:proctab[curlockid].pinh);
		if(priocompare>maxprio){
			maxprio=priocompare;
		}
		curlockid=q[curlockid].qprev;
	}
	tmplptr->lprio=maxprio;
}
