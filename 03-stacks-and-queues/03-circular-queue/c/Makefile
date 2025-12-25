# Makefile for 03-circular-queue (C)  # Provide a short English header so the file is self-describing.

CC = gcc  # Use GCC as the default C compiler.
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -O2  # Enable warnings and use C11 for consistent builds.

DEMO = circular_queue_demo  # Name of the demo executable.
TEST = test_circular_queue  # Name of the test executable.

.PHONY: all clean run test  # Declare phony targets for convenience.

all: $(DEMO) $(TEST)  # Build both the demo and the tests by default.

$(DEMO): circular_queue_main.c circular_queue.c circular_queue.h  # Build the demo CLI + library sources.
	$(CC) $(CFLAGS) circular_queue_main.c circular_queue.c -o $(DEMO)  # Compile and link the demo executable.

$(TEST): test_circular_queue.c circular_queue.c circular_queue.h  # Build the test runner using the library file.
	$(CC) $(CFLAGS) test_circular_queue.c circular_queue.c -o $(TEST)  # Compile and link the test executable.

run: $(DEMO)  # Run the demo after building it.
	./$(DEMO)  # Execute the demo to print copy/shift-cost tables and a FIFO example.

test: $(TEST)  # Run the test suite after building it.
	./$(TEST)  # Execute the tests and return non-zero on failure.

clean:  # Remove build outputs to restore a clean working directory.
	rm -f $(DEMO) $(TEST)  # Delete compiled executables.

