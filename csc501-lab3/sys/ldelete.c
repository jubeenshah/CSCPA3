#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <stdio.h>

int ldelete(int ldes){
    //kprintf("ldelete - To be implemented\n");
    STATWORD ps;
    disable(ps);

    int i = 0, j = 0;
    for(i; i < NPROC; i++){
        if(locktab[ldes].lock_used[i] == LOCKUSED){
            proctab[i].pwaitret = DELETED;
            if(locktab[ldes].waitlist[i] != -1){
                ready(i, RESCHNO);
            }
            if(proctab[i].pinh != 0){
                chprio(i, proctab[i].pinh);
                proctab[i].pinh = 0;
                
            }
        }
    }


    locktab[ldes].lprio = 0;
    locktab[ldes].lstate = LFREE;
    locktab[ldes].ltype = -1;
    for(j = 0; j<NPROC; j++){
        locktab[ldes].plist[j] = -1;    
        locktab[ldes].waitlist[j] = -1;
        locktab[ldes].lock_used[j] = LOCKUNUSED;
    }
    locktab[ldes].status = UNLOCKED;

    resched();
    restore(ps);
    return OK;
}
