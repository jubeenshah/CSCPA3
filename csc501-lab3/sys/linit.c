#include <conf.h>
#include <kernel.h>
#include "lock.h"
#include <proc.h>
#include <q.h>
#include <stdio.h>

#define SETONE  1
#define SETZERO 0

struct lentry locks[50];
int nextlock, lockaround;

void linit() {
  register struct lentry *lptr;
  lockaround    = SETZERO;
  nextlock      = NUMLOCKS;
  int index     = SETZERO;
  int indexDos  = SETZERO;

  while (index < NUMLOCKS) {
    /* code */
    lptr=&locks[index];
    lptr->lstate  = SETONE + (lptr->lqhead=newqueue());
    lptr->lprio   = SETZERO;
    index = index + 1;
  }

  while (indexDos < NPROC) {
    /* code */
    lptr->pidheld[indexDos] = SETZERO;
    indexDos = indexDos + 1;
  }
}

// /* linit.c - linit() */
//
// #include<conf.h>
// #include<kernel.h>
// #include<proc.h>
// #include<q.h>
// #include<lock.h>
// #include<stdio.h>
//
// struct lentry locks[NLOCKS];
// int nextlock;
//
// int lockaround;
//
// /* call in initialize.c */
// void linit(){
// 	register struct lentry *lptr;
// 	lockaround=0;
// 	nextlock=NLOCKS-1;
// 	int i,j;
// 	for(i=0;i<NLOCKS;++i){
// 		lptr=&locks[i];
// 		lptr->lstate=LFREE;
// 		lptr->lqtail=1+(lptr->lqhead= newqueue());
// 		lptr->lprio=0;
//
// 		for(j=0;j<NPROC;++j){
// 			lptr->pidheld[j]=0;
// 		}
//
// 	}
// }
