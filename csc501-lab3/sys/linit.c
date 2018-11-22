#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <stdio.h>

int linit(){
    STATWORD ps;
    disable(ps);
//    kprintf("Linit - To be implemented\n");
    int i = 0, j;
    for(i;i<NLOCKS;i++){
        // initialize the locks
        locktab[i].lprio = 0;
        locktab[i].lstate = LFREE;
        locktab[i].ltype = -1;
        for(j = 0; j<NPROC; j++){
            locktab[i].plist[j] = -1;    
            locktab[i].waitlist[j] = -1;
            locktab[i].lock_used[j] = LOCKUNUSED;
        }
        locktab[i].status = UNLOCKED;
    }

  //  printLockTab();

    restore(ps);
    return OK;
}


void printLockTab(){
    int i = 0;
    for(i;i<NLOCKS;i++){
        kprintf("%d : %d : %d \n", i, locktab[i].lprio, locktab[i].lstate);
    }
}