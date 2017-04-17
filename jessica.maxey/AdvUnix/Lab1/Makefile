CC=gcc
CFLAGS=-Wall -D_POSIX_C_SOURCE -g

all: lab1.o
	$(CC) $(CFLAGS) lab1.o -o lab1

lab1.o: lab1.c
	$(CC) $(CFLAGS) -c lab1.c

.PHONY: clean
clean:
	rm  lab1.o lab1
