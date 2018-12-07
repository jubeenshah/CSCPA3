
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
  int tmppid, checkQHead, maxSet

  tmppid     = q[lptr->lqtail].qprev;
  checkQHead = lptr->lqhead;
  maxSet     = -LARGENUM;

  while(tmppid != checkQHead){
    int checkQtype  = q[tmppid].qtype;
    int checkQKey   = q[tmppid].qkey;
    if(checkQtype == (SETONE + SETONE) &&
              checkQKey>maxSet){
      int setQkey = q[tmppid].qkey;
      maxSet=setQkey;
	  break;
    }
    tmppid=q[tmppid].qprev;
  }

  int setQTail = lptr->lqtail;

  tmppid=q[setQTail].qprev;
  while(tmppid!=lptr->lqhead){
    if(q[tmppid].qtype==1 && q[tmppid].qkey>= maxSet){
      //int help=q[tmppid].qprev;
      release(lock,tmppid);
      tmppid=q[tmppid].qprev;
    }
	else{
	tmppid=q[tmppid].qprev;
	}
  }
}
