#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "mymalloc.h"

#define TASK_ITERATIONS 50

void test1(long* timeTaken)
{
	struct timeval start, end;
	
	gettimeofday(&start, NULL);
	for(int i = 0; i < 120; i++)
	{
		char* ptr = (char*)malloc(1);
		free(ptr);
	}
	gettimeofday(&end, NULL);
	long endTime = end.tv_usec;
        long startTime = start.tv_usec;
        *timeTaken = endTime - startTime;

}

void test2(long* timeTaken)
{
	char* ptrArray[120];
	
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);	
	for(int i = 0; i < 120; i++)
	{
		ptrArray[i] = (char*)malloc(1);
	}

	for(int i = 0; i < 120; i++)
	{
		free(ptrArray[i]);
	}
	gettimeofday(&end, NULL);
	long endTime = end.tv_usec;
	long startTime = start.tv_usec;
	*timeTaken = endTime - startTime;
}

void test3(long* timeTaken)
{
	char* ptrArray[120];
	int allocated[120] = {0};
	int location = 0;
	
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	for(int i = 0; i < 120; i++)
	{
		if(location == 0 || (rand() % 2 == 0 && location < 120))
		{
			//printf("Allocated location: %d\n", location);
			ptrArray[location] = malloc(1);
			allocated[location] = 1;
			location++;
		}
		else
		{
			//release the memory
			location--;
			//printf("free location: %d\n", location);
			free(ptrArray[location]);
			allocated[location] = 0;
		}
	}

	//clean up any unreleased memory
	for(int i = 0; i < 120; i++)
	{
		if(allocated[i] == 1)
		{
			free(ptrArray[i]);
		}
	}
	gettimeofday(&end, NULL);
	long endTime = end.tv_usec;
        long startTime = start.tv_usec;
        *timeTaken = endTime - startTime;
	
}

//for test4, we will stress test by trying to allocate larger sizes and immediately freeing them
void test4(long* timeTaken)
{
	struct timeval start;
        struct timeval end;
        gettimeofday(&start, NULL);
	
	for(int i = 0; i < 120; i++)
        {
                char* ptr2 = (char*)malloc(3000);
                free(ptr2);
        }

	

	gettimeofday(&end, NULL);
	long endTime = end.tv_usec;
        long startTime = start.tv_usec;
        *timeTaken = endTime - startTime;


}

//for this test, we will malloc 120 times, and then free the space backwards to see if we coalesce properly
void test5(long* timeTaken)
{
	char* ptrArray[120];

        struct timeval start;
        struct timeval end;
        gettimeofday(&start, NULL);
        for(int i = 0; i < 120; i++)
        {
                ptrArray[i] = (char*)malloc(1);
        }

        for(int i = 119; i >= 0; i--)
        {
                free(ptrArray[i]);
        }
        gettimeofday(&end, NULL);
        long endTime = end.tv_usec;
        long startTime = start.tv_usec;
        *timeTaken = endTime - startTime;


}


int main(int argc, char **argv) {
	
	long totalTest1 = 0; 
	long time_test1 = 0;
	long totalTest2 = 0;
	long time_test2 = 0;
	long totalTest3 = 0;
	long time_test3 = 0;
	long totalTest4 = 0;
	long time_test4 = 0;
	long totalTest5 = 0;
	long time_test5 = 0;

	//repeat each task 50 times and get the total time taken to complete it	
	for(int i = 0; i < TASK_ITERATIONS; i++)
	{
		test1(&time_test1);
		totalTest1 += time_test1;
		time_test1 = 0;
	}	

	for(int i = 0; i < TASK_ITERATIONS; i++)
	{
		test2(&time_test2);
                totalTest2 += time_test2;
                time_test2 = 0;
	}
	
	
	for(int i = 0; i < TASK_ITERATIONS; i++)
	{
		test3(&time_test3);
                totalTest3 += time_test3;
                time_test3 = 0;
	}
	
	
	for(int i = 0; i < TASK_ITERATIONS; i++)
        {
                test4(&time_test4);
                totalTest4 += time_test4;
                time_test4 = 0;
        }
	
	for(int i = 0; i < TASK_ITERATIONS; i++)
        {
                test5(&time_test5);
                totalTest5 += time_test5;
                time_test5 = 0;
        }

	printf("Average Time taken for test1: %lu microseconds.\n", totalTest1/TASK_ITERATIONS);
	printf("Average Time taken for test2: %lu microseconds.\n", totalTest2/TASK_ITERATIONS);
	printf("Average Time taken for test3: %lu microseconds.\n", totalTest3/TASK_ITERATIONS);
	printf("Average Time taken for test4: %lu microseconds.\n", totalTest4/TASK_ITERATIONS);
	printf("Average Time taken for test5: %lu microseconds.\n", totalTest5/TASK_ITERATIONS);
	
	malloc(5000);

	return 0;
}

int alt_main(int argc, char **argv) {

	int x;
	free(&x);
/*
	int *m = malloc(sizeof(int) * 2);
	free(m + 1);
	
	int *p = malloc(sizeof(int) * 100);
	int *q = p;
	free(p);
	free(q);
*/
	return 0;
}
