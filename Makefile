CC = $(shell which clang++)
CC ?= $(shell which g++) # fallback compiler

CPP_SRC = $(wildcard src/*.cpp)
HEADERS = $(wildcard include/*.hpp)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(CPP_SRC))

NBL_FILES = $(shell find test -name '*.nbl')
EXPECTED_FILES = $(patsubst %.nbl, %.nbl.expected, $(NBL_FILES))

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

test: compile
	@failed=0; \
	for nbl in $(NBL_FILES); do \
		expected=$${nbl}.expected; \
		echo "Running test case $$nbl..."; \
		if ! ./bin/nimble $$nbl | diff -u --color $$expected -; then \
			echo "Test case $$nbl failed!"; \
			failed=$$((failed + 1)); \
		fi; \
	done; \
	if [ $$failed -eq 0 ]; then \
		echo; \
		echo "All test cases passed"; \
	else \
		echo "Total failed test cases: $$failed"; \
	fi; \
	exit $$failed

ast:
	$(CC) $(CFLAGS) -c src/token.cpp -o obj/token.o
	$(CC) $(CFLAGS) -c src/expr.cpp -o obj/expr.o
	$(CC) $(CFLAGS) -c src/ast_printer.cpp -o obj/ast_printer.o
	$(CC) $(CFLAGS) -c src/ast_printer_driver.cpp -o obj/ast_printer_driver.o
	$(CC) -o bin/ast obj/token.o obj/expr.o obj/ast_printer.o obj/ast_printer_driver.o

.PHONY: run compile clean test
