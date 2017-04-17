#*************************************************************
# Author:        Philip Howard
# Email:         phil.howard@oit.edu
# Filename:      Makefile
# Date Created:  2016-04-13
# Edited by:	 Jessica Maxey
#*************************************************************

COPTS = -Wall -g -O0 -c -pthread

OBJS = list.o \
       mytest.o \
       hhlist.o \
       hhmytest.o \

all: mytest hhmytest

clean:
	rm -rf mytest
	rm -rf hhmytest
	rm -rf $(OBJS)

.c.o:
	gcc $(COPTS) $? -o $@

mytest: mytest.c list.o mytest.o
	gcc list.o mytest.o -o mytest -pthread

hhmytest: hhmytest.c hhlist.o hhmytest.o
	gcc hhlist.o hhmytest.o -o hhmytest -pthread
