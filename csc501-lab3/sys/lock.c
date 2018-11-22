#include<kernel.h>
#include<proc.h>
#include<sem.h>
#include<lock.h>
#include<q.h>
#include<sleep.h>
int lock (int ldes1, int type, int priority)
{
	
	STATWORD ps;
	disable(ps);
	struct lentry *lptr;
	struct pentry *pptr;
	lptr=&locks[ldes1];
	pptr=&proctab[currpid];
	if(lptr->l_state== AVAIL)
	{
		kprintf("WRONG LOCK STATE\n");
		restore(ps);
		return SYSERR;
	}
	int i=0;
	for(i=0;i<NLOCKS;i++)
	{
		if(locks[i].writer >=2)
		{
			kprintf("WRONG WRITER COUNTER\n");
			restore(ps);
	                return SYSERR;

		}
	}
	if(lptr->readers <0 && lptr->writer<0)
	{
			kprintf("WRONG  READ AND WRITER COUNTER\n");
                        restore(ps);
                        return SYSERR;
	}
	int flag=0;
	//kprintf("Pid, lock, readers. %d, %d, %d\n", currpid, ldes1,lptr->readers);
	if(lptr->readers ==0 && lptr->writer ==0)
	{
		lock_proc[currpid][ldes1]++;
		if(type== READ)
		{
			flag=1;
			lptr->readers++;
			//kprintf("Pid, lock, readers. %d, %d, %d\n", currpid, ldes1,lptr->readers);
			restore(ps);
			resched();
                        return OK;
		}
		else if (type==WRITE)
		{
			lptr->writer++;
		//	kprintf("Pid, lock, writer %d, %d, %d\n", currpid, ldes1,lptr->writer);
			restore(ps);
			resched();
                        return OK;
		}
		else
		{
			kprintf("Wrong type\n");
			restore(ps);
                        return SYSERR;
		}
	}
	else if(lptr->readers >0 && lptr->writer ==0)
	{
		if(type==WRITE)
		{
			//kprintf("FOR C\n");
			pptr=&proctab[currpid];
			pptr->lock=ldes1;
			pptr->plwaitret=OK;
			pptr->pstate= PRWAIT;
			insert(currpid, lptr -> qhead, priority);
			q[currpid].qtype = type;
			q[currpid].qwait = clktime;
			priority_inheritance(ldes1, getprio(currpid),0);
			resched();
			restore(ps);
			resched();
			return pptr -> plwaitret;
		}
		else if(type==READ)
		//if(type==READ)
		{
			//flag=1;
			int i=0;
			for(i=q[lptr->qtail].qprev; (i!= lptr->qhead) && (priority < q[i].qkey); i=q[i].qprev)
			{
			if(q[i].qtype==WRITE)
			{
					//kprintf("In E\n");
					flag=1;
					pptr=&proctab[currpid];
                        		pptr->lock=ldes1;
                        		pptr->plwaitret=OK;
					pptr->pstate= PRWAIT;
                    		   	insert(currpid, lptr -> qhead, priority);
                  	      		q[currpid].qtype = type;
                     		   	q[currpid].qwait = clktime;
					priority_inheritance(ldes1, getprio(currpid),0);
                        		resched();
                        		restore(ps);
					resched();
                        		return pptr -> plwaitret;
			}
			}
			if(flag==0)
        {
                        lock_proc[currpid][ldes1]++;
                        lptr->readers++;
                  //      kprintf("Pid, lock, readers..... %d, %d, %d\n", currpid, ldes1,lptr->readers);
                        restore(ps);
                        resched();
                        return OK;

        }
		}
	}
	/*if(flag==0)
	{
			lock_proc[currpid][ldes1]++;
			lptr->readers++;
			kprintf("Pid, lock, readers..... %d, %d, %d\n", currpid, ldes1,lptr->readers);
			restore(ps);
			resched();
			return OK;
	
	} */
	else if(lptr->readers == 0 && lptr->writer ==1)
	{
			//	kprintf("Test\n");
				pptr=&proctab[currpid];
                                pptr->lock=ldes1;
                                pptr->plwaitret=OK;
				pptr->pstate= PRWAIT;
                                insert(currpid, lptr -> qhead, priority);
                                q[currpid].qtype = type;
                                q[currpid].qwait = clktime;
				priority_inheritance(ldes1, getprio(currpid),0);
                                resched();
                                restore(ps);
                                return pptr -> plwaitret;
	}
	else
	{
		restore(ps);
		return OK;
	}

}

void priority_inheritance(int ldes, int prio,int type)
{
	//kprintf("In priority inversion\n");
	int i=0;
	int max=-1;
	if(type == -1)
	{
		//kprintf("pid after kill  %d\n", prio);		
		struct lentry *lptr= &locks[ldes];
		for(i=0;i<NPROC;i++)
		{
			//kprintf("lock_prc %d \n", lock_proc[i][ldes]);
			if((getprio(i)> max) && i!= prio && lock_proc[i][ldes] == 0)
			{
		//		kprintf("in if after kill %d %d", i,getprio(i));
				max=getprio(i);
			}

		}
		prio=max;
		//kprintf("MAx prio = %d\n", prio);
	}
	for(i=0;i<NPROC;i++)
	{
		//printf("--------------------\n");
		if(type==-1)
		{
			if(lock_proc[i][ldes] > 0){
			chprio(i,prio);
			}
		}
		if(lock_proc[i][ldes] > 0 && (getprio(i) < prio))
		{
			//kprintf("In priority inversion i %d\n", i);
			chprio(i,prio);
		}
	}	

}


































