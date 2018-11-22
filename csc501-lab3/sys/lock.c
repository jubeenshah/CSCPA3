#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <stdio.h>

int lock(int ldes1, int type, int priority){
    //kprintf("lock - To be implemented\n");
    STATWORD ps;
    disable(ps);

    //kprintf("%s has requested a lock of type %d\n", proctab[currpid].pname, type);
    struct pentry *pptr = &proctab[currpid];
    locktab[ldes1].lock_used[currpid] = LOCKUSED;

    if(pptr->pwaitret == DELETED){
        kprintf("Lock has been deleted\n");
        restore(ps);
        return SYSERR;
    }

    if(ldes1 < 0 || ldes1 >= NLOCKS){
        kprintf("invalid ldes\n");
        restore(ps);
        return SYSERR;
    }

    if(type != READ && type != WRITE){
        kprintf("invalid type %d\n", type);
        restore(ps);
        return SYSERR;
    }

    if(locktab[ldes1].lstate == LFREE){
        kprintf("Lock %d doesnt exist\n", ldes1);
        restore(ps);
        return SYSERR;
    }

    
    if(locktab[ldes1].status == UNLOCKED){
       // kprintf("%s has acquired the lock normally\n", pptr->pname);
        locktab[ldes1].status = LOCKED;
        locktab[ldes1].ltype = type;
        locktab[ldes1].lprio = priority;
        locktab[ldes1].plist[currpid] = 1;
        // update the proctab as well
        restore(ps);
        return OK;
    }

    if(locktab[ldes1].ltype == WRITE){
        // insert into wait list according to the lock priority and wait no matter what
        locktab[ldes1].waitlist[currpid] = type;
        locktab[ldes1].priority[currpid] = priority;
        locktab[ldes1].waitTime[currpid] = ctr1000;
        // also update the priority of the process holding the lock, if needed.
        // take care of the recursion.
        int i = 0;
        for(i; i < NPROC; i++){
            if(locktab[ldes1].plist[i] != -1)
                invertPriority(ldes1, i);
        }

        //kprintf("Pushing %s into wait state as lock is busy\n", pptr->pname);
        pptr->pstate = PRWAIT;
        //pptr->pwaitret = OK;
        resched();
        //kprintf("%s should come after releaseall \n", pptr->pname);
        restore(ps);
        return pptr->pwaitret;
    }

    if(locktab[ldes1].ltype == READ){
        
        if(type == WRITE){
            
            //kprintf("Pushing %s into wait state as lock is busy\n", pptr->pname);
            locktab[ldes1].waitlist[currpid] = type;
            locktab[ldes1].priority[currpid] = priority;
            locktab[ldes1].waitTime[currpid] = ctr1000;
            // also update the priority of the process holding the lock, if needed.
            // take care of the recursion.
            int i = 0;
            for(i; i < NPROC; i++){
                if(locktab[ldes1].plist[i] != -1)
                    invertPriority(ldes1, i);
            }

            pptr->pstate = PRWAIT;
            //pptr->pwaitret = OK;
            resched();
            //kprintf("%s should come after releasall\n", pptr->pname);
            restore(ps);
            return pptr->pwaitret;
        }else{
            int maxPrio = getHighestWritePriority(ldes1);
            if(priority >= maxPrio){
                //kprintf("%s has acquired the lock already held by some other process\n", pptr->pname);
                locktab[ldes1].plist[currpid] = 1;
                // also update the priority of the new process, if needed.
            }else{
                locktab[ldes1].waitlist[currpid] = type;
                locktab[ldes1].priority[currpid] = priority;
                locktab[ldes1].waitTime[currpid] = ctr1000;
                // also update the priority of the process holding the lock, if needed.
                // take care of the recursion.
                int i = 0;
                for(i; i < NPROC; i++){
                    if(locktab[ldes1].plist[i] != -1)
                        invertPriority(ldes1, i);
                }
                    
              //  kprintf("Pushing %s into wait state as lock is busy\n", pptr->pname);
                pptr->pstate = PRWAIT;
                //pptr->pwaitret = OK;
                resched();
                //kprintf("%s should come after releasall\n", pptr->pname);
                restore(ps);
                return pptr->pwaitret;
            }
        }
    }
    restore(ps);
    return OK;
}

int getHighestWritePriority(int ldes1){
    int i = 0;
    int max = -1;
    for(i; i < NPROC; i++){
        if(locktab[ldes1].waitlist[i] == WRITE && locktab[ldes1].priority[i] > max){
            max = locktab[ldes1].priority[i];
        }
    }
    return max;
}

void invertPriority(int ld, int pid){
    int i = 0, max = -1;
    for(;i < NPROC;i++){
        if(locktab[ld].waitlist[i] != -1 && getProcPrio(max) < getProcPrio(i)){
            max = i;
        }
    }

    int pid_prio = getProcPrio(pid), max_prio = getProcPrio(max);
    if(max != -1 && proctab[pid].pinh < max_prio){
        //kprintf("%s - invertPriority, %d - updated priority, %d - actual priority\n", proctab[pid].pname, max_prio, pid_prio);
        if(proctab[pid].pinh == 0){
            proctab[pid].pinh = proctab[pid].pprio;
        }
        
        chprio(pid, max_prio);
        
        if(proctab[pid].wait_lock != -1){
            int temp_ld = proctab[pid].wait_lock;
            int i = 0;
            for(i; i < NPROC;i++){
                if(locktab[temp_ld].plist[i] != -1){
                    invertPriority(temp_ld, i);
                }
            }
        }
    }
}

int getProcPrio(int pid){
    if(pid == -1)
        return -1;
        return (proctab[pid].pinh > proctab[pid].pprio)? proctab[pid].pinh:proctab[pid].pprio;
    /*if(proctab[pid].pinh != 0)
        return proctab[pid].pinh;
    return proctab[pid].pprio;*/
}

int remove_waits(int pid){
    STATWORD ps;
    disable(ps);
    
    int i = 0;
    for(i; i < NLOCKS;i++){
        //kprintf("%s remove waits %d\n", proctab[pid].pname, locktab[i].waitlist[pid]);
        if(locktab[i].waitlist[pid] != -1){
            locktab[i].waitlist[pid] = -1;
            locktab[i].priority[pid] = -1;
            //kprintf("what the hell?\n");
            int j = 0; 
            for(;j < NPROC; j++){
                if(locktab[i].plist[j] != -1){
                    if(proctab[j].pinh != 0){
                        proctab[j].pprio = proctab[j].pinh;
                        proctab[j].pinh = 0;
                    }
                    //kprintf("yolo \n");
                    invertPriority(i, j);
                }
            }
        }
    }

    restore(ps);
    return(OK);
}

int reset_locks(int pid){
    STATWORD ps;
    disable(ps);

    int i = 0;
    for(i; i < NLOCKS;i++){
        locktab[i].lock_used[pid] = LOCKUNUSED;
    }
    restore(ps);
    return OK;
}