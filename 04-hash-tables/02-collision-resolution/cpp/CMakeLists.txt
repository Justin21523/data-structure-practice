# CMakeLists.txt - 碰撞解決方法 C++ 專案建置設定
# Collision Resolution C++ Project Build Configuration

cmake_minimum_required(VERSION 3.14)
project(CollisionResolution
    VERSION 1.0
    DESCRIPTION "Hash Table Collision Resolution Implementation"
    LANGUAGES CXX
)

# C++ 標準設定 - C++ Standard Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 編譯選項 - Compiler Options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
    add_compile_options(/W4)
endif()

# Header-only 函式庫 - Header-only library
add_library(collision_resolution INTERFACE)
target_include_directories(collision_resolution INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# 測試執行檔 - Test Executable
add_executable(test_collision test_collision.cpp)
target_link_libraries(test_collision PRIVATE collision_resolution)

# 啟用測試 - Enable Testing
enable_testing()
add_test(NAME CollisionResolutionTests COMMAND test_collision)

# 安裝規則（可選） - Installation Rules (Optional)
# install(FILES Chaining.hpp OpenAddressing.hpp DESTINATION include)

# 顯示建置資訊 - Display Build Information
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
