cmake_minimum_required(VERSION 3.16)  # Require a reasonably modern CMake for CTest ergonomics.
project(hash_functions_demo LANGUAGES CXX)  # Declare a small C++ project for this unit.

set(CMAKE_CXX_STANDARD 17)  # Use C++17 for std::optional and basic modern features.
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Enforce the chosen standard.

add_executable(hash_functions_demo hash_functions_demo.cpp)  # Build the CLI demo executable.
target_compile_options(hash_functions_demo PRIVATE -Wall -Wextra -Wpedantic)  # Enable warnings for cleaner teaching code.

add_executable(test_hash_functions test_hash_functions.cpp)  # Build the test runner executable.
target_compile_options(test_hash_functions PRIVATE -Wall -Wextra -Wpedantic)  # Enable warnings for cleaner teaching code.

enable_testing()  # Enable CTest integration for this directory.
add_test(NAME HashFunctionsTests COMMAND test_hash_functions)  # Register the test executable as a CTest test.

