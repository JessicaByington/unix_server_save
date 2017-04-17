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

OBJS = list memalloc

all: $(OBJS)
	   
$(OBJS): $(OBJS).c
	     $(CC) $(CFLAGS) -o $(OBJS).o $(OBJS).c

clean:
	$(RM) $(OBJS)
