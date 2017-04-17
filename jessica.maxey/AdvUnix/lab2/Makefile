##****************************
## Makefile
##
## Build routine for pingpong
## program
## 
## Author: Jessica Maxey
##
## Date: 4/14/16
##

CC = gcc

CFLAGS = -g -Wall -O0 -std=c99

OBJS = pingpong 

##all: $(OBJS)
	   
pingpong: pingpong.o
	     $(CC) -o pingpong pingpong.o $(CFLAGS)

clean:
	$(RM) $(OBJS)
