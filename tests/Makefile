TARGET_EXEC ?= tests

BUILD_DIR ?= build

SRCS := $(shell find ../src/classes -name \*.cpp) $(shell find ./ -name \*.cpp)
OBJS := $(addsuffix .o, $(basename $(SRCS)))

INC_DIRS := $(shell find ../src/includes -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -Wall -std=c++11
CXX=g++


$(TARGET_EXEC): $(OBJS)
	$(CXX) $(addprefix $(BUILD_DIR)/, $(notdir $(OBJS))) -o ./build/$@

%.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(basename $@).cpp -o $(BUILD_DIR)/$(notdir $@)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)/*

MKDIR_P := mkdir $(BUILD_DIR)