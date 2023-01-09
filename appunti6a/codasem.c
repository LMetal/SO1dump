#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "semfun.h"

/* verifica gestione coda semafori */

int semid;

void proc(i)
int i;
{
  printf("Processo %d con pid %d prima della down\n",i,getpid());
  down(semid,0);
  printf("Processo %d con pid %d dopo la down\n",i,getpid());
}

int main()
{
int i;
pid_t pid;

 
if ((semid = semget(IPC_PRIVATE,1,0666))==-1)
     perror("semget");

seminit(semid,0,0);	 /* setta "rosso" */

for(i=0;i<5;i++)
	{
	if (fork()==0)
		{ proc(i); exit(1);}
	sleep(1);
	}
for(i=0;i<5;i++) 
	{
	up(semid,0);
	sleep(1);
	}
for(i=0;i<5;i++)
	{ pid=wait(0);
	  printf("Terminato processo %d\n",pid);
	}
if (semctl(semid,0,IPC_RMID) == -1)
     perror("semctl");
}
