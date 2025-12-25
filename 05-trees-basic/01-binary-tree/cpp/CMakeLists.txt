cmake_minimum_required(VERSION 3.16)  # Require a reasonably modern CMake for CTest ergonomics.
project(binary_tree_demo LANGUAGES CXX)  # Declare a small C++ project for this unit.

set(CMAKE_CXX_STANDARD 17)  # Use C++17 for std::optional and basic modern features.
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Enforce the chosen standard.

add_executable(binary_tree_demo binary_tree_demo.cpp)  # Build the CLI demo executable.
target_compile_options(binary_tree_demo PRIVATE -Wall -Wextra -Wpedantic)  # Enable warnings for cleaner teaching code.

add_executable(test_binary_tree test_binary_tree.cpp)  # Build the test runner executable.
target_compile_options(test_binary_tree PRIVATE -Wall -Wextra -Wpedantic)  # Enable warnings for cleaner teaching code.

enable_testing()  # Enable CTest integration for this directory.
add_test(NAME BinaryTreeTests COMMAND test_binary_tree)  # Register the test executable as a CTest test.

