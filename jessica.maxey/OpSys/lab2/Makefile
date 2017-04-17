##****************************
## Makefile
##
## Build routine for memory allocator
## program
## 
## Author: Jessica Maxey
##
## Date: 4/8/16
##

CC = gcc

CFLAGS = -g -Wall -O0 -std=c99

OBJS = list memalloc main

##all: $(OBJS)
	   
memalloc: memalloc.o list.o main.o
	     $(CC) -o memalloc memalloc.o list.o main.o $(CFLAGS)

clean:
	$(RM) $(OBJS)
