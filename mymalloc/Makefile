CC = gcc
CFLAGS = -g -std=c99 -Wvla -Wall -fsanitize=address,undefined

memgrind: memgrind.o mymalloc.o
	$(CC) $(CFLAGS) $^ -o $@

mymalloc.o memgrind.o: mymalloc.h

clean: 
	rm -f *.o memgrind

test:
	./memgrind
