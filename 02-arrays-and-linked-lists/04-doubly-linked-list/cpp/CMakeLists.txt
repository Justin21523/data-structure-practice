cmake_minimum_required(VERSION 3.16)  # Require a reasonably modern CMake for CTest ergonomics.
project(doubly_linked_list_demo LANGUAGES CXX)  # Declare a small C++ project for this unit.

set(CMAKE_CXX_STANDARD 17)  # Use C++17 for basic modern features.
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Enforce the chosen standard.

add_executable(doubly_linked_list_demo doubly_linked_list_demo.cpp)  # Build the CLI demo executable.
target_compile_options(doubly_linked_list_demo PRIVATE -Wall -Wextra -Wpedantic)  # Enable warnings for cleaner teaching code.

add_executable(test_doubly_linked_list test_doubly_linked_list.cpp)  # Build the test runner executable.
target_compile_options(test_doubly_linked_list PRIVATE -Wall -Wextra -Wpedantic)  # Enable warnings for cleaner teaching code.

enable_testing()  # Enable CTest integration for this directory.
add_test(NAME DoublyLinkedListTests COMMAND test_doubly_linked_list)  # Register the test executable as a CTest test.

