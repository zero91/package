AR := ar crs
LOCAL_ROOT=..

CXX=g++
CXXFLAGS=-pipe -Wall -W -g -fPIC -O

OUTPUT_DIR=$(LOCAL_ROOT)/make/output

SOURCE_DIR=$(LOCAL_ROOT)/src
SOURCE=$(foreach dir,$(subst :, ,$(SOURCE_DIR)),$(wildcard $(dir)/*.cc))

OBJECT_DIR=$(OUTPUT_DIR)/obj
OBJECT=$(addprefix $(OBJECT_DIR)/,$(foreach obj,$(subst .cc,.o,$(SOURCE)),$(shell basename $(obj))))

TARGET_DIR=$(OUTPUT_DIR)/lib
TARGET=$(TARGET_DIR)/libbm_log.a

MAKE_DIR := $(shell mkdir -p $(OBJECT_DIR) $(TARGET_DIR))

all: $(TARGET)
	@echo "\033[1;5;32mBuild $^ succeed\033[0m"

$(OBJECT): $(SOURCE)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(TARGET): $(OBJECT)
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

.PHONY: all clean publish
