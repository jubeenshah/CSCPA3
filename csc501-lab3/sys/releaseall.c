#include <kernel.h>
#include "lock.h"
#include <proc.h>
#include <q.h>
#include <stdio.h>

#define SETONE  1
#define SETZERO 0

int releaseall(int numlocks, int ldes1, ...){
  STATWORD ps;
  int i,ldes,lock;

  register struct lentry *lptr;
  unsigned long *a=(unsigned long *)(&ldes1);
  /* for equal lock priorities */
  int longreadertime=SETZERO,longwritertime=SETZERO;
  int readerpid=-SETONE,writerpid=-SETONE;
  int tmpprio;

  Bool not_held=0;

  disable(ps);

  while(numlocks-->SETZERO){
    ldes=*a++;
    lock=ldes/10000;

    lptr=&locks[lock];
	int ret=lock_err(ldes);
    if(ret==-1||ret==DELETED){
      not_held=1;
      continue;
    }
    lptr->pidheld[currpid]=SETZERO;
    proctab[currpid].lockheld[lock]=SETZERO;
    newpinh(currpid);

    if(lptr->nwriters){
      lptr->nwriters--;
    }
    else if(lptr->nreaders){
      lptr->nreaders--;
    }

    if(isempty(lptr->lqhead)){
      continue;
    }


  int tmppid=q[lptr->lqtail].qprev;
  if(q[tmppid].qkey==q[q[tmppid].qprev].qkey){

    tmpprio=q[tmppid].qkey;
    while(q[tmppid].qkey==tmpprio){
        if(q[tmppid].qtype==READ && q[tmppid].qtime>longreadertime){
          longreadertime=q[tmppid].qtime;
          readerpid=tmppid;
        }
        else if(q[tmppid].qtype==WRITE && q[tmppid].qtime>longwritertime){
          longwritertime=q[tmppid].qtime;
          writerpid=tmppid;
        }
        tmppid=q[tmppid].qprev;
    }
    if(readerpid>=SETZERO&&writerpid>=SETZERO){
      if(longreadertime-longwritertime<=500){
        admit_valid_readers(lock);
      }
      else{
        release(lock,writerpid);
      }
    }
    else if(readerpid>=SETZERO){
      admit_valid_readers(lock);
    }
    else if(writerpid>=SETZERO){
      release(lock,writerpid);
    }

  }
  else{
      if(q[tmppid].qtype==READ && lptr->nwriters==SETZERO){
        admit_valid_readers(lock);
      }
      else if(q[tmppid].qtype==WRITE && lptr->nreaders==SETZERO){
		  release(lock,tmppid);
      }
  }

  }

if(not_held==1){
  restore(ps);
  return -1;
}
else{
  restore(ps);
  resched();
  return (1);
}

}
