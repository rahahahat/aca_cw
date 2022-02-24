TARGET_EXEC ?= tests

BUILD_DIR ?= build

VPATH = ../src/classes ../tests/ $(shell find . -type d)
SRCS := $(shell find ../src/classes -name \*.cpp) $(shell find ./ -name \*.cpp)

CPP_OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))

INC_DIRS := $(shell find ../src/includes -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -Wall -std=c++11
CXX=g++

$(TARGET_EXEC): $(CPP_OBJS)
	$(CXX) $(CPPFLAGS) -o build/$@ $^

build/%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)/*

MKDIR_P := mkdir $(BUILD_DIR)