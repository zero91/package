AR := ar cqs

CXX = g++
CXXFLAGS=-pipe -Wall -W -g -fPIC -O
INCPATH=-I. -I../utilities

all: libconf.a
	@echo "build libconf.a over"

.PHONY: all clean

.cc.o:
	$(CXX) -c -o $@ $<  $(CXXFLAGS) $(INCPATH)


libconf_a_OBJS := configure.o

libconf.a : $(libconf_a_OBJS)
	rm -f $@
	ar cr $@ $^

clean:
	rm -rf *.o
	rm -rf libconf.a
