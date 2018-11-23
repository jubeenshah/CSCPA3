/* kill.c - kill */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <io.h>
#include <q.h>
#include <stdio.h>
#include "lock.h"

#define SETONE 	1
#define SETZERO	0
/*------------------------------------------------------------------------
 * kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL kill(int pid)
{
	STATWORD ps;
	struct	pentry	*pptr;		/* points to proc. table for pid*/
	int	dev;

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate==PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	if (--numproc == 0)
		xdone();

	dev = pptr->pdevs[0];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->pdevs[1];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->ppagedev;
	if (! isbaddev(dev) )
		close(dev);

	send(pptr->pnxtkin, pid);

	freestk(pptr->pbase, pptr->pstklen);
	switch (pptr->pstate) {

	case PRCURR:	pptr->pstate = PRFREE;	/* suicide */
			resched();

	case PRWAIT:	semaph[pptr->psem].semcnt++;

	case PRREADY:	dequeue(pid);
			pptr->pstate = PRFREE;
			break;

	case PRSLEEP:
	case PRTRECV:	unsleep(pid);
						/* fall through	*/
	case PRLOCK:
				dequeue(pid);
				int getLockID = pptr->lockid;
				locks[getLockID].pidheld[pid] = SETZERO;
				newlprio(getLockID);
				int index = SETZERO;
				while (index < 30) {
					/* code */
					int getLockIDAgain = pptr->lockid;
					int checkVal = locks[getLockIDAgain].pidheld[index];
					if (checkVal == SETONE) {
						/* code */
						newpinh(index);
					}
					index = index + SETONE;
				}
				pptr->pstate = PRFREE;
				break;

	default:	pptr->pstate = PRFREE;
	}
	restore(ps);
	return(OK);
}
