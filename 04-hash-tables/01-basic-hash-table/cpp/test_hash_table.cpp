/** Doc block start
 * 雜湊表單元測試 / Hash Table Unit Tests
 *(blank line)
 * 使用簡單的測試框架 / Using simple testing framework
 */  // End of block comment

#include <iostream>  // Execute this statement as part of the data structure implementation.
#include <string>  // Execute this statement as part of the data structure implementation.
#include <cassert>  // Execute this statement as part of the data structure implementation.
#include <stdexcept>  // Execute this statement as part of the data structure implementation.
#include "HashTable.hpp"  // Execute this statement as part of the data structure implementation.

// 簡單的測試框架 - Simple testing framework
#define TEST(name) void name()  // Execute this statement as part of the data structure implementation.
#define RUN_TEST(name) do { \
    std::cout << "測試 Testing: " << #name << "... "; \
    try { \
        name(); \
        std::cout << "通過 PASSED" << std::endl; \
        passed++; \
    } catch (const std::exception& e) { \
        std::cout << "失敗 FAILED: " << e.what() << std::endl; \
        failed++; \
    } \
} while(0)  // Execute this statement as part of the data structure implementation.

int passed = 0;  // Assign or update a variable that represents the current algorithm state.
int failed = 0;  // Assign or update a variable that represents the current algorithm state.

// ========== 基本操作測試 Basic Operations Tests ==========

TEST(test_create_empty_hash_table) {  // Compute a hash-based index so keys map into the table's storage.
    HashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    assert(ht.size() == 0);  // Execute this statement as part of the data structure implementation.
    assert(ht.empty());  // Execute this statement as part of the data structure implementation.
    assert(ht.capacity() == 16);  // 預設容量
}  // Close the current block scope.

TEST(test_create_with_custom_capacity) {  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int> ht(32);  // Execute this statement as part of the data structure implementation.
    assert(ht.capacity() == 32);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_invalid_capacity) {  // Execute this statement as part of the data structure implementation.
    bool threw = false;  // Assign or update a variable that represents the current algorithm state.
    try {  // Execute this statement as part of the data structure implementation.
        HashTable<std::string, int> ht(0);  // Execute this statement as part of the data structure implementation.
    } catch (const std::invalid_argument&) {  // Execute this statement as part of the data structure implementation.
        threw = true;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.
    assert(threw);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_insert_and_search) {  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
    ht.insert("banana", 200);  // Execute this statement as part of the data structure implementation.

    assert(ht.search("apple").value() == 100);  // Execute this statement as part of the data structure implementation.
    assert(ht.search("banana").value() == 200);  // Execute this statement as part of the data structure implementation.
    assert(!ht.search("cherry").has_value());  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_insert_update_existing) {  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 150);  // Execute this statement as part of the data structure implementation.

    assert(ht.search("apple").value() == 150);  // Execute this statement as part of the data structure implementation.
    assert(ht.size() == 1);  // 大小不應增加
}  // Close the current block scope.

