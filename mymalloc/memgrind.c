#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

int main(int argc, char **argv) {
	malloc(1);
	malloc(2);
	return 0;
}

