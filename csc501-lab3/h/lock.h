#ifndef __LOCK_H__
#define __LOCK_H__

#ifndef NLOCKS
#define NLOCKS    50
#endif

#define READ      10
#define WRITE     20
#define LDELETED  3
#define AVAIL	  1
#define HELD      0
#define isbadlock(a) (a>0 && a<=NLOCKS)

struct lentry{
	int l_state;
	int l_num;
	int readers;
	int writer;
	int qhead;
	int qtail;	
};

extern struct lentry locks[NLOCKS];
extern int lock_proc[][NLOCKS];

void linit();
int lcreate();
int ldelete(int);
int lock (int , int ,  int );
int releaseall(int,int,...);
#endif
