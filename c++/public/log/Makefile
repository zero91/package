AR := ar cqs

CXX = g++
CXXFLAGS=-pipe -Wall -W -g -fPIC -O
INCPATH=-I.

all: liblog.a
	@echo "build liblog.a over"

.PHONY: all clean

.cc.o:
	$(CXX) -c -o $@ $<  $(CXXFLAGS) $(INCPATH)


liblog_a_OBJS := logger.o

liblog.a : $(liblog_a_OBJS)
	rm -f $@
	ar cr $@ $^

clean:
	rm -rf *.o
	rm -rf liblog.a
