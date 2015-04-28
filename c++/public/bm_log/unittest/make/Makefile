all : test_logger

BASE_ROOT=../../../..
LOCAL_ROOT=../..
LIB_PATH=-L$(BASE_ROOT)/public/bm_log/lib
INC_PATH=-I$(BASE_ROOT)/public/bm_log/include

test_logger : $(LOCAL_ROOT)/unittest/case/test_logger.cc
	g++ $(LOCAL_ROOT)/unittest/case/test_logger.cc -o test_logger $(LIB_PATH) $(INC_PATH) -lbm_log -DNDEBUG

clean:
	rm -rf test_logger
