AR := ar cqs

CXX = g++
CXXFLAGS=-pipe -Wall -W -g -fPIC -O
INCPATH=-I.

all: libbmhead.a
	@echo "build libbmhead.a over"

.PHONY: all clean

.cc.o:
	$(CXX) -c -o $@ $<  $(CXXFLAGS) $(INCPATH)


libbmhead_a_OBJS := bmhead.o

libbmhead.a : $(libbmhead_a_OBJS)
	rm -f $@
	ar cr $@ $^

clean:
	rm -rf *.o
	rm -rf libbmhead.a
