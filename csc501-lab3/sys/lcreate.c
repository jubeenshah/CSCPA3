#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

int newlock();

int lcreate(void)
{
	STATWORD ps;
        int     lock;

        disable(ps);
//        if ((lock=newlock())==SYSERR) 
	//{
        //        restore(ps);
      //          return(SYSERR);
        //}
        //restore(ps);
        //return(lock);
//}

//LOCAL int newlock()
//{
  //      int     lock;
        int     i=0, j;

        //for (i=0 ; i<NLOCKS ; i++)
        while(i<NLOCKS) 
	{
                lock=i;
                //if (nextlock < 0)
                  //      nextlock = NLOCKS-1;
            if (locks[lock].lstate==LFREE) 
					{
            locks[lock].lstate = LUSED;
            locks[lock].version=locks[lock].version++;
	//	kprintf("version is %d",locks[lock].version);
            if (locks[lock].version==100)
            	locks[lock].version=0;
			locks[lock].locknum = lock*VERSION_POSS+locks[lock].version;
			locks[lock].nreaders = 0;
			locks[lock].nwriters = 0;
	
			for(j = NPROC; j >0; j--) lockholdtab[j][lock] = 0;
                        return(locks[lock].locknum);
            		}
           i++;
    }
        return(SYSERR);
}
	
