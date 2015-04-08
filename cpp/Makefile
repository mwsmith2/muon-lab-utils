# Grab the targets from the src directory.
TARGETS = $(patsubst src/%.cxx, bin/%, $(wildcard src/*.cxx))

# Figure out the architecture
UNAME_S = $(shell uname -s)

# Clang compiler
ifeq ($(UNAME_S), Darwin)
	CXX = clang++
	CC  = clang
	FLAGS = -std=c++11 -DOS_DARWIN
endif

# Gnu compiler
ifeq ($(UNAME_S), Linux)
	CXX = g++
	CC  = gcc
	FLAGS = -std=c++0x -DOS_LINUX
endif

ifdef DEBUG
	FLAGS += -g -O3
else
	FLAGS += -O3
endif

# ROOT libs and flags
FLAGS += $(shell root-config --cflags)
LIBS = $(shell root-config --libs)

FLAGS += -Iinclude
LIBS += -lm -lzmq -lutil

all: $(TARGETS)

bin/%: src/%.cxx 
	$(CXX) -o $@ $+ $(FLAGS) $(LIBS)

clean:
	rm -f $(TARGETS)
