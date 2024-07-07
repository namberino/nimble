CC = $(shell which clang++)
CC ?= $(shell which g++) # fallback compiler

CPP_SRC = $(wildcard src/*.cpp)
HEADERS = $(wildcard include/*.hpp)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(CPP_SRC))

CFLAGS = -g -std=c++20 -Wall -pedantic -Iinclude

run: compile
	./bin/nimble

compile: bin/nimble

bin/nimble: $(OBJ) | bin
	$(CC) -o $@ $(OBJ)

bin:
	mkdir -p bin

obj/%.o: src/%.cpp $(HEADERS) | obj
	${CC} $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj

clean:
	rm -f bin/* obj/*.o

.PHONY: run compile clean
