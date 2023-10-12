#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

int main(int argc, char **argv) {
	double *ptr = (double *)malloc(6 * sizeof(double));
	printf("payload is: %p\n", ptr);
	int *ptr2 = (int *)malloc(64 * sizeof(int));
	printf("payload is: %p\n", ptr2);
	malloc(3760);
//	malloc(4072);
	malloc(8);
	return 0;
}

