#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <stdio.h>

int lcreate(){
    STATWORD ps;
    disable(ps);

    int lock_id = 0;
    for(; lock_id < NLOCKS; lock_id++){
        if(locktab[lock_id].lstate == LFREE){
            locktab[lock_id].lstate = LUSED;
            restore(ps);
            return (lock_id);
        }
    }

    restore(ps);
    return SYSERR;
}
