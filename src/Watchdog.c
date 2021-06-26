#include<unistd.h>
#include<stdio.h>
#include<signal.h>
#include<sys/wait.h>
#include<errno.h>
#include<pthread.h>
#include<sys/time.h>

#define SIG_STOP SIGRTMIN+5		//Defining macro for RTS.

/*
struct timespec{
	time_t tv_sec;              --  seconds
    long   tv_nsec;             --  nanoseconds
}
*/
//Declaring structure objects for timespec

struct timespec start[3];
pid_t pid[3];   //pid for 3 dummy child processes

void handler(int sig, siginfo_t *info, void *extra)   //Handler for Real Time Signals from processes. 
{
	int data;
	void *pte_val = info->si_value.sival_ptr;
	data = info->si_value.sival_int;
	int proc,cp;
	proc = data/10 + 1;                             //Here the first digit of the integer sent gives the process number.
	cp = data%10;									//And the second digit gives the checkpoint number.
	struct timespec end;                            //timespec object created to get the current time.
	clock_gettime(CLOCK_REALTIME,&end);
	double time_taken = ((end.tv_sec - start[proc-1].tv_sec)*1000000000+(end.tv_nsec - start[proc-1].tv_nsec));   //Time_taken in nanoseconds
	if(proc == 1)									//Resetng the timer process that sent the signal.
		start[0] = end;
	else if(proc == 2)
		start[1] = end;
	else
		start[2] = end;
	printf("\nRecieved signal from process no: %d   Checkpoint no: %d\n Time Taken: %f\n",proc,cp,time_taken);
}

void *watchdog(void *pi)
{
	pid_t *p = (pid_t*) pi;
	printf("Parent watchdog: \n");        
	sigset_t newmask;
	struct sigaction action;
	sigemptyset(&newmask);
	sigaddset(&newmask,SIG_STOP);		//Declaring sa_sigaction as "handler". For more detail refer man page of sigaction.
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = handler;
	sigfillset(&action.sa_mask);
	sigaction(SIG_STOP, &action, NULL);
	while(1){
		usleep(5);						//Checking the condition of each process in an interval of 5 micro seconds.
		struct timespec check;
		for(int i=0;i<3;i++)
		{
			clock_gettime(CLOCK_REALTIME,&check);		//Getting the current time and subtracting it from the start time of a checkpoint
			double time = ((check.tv_sec - start[i].tv_sec)*1000000000+(check.tv_nsec - start[i].tv_nsec));
			if(time>6000000000)
			{
				kill(p[i],SIGKILL);					//Killing the process if the time is more than 6 sec.
			}
		}
	}
	return 	NULL;
}

int main()
{
	for(int i=0;i<3;i++)
		clock_gettime(CLOCK_REALTIME,&start[i]);  //Starting the clock for all the processes.
	char *args[] = {"./s1",NULL};			//The address of three dummy processes.
	char *args1[] = {"./s2",NULL};
	char *args2[] = {"./s3",NULL};
	pid[0] = fork();

	if(pid[0] == 0)
	{
		//In the first child
		execv(args[0],args);
	}

	else
	{
		pid[1] = fork();
		if(pid[1] == 0)
		{
			//In the second child
			execv(args1[0],args1);
		}
		else
		{
			pid[2] = fork();
			if(pid[2]==0)
			{
				//In the third child
				execv(args2[0],args2);
			}
		}
	}
	//In the main() process.
	pthread_t id;
	pthread_create(&id, NULL, watchdog,(void*)pid);			//Creating the watchdog thread and passing the pids of child processes in an array
	sigset_t main_mask;
	sigemptyset(&main_mask);
	sigaddset(&main_mask, SIG_STOP);
	sigprocmask(SIG_SETMASK, &main_mask, NULL);				//Masking the RTS in the main function so that only watchdog gets the signals
	pthread_join(id,NULL);
	printf("Watchdog STOPPED!!\n");
	return 0;
}