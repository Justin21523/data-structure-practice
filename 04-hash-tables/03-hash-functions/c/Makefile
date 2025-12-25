# Makefile for 03-hash-functions (C)  # Provide a short English header so the file is self-describing.

CC = gcc  # Use GCC as the default C compiler.
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -O2  # Enable warnings and use C11 for consistent builds.

DEMO = hash_functions_demo  # Name of the demo executable.
TEST = test_hash_functions  # Name of the test executable.

.PHONY: all clean run test  # Declare phony targets for convenience.

all: $(DEMO) $(TEST)  # Build both the demo and the tests by default.

$(DEMO): hash_functions_main.c hash_functions.c universal_hashing.c hash_functions.h universal_hashing.h  # Build demo + library sources.
	$(CC) $(CFLAGS) hash_functions_main.c hash_functions.c universal_hashing.c -lm -o $(DEMO)  # Compile and link the demo (needs -lm).

$(TEST): test_hash_functions.c hash_functions.c universal_hashing.c hash_functions.h universal_hashing.h  # Build tests + library sources.
	$(CC) $(CFLAGS) test_hash_functions.c hash_functions.c universal_hashing.c -lm -o $(TEST)  # Compile and link the tests (needs -lm).

run: $(DEMO)  # Run the demo after building it.
	./$(DEMO)  # Execute the demo to print sample hash outputs and distribution stats.

test: $(TEST)  # Run the test suite after building it.
	./$(TEST)  # Execute the tests and return non-zero on failure.

clean:  # Remove build outputs to restore a clean working directory.
	rm -f $(DEMO) $(TEST)  # Delete compiled executables.

