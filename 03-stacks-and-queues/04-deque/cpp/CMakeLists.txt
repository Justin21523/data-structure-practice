cmake_minimum_required(VERSION 3.16)  # Require a reasonably modern CMake for CTest ergonomics.
project(deque_demo LANGUAGES CXX)  # Declare a small C++ project for this unit.

set(CMAKE_CXX_STANDARD 17)  # Use C++17 for basic modern features.
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Enforce the chosen standard.

add_executable(deque_demo deque_demo.cpp)  # Build the CLI demo executable.
target_compile_options(deque_demo PRIVATE -Wall -Wextra -Wpedantic)  # Enable warnings for cleaner teaching code.

add_executable(test_deque test_deque.cpp)  # Build the test runner executable.
target_compile_options(test_deque PRIVATE -Wall -Wextra -Wpedantic)  # Enable warnings for cleaner teaching code.

enable_testing()  # Enable CTest integration for this directory.
add_test(NAME DequeTests COMMAND test_deque)  # Register the test executable as a CTest test.

