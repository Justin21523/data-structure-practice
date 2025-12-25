# Makefile for 02-binary-search-tree (C)  # Provide a short English header so the file is self-describing.

CC = gcc  # Use GCC as the default C compiler.
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -O2  # Enable warnings and use C11 for consistent builds.

DEMO = bst_demo  # Name of the demo executable.
TEST = test_bst  # Name of the test executable.

.PHONY: all clean run test  # Declare phony targets for convenience.

all: $(DEMO) $(TEST)  # Build both the demo and the tests by default.

$(DEMO): bst_main.c bst.c bst.h  # Build the demo CLI + library sources.
	$(CC) $(CFLAGS) bst_main.c bst.c -o $(DEMO)  # Compile and link the demo executable.

$(TEST): test_bst.c bst.c bst.h  # Build the test runner using the library file.
	$(CC) $(CFLAGS) test_bst.c bst.c -o $(TEST)  # Compile and link the test executable.

run: $(DEMO)  # Run the demo after building it.
	./$(DEMO)  # Execute the demo to print a summary and inorder sequences.

test: $(TEST)  # Run the test suite after building it.
	./$(TEST)  # Execute the tests and return non-zero on failure.

clean:  # Remove build outputs to restore a clean working directory.
	rm -f $(DEMO) $(TEST)  # Delete compiled executables.

