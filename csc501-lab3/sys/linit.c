#include <conf.h>
#include <kernel.h>
#include "lock.h"
#include <proc.h>
#include <q.h>
#include <stdio.h>

#define SETONE  1
#define SETZERO 0

struct lentry locks[NUMLOCKS];
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
