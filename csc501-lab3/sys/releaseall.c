#include<kernel.h>
#include<proc.h>
#include<sem.h>
#include<lock.h>
#include<q.h>
#include<stdio.h>
#include<sleep.h>
int releaseall(int numlocks, int ldes1, ...)
{	
	/*unsigned long *ldptr= (unsigned long *)(&ldes1);
        int i=numlocks;
        int curr;
	for(i=numlocks; i>0; i--)
        {
                //curr=*ldptr++;
		kprintf(" %d\n", *ldptr);
		ldptr++;
	}
	return 0;*/
	STATWORD ps;
	disable(ps);
	//printf("In releaseall\n");
	unsigned long *ldptr= (unsigned long *)(&ldes1);
	int i=numlocks;
	int curr;
	int flag=0;
	struct lentry *lptr;
	for(i=numlocks; i>0; i--)
	{
		curr=*ldptr++;
		//kprintf("curr = %d\n",curr);
		lptr=&locks[curr];
		if(lptr->l_state == AVAIL)
		{
			restore(ps);
			return SYSERR;
		}
		/*if(lock_proc[currpid][curr] == 0)
		{
			kprintf("Not holding the lock\n");
			flag=1;
		}*/
		else if(lock_proc[currpid][curr] >0)
		{
			lock_proc[currpid][curr]--;	
			//int j=0;
			/*for(j=0;j<NPROC;j++)
			{
				lock_proc[j][curr]=0;
			}*/
			
		}
		if(lptr->readers >0 && lptr->writer ==0)
		{
			//kprintf("Before: Pid, lock, readers. %d, %d, %d\n", currpid, curr,lptr->readers);
			lptr->readers--;
			lock_proc[currpid][curr]=0;
			//kprintf("After: Pid, lock, readers. %d, %d, %d\n", currpid, curr,lptr->readers);
			
		}
		else if(lptr->readers == 0 && lptr->writer ==1)
                {
                        lptr->writer--;
			lock_proc[currpid][curr]=0;
                }
		else
		{
		//kprintf("Reader, writer %d, %d \n",lptr->readers ,lptr->writer);
		kprintf("Error\n");
		}
		if(lptr->readers ==0 && lptr->writer==0)
		{
			//kprintf("In equal to loop\n");
		
			if(q[lptr->qtail].qprev == lptr->qhead)
			{
				//kprintf("In e head1\n");
				restore(ps);
				resched();
				return OK;
			}
			int max;
			//kprintf("qtailprev ,qhead , qtail %d,%d\n", q[lptr->qtail].qprev, lptr->qhead,lptr->qtail);
			max=q[lptr->qtail].qprev;
			if(q[max].qtype == WRITE)
			{
				lptr->writer++;
				lock_proc[currpid][curr]++;
                                dequeue(max);
                                ready(max,RESCHNO);
			}
			else if(q[max].qtype== READ)
			{
				//kprintf("In e head2\n");
				//kprintf("Tail.prev ,head %d %d\n",max,lptr->qhead);
				if(q[max].qprev == lptr->qhead)
				{
					lptr->readers++;
					lock_proc[currpid][curr]++;
                                        dequeue(max);
                                        ready(max,RESCHNO);
					resched();
                        		restore(ps);
                       		 	return OK;
				}
				int temp_node;
				//temp_node = lptr ->qhead;
				//for(; temp_node!= lptr->qhead; temp_node=q[temp_node].qprev)
				for(temp_node = q[lptr->qtail].qprev; temp_node!= lptr->qhead; temp_node=q[temp_node].qprev)
				{
					//kprintf("temp_node, type %d,%d\n", temp_node, q[temp_node].qtype);
					if(q[max].qkey==q[temp_node].qkey)
					{
						if(q[temp_node].qtype==READ)
						{
							lptr->readers++;
							lock_proc[currpid][curr]++;
                            				dequeue(temp_node);
                                			ready(temp_node,RESCHNO);
						}	
						else if(q[temp_node].qtype== WRITE)
						{
							if(((q[temp_node].qwait - q[max].qwait) <=1000) || ((q[temp_node].qwait - q[max].qwait) >=-1000))
							{
								lptr->writer++;
								//lptr->reader++;
                                                        	lock_proc[currpid][curr]++;
                                                  	        dequeue(temp_node);
                                                        	ready(temp_node,RESCHNO);
							}
							
						}
					}
				}
			}
			resched();
			restore(ps);
			return OK;
					
		}
}
}			
/*holder=replace(curr);
			if(q[holder].qtype==READ)
			{
				lptr->readers++;
				lock_proc[currpid][curr]++;
				dequeue(holder);
				ready(holder,RESCHNO);
			}
			holder=replace(curr);
			if((holder!= -1) && (q[holder].qtype==WRITE))
			{
				temp=q[lptr->qtail].qprev;
				while(temp!=lptr->qhead)
				{
					if(q[temp].qkey<0)
						flag=1;
					temp=q[temp].qprev;
				}
				if(temp == lptr->qhead)
					holder=-1;
			}
		
			else if(q[holder].qtype==WRITE)
			{
				lptr->writer++;
                                lock_proc[currpid][curr]++;
                                dequeue(holder);
                                ready(holder,RESCHNO);
			}
		
		}
		//ldptr++;	
	}
	resched();
	if(flag==1)
	{
		restore(ps);
		return SYSERR;
	}
	else 
	{
		restore(ps);
		return 0;
	}*/


/*int replace(int ldes)
{
	struct lentry *lptr= &locks[ldes];
	int max, curr;
	max=q[lptr->qtail].qprev;
	curr=q[lptr->qtail].qprev;
	if(max== lptr->qhead)
	{
		return -1;
	}
	else
	{
		for(curr=q[curr].qprev; (curr!= lptr->qhead); curr=q[curr].qprev)
		{
			if(q[max].qtype== WRITE)
				return max;
			else
			{
				if(q[curr].qkey == q[max].qkey)
				{
					if(q[max].qtype== WRITE)
					{
						return max;
					}
					else
					{
					
					}
				if((((clktime-q[max].qwait)-(clktime-q[curr].qwait)) <=1)&& (((clktime-q[max].qwait)-(clktime-q[curr].qwait)) >= -1)) 
				{
					if(q[max].qtype == READ && q[curr].qtype==WRITE) 
					{
						max=curr;
					}
				}
				else if((clktime-q[max].qwait)	< (clktime-q[curr].qwait))
				{
						max=curr;
				}
			}
			}
		}
	}
	return max;
}*/
