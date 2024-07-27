CC = $(shell which clang++)
CC ?= $(shell which g++) # fallback compiler

CPP_SRC = $(wildcard src/*.cpp)
HEADERS = $(wildcard include/*.hpp)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(CPP_SRC))

CFLAGS = -std=c++20 -Wall -pedantic -Iinclude
DEP_FLAGS = -MMD -MP

RELEASE_CFLAGS = -O2
DEBUG_CFLAGS = -g -O0

compile: bin/nimble

bin/nimble: $(OBJ) | bin
	"$(CC)" -o $@ $(OBJ)

bin:
	mkdir -p bin

obj/%.o: src/%.cpp $(HEADERS) | obj
	"${CC}" $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj

run: compile
	./bin/nimble

clean:
	rm -f bin/* obj/*.o

test: compile
	./tools/test.sh

bench: compile
	./tools/bench.sh

release: CFLAGS += $(RELEASE_CFLAGS)
release: clean compile

debug: CFLAGS += $(DEBUG_CFLAGS)
debug: clean compile

.PHONY: compile run clean test bench
