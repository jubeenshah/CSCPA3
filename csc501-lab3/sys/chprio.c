/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>
#include <lock.h>

/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
SYSCALL chprio(int pid, int newprio)
{
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	pptr->pprio = newprio;

	//kprintf("%s - chprio \n", pptr->pname);
	if(pptr->pstate == PRREADY){
		removerq(rdyhead, pid);
		insert(pid, rdyhead, pptr->pprio);
	}
	///*if(newprio == pptr->pinh){
		int i = 0;
		for(i; i < NLOCKS; i++){
			if(pptr->plocks[i] == 1){
				kprintf("Here\n");
				invertPriority(i, pid);
			}
		}
	//}
	//*/
	restore(ps);
	return(newprio);
}


void removerq(int head, int proc){
	//int curr = q[head].next;

	q[proc].qkey = 0;
	q[q[proc].qprev].qnext = q[proc].qnext;
	q[q[proc].qnext].qprev = q[proc].qprev;
	q[proc].qnext = q[proc].qprev = -1;

}