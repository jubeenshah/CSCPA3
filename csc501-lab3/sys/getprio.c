/* getprio.c - getprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

#define SETZERO 	0
/*------------------------------------------------------------------------
 * getprio -- return the scheduling priority of a given process
 *------------------------------------------------------------------------
 */
SYSCALL getprio(int pid)
{
	STATWORD ps;
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	restore(ps);
	if (pptr->pinh == SETZERO) {
		/* code */
		return(pptr->pprio);
	}
	return(pptr->pinh);

}

// /* getprio.c - getprio */
//
// #include <conf.h>
// #include <kernel.h>
// #include <proc.h>
// #include <stdio.h>
//
// /*------------------------------------------------------------------------
//  * getprio -- return the scheduling priority of a given process
//  *------------------------------------------------------------------------
//  */
// SYSCALL getprio(int pid)
// {
// 	STATWORD ps;
// 	struct	pentry	*pptr;
//
// 	disable(ps);
// 	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate == PRFREE) {
// 		restore(ps);
// 		return(SYSERR);
// 	}
// 	restore(ps);
// 	return(pptr->pinh==0?pptr->pprio:pptr->pinh);
// }