TEST(test_remove) {  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
    ht.insert("banana", 200);  // Execute this statement as part of the data structure implementation.

    assert(ht.remove("apple") == true);  // Execute this statement as part of the data structure implementation.
    assert(!ht.search("apple").has_value());  // Execute this statement as part of the data structure implementation.
    assert(ht.size() == 1);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_remove_nonexistent) {  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.

    assert(ht.remove("banana") == false);  // Execute this statement as part of the data structure implementation.
    assert(ht.size() == 1);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_contains) {  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.

    assert(ht.contains("apple") == true);  // Execute this statement as part of the data structure implementation.
    assert(ht.contains("banana") == false);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 容量測試 Capacity Tests ==========

TEST(test_clear) {  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert("a", 1);  // Execute this statement as part of the data structure implementation.
    ht.insert("b", 2);  // Execute this statement as part of the data structure implementation.
    ht.clear();  // Execute this statement as part of the data structure implementation.

    assert(ht.empty());  // Execute this statement as part of the data structure implementation.
    assert(ht.size() == 0);  // Execute this statement as part of the data structure implementation.
    assert(!ht.search("a").has_value());  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_load_factor) {  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int> ht(10);  // Execute this statement as part of the data structure implementation.
    ht.insert("a", 1);  // Execute this statement as part of the data structure implementation.
    ht.insert("b", 2);  // Execute this statement as part of the data structure implementation.
    ht.insert("c", 3);  // Execute this statement as part of the data structure implementation.

    assert(ht.loadFactor() == 0.3);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 擴容測試 Rehashing Tests ==========

TEST(test_rehash_on_load_factor) {  // Rehash entries into a larger table to keep operations near O(1) on average.
    // 使用小容量以快速觸發擴容 / Use 小 capacity 以快速觸發 resize
    HashTable<std::string, int> ht(4);  // Execute this statement as part of the data structure implementation.

    // 插入足夠多的元素使負載因子超過 0.75 / Insert 足夠多的 elements 使 load factor 超過 0.75
    ht.insert("key0", 0);  // Execute this statement as part of the data structure implementation.
    ht.insert("key1", 1);  // Execute this statement as part of the data structure implementation.
    ht.insert("key2", 2);  // Execute this statement as part of the data structure implementation.
    ht.insert("key3", 3);  // Execute this statement as part of the data structure implementation.

    // 應該已經擴容 / Should 已經 resize
    assert(ht.capacity() > 4);  // Execute this statement as part of the data structure implementation.

    // 所有元素應該仍然可以存取 / 所有 elements should still 然 can 存取
    for (int i = 0; i < 4; ++i) {  // Iterate over a range/collection to process each item in sequence.
        assert(ht.search("key" + std::to_string(i)).value() == i);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.
}  // Close the current block scope.

// ========== 運算子測試 Operator Tests ==========

TEST(test_bracket_operator_access) {  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.

    assert(ht["apple"] == 100);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_bracket_operator_insert) {  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    ht["apple"] = 100;  // Assign or update a variable that represents the current algorithm state.

    assert(ht.search("apple").value() == 100);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_bracket_operator_default_value) {  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    int& value = ht["newkey"];  // 應該插入預設值 0

    assert(value == 0);  // Execute this statement as part of the data structure implementation.
    assert(ht.contains("newkey"));  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_at_method) {  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.

    assert(ht.at("apple") == 100);  // Execute this statement as part of the data structure implementation.

    bool threw = false;  // Assign or update a variable that represents the current algorithm state.
    try {  // Execute this statement as part of the data structure implementation.
        ht.at("banana");  // Execute this statement as part of the data structure implementation.
    } catch (const std::out_of_range&) {  // Execute this statement as part of the data structure implementation.
        threw = true;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.
    assert(threw);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 迭代器測試 Iterator Tests ==========

TEST(test_iterator) {  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert("a", 1);  // Execute this statement as part of the data structure implementation.
    ht.insert("b", 2);  // Execute this statement as part of the data structure implementation.
    ht.insert("c", 3);  // Execute this statement as part of the data structure implementation.

    int count = 0;  // Assign or update a variable that represents the current algorithm state.
    int sum = 0;  // Assign or update a variable that represents the current algorithm state.
    for (auto& pair : ht) {  // Iterate over a range/collection to process each item in sequence.
        count++;  // Execute this statement as part of the data structure implementation.
        sum += pair.second;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    assert(count == 3);  // Execute this statement as part of the data structure implementation.
    assert(sum == 6);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 碰撞測試 Collision Tests ==========

TEST(test_multiple_items_same_bucket) {  // Access or update the bucket storage used to hold entries or chains.
    // 使用容量 1 強制所有元素碰撞 / Use capacity 1 強制所有 elements collision
    HashTable<std::string, int> ht(1);  // Execute this statement as part of the data structure implementation.

    ht.insert("a", 1);  // Execute this statement as part of the data structure implementation.
    ht.insert("b", 2);  // Execute this statement as part of the data structure implementation.
    ht.insert("c", 3);  // Execute this statement as part of the data structure implementation.

    assert(ht.search("a").value() == 1);  // Execute this statement as part of the data structure implementation.
    assert(ht.search("b").value() == 2);  // Execute this statement as part of the data structure implementation.
    assert(ht.search("c").value() == 3);  // Execute this statement as part of the data structure implementation.

    ht.remove("b");  // Execute this statement as part of the data structure implementation.
    assert(!ht.search("b").has_value());  // Execute this statement as part of the data structure implementation.
    assert(ht.search("a").value() == 1);  // Execute this statement as part of the data structure implementation.
    assert(ht.search("c").value() == 3);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 不同鍵類型測試 Different Key Types Tests ==========

TEST(test_int_key) {  // Execute this statement as part of the data structure implementation.
    HashTable<int, std::string> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert(1, "one");  // Execute this statement as part of the data structure implementation.
    ht.insert(2, "two");  // Execute this statement as part of the data structure implementation.
    ht.insert(3, "three");  // Execute this statement as part of the data structure implementation.

    assert(ht.search(1).value() == "one");  // Execute this statement as part of the data structure implementation.
    assert(ht.search(2).value() == "two");  // Execute this statement as part of the data structure implementation.
    assert(ht.search(3).value() == "three");  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 主函式 Main Function ==========

int main() {  // Execute this statement as part of the data structure implementation.
    std::cout << "=== 雜湊表單元測試 Hash Table Unit Tests ===" << std::endl  // Execute this statement as part of the data structure implementation.
              << std::endl;  // Execute this statement as part of the data structure implementation.

    // 基本操作測試 - Basic operations tests
    RUN_TEST(test_create_empty_hash_table);  // Compute a hash-based index so keys map into the table's storage.
    RUN_TEST(test_create_with_custom_capacity);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_invalid_capacity);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_insert_and_search);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_insert_update_existing);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_remove);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_remove_nonexistent);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_contains);  // Execute this statement as part of the data structure implementation.

    // 容量測試 - Capacity tests
    RUN_TEST(test_clear);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_load_factor);  // Execute this statement as part of the data structure implementation.

    // 擴容測試 - Rehashing tests
    RUN_TEST(test_rehash_on_load_factor);  // Rehash entries into a larger table to keep operations near O(1) on average.

    // 運算子測試 - Operator tests
    RUN_TEST(test_bracket_operator_access);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_bracket_operator_insert);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_bracket_operator_default_value);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_at_method);  // Execute this statement as part of the data structure implementation.

    // 迭代器測試 - Iterator tests
    RUN_TEST(test_iterator);  // Execute this statement as part of the data structure implementation.

    // 碰撞測試 - Collision tests
    RUN_TEST(test_multiple_items_same_bucket);  // Access or update the bucket storage used to hold entries or chains.

    // 不同鍵類型測試 - Different key types tests
    RUN_TEST(test_int_key);  // Execute this statement as part of the data structure implementation.

    // 結果摘要 - Results summary
    std::cout << std::endl;  // Execute this statement as part of the data structure implementation.
    std::cout << "========================================" << std::endl;  // Execute this statement as part of the data structure implementation.
    std::cout << "測試結果 Test Results: " << passed << " 通過 passed, "  // Execute this statement as part of the data structure implementation.
              << failed << " 失敗 failed" << std::endl;  // Execute this statement as part of the data structure implementation.
    std::cout << "========================================" << std::endl;  // Execute this statement as part of the data structure implementation.

    return failed > 0 ? 1 : 0;  // Return the computed result to the caller.
}  // Close the current block scope.
