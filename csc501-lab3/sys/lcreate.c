/* lcreate.c - lcreate, newlock  */

#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<lock.h>
#include<stdio.h>

#define SETONE  1
#define SETZERO 0

LOCAL int newlock();

SYSCALL lcreate(){
	STATWORD ps;

	int lock;

	disable(ps);

	if((lock=newlock() == -1){
		restore(ps);
		return (-1);
	}
	restore(ps);
	return (lock);
}

LOCAL int newlock(){
	int lock;
	int index;

	// for(index=SETZERO;index<NLOCKS;++index){
	// 	lock=nextlock--;
	// 	if(nextlock<SETZERO){
	// 		nextlock=NLOCKS-SETONE;
	// 		lockaround++;
	// 	}
	// 	if(locks[lock].lstate!='\01'){
	// 		locks[lock].lstate='\01';
	// 		locks[lock].nreaders=SETZERO;
	// 		locks[lock].nwriters=SETZERO;
	// 		return (lock*10000+lockaround);
	// 	}
	// }
  index = SETZERO;
  while (index < NLOCKS) {
    /* code */
    lock=nextlock--;
    if(nextlock<SETZERO){
      nextlock=NLOCKS-SETONE;
      lockaround++;
    }
    if(locks[lock].lstate!='\01'){
      locks[lock].lstate='\01';
      locks[lock].nreaders=SETZERO;
      locks[lock].nwriters=SETZERO;
      return (lock*10000+lockaround);
    }
    index = index + SETONE;
  }
	/*no lockid available */
	return (-1);
}
