#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MEMSIZE 500		//Size of shared memory
#define MSGID 1000

int main()
{
	struct msgq		//Structure of Message Queue
	{
		long type;
		int value;
	} msg;
	char *pbuff, *cbuff;	
	int seg_id, msqid, stat = 0, temp = 0, msgsize = 0, rc = 0;
	uint msgno = 0;
	
	seg_id = shmget(IPC_PRIVATE, MEMSIZE, S_IRUSR|S_IWUSR);//Create Shared Memory
	pbuff = (char *)shmat(seg_id, NULL, 0);		       //Attach memory location to cahr* pointer
	sprintf(pbuff, "5");	//Store 5 in the Memory
	msqid = msgget(MSGID, 0600|IPC_CREAT);			//Create Message Queue
	printf("\nUsing message queue (for writting) #%d\n", msqid);
	
	struct msqid_ds buf;	
	rc = msgctl(msqid, IPC_STAT, &buf);
	msgno = (uint)(buf.msg_qnum);//Find number of messages currently in Queue (0 for now)
    	printf("\nNumber of messages in message queue before writting: %u\n", msgno);
	printf("\nWRITTING PROCESS\n");
	printf("-------- -------\n");
	
	pid_t pid = fork();
	if(pid < 0)	//fork()ing error
	{
		perror("fork()");
		exit(EXIT_FAILURE);
	}
	else if(pid > 0)//Parent process ->Sends 1st message to Message Queue ->Increments value in shared memory by 1
	{
		temp = atoi(pbuff);
		msg.type=100;
		msg.value=temp;
		msgsize = sizeof(msg) - sizeof(long);

		stat= msgsnd(msqid, &msg, msgsize, 0);
		if(stat != 0)
		{
			perror("msgsnd");
			exit(EXIT_FAILURE);
		}
		else
			printf("\nWriter #1 has successfully written message (%d) of type 100 to the message queue\n", temp);
		
		temp++;
		sprintf(pbuff, "%d", temp);

		shmdt(pbuff);
	}
	else if(pid == 0)//Child process ->Send 2nd message to Message Queue ->Removes shared memory location
	{
		wait(NULL);
		cbuff = (char *)shmat(seg_id, NULL, 0);
		temp = atoi(cbuff);
		msg.type = 200;
		msg.value = temp;
		msgsize = sizeof(msg) - sizeof(long);
		shmdt(cbuff);

		stat = msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0);
		if(stat != 0)
		{
			perror("msgsnd");
			exit(EXIT_FAILURE);
		}
		else
			printf("\nWriter #2 has successfully written message (%d) of type 200 to the message queue\n", temp);

		shmctl(seg_id, IPC_RMID, NULL);
	}
	
	rc = msgctl(msqid, IPC_STAT, &buf);
	msgno = (uint)(buf.msg_qnum);//Finds number of messages in Message Queue after each process execution
    	printf("\nNumber of messages in message queue after writting: %u\n", msgno);

	return 0;
}
