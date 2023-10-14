#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

int main(int argc, char **argv) {
	
	double* ptr = (double *)malloc(6 * sizeof(double));
	totalBytes();

	void* ptr2 = malloc(64);
	totalBytes();

	void* ptr3 = malloc(240);
	totalBytes();

	void* ptr4 = malloc(480);
	totalBytes();

	void* ptr5 = malloc(160);
	totalBytes();

	void* ptr6 = malloc(240);
	totalBytes();

	free(ptr);
	totalBytes();

	free(ptr2);
	totalBytes();

	free(ptr3);
	totalBytes();

	free(ptr3);
	totalBytes();

	free(ptr4);
	free(ptr5);
	free(ptr6);
	return 0;
}

