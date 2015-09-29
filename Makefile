
CC = gcc
CFLAGS = -O -g

objs := main.o

objs += neu-worker.o neu-service.o neu-session.o neu-pdu.o neulog.o

%.o : %.c
	$(CC) -c -o $@ $(CFLAGS) $<

all : $(objs)
	$(CC) -o neutest  $(objs) -pthread -levent



