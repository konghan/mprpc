
CC = gcc
CXX = g++
LEX = flex
YACC = bison


CFLAGS = -I. -O3
CXXFLAGS = -I. -O3


objs = thriftl.o thrifty.o main.o md5.o parse.o t_typedef.o t_generator.o

vpath %.cc parse
vpath %.cc generate


all : $(objs)
	$(CXX) $(LDFLAGS) -o thrift $(objs)

clean : 
	$(RM) -f $(objs) thriftl.cc thriftl.hh thrifty.hh thrifty.cc thrift

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<


thriftl.cc : thriftl.ll thrifty.cc
	$(LEX) -o thriftl.cc --header-file=thriftl.hh thriftl.ll

thrifty.cc : thrifty.yy
	$(YACC) -o thrifty.cc --defines=thrifty.hh thrifty.yy


