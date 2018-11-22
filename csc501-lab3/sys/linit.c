#include<kernel.h>
#include<proc.h>
#include<sem.h>
#include<lock.h>
#include<q.h>

struct lentry locks[NLOCKS];
int lock_proc[NPROC][NLOCKS];

void linit()
{
	int i=0;
	struct lentry *lptr;
	for(i=0;i<NLOCKS;i++)	
	{	
		locks[i].l_state = AVAIL;
		locks[i].l_num=i;
		locks[i].writer=0;
		locks[i].readers=0;	
		lptr=&locks[i];
	//	kprintf("In linit");
		lptr->qhead=newqueue();
		lptr->qtail=1+lptr->qhead;
	}
	int j=0;
	for(i=0;i<NLOCKS;i++)
	{
		for(j=0;j<NPROC;j++)
       		{	
			lock_proc[j][i]=0;
        	}
	}
	//kprintf(" Exit linit\n");

}	
