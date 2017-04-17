##****************************
## Makefile
##
## Build routine for filter program
## 
## Author: Jessica Maxey
##
## Date: 3/31/16
##

CC = gcc

CFLAGS = -g -Wall -O0 -std=c99

TARGET_PRE = prefilter
TARGET_POST = postfilter
TARGET_PROCESS = process

all: $(TARGET_PRE) $(TARGET_POST) $(TARGET_PROCESS)

$(TARGET_PRE): $(TARGET_PRE).c
	     $(CC) $(CFLAGS) -o $(TARGET_PRE) $(TARGET_PRE).c

	   
$(TARGET_POST): $(TARGET_POST).c
	     $(CC) $(CFLAGS) -o $(TARGET_POST) $(TARGET_POST).c


$(TARGET_PROCESS): $(TARGET_PROCESS).c
	     $(CC) $(CFLAGS) -o $(TARGET_PROCESS) $(TARGET_PROCESS).c
clean:
	$(RM) $(TARGET_PRE)
	$(RM) $(TARGET_POST)
	$(RM) $(TARGET_PROCESS)
