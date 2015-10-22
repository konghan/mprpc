
CC = gcc
CFLAGS = -Wall -O -g -I/usr/local/include -I../common -I.

objs := main.o

objs += neu-worker.o neu-service.o neu-session.o neu-pdu.o neulog.o fixarray.o

vpath %.c ../common

%.o : %.c
	$(CC) -c -o $@ $(CFLAGS) $<

all : $(objs)
	$(CC) -o neutest  $(objs) -lpthread -levent

clean :
	rm -f $(objs)

