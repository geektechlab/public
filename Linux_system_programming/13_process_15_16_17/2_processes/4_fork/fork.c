#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

# define CHILD 0
void sighand(int signum){
	int childstatus;
	printf(" I am in sig handler \n");
}

main(){
	pid_t pid;	
	int childstatus;
	pid = fork();
	if( pid == CHILD){
		printf("pid:%d\n", getpid());
		sleep(10);
		printf("child exiting\n");
		exit(1);
	}
	// parent
	else{
		signal(SIGCHLD,sighand);
		wait(&childstatus);
		if(WIFSIGNALED(childstatus)){
			printf(" child was killed due to uncaught sig\n");
		}
	}
}
