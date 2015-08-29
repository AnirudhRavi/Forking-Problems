#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *runner(void *param);
int prime(int i);

int **a, **prim, *ra, n;

int main(int argc,char *argv[])
{
	//Repeats until user enter N>5	
	do
	{
		printf("\nEnter N (N>5) for the N*N Matrix: ");
		scanf("%d", &n);	
		if(n < 5)
			printf("\nValue of N<5. Please try again.");
	}while(n < 5);
	
	int i = 0, j = 0, k = 0, sum = 0, size = n*n;
 	a = (int **)malloc(n*sizeof(int*));	//Pointer to all the row pointers in the Matrix
	prim = (int **)malloc(n*sizeof(int*));	//Pointer to all the row pointers in the prime number Matrix
	ra = (int *)malloc(size*sizeof(int));	//Array containing values ranging from [2, N*N+1]	
	
	for(i = 0; i < n; i++)						
		a[i] = (int *)malloc(n*sizeof(int));

	for(i = 0; i < n; i++)
		prim[i] = (int *)malloc(n*sizeof(int));

	for(i = 0; i < size; i++)		//Initialize array with index+2
		ra[i] = i+2;

	for(i = 0; i < size; i++)		//Shuffle the values in the Array
	{
		j = i + rand()%(size-i);
		k = ra[i];
		ra[i] = ra[j];
		ra[j] = k;
	} 
 
	for(i = 0, k = 0; i < n; i++)		//Fill all the elements in the Matrix with unique random values with range [2, N*N+1]
		for(j = 0; j < n; j++, k++)
		{	*(*(a+i)+j) = ra[k];
			*(*(prim+i)+j) = ra[k];
		}
			
	printf("\nThe N*N Matrix with unique random values [2, N*N+1] is:\n");	//Prints the N*N Matrix	
	for(i = 0; i < n; i++)
	{
		for(j=0;j<n;j++)
			printf("%d\t ",*(*(a+i)+j));
		printf("\n");
	}
	printf("\n\n");

	i = 0;
	pthread_t tid[n];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	while(i<n)				//Create N threads that compute prime numbers for 1 row in the Matrix
	{
		pthread_create(&tid[i],NULL,runner,(void*)*(prim+i));
		pthread_join(tid[i],NULL);
		i++;
	}
	
	printf("\nThe prime numbers in the N*N matrix are:\n");//Display prime numbers computed by the N threads	
	for(i = 0; i < n; i++)
	{
		for(j=0;j<n;j++)
			printf("%d\t ",*(*(prim+i)+j));
		printf("\n");
	}
	
	for(i = 0; i < n; i++)			//Compute sum of all prime numbers in the Main thread
		for(j = 0; j < n; j++)
			sum=sum+(*(*(prim+i)+j));
	printf("\n\nSum of all the primes in N*N Matrix is: %d\n",sum); 
	return 1;
}

void *runner(void *param)			//Called by all N threads to calculate prime values for the corresponding row
{
	int i;
	int* x=(int *)param;
	for(i = 0; i < n; i++)
		if(prime(*(x+i)) == 0)
			*(x+i)=0;
	pthread_exit(0);
}

int prime(int i)				//Called by runner() for checking if element is prime or not
{
	int m;
	for(m = 2; m <= i/2; m++)
		if(i%m == 0)
			return 0;
	return 1;
}
