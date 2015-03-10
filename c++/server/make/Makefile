CXX = g++
CXXFLAGS=-g -Wall
INCPATH=-I ../src
PROTOC=protoc

src_path=../src
ut_path=../unittest

all: $(ut_path)/case/test_conf \
	$(ut_path)/case/test_utilities \
	$(ut_path)/case/test_logger \
	$(src_path)/server \
	$(src_path)/client
	@echo "build all over"

PHONY:ut
ut: $(ut_path)/case/test_conf \
	$(ut_path)/case/test_utilities \
	$(ut_path)/case/test_logger
	@echo "build ut over"

$(ut_path)/case/test_conf: $(ut_path)/case/test_conf.o $(src_path)/configure.o $(src_path)/bm_utilities.o
	$(CXX) $(CXXFLAGS) $(ut_path)/case/test_conf.o $(src_path)/configure.o $(src_path)/bm_utilities.o -o $(ut_path)/case/test_conf $(INCPATH)
$(ut_path)/case/test_utilities: $(ut_path)/case/test_utilities.o $(src_path)/bm_utilities.o
	$(CXX) $(CXXFLAGS) $(ut_path)/case/test_utilities.o $(src_path)/bm_utilities.o -o $(ut_path)/case/test_utilities $(INCPATH)
$(ut_path)/case/test_logger: $(ut_path)/case/test_logger.o $(src_path)/logger.o $(src_path)/bm_utilities.o
	$(CXX) $(CXXFLAGS) $(ut_path)/case/test_logger.o $(src_path)/logger.o $(src_path)/bm_utilities.o -o $(ut_path)/case/test_logger $(INCPATH)

$(ut_path)/case/test_conf.o: $(ut_path)/case/test_conf.cc
	$(CXX) $(CXXFLAGS) -c $(ut_path)/case/test_conf.cc -o $(ut_path)/case/test_conf.o $(INCPATH)
$(ut_path)/case/test_utilities.o: $(ut_path)/case/test_utilities.cc
	$(CXX) $(CXXFLAGS) -c $(ut_path)/case/test_utilities.cc -o $(ut_path)/case/test_utilities.o $(INCPATH)
$(ut_path)/case/test_logger.o: $(ut_path)/case/test_logger.cc
	$(CXX) $(CXXFLAGS) -c $(ut_path)/case/test_logger.cc -o $(ut_path)/case/test_logger.o $(INCPATH)

$(src_path)/configure.o: $(src_path)/configure.cc
	$(CXX) $(CXXFLAGS) -c $(src_path)/configure.cc -o $(src_path)/configure.o $(INCPATH)
$(src_path)/bm_utilities.o: $(src_path)/bm_utilities.cc
	$(CXX) $(CXXFLAGS) -c $(src_path)/bm_utilities.cc -o $(src_path)/bm_utilities.o $(INCPATH)
$(src_path)/logger.o: $(src_path)/logger.cc
	$(CXX) $(CXXFLAGS) -c $(src_path)/logger.cc -o $(src_path)/logger.o $(INCPATH)

$(src_path)/interface.pb.o: $(src_path)/interface.pb.cc
	$(CXX) $(CXXFLAGS) -c $(src_path)/interface.pb.cc -o $(src_path)/interface.pb.o $(INCPATH)

$(src_path)/interface.pb.cc: $(src_path)/interface.proto
	$(PROTOC) --proto_path=$(src_path) --cpp_out=$(src_path) $(src_path)/interface.proto

$(src_path)/interface.pb.h: $(src_path)/interface.proto
	$(PROTOC) --proto_path=$(src_path) --cpp_out=$(src_path) $(src_path)/interface.proto

$(src_path)/server.o: $(src_path)/server.cc $(src_path)/interface.pb.h
	$(CXX) $(CXXFLAGS) -c $(src_path)/server.cc -o $(src_path)/server.o $(INCPATH)

$(src_path)/main.o: $(src_path)/main.cc $(src_path)/interface.pb.h
	$(CXX) $(CXXFLAGS) -c $(src_path)/main.cc -o $(src_path)/main.o $(INCPATH)

$(src_path)/server: $(src_path)/main.o $(src_path)/server.o $(src_path)/interface.pb.o $(src_path)/configure.o $(src_path)/bm_utilities.o
	$(CXX) $(CXXFLAGS) $(src_path)/main.o $(src_path)/server.o $(src_path)/interface.pb.o $(src_path)/configure.o $(src_path)/bm_utilities.o \
		-o $(src_path)/server -lpthread -lprotobuf -levent

$(src_path)/client.o: $(src_path)/client.cc $(src_path)/interface.pb.h
	$(CXX) $(CXXFLAGS) -c $(src_path)/client.cc -o $(src_path)/client.o $(INCPATH)

$(src_path)/client: $(src_path)/client.o $(src_path)/interface.pb.o
	$(CXX) $(CXXFLAGS) $(src_path)/client.o $(src_path)/interface.pb.o -o $(src_path)/client -lprotobuf -levent

clean:
	rm -rf $(ut_path)/case/*.o
	rm -rf $(src_path)/*.o
	rm -rf $(src_path)/interface.pb.*
	rm -rf $(ut_path)/case/test_conf
	rm -rf $(ut_path)/case/test_utilities
	rm -rf $(ut_path)/case/test_logger
	rm -rf $(src_path)/server
	rm -rf $(src_path)/client
