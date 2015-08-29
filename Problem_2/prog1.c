#include <stdio.h>
#include <stdlib.h>

//Displays all arguments passed to the process
int main(int argc, char *argv[])
{
	int j;
        printf("\nIn prog1.o\n");
	printf("-- -------\n");
	printf("\nArguments passed to this function are:\n");
        for (j = 0; j < argc; j++)
	        printf("argv[%d]: %s\n", j, argv[j]);
	exit(EXIT_SUCCESS);
}
