

#include <kernel.h>
#include "lock.h"
#include <proc.h>
#include <q.h>
#include <stdio.h>

LOCAL int newLock();
#define SETONE  1
#define SETZERO 0

SYSCALL lcreate() {
  STATWORD ps;

  int lock;
  disable(ps);

  lock = newLock();

  if (lock == -SETONE) {
    /* code */
    restore(ps);
    return(-SETONE);
  }
  restore(ps);
  return(lock);
}

LOCAL int newLock() {
  int lock, index;
  index = SETZERO
  while (index < NUMLOCKS) {
    /* code */
    lock = nextlock - 1;

    if (nextlock < SETZERO) {
      /* code */
      nextlock = NUMLOCKS - 1;
      lockaround = lockaround + 1;
    }
    int checkState = locks[lock].lstate;

    if (checkState != LUSED) {
      /* code */
      locks[lock].lstate    = LUSED;
      locks[lock].nreaders  = SETZERO;
      locks[lock].nwriters  = SETZERO;
      int returnValue       = lock * 10000;
      returnValue           = returnValue + lockaround;
      return (returnValue);
    }
    index = index + 1;
  }

  return (-SETONE);
}
