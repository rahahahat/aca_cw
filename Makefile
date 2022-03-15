TARGET_EXEC ?= out

BUILD_DIR ?= build
SRC_DIRS ?= ./src
VPATH = $(shell find ./ -type d)
TMP_VPATH := $(VPATH)
VPATH = $(filter-out ./src/lib/% ./.git%, $(TMP_VPATH))

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
TMP_SRCS := $(SRCS)
SRCS = $(filter-out ./src/lib/fmt/*.cpp , $(TMP_SRCS))

OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
TMP_INC_DIRS := $(INC_DIRS)
INC_DIRS = $(filter-out ./src/lib/% -I./.git%, $(TMP_INC_DIRS))
INC_FLAGS := $(addprefix -I,$(INC_DIRS)) 


CPPFLAGS ?= $(INC_FLAGS) -Wall
CXX=g++

$(TARGET_EXEC): $(OBJS)
	$(CXX) $(CPPFLAGS) -o build/$@ $^

build/%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)/*

MKDIR_P := mkdir $(BUILD_DIR)