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

	//just in case ahah
	if (size <= 0) {
		printf("Malloc for 0 or less bytes at: Line %d, %s\n", line, file);
		return NULL;
	}

	//do 8 bit alignment
	if (size % 8 != 0) {
		size = (size + 7) & ~7;
	}

	char *p = heap;
	char *payload;

	//check if the first time
	if (((metadata *)p)->size == 0) {
		//if size of a header + the requested amount + the size of another header can fit,
		//adjust the headers info, then add a header for the remainder
		//dont need to check if free for the first time
		//move the pointer up, then create the new header
		//return the payload, not the header
		if (sizeof(metadata) + size <= sizeof(mem)) {
	
			((metadata *)p)->size = size;
			((metadata *)p)->free = 0;
	
			payload = p + sizeof(metadata);
	
			//move the pointer to create the new header
			int offset = (sizeof(metadata) + ((metadata *)p)->size);
			p += offset;
	
			//new header, if there is space
			if (offset != sizeof(mem)) {
				((metadata *)p)->size = sizeof(mem) - offset - sizeof(metadata);
				((metadata *)p)->free = 1;
			}

			//successful malloc
			return ((void *)payload);

		} else {
			//not enough space for memory and a header
			printf("(Initial Malloc): Not enough space to malloc at: Line %d, %s\n", line, file);
			return NULL;
		}
	} else {
		//not the initial malloc, go here
//		if(DEBUG == 1) {printf("Not the initial call to malloc\n");}

		int i = 0;
		int offset;
		
		while (i < sizeof(mem)) {
			//if the block is free and there is enough space:
			//for the memory and a new header
			if (((metadata *)p)->free == 1) {
				if (((metadata *)p)->size >= size) {
					//store the size of the current header to know the size new one
					int oldSize = ((metadata *)p)->size;
					((metadata *)p)->size = size;
					((metadata *)p)->free = 0;
					payload = p + sizeof(metadata);

					//move the pointer to write the new header, if there is enough space
					offset = (sizeof(metadata) + ((metadata *)p)->size);
					p += offset;
					i += offset;

					//new header should be the older pointers free space
					//minus the new size minus the headers size
					//and will be marked as free regardless of total free space
					if (oldSize - size >= sizeof(metadata) && i != sizeof(mem)) {
						((metadata *)p)->size = oldSize - size - sizeof(metadata);
						((metadata *)p)->free = 1;
					}

					//make sure to return the payload
					//printf("header address: %p\n", payload - sizeof(metadata));
					return ((void *)payload);
				} else {
					//not enough memory
				 	//printf("Not enough space to malloc at: Line %d, %s\n", line, file);
                                       	offset = (sizeof(metadata) + ((metadata *)p)->size);
				       	p += offset;
                                       	i += offset;

				}
			} else {
				//not free, need to move to the next chunk
				offset = sizeof(metadata) + ((metadata *)p)->size;
				p += offset;
                                i += offset;
			}
		
		}
	}

       	printf("Not enough space to malloc at: Line %d, %s\n", line, file);

	return NULL;
}
	
void myfree(void *ptr, char *file, int line) {

	if(ptr == NULL){
		printf("Pointer input is null at: Line %d, %s\n", line, file);
		return;
	}
	
	char* p = heap;
	char* prev = heap;

	//check if pointer exists
	int i = 0;

	while (i < sizeof(mem)) {
		
		//compares addresses, this means it matches
		if (ptr == p + sizeof(metadata)) {
			break;
		} else {
			//continue searching, make sure the prev points to where p was to coalesce
			prev = p;
			i += sizeof(metadata) + ((metadata *)p)->size;
			p += sizeof(metadata) + ((metadata *)p)->size;
		}
	}
	
	//bounds check
	if (i >= sizeof(mem)) {
		printf("Bad pointer given (not in heap) at: Line %d, %s\n", line, file);
        	return;
	}
	//once we find the pointer, check if free
	if (((metadata *)p)->free == 1) {
		printf("Chunk has already beem freed at: Line %d, %s\n", line, file);
	//	printf("size of chunk is: %d\n", ((metadata *)p)->size);
		return;
	} else {
		//mark it as free
		((metadata *)p)->free = 1; 
	}


	//now we coalesce free chunks
	//there are only four cases:
	//-no merging
	//-prev needs to be merged
	//-next needs to be merged
	//-prev and next need to be merged
	//consider edge cases (first or last chunk freed, only one chunk allocated, only two allocated)

	char *next = NULL;

	if (i + sizeof(metadata) + ((metadata *)p)->size < sizeof(mem)) {
		next = p + sizeof(metadata) + ((metadata *)p)->size;
	}

	//printf("gets here\n");

	//if the chunk is the first, there is no actual prev
	//in order, checks if it can merge with the one before
	//then, checks if it can merge with the one after
	//then, checks if it can only merge with the one after
	if (prev != p && ((metadata*)prev)->free == 1) {
		((metadata*)prev)->size += sizeof(metadata) + ((metadata *)p)->size;
	//	((metadata*)p)->size = 0;
	        if (next != NULL && ((metadata *)next)->free == 1) {
               		(((metadata *)prev)->size += sizeof(metadata) + ((metadata *)next)->size);
			(((metadata *)next)->size = 0);
		}
	} else {
		//prior isnt free, check the next
		if (next != NULL && ((metadata *)next)->free == 1) {
			(((metadata *)p)->size += sizeof(metadata) + ((metadata *)next)->size);
			(((metadata *)next)->size = 0);
			
		}
	}	
	
	return;
	
}

//check that total bytes still equal 4096 when testing
void totalBytes() {

	int total = 0;
	int offset = 0;
	char *p = heap;

	while (total <= sizeof(mem)) {

		if (total == sizeof(mem)) {
			printf("TOTAL(expected) = %d\n", total);
			return;
		}
		offset = sizeof(metadata) + ((metadata *)p)->size;
		total += offset;
		p += offset;

	}

	printf("TOTAL(error) = %d\n", total);

	return;
}
