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
	int i;
  int indexDos;

	// for(i=SETZERO;i<NLOCKS;++i){
	// 	lptr=&locks[i];
	// 	lptr->lstate=LFREE;
	// 	lptr->lqtail=SETONE+(lptr->lqhead= newqueue());
	// 	lptr->lprio=SETZERO;
  //
  //   indexDos = SETZERO;
  //   while (indexDos < NPROC) {
  //     /* code */
  //     lptr->pidheld[indexDos]=SETZERO;
  //     indexDos = indexDos + SETONE;
  //   }
  //
	// }
  i = SETZERO;
  while (i < NLOCKS) {
    /* code */
    lptr=&locks[i];
		lptr->lstate=LFREE;
		lptr->lqtail=SETONE+(lptr->lqhead= newqueue());
		lptr->lprio=SETZERO;

    indexDos = SETZERO;
    while (indexDos < NPROC) {
      /* code */
      lptr->pidheld[indexDos]=SETZERO;
      indexDos = indexDos + SETONE;
    }
    i = i + SETONE;


  }
}
