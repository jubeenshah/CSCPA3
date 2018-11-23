/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>

#define SETONE 	1
#define SETZERO	0

/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
SYSCALL chprio(int pid, int newprio)
{
	STATWORD ps;
	struct	pentry	*pptr;

	disable(ps);
	pptr = &proctab[pid];
	int checkStatePID = pptr->pstate;
	if (isbadpid(pid) || newprio<=SETZERO ||
	    checkStatePID == PRFREE) {
		restore(ps);
		return(-1);
	}
	pptr->pprio = newprio;
	newpinh(pid);

	int checkState = pptr->pstate;

	if(checkState == PRLOCK) {
		int setLockID = pptr->lockid;
		newlprio(setLockID);
	}
	restore(ps);
	return(newprio);
}

// /* chprio.c - chprio */
//
// #include <conf.h>
// #include <kernel.h>
// #include <proc.h>
// #include <q.h>
// #include <stdio.h>
//
// /*------------------------------------------------------------------------
//  * chprio  --  change the scheduling priority of a process
//  *------------------------------------------------------------------------
//  */
// SYSCALL chprio(int pid, int newprio)
// {
// 	STATWORD ps;
// 	struct	pentry	*pptr;
//
// 	disable(ps);
// 	if (isbadpid(pid) || newprio<=0 ||
// 	    (pptr = &proctab[pid])->pstate == PRFREE) {
// 		restore(ps);
// 		return(SYSERR);
// 	}
// 	pptr->pprio = newprio;
// 	newpinh(pid);
//
// 	if(pptr->pstate== PRLOCK){
// 		newlprio(pptr->lockid);
// 	}
// 	restore(ps);
// 	return(newprio);
// }
