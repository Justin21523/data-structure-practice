cmake_minimum_required(VERSION 3.16)  # Require a reasonably modern CMake for CTest ergonomics.
project(dynamic_array_demo LANGUAGES CXX)  # Declare a small C++ project for this unit.

set(CMAKE_CXX_STANDARD 17)  # Use C++17 for std::make_unique and general modern features.
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Enforce the chosen standard.

add_executable(dynamic_array_demo dynamic_array_demo.cpp)  # Build the CLI demo executable.
target_compile_options(dynamic_array_demo PRIVATE -Wall -Wextra -Wpedantic)  # Enable warnings for cleaner teaching code.

add_executable(test_dynamic_array test_dynamic_array.cpp)  # Build the test runner executable.
target_compile_options(test_dynamic_array PRIVATE -Wall -Wextra -Wpedantic)  # Enable warnings for cleaner teaching code.

enable_testing()  # Enable CTest integration for this directory.
add_test(NAME DynamicArrayTests COMMAND test_dynamic_array)  # Register the test executable as a CTest test.

