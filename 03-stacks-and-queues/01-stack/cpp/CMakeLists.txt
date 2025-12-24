cmake_minimum_required(VERSION 3.16)  # Require a reasonably modern CMake for CTest ergonomics.
project(stack_demo LANGUAGES CXX)  # Declare a small C++ project for this unit.

set(CMAKE_CXX_STANDARD 17)  # Use C++17 for basic modern features.
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Enforce the chosen standard.

add_executable(stack_demo stack_demo.cpp)  # Build the CLI demo executable.
target_compile_options(stack_demo PRIVATE -Wall -Wextra -Wpedantic)  # Enable warnings for cleaner teaching code.

add_executable(test_stack test_stack.cpp)  # Build the test runner executable.
target_compile_options(test_stack PRIVATE -Wall -Wextra -Wpedantic)  # Enable warnings for cleaner teaching code.

enable_testing()  # Enable CTest integration for this directory.
add_test(NAME StackTests COMMAND test_stack)  # Register the test executable as a CTest test.

