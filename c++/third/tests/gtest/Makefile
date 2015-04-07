all: test_gtest

BASE_ROOT=../..

THIRD_INCLUDE_PATH=-I$(BASE_ROOT)/gtest/include
THIRD_LIB_PATH=-L$(BASE_ROOT)/gtest/lib

test_gtest: test_gtest.cc
	g++ test_gtest.cc -o test_gtest -Wall $(THIRD_LIB_PATH) $(THIRD_INCLUDE_PATH) -lgtest

clean:
	rm test_gtest
