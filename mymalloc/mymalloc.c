#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

#ifndef DEBUG
#define DEBUG 1
#endif

#define MEM_SIZE 512
static double mem[MEM_SIZE];
#define heap ((char *)mem)

void *mymalloc(size_t size, char *file, int line) {

	//do 8 bit alignment

	char *p = heap;
	char *payload;

	if (DEBUG) {printf("First header beginning size = %d\n", ((metadata *)p)->size);}

	//check if the first time
	if (((metadata *)p)->size == 0) {
	
		//make first header
		((metadata *)p)->size = sizeof(mem) - sizeof(metadata);
		((metadata *)p)->free = 1;

		if (DEBUG) {printf("First header size after malloc = %d\n", ((metadata *)p)->size);}
		
		//if the requested amount + the size of another header can fit,
		//adjust the headers info, then add a header for the remainder
		//dont need to check if free for the first time
		//move the pointer up, then create the new header
		//return the payload, not the header
		if (size + sizeof(metadata) <= ((metadata *)p)->size) {
			((metadata *)p)->size = size;
			((metadata *)p)->free = 0;
			payload = p + sizeof(metadata);
			//move the pointer to create the new header
			int offset = (sizeof(metadata) + ((metadata *)p)->size);
			p += offset;
			//new header
			((metadata *)p)->size = sizeof(mem) - offset - sizeof(metadata);
			((metadata *)p)->free = 1;
	//		if (DEBUG) {printf("new header p->size is: %d\n", ((metadata *)p)->size);}
			p += sizeof(metadata);
	//		if (DEBUG){printf("at the end p is: %p\n", p);}
			//make sure to return the payload
			printf("header address: %p\n", payload - sizeof(metadata));
			return ((void *)payload);
		} else {
			//not enough space for memory and a header
			if (DEBUG) {printf("Not enough space to malloc\n");}
			return NULL;
		}
	} else {
		//not the initial malloc, go here
		if(DEBUG) {printf("Not the initial call to malloc\n");}

		int i = 0;
		int offset;
		
		while (i < sizeof(mem)) {
			//if the block is free and there is enough space:
			//for the memory and a new header
			if (((metadata *)p)->free == 1) {
				if (((metadata *)p)->size >= size + sizeof(metadata)) {
					//store the size of the current header to know the size new one
					int oldSize = ((metadata *)p)->size;
					if (DEBUG) {printf("Enough space, size at the current index: %d\n", oldSize);}
					((metadata *)p)->size = size;
					((metadata *)p)->free = 0;
					payload = p + sizeof(metadata);
					//move the pointer to write the new header
					offset = (sizeof(metadata) + ((metadata *)p)->size);
					p += offset;
					i += offset;
					//new header should be the older pointers free space
					//minus the data written minus the headers size
					//and will be marked as free regardless of total free space
					((metadata *)p)->size = oldSize - size - sizeof(metadata);
					((metadata *)p)->free = 1;
					//make sure to return the payload
					printf("header address: %p\n", payload - sizeof(metadata));
					return ((void *)payload);
				} else {
					//free, but not enough space
					if (DEBUG) {printf("Free, but not enough space\n");}
					return NULL;
				}
			} else {
				//not free, need to move to the next chunk
				offset = sizeof(metadata) + ((metadata *)p)->size;
				printf("offset: %d\n", offset);
				p += offset;
                                i += offset;
				printf("size after offsetting: %d\n", ((metadata *)p)->size);
			}
		
		}
	}

	if (DEBUG) {printf("Payload return size = %d\n", ((metadata *)p)->size);}
	
	return NULL;
}

void myfree(void *ptr, char *file, int line) {
	return;
}
