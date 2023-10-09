#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#define MEM_SIZE 512
static double mem[MEM_SIZE];

void *mymalloc(size_t size, char *file, int line) {

	//double *p = mem;

	//if there is no initial header for the memory, aka
	//first time calling malloc
	if(mem[0] == 0) {
		printf("First malloc called\n");
		
		metadata *root = (metadata *)&mem[0];
		root->free = 1;
		root->size = sizeof(mem) - sizeof(root);
		printf("Size of free space is: %d\n", ((metadata *)&mem[0])->size);
	} else {
		printf("Second malloc called\n");
	}

	return 0;
}

void myfree(void *ptr, char *file, int line) {
	return;
}
