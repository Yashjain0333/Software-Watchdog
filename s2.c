//Dummy process 2
#include<unistd.h>
#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>

#define SIG_STOP SIGRTMIN+5
//Sending real time signal to the watchdog through the sendsignal function.

void sendsignal(int cp)
{
	int stat;
	pid_t pid = getppid();
	union sigval sval;
	sval.sival_int = cp+10;
	stat=sigqueue(pid,SIG_STOP,sval);
	//if(stat=-1)
	//	perror("sigqueue");
}

int main()
{
	sleep(2);
	sendsignal(1);  //Checkpoint 1
	int n = 100;
	while(n){n--;}
	sleep(10);
	sendsignal(2);  //Checkpoint 2
	int m = 100;
	while(m){m--;}
	sleep(10);
	sendsignal(3);  //Checkpoint 3
	return 0;
}