#include<kernel.h>
#include<proc.h>
#include<sem.h>
#include<lock.h>
#include<q.h>

int ldelete(int lockdescriptor)
{
	STATWORD ps;	
	disable(ps);
	struct lentry *lptr;
	int pid;
	lptr=&locks[lockdescriptor];
	if(lptr->l_state == AVAIL)
	{	
		restore(ps);
		return SYSERR;
	}
	else
	{
		lptr->readers=0;
		lptr->writer=0;
		lptr->l_state= AVAIL;
		int i=0;
		if(nonempty(lptr->qhead))
		{
			while( (pid=getfirst(lptr->qhead)) != EMPTY)
			{
				proctab[pid].plwaitret = DELETED;
                   		ready(pid,RESCHNO);
			}
			resched();
		}
		for(i=0;i<NPROC;i++)
		{
			lock_proc[i][lockdescriptor]=0;
		}
	restore(ps);
	return OK;
	}
}
