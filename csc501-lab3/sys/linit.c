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
	int index;
  int indexDos;


  index = SETZERO;
  while (index < NLOCKS) {
    /* code */
    lptr=&locks[index];
		lptr->lstate='\01';
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
