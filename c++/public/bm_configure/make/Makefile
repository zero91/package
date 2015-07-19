AR := ar crs
WORK_ROOT=../../..
LOCAL_ROOT=..

CXX=g++
CXXFLAGS=-pipe -Wall -W -fPIC -O
INCLUDE=-I$(WORK_ROOT)/public/bm_log/include
LIBRARY=-L$(WORK_ROOT)/public/bm_log/lib

SOURCE_DIR=$(LOCAL_ROOT)/src
OUTPUT_DIR=$(LOCAL_ROOT)/make/output
OBJECT_DIR=$(OUTPUT_DIR)/obj
TARGET_DIR=$(OUTPUT_DIR)/lib

SOURCE=$(foreach dir,$(subst :, ,$(SOURCE_DIR)),$(wildcard $(dir)/*.cc))
OBJECT = $(addprefix $(OBJECT_DIR)/, \
			$(foreach obj,$(subst .cc,.o,$(SOURCE)),$(shell basename $(obj))))
TARGET=$(TARGET_DIR)/libbm_configure.a

MAKE_DIR := $(shell mkdir -p $(OBJECT_DIR) $(TARGET_DIR))

all: $(TARGET)
	@echo "\033[1;5;32mBuild $^ succeed\033[0m"

$(OBJECT): $(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(INCLUDE)

$(TARGET): $(OBJECT) $(WORK_ROOT)/public/bm_log/lib/libbm_log.a
	@rm -rf $@
	$(AR) $@ $^ 

publish: $(TARGET)
	rm -rf $(LOCAL_ROOT)/include
	rm -rf $(LOCAL_ROOT)/lib
	mkdir -p $(LOCAL_ROOT)/include
	mkdir -p $(LOCAL_ROOT)/lib
	cp $(SOURCE_DIR)/*.h $(LOCAL_ROOT)/include
	cp $(TARGET_DIR)/*.a $(LOCAL_ROOT)/lib
	@echo "\033[1;5;32mPublish succeed!\033[0m"

clean:
	@rm -rf $(OUTPUT_DIR)

clean-all:
	@rm -rf $(OUTPUT_DIR)
	@rm -rf $(LOCAL_ROOT)/include
	@rm -rf $(LOCAL_ROOT)/lib

.PHONY: all clean clean-all publish
