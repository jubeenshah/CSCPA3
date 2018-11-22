#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <stdio.h>
#include <lock.h>

int global_var = 5;

void p1(int lck){
      //  kprintf("Here p1\n");
      sleep(1);
        int l[1];
        l[0] = lck;
        int x = lock(lck,(int) READ, 20);
        if(x != SYSERR){
                int temp = global_var;
                kprintf("In p1 : %d\n", temp);
                sleep(2);
                //releaseall(1, l);
                releaseall(1,lck);
        }else{
                kprintf("SYSERR\n");
        }

        sleep(3);
        x = lock(lck, (int)READ, 10);
        if(x != SYSERR){
                int temp = global_var;
                kprintf("In p1 : %d\n", temp);
                sleep(2);
                //releaseall(1, l);
                releaseall(1, lck);
        }else{
                kprintf("SYSERR\n");
        }
        
        kprintf("%s done\n",proctab[currpid].pname);
}

void p2(int lck){
       // kprintf("Here p2\n");
        int l[1];
        l[0] = lck;
        int x = lock(lck,(int) WRITE, 20);
        if(x != SYSERR){
                global_var++;
                int temp = global_var;
                kprintf("Updating global var In p2 : %d\n", temp);
                sleep(2);
                releaseall(1, lck);
        }else{
                kprintf("SYSERR\n");
        }

        x = lock(lck,(int) WRITE, 20);
        if(x != SYSERR){
                global_var++;
                int temp = global_var;
                kprintf("Updating global var In p2 : %d\n", temp);
                sleep(2);
                releaseall(1, lck);
        }else{
                kprintf("SYSERR\n");
        }
        kprintf("%s done\n",proctab[currpid].pname);
}

void p3(int lck){
        //  kprintf("Here p1\n");
        sleep(1);
          int l[1];
          l[0] = lck;
          int x = lock(lck,(int) READ, 20);
          if(x != SYSERR){
                  int temp = global_var;
                  kprintf("In p3 : %d\n", temp);
                  sleep(2);
                  releaseall(1, lck);
          }else{
                  kprintf("SYSERR\n");
          }
  
          //sleep(3);
          x = lock(lck,(int) READ, 10);
          if(x != SYSERR){
                  int temp = global_var;
                  kprintf("In p3 : %d\n", temp);
                  sleep(2);
                  releaseall(1, lck);
          }else{
                  kprintf("SYSERR\n");
          }
  
          kprintf("%s done\n",proctab[currpid].pname);
}

int main( )
{
        int lck = lcreate();
        if(lck == SYSERR){
                kprintf("invalid lock\n");
                shutdown();
        }
        kprintf("lck = %d\n", lck);

        int pid1 = create(p1, 2000, 80, "First process", 1, lck);
        int pid2 = create(p2, 2000, 25, "Second process", 1, lck);
        resume(pid1);
        resume(pid2);
        sleep(1);
        int pid4 = create(p2, 2000, 90, "Fourth process", 1, lck);
        int pid3 = create(p3, 2000, 95, "Third process", 1, lck);
        resume(pid3);
        resume(pid4);
        sleep(20);
        shutdown();
}



