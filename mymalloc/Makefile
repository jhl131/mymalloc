CC = gcc
CFLAGS = -g -std=c99 -Wvla -Wall -fsanitize=address,undefined

memgrind: memgrind.o mymalloc.o
	$(CC) $(CFLAGS) $^ -o $@

memtest: memtest.o mymalloc.o
	$(CC) $(CFLAGS) $^ -o $@

mymalloc.o memgrind.o memtest.o: mymalloc.h

clean: 
	rm -f *.o memgrind

test: memgrind
	./memgrind

test2: memtest
	./memtest
