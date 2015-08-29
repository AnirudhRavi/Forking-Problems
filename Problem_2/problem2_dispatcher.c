#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

#define MSGID 1000

int main(int argc, char *argv[])
{
	struct msgq		//Message Queue structure
	{
		long type;
		int value;
	} msg;	
	int msqid, stat = 0, temp = 0, res = 0, rc = 0, msgsize = sizeof(msg) - sizeof(long);
	uint msgno = 0;
	
	msqid = msgget(MSGID, 0644);//Creates Message Queue or fetches existing Message Queue
	printf("\nUsing message queue (for reading) #%d\n", msqid);

	struct msqid_ds buf;	
	rc = msgctl(msqid, IPC_STAT, &buf);
	msgno = (uint)(buf.msg_qnum);//Finds number of messages in Queue presently
    	printf("\nNumber of messages in message queue before reading: %u\n", msgno);
	printf("\nREADING PROCESS\n");
	printf("------- -------\n");
	
	while(1)
	{
		stat = msgrcv(msqid, &msg, msgsize, 0, 0);
		
		if(msg.type == 100)//Executes this process for type 100
		{
			char buff[5];
			char *newargv[] = {"prog1", NULL, NULL };
			char *newenviron[] = {NULL};
			pid_t pid;
			temp = msg.value;
			snprintf(buff, sizeof(buff), "%d", temp);
			newargv[1] = buff;        		               	
						
			switch(pid=fork())
          		{
          			case -1://Error fork()ing
          				perror("fork()");
          				exit(EXIT_FAILURE);
           			case 0://In Child Process
               				res = execve(newargv[0], newargv, newenviron);
               				exit(res); 
            			default://In Parent Process
            				if(waitpid(pid,&res,0)<0)
              				{
						perror("waitpid()");
              					exit(EXIT_FAILURE);
					}
            		} 
		}   
		if(msg.type == 200)//Executes this process for type 200
		{
			char buff[5];			
			char *newargv[] = {"prog2", NULL, NULL };
			char *newenviron[] = {NULL};
			pid_t pid;
			temp = msg.value;
			snprintf(buff, sizeof(buff), "%d", temp);
			newargv[1] = buff;
						
			switch(pid=fork())
          		{
          			case -1://Error fork()ing
          				perror("fork()");
          				exit(EXIT_FAILURE);
           		        case 0://In Child Process
               				res = execve(newargv[0], newargv, newenviron);
               				exit(res); 
            			default://In Parent Process
            		 	        if(waitpid(pid,&res,0) < 0)
              				{
						perror("waitpid()");
              					exit(EXIT_FAILURE);
					}
            		}    
		}
		rc = msgctl(msqid, IPC_STAT, &buf);
		msgno = (uint)(buf.msg_qnum);
		if(msgno == 0)//If message queue is empty ->Remove Queue ->Exit program
		{
			printf("\nNumber of messages in message queue is 0. Exiting.\n\n");
                        rc = msgctl(msqid, IPC_RMID, 0);
			exit(EXIT_SUCCESS);
		}
		
	}
	return 0;
}
