# Makefile for 03-tree-traversal (C)  # Provide a short English header so the file is self-describing.

CC = gcc  # Use GCC as the default C compiler.
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -O2  # Enable warnings and use C11 for consistent builds.

DEMO = tree_traversal_demo  # Name of the demo executable.
TEST = test_tree_traversal  # Name of the test executable.

.PHONY: all clean run test  # Declare phony targets for convenience.

all: $(DEMO) $(TEST)  # Build both the demo and the tests by default.

$(DEMO): tree_traversal_main.c tree_traversal.c tree_traversal.h  # Build the demo CLI + library sources.
	$(CC) $(CFLAGS) tree_traversal_main.c tree_traversal.c -o $(DEMO)  # Compile and link the demo executable.

$(TEST): test_tree_traversal.c tree_traversal.c tree_traversal.h  # Build the test runner using the library file.
	$(CC) $(CFLAGS) test_tree_traversal.c tree_traversal.c -o $(TEST)  # Compile and link the test executable.

run: $(DEMO)  # Run the demo after building it.
	./$(DEMO)  # Execute the demo to print traversal sequences for both implementations.

test: $(TEST)  # Run the test suite after building it.
	./$(TEST)  # Execute the tests and return non-zero on failure.

clean:  # Remove build outputs to restore a clean working directory.
	rm -f $(DEMO) $(TEST)  # Delete compiled executables.

