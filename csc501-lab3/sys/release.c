#include <kernel.h>
#include "lock.h"
#include <proc.h>
#include <q.h>
#include <stdio.h>

#define SETONE  1
#define SETZERO 0
void release(int lock,int tmppid){
	int index = SETZERO;
    register struct lentry *lptr=&locks[lock];
    int newNReaders = lptr->nreaders++;
    int newNWriters = lptr->nwriters++
	q[tmppid].qtype==READ?newNReaders:newNWriters;

    lptr->pidheld[tmppid]=index;
    proctab[currpid].lockheld[lock]=index;
    newlprio(lock);

    while (index < NPROC) {
      /* code */
      int checkPID = lptr->pidheld[index];
      if (checkPID == SETONE) {
        /* code */
        newpinh(index);
      }
      index = index + SETONE;
    }

    dequeue(tmppid);
    /* this process hold the lock and is put into ready list*/
    ready(tmppid,RESCHNO);

}
