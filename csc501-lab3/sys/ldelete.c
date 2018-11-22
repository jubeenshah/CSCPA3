#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

int ldelete(int lockdescriptor)
{
        STATWORD ps;
        int     pid, i;
        struct  lentry  *lptr;
	int lockindex = lockdescriptor / 100;

        disable(ps);
        if (((lptr=&locks[lockindex])->lstate==LFREE) || (lptr -> version != lockdescriptor %100)) 
	{
                restore(ps);
                return(SYSERR);
        }
		lptr -> nreaders = 0;
		lptr -> nwriters = 0;
		i=0;
		while (i < NPROC)
		{
			lockholdtab[i][lockindex] = 0;
			i++;
		}
        lptr->lstate = LFREE;
        lptr -> nreaders = 0;
		lptr -> nwriters = 0;
	//	lptr -> locknum  = lockindex;
        if(nonempty(lptr->lqhead)) 
	{
                while( (pid=getfirst(lptr->lqhead)) != EMPTY)
                {
                    proctab[pid].plwaitret = DELETED;
                    ready(pid,RESCHNO);
                }
                resched();
        }
		//lptr -> nreaders = 0;
		//lptr -> nwriters = 0;
		int j=0;
		while (j < NPROC)
			{
				lockholdtab[i][lockindex] = 0;
				j++;
			}
        restore(ps);
        return(OK);
}

