#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void catch(int sig_num)		//Checks if child process has finished executing
{
	int status;
	wait(&status);
	printf("\nChild process computing sum of all the odd numbers in the array has finished execution\n");
}

int main()
{
	int a[10], i=0, sall=0, sodd=0, seven=0, ceven=0, codd=0;
	printf("\nEnter 3 even numbers and 5 odd numbers:");
	do			//Runs until 5 odd and 3 even numbers are entered
	{	
		ceven = 0;
		codd = 0;
		printf("\n");
		for(i=0;i<8;i++)
		{
			printf("Enter Number #%d: ", i+1);
			scanf("%d",&a[i]);
			if(a[i]%2 != 0)
				codd++;
			else
				ceven++;
		}
		if((codd != 5)&&(ceven != 3))
			printf("\nIncorrect number of odd or even numbers entered. Please try again.");
	}while((codd != 5)&&(ceven != 3));
	fflush(stdin);
	
	for(i=0;i<8;i++)	//Finds sum of all elements in Array
		sall += a[i];
	printf("\nSum of all numbers in the array is: %d\n",sall); 
	
	signal(SIGCHLD, catch);	//Checks if child has executed by passing SIGCHLD and catch() as arguments to system function signal()
	pid_t pid = fork();
	if(pid < 0)		//If fork() is unsuccessful
	{
		perror("fork()");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0)
	{
				//In Child Process
		for(i=0;i<8;i++)
			if((a[i]%2) != 0)
 				sodd += a[i];
		printf("\nSum of all odd numbers in the array is: %d\n", sodd);
		pid_t pid2 = fork();
		if(pid2 < 0)
		{	
			perror("fork()");
			exit(EXIT_FAILURE);
		}
		else if(pid2 == 0)//Creates Grandchild process
		{
			sleep(20);
			printf("\nGrandchild has slept for 20s\n");
			for(i=0;i<8;i++)
				if((a[i]%2) == 0)
					seven += a[i];
			printf("\nSum of all even numbers in the array is: %d\n", seven);
		}
		else
		{
			sleep(1);
			printf("\nChild slept for 1s\n");
		}
	}
	else 			//In Parent Process
		wait(NULL);
	return 0;
}
