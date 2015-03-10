AR := ar cqs

CXX = g++
CXXFLAGS=-pipe -Wall -W -g -fPIC -O
INCPATH=-I.

all: libutilities.a
	@echo "build liblog.a over"

.PHONY: all clean

.cc.o:
	$(CXX) -c -o $@ $<  $(CXXFLAGS) $(INCPATH)


libutilities_a_OBJS := bm_utilities.o

libutilities.a : $(libutilities_a_OBJS)
	rm -f $@
	ar cr $@ $^

clean:
	rm -rf *.o
	rm -rf libutilities.a
