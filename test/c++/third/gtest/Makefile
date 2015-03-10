all: test_gtest

THIRD_INCLUDE_PATH=-I../c++/third/gtest/include 
THIRD_LIB_PATH=-L../c++/third/gtest/lib

test_gtest: test_gtest.cc
	g++ test_gtest.cc -o test_gtest -Wall $(THIRD_LIB_PATH) $(THIRD_INCLUDE_PATH) -lgtest

clean:
	rm test_gtest
