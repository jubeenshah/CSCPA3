#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <stdio.h>

int flag = 0;

int releaseall(int numlocks, ...){
    STATWORD ps;
    disable(ps);

    int ldes[numlocks], i = 0, retVal = OK;
    unsigned long *a = (unsigned long *)(&numlocks) + (numlocks); 
    
    for (i=0 ; i < numlocks ; i++){
        ldes[i] = *a--;
    }

    //kprintf("%s has released the lock\n", proctab[currpid].pname);
    
    for(i = 0; i < numlocks; i++){
        int ld = ldes[i];
        if(flag == 1)
        kprintf("%s :/\n", proctab[currpid].pname);
        if(locktab[ld].plist[currpid] != 1){
            
            retVal = SYSERR;
        }else{
            locktab[ld].plist[currpid] = 0;
            revertPriority(currpid);
            int isLast = isPlistEmpty(ld);
            // check for other processes and change everything if no process is holding the lock.
            // also update priority
            if(isLast == 1){
                int pid = getMaxWriter(ld);
                int rpid = getMaxReader(ld);
                int waitDiff = 2000;
                if(pid != -1 && pid == rpid){
                    //unsigned long curTime = ctr1000;
                    waitDiff = (locktab[ld].waitTime[pid] > locktab[ld].waitTime[rpid]) ? 
                                    (locktab[ld].waitTime[pid] - locktab[ld].waitTime[rpid]) :
                                        (locktab[ld].waitTime[rpid] - locktab[ld].waitTime[pid]);
                }
                int maxPrio = (pid == -1)?-1:locktab[ld].priority[pid];
                int count = 0, j = 0;

                if(waitDiff > 1000){
                    for(j = 0; j < NPROC; j++){
                        if(locktab[ld].priority[j] > maxPrio && locktab[ld].waitlist[j] == READ){
        //                  kprintf("pushing %s to ready queue for read\n", proctab[j].pname);
                            count++;
                            locktab[ld].plist[j] = 1;
                            locktab[ld].waitlist[j] = -1;
                            locktab[ld].priority[j] = -1;
                            invertPriority(ld, j);
                            ready(j, RESCHNO);
                        }
                    }
                }
                if(count > 0){
                    locktab[ld].ltype = READ;
                    locktab[ld].status = LOCKED;
                    if(flag == 0)
                        resched();
                }else if(pid != -1){
        //            kprintf("pushing %s, %d to ready queue for write\n", proctab[pid].pname, pid);
                    locktab[ld].ltype = WRITE;
                    locktab[ld].status = LOCKED;
                    locktab[ld].waitlist[pid] = -1;
                    locktab[ld].priority[pid] = -1;
                    locktab[ld].plist[pid] = 1;
                    invertPriority(ld, pid);
                    ready(pid, RESCHNO);
                    if(flag == 0){
                        resched();
                    }
                }else{
          //          kprintf("No process waiting\n");
                    locktab[ld].status = UNLOCKED;
                    locktab[ld].ltype = -1;
                }
            }
        }
    }
   
    restore(ps);
    return retVal;
    
}

int isPlistEmpty(int ld){
    int i = 0;
    for(i;i<NPROC;i++){
        if(locktab[ld].plist[i] == 1)
            return 0;
    }
    return 1;
}

int getMaxPrioProc(int ld){
    int i = 0,max = -1, proc_id = -1;
    for(i; i < NPROC; i++){
        if(locktab[ld].waitlist[i] != -1 && locktab[ld].priority[i] > max){
            max = locktab[ld].priority;
            proc_id = i;
        }
    }

    return proc_id;
}

int getMaxReader(int ld){
    int i = 0, max = -1, proc_id = -1;
    unsigned long max_wait = 0;

    for(i; i < NPROC; i++){
        if(locktab[ld].waitlist[i] == READ && locktab[ld].priority[i] > max){
            max = locktab[ld].priority[i];
            proc_id = i;
            max_wait = locktab[ld].waitTime[i];
        }else if(locktab[ld].waitlist[i] == READ && locktab[ld].priority[i] > max){
            max = locktab[ld].priority[i];
            proc_id = i;
            max_wait = locktab[ld].waitTime[i];
        }

    }
    return proc_id;
}

int getMaxWriter(int ld){
    int i = 0, max = -1, proc_id = -1;
    unsigned long max_wait = 0;
    for(i; i < NPROC; i++){
        if(locktab[ld].waitlist[i] == WRITE && locktab[ld].priority[i] > max){
            max = locktab[ld].priority[i];
            proc_id = i;
            max_wait = locktab[ld].waitTime[i];
        }else if(locktab[ld].waitlist[i] == WRITE && locktab[ld].priority[i] == max && max_wait > locktab[ld].waitTime[i]){
            max = locktab[ld].priority[i];
            proc_id = i;
            max_wait = locktab[ld].waitTime[i];
        }
    }
    return proc_id;
}

int getMax(int a, int b){
    return (a>b)?a:b;
}

int revertPriority(int pid){
    int i = 0, j = 0, max = -1;

    for(i; i < NLOCKS; i++){
        if(proctab[pid].plocks[i] == 1){
            for(j = 0; j < NPROC; j++){
                if(locktab[i].plist[j] != -1){
                //    kprintf("Whye the hell? %s\n", proctab[pid].pname);
                    max = getMax(max, revertPriority(j));
                }
            }
        }
    }

    if(max > proctab[pid].pinh){
        chprio(pid, max);
    }else{
        chprio(pid, proctab[pid].pinh);
        proctab[pid].pinh = 0;
    }

    
 //       if(max > proctab[pid].pprio){
 //           if(proctab[pid].pinh == 0){
 //               proctab[pid].pinh = proctab[pid].pprio;
 //           }
 //           chprio(pid, max);
 //       }
    
    // proctab[pid].pinh = max;
    //kprintf("%s - %d - %d - %d\n", proctab[pid].pname, proctab[pid].pprio, proctab[pid].pinh, max);
    //if(proctab[pid].pprio >= proctab[pid].pinh){
    //    chprio(pid, proctab[pid].pprio);
    //    proctab[pid].pinh = 0;
    //}
    //else{
    //    chprio(pid, proctab[pid].pinh);
     //}

    return (proctab[pid].pinh < proctab[pid].pprio)?proctab[pid].pprio:proctab[pid].pinh; 
}


int release_locks(int pid){
    STATWORD ps;
    disable(ps);
    
    flag = 1;

    int i = 0;
    for(;i < NLOCKS;i++){
        //kprintf("%s release locks %d\n", proctab[currpid].pname, locktab[i].plist[currpid]);
        if(locktab[i].plist[pid] == 1){      
            //kprintf("Temporary print %s\n", proctab[currpid].pname);
            releaseall(1, i);
        }
    }

    flag = 0;

    restore(ps);
    return(OK);
}