#include<kernel.h>
#include<proc.h>
#include<sem.h>
#include<lock.h>
#include<q.h>

struct lentry locks[NLOCKS];
int lock_proc[NPROC][NLOCKS];

int lcreate()
{
	STATWORD ps;
	disable(ps);
	int i=0, j=0;
	for(i=0;i<NLOCKS;i++)
	{
		if(locks[i].l_state == AVAIL)
		{
			locks[i].l_state= HELD;
			locks[i].writer=0;
			locks[i].readers=0;
			for(j=0;j<NPROC;j++)
				lock_proc[j][i]=0;
			restore(ps);
			return locks[i].l_num;
		}
	}
	restore(ps);
	return SYSERR;

}
