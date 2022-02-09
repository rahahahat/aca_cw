TARGET_EXEC ?= out

BUILD_DIR ?= build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o, $(basename $(SRCS)))

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
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