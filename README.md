**Program**: 3 forking related problems implemented

**Author**: Anirudh Ravi

**About**:

*Problem_1*:

Wrote a program (parent process) that takes 8 positive integers from command line (5 odd numbers, and 3 even numbers) and displays their sum on the monitor. Then it forks a child process. Child goes through all these 8 integers (as it has inherited all these 8 integers from parent) and computes the sum of only the odd integers (here 5 such integers are there) and outputs the sum onto the monitor, and further it forks another child process (grandchild of the parent process) which sleeps for 20 seconds and then computes the sum of the even integers (here 3 such integers are there) and outputs the sum onto the monitor. The first child sleeps for 1 second and then exits without worrying whether its’ child (i.e. grandchild of the parent process) is still running. Parent catches the SIGCHLD signal when the 1st child exits and prints a message within the signal handler (“Sum of odd integers is displayed”), and then exits. A Zombie process exists which can be seen from the sreenshot of the pstree command.

*Problem_2*:

Wrote a set of programs using message queues, shared memory, fork, and exec etc. to implement interactions between different processes and Inter Process Communication elements are described below. There are two writers who read from a Shared memory (SM) and write the contents to a message queue (MQ).

It is assumed that the SM contains an integer data item. The writer that gets the SM first reads this integer and writes a message to the MQ with the data part as this integer and type as 100. Then it increments the contents of the SM by 1 and detaches itself from the SM. Other writer then gets attached to the SM and reads the integer (the incremented one) and writes it to the MQ as a message with type 200. The Dispatcher process continuously reads the MQ and invokes appropriate reader decided by the conditions given next. If the message type is
100, it executes a new program (assume /usr/bin/prog1.o) and passes the integer data item.

*Problem_3*:

Created an N x N matrix with unique random values (integers) ranging from 2 to (N*N + 1) as its elements. The value of N is greater than or equal to 5. Created N threads, where each thread takes the elements of one row of the N x N matrix as its argument (one thread for each row). Each thread looks for a prime number in its argument (row) and reports it to the master thread. Master thread waits till all threads report back the prime numbers that are seen in their respective rows (within the NxN matrix). Master thread then adds up (sums) these primes numbers reported by each of the child threads (siblings). Each of thread dies or exits after reporting the prime numbers they see in their respective rows. Master thread exits after printing the sum. This program is written using using Posix thread API (pthread_create, pthread_join, pthread_exit, etc.).
