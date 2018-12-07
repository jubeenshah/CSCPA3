
#include<kernel.h>
#include<proc.h>
#include<stdio.h>
#include<lock.h>
#include<q.h>

#define LARGENUM  9999999
#define SETZERO   0
#define SETONE    1
void admit_valid_readers(int lock){
  register struct lentry *lptr=&locks[lock];
  int tmppid=q[lptr->lqtail].qprev;
  int checkQHead = lptr->lqhead;
  int maxpriowriter=-LARGENUM;
  while(tmppid != checkQHead){
    int checkQtype  = q[tmppid].qtype;
    int checkQKey   = q[tmppid].qkey;
    if(checkQtype == (SETONE + SETONE) &&
              checkQKey>maxpriowriter){
      int setQkey = q[tmppid].qkey;
      maxpriowriter=setQkey;
	  break;
    }
    tmppid=q[tmppid].qprev;
  }

  int setQTail = lptr->lqtail;

  tmppid=q[setQTail].qprev;
  while(tmppid!=lptr->lqhead){
    if(q[tmppid].qtype==1 && q[tmppid].qkey>= maxpriowriter){
      int help=q[tmppid].qprev;
      release(lock,tmppid);
      tmppid=help;
    }
	else{
	tmppid=q[tmppid].qprev;
	}
  }
}
