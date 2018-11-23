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

	if((lock=newlock())==SYSERR){
		restore(ps);
		return (SYSERR);
	}

	restore(ps);
	return (lock);
}

LOCAL int newlock(){
	int lock;
	int i;

	for(i=SETZERO;i<NLOCKS;++i){
		lock=nextlock--;
		if(nextlock<SETZERO){
			/* next NLOCKS around */
			nextlock=NLOCKS-SETONE;
			lockaround++;
		}
		if(locks[lock].lstate!=LUSED){
			locks[lock].lstate=LUSED;
			locks[lock].nreaders=SETZERO;
			locks[lock].nwriters=SETZERO;
			return (lock*LOCKMAXAROUND+lockaround);
		}
	}
	/*no lockid available */
	return (SYSERR);
}
