#include <q.h>
#include <lock.h>
#include <sleep.h>

int bestone(int lock)
{
	struct lentry *lptr = &locks[lock];
        int highest; 
        int item,member,i;
	//kprintf("IN best\n");

        item = q[lptr -> lqtail].qprev;
        highest = q[lptr -> lqtail].qprev;
        member = q[lptr -> lqtail].qprev;

        if(highest == lptr -> lqhead) return -1; //null
	//kprintf("before dead \n");		
        for(i=0;i<NLOCKS;i++)
        {
        	member = q[item].qprev;
        	if(q[member].qkey < 0)
        		{
 					break;       		
        		}
        
        }
	//kprintf("calculating best\n");
       	for(item = q[item].qprev;q[item].qprev != lptr -> lqhead;item=q[item].qprev)
        {
                //item = q[item].qprev;
                if(q[item].qkey < q[highest].qkey) break;
                else if(q[item].qkey > q[highest].qkey) kprintf("Wrong! priority inverse!\n");
                else
                {
                        //int wait1 = clktime - q[highest].qwait;
                        //int wait2 = clktime - q[item].qwait;
                        if( (((clktime - q[highest].qwait) - (clktime - q[item].qwait)) <= 1) && (((clktime - q[highest].qwait)- (clktime - q[item].qwait)) >= -1))
                        {
                                if((q[highest].qtype == READ) && (q[item].qtype == WRITE))
                                        highest = item;
                                else highest = highest;
                                break;
                        }
                       // else if(wait1 > wait2) highest = highest;
                        else if((clktime - q[highest].qwait) < (clktime - q[item].qwait)) highest = item;
                        else highest = highest;
                        break;
                }
        }
        return highest;
}   
