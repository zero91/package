all: test

WORK_ROOT=../../..

CC=g++
INCLUDE=-I$(WORK_ROOT)/public/bm_log/include -I$(WORK_ROOT)/public/bm_configure/include
LIBRARY=-L$(WORK_ROOT)/public/bm_log/lib -L$(WORK_ROOT)/public/bm_configure/lib

test.o : test.cc
	$(CC) -o $@ -c $< $(INCLUDE)

test : test.o
	$(CC) $^ -o $@ $(INCLUDE) $(LIBRARY) -lbm_log -lbm_configure

clean: 
	rm -f *.o
	rm -f test
