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



    if(isempty(lptr->lqhead)){
      continue;
    } else if(lptr->nwriters){
      lptr->nwriters--;
    } if(lptr->nreaders){
      lptr->nreaders--;
    }


  int checkPIDSetIs=q[lptr->lqtail].qprev;
  if(q[checkPIDSetIs].qkey==q[q[checkPIDSetIs].qprev].qkey){

    tmpprio=q[checkPIDSetIs].qkey;
    while(q[checkPIDSetIs].qkey==tmpprio){
      int checkPIDSTtate = q[checkPIDSetIs].qtype;
        if(checkPIDSTtate==1 && q[checkPIDSetIs].qtime>longreadertime){
          longreadertime=q[checkPIDSetIs].qtime;
          readerpid=checkPIDSetIs;
        }
        else if(q[checkPIDSetIs].qtype==(1+1) && q[checkPIDSetIs].qtime>longwritertime){
          longwritertime=q[checkPIDSetIs].qtime;
          writerpid=checkPIDSetIs;
        }
        checkPIDSetIs=q[checkPIDSetIs].qprev;
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
      if(q[checkPIDSetIs].qtype==READ && lptr->nwriters==SETZERO){
        admit_valid_readers(lock);
      }
      else if(q[checkPIDSetIs].qtype==WRITE && lptr->nreaders==SETZERO){
		  release(lock,checkPIDSetIs);
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
