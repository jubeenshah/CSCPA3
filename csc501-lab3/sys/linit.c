/* linit.c - linit() */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include "lock.h"
#include <stdio.h>

#define SETONE  1
#define SETZERO 0
struct lentry locks[NLOCKS];
int nextlock, lockaround;
void linit(){
	register struct lentry *lptr;
	lockaround = SETZERO;
  int setNextLock = NLOCKS-SETONE;
	nextlock   = setNextLock ;
	int i,j;

	for(i=SETZERO;i<NLOCKS;++i){
		lptr=&locks[i];
		lptr->lstate=LFREE;
		lptr->lqtail=SETONE+(lptr->lqhead= newqueue());
		lptr->lprio=SETZERO;

		// for(j=SETZERO;j<NPROC;++j){
		// 	lptr->pidheld[j]=SETZERO;
		// }
    j = SETZERO;
    while (j < NPROC) {
      /* code */
      lptr->pidheld[j]=SETZERO;
      j = j + SETONE;
    }
	}
}
