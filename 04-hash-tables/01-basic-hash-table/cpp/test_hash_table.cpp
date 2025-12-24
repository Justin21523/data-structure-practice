/**
 * 雜湊表單元測試
 * Hash Table Unit Tests
 *
 * 使用簡單的測試框架
 * Using simple testing framework
 */

#include <iostream>
#include <string>
#include <cassert>
#include <stdexcept>
#include "HashTable.hpp"

// 簡單的測試框架 - Simple testing framework
#define TEST(name) void name()
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
} while(0)

int passed = 0;
int failed = 0;

// ========== 基本操作測試 Basic Operations Tests ==========

TEST(test_create_empty_hash_table) {
    HashTable<std::string, int> ht;
    assert(ht.size() == 0);
    assert(ht.empty());
    assert(ht.capacity() == 16);  // 預設容量
}

TEST(test_create_with_custom_capacity) {
    HashTable<std::string, int> ht(32);
    assert(ht.capacity() == 32);
}

TEST(test_invalid_capacity) {
    bool threw = false;
    try {
        HashTable<std::string, int> ht(0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}

TEST(test_insert_and_search) {
    HashTable<std::string, int> ht;
    ht.insert("apple", 100);
    ht.insert("banana", 200);

    assert(ht.search("apple").value() == 100);
    assert(ht.search("banana").value() == 200);
    assert(!ht.search("cherry").has_value());
}

TEST(test_insert_update_existing) {
    HashTable<std::string, int> ht;
    ht.insert("apple", 100);
    ht.insert("apple", 150);

    assert(ht.search("apple").value() == 150);
    assert(ht.size() == 1);  // 大小不應增加
}

TEST(test_remove) {
    HashTable<std::string, int> ht;
    ht.insert("apple", 100);
    ht.insert("banana", 200);

    assert(ht.remove("apple") == true);
    assert(!ht.search("apple").has_value());
    assert(ht.size() == 1);
}

TEST(test_remove_nonexistent) {
    HashTable<std::string, int> ht;
    ht.insert("apple", 100);

    assert(ht.remove("banana") == false);
    assert(ht.size() == 1);
}

TEST(test_contains) {
    HashTable<std::string, int> ht;
    ht.insert("apple", 100);

    assert(ht.contains("apple") == true);
    assert(ht.contains("banana") == false);
}

// ========== 容量測試 Capacity Tests ==========

TEST(test_clear) {
    HashTable<std::string, int> ht;
    ht.insert("a", 1);
    ht.insert("b", 2);
    ht.clear();

    assert(ht.empty());
    assert(ht.size() == 0);
    assert(!ht.search("a").has_value());
}

TEST(test_load_factor) {
    HashTable<std::string, int> ht(10);
    ht.insert("a", 1);
    ht.insert("b", 2);
    ht.insert("c", 3);

    assert(ht.loadFactor() == 0.3);
}

// ========== 擴容測試 Rehashing Tests ==========

TEST(test_rehash_on_load_factor) {
    // 使用小容量以快速觸發擴容
    HashTable<std::string, int> ht(4);

    // 插入足夠多的元素使負載因子超過 0.75
    ht.insert("key0", 0);
    ht.insert("key1", 1);
    ht.insert("key2", 2);
    ht.insert("key3", 3);

    // 應該已經擴容
    assert(ht.capacity() > 4);

    // 所有元素應該仍然可以存取
    for (int i = 0; i < 4; ++i) {
        assert(ht.search("key" + std::to_string(i)).value() == i);
    }
}

// ========== 運算子測試 Operator Tests ==========

TEST(test_bracket_operator_access) {
    HashTable<std::string, int> ht;
    ht.insert("apple", 100);

    assert(ht["apple"] == 100);
}

TEST(test_bracket_operator_insert) {
    HashTable<std::string, int> ht;
    ht["apple"] = 100;

    assert(ht.search("apple").value() == 100);
}

TEST(test_bracket_operator_default_value) {
    HashTable<std::string, int> ht;
    int& value = ht["newkey"];  // 應該插入預設值 0

    assert(value == 0);
    assert(ht.contains("newkey"));
}

TEST(test_at_method) {
    HashTable<std::string, int> ht;
    ht.insert("apple", 100);

    assert(ht.at("apple") == 100);

    bool threw = false;
    try {
        ht.at("banana");
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
}

// ========== 迭代器測試 Iterator Tests ==========

TEST(test_iterator) {
    HashTable<std::string, int> ht;
    ht.insert("a", 1);
    ht.insert("b", 2);
    ht.insert("c", 3);

    int count = 0;
    int sum = 0;
    for (auto& pair : ht) {
        count++;
        sum += pair.second;
    }

    assert(count == 3);
    assert(sum == 6);
}

// ========== 碰撞測試 Collision Tests ==========

TEST(test_multiple_items_same_bucket) {
    // 使用容量 1 強制所有元素碰撞
    HashTable<std::string, int> ht(1);

    ht.insert("a", 1);
    ht.insert("b", 2);
    ht.insert("c", 3);

    assert(ht.search("a").value() == 1);
    assert(ht.search("b").value() == 2);
    assert(ht.search("c").value() == 3);

    ht.remove("b");
    assert(!ht.search("b").has_value());
    assert(ht.search("a").value() == 1);
    assert(ht.search("c").value() == 3);
}

// ========== 不同鍵類型測試 Different Key Types Tests ==========

TEST(test_int_key) {
    HashTable<int, std::string> ht;
    ht.insert(1, "one");
    ht.insert(2, "two");
    ht.insert(3, "three");

    assert(ht.search(1).value() == "one");
    assert(ht.search(2).value() == "two");
    assert(ht.search(3).value() == "three");
}

// ========== 主函式 Main Function ==========

int main() {
    std::cout << "=== 雜湊表單元測試 Hash Table Unit Tests ===" << std::endl
              << std::endl;

    // 基本操作測試 - Basic operations tests
    RUN_TEST(test_create_empty_hash_table);
    RUN_TEST(test_create_with_custom_capacity);
    RUN_TEST(test_invalid_capacity);
    RUN_TEST(test_insert_and_search);
    RUN_TEST(test_insert_update_existing);
    RUN_TEST(test_remove);
    RUN_TEST(test_remove_nonexistent);
    RUN_TEST(test_contains);

    // 容量測試 - Capacity tests
    RUN_TEST(test_clear);
    RUN_TEST(test_load_factor);

    // 擴容測試 - Rehashing tests
    RUN_TEST(test_rehash_on_load_factor);

    // 運算子測試 - Operator tests
    RUN_TEST(test_bracket_operator_access);
    RUN_TEST(test_bracket_operator_insert);
    RUN_TEST(test_bracket_operator_default_value);
    RUN_TEST(test_at_method);

    // 迭代器測試 - Iterator tests
    RUN_TEST(test_iterator);

    // 碰撞測試 - Collision tests
    RUN_TEST(test_multiple_items_same_bucket);

    // 不同鍵類型測試 - Different key types tests
    RUN_TEST(test_int_key);

    // 結果摘要 - Results summary
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "測試結果 Test Results: " << passed << " 通過 passed, "
              << failed << " 失敗 failed" << std::endl;
    std::cout << "========================================" << std::endl;

    return failed > 0 ? 1 : 0;
}
