
CC = gcc
CFLAGS = -Wall -DDEBUG -O2 -g -I../common
LDFLAGS = -lpthread -levent

objs := main.o

objs += neu-worker.o neu-service.o neu-session.o neu-pdu.o neulog.o fixarray.o

.PHONY : neuworker-test

vpath %.c ../common

%.o : %.c
	$(CC) -c -o $@ $(CFLAGS) $<

objs-neuworker-test := neuworker.o neuworker-test.o fixmap.o fixarray.o logger.o rbtree.o
neuworker-test : $(objs-neuworker-test)
	$(CC) -O2 -o neuworker-test $(objs-neuworker-test) $(LDFLAGS)


objs-neupdu-test := neupdu.o neupdu-test.o logger.o
neupdu-test : $(objs-neupdu-test)
	$(CC) -O2 -o neupdu-test $(objs-neupdu-test) $(LDFLAGS)


all : $(objs)
	$(CC) -o neutest  $(objs) -lpthread -levent

clean :
	rm -f $(objs) $(objs-neuworker-test) $(objs-neupdu-test)
	rm -f neuworker-test neupdu-test

