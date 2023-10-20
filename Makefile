CXX=clang++
CPPFLAGS=-std=c++11
CPPLIBS=`pkg-config --libs glew glfw3` -framework Cocoa -framework IOKit

DEP_DIR=./deps/advmath

LIB_DIR=./lib
INCLUDE_DIR=-I./include -I$(DEP_DIR)/include -I./deps/stb/include
SRC_DIRS=./src
BUILD_DIR=./dist


DEPS_OBJS := $(shell find $(DEP_DIR) -name '*.o')

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

EXAMPLES_SRC := $(shell find examples -name '*.cpp')
EXAMPLES_OUT := $(EXAMPLES_SRC:%.cpp=%)

all:
	$(MAKE) deps
	$(MAKE) build

build: $(LIB_DIR)/libsnbose.a $(LIB_DIR)/libsnbose.so

deps: FORCE
	$(MAKE) -C deps/advmath

examples: all $(EXAMPLES_OUT)


$(LIB_DIR)/libsnbose.so: $(OBJS) $(DEPS_OBJS)
	mkdir -p $(dir $@)
	$(CXX) -shared -o $(LIB_DIR)/libsnbose.so $(DEPS_OBJS) $(CPPLIBS) $(OBJS)

$(LIB_DIR)/libsnbose.a: $(OBJS) $(DEPS_OBJS)
	mkdir -p $(dir $@)
	ar cr $(LIB_DIR)/libsnbose.a $(DEPS_OBJS) $(OBJS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(INCLUDE_DIR) -c $< -o $@

$(EXAMPLES_OUT): $(EXAMPLES_SRC)
	$(CXX) $(CPPFLAGS) $(INCLUDE_DIR) $(CPPLIBS) $(LIB_DIR)/libsnbose.a $@.cpp -o $@


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(EXAMPLES_OUT) $(LIB_DIR)
	make -C deps/advmath clean

FORCE: ;