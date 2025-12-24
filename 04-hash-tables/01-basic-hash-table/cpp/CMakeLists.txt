# CMakeLists.txt - 雜湊表 C++ 專案建置設定
# Hash Table C++ Project Build Configuration

cmake_minimum_required(VERSION 3.14)
project(HashTable
    VERSION 1.0
    DESCRIPTION "Hash Table Implementation"
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
add_library(hash_table INTERFACE)
target_include_directories(hash_table INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# 應用範例執行檔 - Example Application Executable
add_executable(example_word_count example_word_count.cpp)
target_link_libraries(example_word_count PRIVATE hash_table)

# 測試執行檔 - Test Executable
add_executable(test_hash_table test_hash_table.cpp)
target_link_libraries(test_hash_table PRIVATE hash_table)

# 啟用測試 - Enable Testing
enable_testing()
add_test(NAME HashTableTests COMMAND test_hash_table)

# 安裝規則（可選） - Installation Rules (Optional)
# install(FILES HashTable.hpp DESTINATION include)

# 顯示建置資訊 - Display Build Information
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
