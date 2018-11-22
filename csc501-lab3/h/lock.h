#ifndef _LOCK_H_
#define _LOCK_H_

#ifndef NLOCKS
#define NLOCKS 50
#endif

#ifndef DELETED
#define DELETED -6
#endif

#ifndef READ
#define READ 1
#endif

#ifndef WRITE
#define WRITE 2
#endif

#define LFREE 1 // AVAILABLE (or) FREE
#define LUSED 2 // HELD by some process(es)

#define LOCKED 1
#define UNLOCKED 2

#define LOCKUSED 3
#define LOCKUNUSED 4

struct pidlist{
    int proc_id;
    struct pidlist *next;
};

struct lentry{
    int lprio;
    int lstate; // LFREE, LUSED
    int ltype; // READ, WRITE
    int status; // LOCKED, UNLOCKED
    // List of processes currently holding the lock. Should contain only 1 entry if the lock is WRITE otherwise it can be any number depending on the situation
    int plist[NPROC];
    int waitlist[NPROC]; 
    int priority[NPROC];
    int lock_used[NPROC];
    unsigned long waitTime[NPROC];
};


extern	struct	lentry locktab[];
extern  void invertPriority(int , int);
extern  int revertPriority(int);
extern  int release_locks(int);
extern  int remove_waits(int);
extern  int reset_locks(int);
extern  unsigned long ctr1000;


#endif