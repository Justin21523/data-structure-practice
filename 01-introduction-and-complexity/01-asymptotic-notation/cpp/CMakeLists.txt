# CMakeLists for 01-asymptotic-notation (C++)  # Provide a short English header.
cmake_minimum_required(VERSION 3.14)  # Require a modern CMake version for consistent behavior.
project(AsymptoticNotationCpp LANGUAGES CXX)  # Define the C++ demo project.

set(CMAKE_CXX_STANDARD 17)  # Use C++17 to match other units in this repository.
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Require the chosen standard.
set(CMAKE_CXX_EXTENSIONS OFF)  # Disable compiler-specific extensions for portability.

if(CMAKE_CXX_COMPILER_ID MATCHES \"GNU|Clang\")  # Apply warnings for GCC/Clang.
    add_compile_options(-Wall -Wextra -Wpedantic)  # Enable common warnings to catch mistakes early.
elseif(MSVC)  # Apply warnings for MSVC.
    add_compile_options(/W4)  # Enable a high warning level on MSVC.
endif()  # End compiler warning selection.

add_executable(asymptotic_demo asymptotic_demo.cpp)  # Build the demo executable that prints the table.
add_executable(test_asymptotic_demo test_asymptotic_demo.cpp)  # Build the test runner executable.

enable_testing()  # Enable CTest so tests can be run via `ctest`.
add_test(NAME AsymptoticDemoTests COMMAND test_asymptotic_demo)  # Register the test executable as a CTest test.

