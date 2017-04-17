COPTS=-Wall -g -c  -O0 
OBJS=myfs.o \

all: test1 test2 test3 test4 

clean:
	rm -f $(OBJS)
	rm -f out

.c.o:
	gcc $(COPTS) $? -o $@

test1: $(OBJS)
	gcc $(OBJS) test1.c -g -o test1

test2: $(OBJS)
	gcc $(OBJS) test2.c -g -o test2

test3: $(OBJS)
	gcc $(OBJS) test3.c -g -o test3

test4: $(OBJS)
	gcc $(OBJS) test4.c -g -o test4

