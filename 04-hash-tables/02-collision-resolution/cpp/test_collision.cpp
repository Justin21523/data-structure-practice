/** Doc block start
 * 碰撞解決方法單元測試 / Collision Resolution Unit Tests
 *(blank line)
 * 使用簡單的測試框架 / Using simple testing framework
 */  // End of block comment

#include <iostream>  // Execute this statement as part of the data structure implementation.
#include <string>  // Execute this statement as part of the data structure implementation.
#include <cassert>  // Execute this statement as part of the data structure implementation.
#include <stdexcept>  // Execute this statement as part of the data structure implementation.
#include "Chaining.hpp"  // Execute this statement as part of the data structure implementation.
#include "OpenAddressing.hpp"  // Execute this statement as part of the data structure implementation.

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

// ========== 鏈結法測試 Chaining Tests ==========

TEST(test_chaining_create_empty) {  // Execute this statement as part of the data structure implementation.
    ChainedHashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    assert(ht.size() == 0);  // Execute this statement as part of the data structure implementation.
    assert(ht.empty());  // Execute this statement as part of the data structure implementation.
    assert(ht.capacity() == 16);  // 預設容量
}  // Close the current block scope.

TEST(test_chaining_insert_and_search) {  // Execute this statement as part of the data structure implementation.
    ChainedHashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
    ht.insert("banana", 200);  // Execute this statement as part of the data structure implementation.
    ht.insert("cherry", 300);  // Execute this statement as part of the data structure implementation.

    assert(ht.search("apple").value() == 100);  // Execute this statement as part of the data structure implementation.
    assert(ht.search("banana").value() == 200);  // Execute this statement as part of the data structure implementation.
    assert(ht.search("cherry").value() == 300);  // Execute this statement as part of the data structure implementation.
    assert(!ht.search("date").has_value());  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_chaining_update_existing) {  // Execute this statement as part of the data structure implementation.
    ChainedHashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 150);  // Execute this statement as part of the data structure implementation.

    assert(ht.search("apple").value() == 150);  // Execute this statement as part of the data structure implementation.
    assert(ht.size() == 1);  // 大小不應增加
}  // Close the current block scope.

TEST(test_chaining_remove) {  // Execute this statement as part of the data structure implementation.
    ChainedHashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
    ht.insert("banana", 200);  // Execute this statement as part of the data structure implementation.

    assert(ht.remove("apple") == true);  // Execute this statement as part of the data structure implementation.
    assert(!ht.search("apple").has_value());  // Execute this statement as part of the data structure implementation.
    assert(ht.size() == 1);  // Execute this statement as part of the data structure implementation.

    assert(ht.remove("cherry") == false);  // 不存在的鍵
}  // Close the current block scope.

TEST(test_chaining_load_factor) {  // Execute this statement as part of the data structure implementation.
    ChainedHashTable<std::string, int> ht(10);  // Execute this statement as part of the data structure implementation.
    ht.insert("a", 1);  // Execute this statement as part of the data structure implementation.
    ht.insert("b", 2);  // Execute this statement as part of the data structure implementation.
    ht.insert("c", 3);  // Execute this statement as part of the data structure implementation.

    assert(ht.loadFactor() == 0.3);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_chaining_probe_count) {  // Advance or track the probing sequence used by open addressing.
    ChainedHashTable<std::string, int> ht(1);  // 強制碰撞

    size_t probes1 = ht.insert("a", 1);  // Advance or track the probing sequence used by open addressing.
    size_t probes2 = ht.insert("b", 2);  // Advance or track the probing sequence used by open addressing.
    size_t probes3 = ht.insert("c", 3);  // Advance or track the probing sequence used by open addressing.

    // 第一次插入應該是 1 次探測 / 第一次 insert should 是 1 次 probe
    assert(probes1 == 1);  // Advance or track the probing sequence used by open addressing.
    // 後續插入會有更多探測（需要遍歷鏈結） / 後續 insert 會有更多 probe (需要遍歷 chaining )
    assert(probes2 >= 1);  // Advance or track the probing sequence used by open addressing.
    assert(probes3 >= 1);  // Advance or track the probing sequence used by open addressing.

    // 測試搜尋的探測次數 / Test search 的 probe 次數
    size_t search_probes;  // Advance or track the probing sequence used by open addressing.
    ht.search("c", search_probes);  // Advance or track the probing sequence used by open addressing.
    assert(search_probes >= 1);  // Advance or track the probing sequence used by open addressing.
}  // Close the current block scope.

TEST(test_chaining_statistics) {  // Execute this statement as part of the data structure implementation.
    ChainedHashTable<std::string, int> ht(4);  // Execute this statement as part of the data structure implementation.
    ht.insert("a", 1);  // Execute this statement as part of the data structure implementation.
    ht.insert("b", 2);  // Execute this statement as part of the data structure implementation.
    ht.insert("c", 3);  // Execute this statement as part of the data structure implementation.
    ht.insert("d", 4);  // Execute this statement as part of the data structure implementation.

    // 測試統計功能 - Test statistics
    assert(ht.getMaxChainLength() >= 1);  // Execute this statement as part of the data structure implementation.
    assert(ht.getAverageChainLength() >= 1.0);  // Execute this statement as part of the data structure implementation.
    assert(ht.getTotalProbes() >= 4);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_chaining_clear) {  // Execute this statement as part of the data structure implementation.
    ChainedHashTable<std::string, int> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert("a", 1);  // Execute this statement as part of the data structure implementation.
    ht.insert("b", 2);  // Execute this statement as part of the data structure implementation.
    ht.clear();  // Execute this statement as part of the data structure implementation.

    assert(ht.empty());  // Execute this statement as part of the data structure implementation.
    assert(ht.size() == 0);  // Execute this statement as part of the data structure implementation.
    assert(ht.getTotalProbes() == 0);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 開放定址法 - 線性探測測試 Open Addressing - Linear Probing Tests ==========

TEST(test_linear_create_empty) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::LINEAR);  // Execute this statement as part of the data structure implementation.
    assert(ht.size() == 0);  // Execute this statement as part of the data structure implementation.
    assert(ht.empty());  // Execute this statement as part of the data structure implementation.
    assert(ht.capacity() == 16);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_linear_insert_and_search) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::LINEAR);  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
    ht.insert("banana", 200);  // Execute this statement as part of the data structure implementation.
    ht.insert("cherry", 300);  // Execute this statement as part of the data structure implementation.

    assert(ht.search("apple").value() == 100);  // Execute this statement as part of the data structure implementation.
    assert(ht.search("banana").value() == 200);  // Execute this statement as part of the data structure implementation.
    assert(ht.search("cherry").value() == 300);  // Execute this statement as part of the data structure implementation.
    assert(!ht.search("date").has_value());  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_linear_update_existing) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::LINEAR);  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 150);  // Execute this statement as part of the data structure implementation.

    assert(ht.search("apple").value() == 150);  // Execute this statement as part of the data structure implementation.
    assert(ht.size() == 1);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_linear_remove_with_tombstone) {  // Handle tombstones so deletions do not break the probing/search sequence.
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::LINEAR);  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
    ht.insert("banana", 200);  // Execute this statement as part of the data structure implementation.

    assert(ht.remove("apple") == true);  // Execute this statement as part of the data structure implementation.
    assert(!ht.search("apple").has_value());  // Execute this statement as part of the data structure implementation.
    assert(ht.size() == 1);  // Execute this statement as part of the data structure implementation.
    assert(ht.getDeletedCount() == 1);  // 應該有一個墓碑

    // 刪除後仍能找到其他鍵 / Delete 後 still 能 be found 其他 key
    assert(ht.search("banana").value() == 200);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_linear_probe_count) {  // Advance or track the probing sequence used by open addressing.
    OpenAddressingHashTable<int, int> ht(10, ProbeMethod::LINEAR);  // Execute this statement as part of the data structure implementation.

    // 插入會產生碰撞的鍵（假設它們雜湊到相同位置） / Insert 會產生 collision 的 key (假設它們 hash 到相同位置)
    size_t probes1 = ht.insert(0, 100);  // Advance or track the probing sequence used by open addressing.
    size_t probes2 = ht.insert(10, 200);  // 可能碰撞
    size_t probes3 = ht.insert(20, 300);  // 可能碰撞

    // 探測次數應該遞增 / Probe 次數 should 遞增
    assert(probes1 >= 1);  // Advance or track the probing sequence used by open addressing.
    assert(probes2 >= 1);  // Advance or track the probing sequence used by open addressing.
    assert(probes3 >= 1);  // Advance or track the probing sequence used by open addressing.
}  // Close the current block scope.

TEST(test_linear_load_factor) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable<std::string, int> ht(10, ProbeMethod::LINEAR);  // Execute this statement as part of the data structure implementation.
    ht.insert("a", 1);  // Execute this statement as part of the data structure implementation.
    ht.insert("b", 2);  // Execute this statement as part of the data structure implementation.
    ht.insert("c", 3);  // Execute this statement as part of the data structure implementation.

    double lf = ht.loadFactor();  // Assign or update a variable that represents the current algorithm state.
    assert(lf == 0.3);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_linear_table_full) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable<int, int> ht(5, ProbeMethod::LINEAR);  // Execute this statement as part of the data structure implementation.

    // 插入直到接近負載因子限制（0.7） / Insert 直到接近 load factor 限制 (0.7)
    ht.insert(0, 0);  // Execute this statement as part of the data structure implementation.
    ht.insert(1, 1);  // Execute this statement as part of the data structure implementation.
    ht.insert(2, 2);  // Execute this statement as part of the data structure implementation.

    // 第4次插入應該會超過負載因子限制 / 第4次 insert should 會超過 load factor 限制
    bool threw = false;  // Assign or update a variable that represents the current algorithm state.
    try {  // Execute this statement as part of the data structure implementation.
        ht.insert(3, 3);  // Execute this statement as part of the data structure implementation.
        ht.insert(4, 4);  // 這應該拋出例外
    } catch (const std::runtime_error&) {  // Execute this statement as part of the data structure implementation.
        threw = true;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.
    assert(threw);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 開放定址法 - 二次探測測試 Open Addressing - Quadratic Probing Tests ==========

TEST(test_quadratic_insert_and_search) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::QUADRATIC);  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
    ht.insert("banana", 200);  // Execute this statement as part of the data structure implementation.
    ht.insert("cherry", 300);  // Execute this statement as part of the data structure implementation.

    assert(ht.search("apple").value() == 100);  // Execute this statement as part of the data structure implementation.
    assert(ht.search("banana").value() == 200);  // Execute this statement as part of the data structure implementation.
    assert(ht.search("cherry").value() == 300);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_quadratic_remove) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::QUADRATIC);  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
    ht.insert("banana", 200);  // Execute this statement as part of the data structure implementation.

    assert(ht.remove("apple") == true);  // Execute this statement as part of the data structure implementation.
    assert(!ht.search("apple").has_value());  // Execute this statement as part of the data structure implementation.
    assert(ht.search("banana").value() == 200);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_quadratic_probe_method) {  // Advance or track the probing sequence used by open addressing.
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::QUADRATIC);  // Execute this statement as part of the data structure implementation.
    assert(ht.getProbeMethod() == ProbeMethod::QUADRATIC);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 開放定址法 - 雙重雜湊測試 Open Addressing - Double Hashing Tests ==========

TEST(test_double_hash_insert_and_search) {  // Compute a hash-based index so keys map into the table's storage.
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::DOUBLE_HASH);  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
    ht.insert("banana", 200);  // Execute this statement as part of the data structure implementation.
    ht.insert("cherry", 300);  // Execute this statement as part of the data structure implementation.

    assert(ht.search("apple").value() == 100);  // Execute this statement as part of the data structure implementation.
    assert(ht.search("banana").value() == 200);  // Execute this statement as part of the data structure implementation.
    assert(ht.search("cherry").value() == 300);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_double_hash_remove) {  // Compute a hash-based index so keys map into the table's storage.
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::DOUBLE_HASH);  // Execute this statement as part of the data structure implementation.
    ht.insert("apple", 100);  // Execute this statement as part of the data structure implementation.
    ht.insert("banana", 200);  // Execute this statement as part of the data structure implementation.

    assert(ht.remove("apple") == true);  // Execute this statement as part of the data structure implementation.
    assert(!ht.search("apple").has_value());  // Execute this statement as part of the data structure implementation.
    assert(ht.search("banana").value() == 200);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_double_hash_probe_method) {  // Advance or track the probing sequence used by open addressing.
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::DOUBLE_HASH);  // Execute this statement as part of the data structure implementation.
    assert(ht.getProbeMethod() == ProbeMethod::DOUBLE_HASH);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_double_hash_collisions) {  // Compute a hash-based index so keys map into the table's storage.
    OpenAddressingHashTable<int, int> ht(10, ProbeMethod::DOUBLE_HASH);  // Execute this statement as part of the data structure implementation.

    // 插入可能碰撞的鍵 / Insert may collision 的 key
    ht.insert(0, 100);  // Execute this statement as part of the data structure implementation.
    ht.insert(10, 200);  // Execute this statement as part of the data structure implementation.
    ht.insert(20, 300);  // Execute this statement as part of the data structure implementation.

    assert(ht.search(0).value() == 100);  // Execute this statement as part of the data structure implementation.
    assert(ht.search(10).value() == 200);  // Execute this statement as part of the data structure implementation.
    assert(ht.search(20).value() == 300);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 通用測試 General Tests ==========

TEST(test_open_addressing_clear) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::LINEAR);  // Execute this statement as part of the data structure implementation.
    ht.insert("a", 1);  // Execute this statement as part of the data structure implementation.
    ht.insert("b", 2);  // Execute this statement as part of the data structure implementation.
    ht.remove("a");  // Execute this statement as part of the data structure implementation.

    assert(ht.getDeletedCount() == 1);  // Execute this statement as part of the data structure implementation.

    ht.clear();  // Execute this statement as part of the data structure implementation.

    assert(ht.empty());  // Execute this statement as part of the data structure implementation.
    assert(ht.size() == 0);  // Execute this statement as part of the data structure implementation.
    assert(ht.getDeletedCount() == 0);  // Execute this statement as part of the data structure implementation.
    assert(ht.getTotalProbes() == 0);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_open_addressing_statistics) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable<int, int> ht(10, ProbeMethod::LINEAR);  // Execute this statement as part of the data structure implementation.

    ht.resetProbeCount();  // Execute this statement as part of the data structure implementation.
    ht.insert(1, 10);  // Execute this statement as part of the data structure implementation.
    ht.insert(2, 20);  // Execute this statement as part of the data structure implementation.

    assert(ht.getTotalProbes() >= 2);  // Execute this statement as part of the data structure implementation.

    ht.resetProbeCount();  // Execute this statement as part of the data structure implementation.
    assert(ht.getTotalProbes() == 0);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_int_keys_chaining) {  // Execute this statement as part of the data structure implementation.
    ChainedHashTable<int, std::string> ht;  // Execute this statement as part of the data structure implementation.
    ht.insert(1, "one");  // Execute this statement as part of the data structure implementation.
    ht.insert(2, "two");  // Execute this statement as part of the data structure implementation.
    ht.insert(3, "three");  // Execute this statement as part of the data structure implementation.

    assert(ht.search(1).value() == "one");  // Execute this statement as part of the data structure implementation.
    assert(ht.search(2).value() == "two");  // Execute this statement as part of the data structure implementation.
    assert(ht.search(3).value() == "three");  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_int_keys_open_addressing) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable<int, std::string> ht(16, ProbeMethod::LINEAR);  // Execute this statement as part of the data structure implementation.
    ht.insert(1, "one");  // Execute this statement as part of the data structure implementation.
    ht.insert(2, "two");  // Execute this statement as part of the data structure implementation.
    ht.insert(3, "three");  // Execute this statement as part of the data structure implementation.

    assert(ht.search(1).value() == "one");  // Execute this statement as part of the data structure implementation.
    assert(ht.search(2).value() == "two");  // Execute this statement as part of the data structure implementation.
    assert(ht.search(3).value() == "three");  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 主函式 Main Function ==========

int main() {  // Execute this statement as part of the data structure implementation.
    std::cout << "=== 碰撞解決方法單元測試 Collision Resolution Unit Tests ===" << std::endl  // Execute this statement as part of the data structure implementation.
              << std::endl;  // Execute this statement as part of the data structure implementation.

    std::cout << "--- 鏈結法測試 Chaining Tests ---" << std::endl;  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_chaining_create_empty);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_chaining_insert_and_search);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_chaining_update_existing);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_chaining_remove);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_chaining_load_factor);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_chaining_probe_count);  // Advance or track the probing sequence used by open addressing.
    RUN_TEST(test_chaining_statistics);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_chaining_clear);  // Execute this statement as part of the data structure implementation.

    std::cout << std::endl;  // Execute this statement as part of the data structure implementation.
    std::cout << "--- 開放定址法 - 線性探測測試 Open Addressing - Linear Probing Tests ---" << std::endl;  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_linear_create_empty);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_linear_insert_and_search);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_linear_update_existing);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_linear_remove_with_tombstone);  // Handle tombstones so deletions do not break the probing/search sequence.
    RUN_TEST(test_linear_probe_count);  // Advance or track the probing sequence used by open addressing.
    RUN_TEST(test_linear_load_factor);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_linear_table_full);  // Execute this statement as part of the data structure implementation.

    std::cout << std::endl;  // Execute this statement as part of the data structure implementation.
    std::cout << "--- 開放定址法 - 二次探測測試 Open Addressing - Quadratic Probing Tests ---" << std::endl;  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_quadratic_insert_and_search);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_quadratic_remove);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_quadratic_probe_method);  // Advance or track the probing sequence used by open addressing.

    std::cout << std::endl;  // Execute this statement as part of the data structure implementation.
    std::cout << "--- 開放定址法 - 雙重雜湊測試 Open Addressing - Double Hashing Tests ---" << std::endl;  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_double_hash_insert_and_search);  // Compute a hash-based index so keys map into the table's storage.
    RUN_TEST(test_double_hash_remove);  // Compute a hash-based index so keys map into the table's storage.
    RUN_TEST(test_double_hash_probe_method);  // Advance or track the probing sequence used by open addressing.
    RUN_TEST(test_double_hash_collisions);  // Compute a hash-based index so keys map into the table's storage.

    std::cout << std::endl;  // Execute this statement as part of the data structure implementation.
    std::cout << "--- 通用測試 General Tests ---" << std::endl;  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_open_addressing_clear);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_open_addressing_statistics);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_int_keys_chaining);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_int_keys_open_addressing);  // Execute this statement as part of the data structure implementation.

    // 結果摘要 - Results summary
    std::cout << std::endl;  // Execute this statement as part of the data structure implementation.
    std::cout << "========================================" << std::endl;  // Execute this statement as part of the data structure implementation.
    std::cout << "測試結果 Test Results: " << passed << " 通過 passed, "  // Execute this statement as part of the data structure implementation.
              << failed << " 失敗 failed" << std::endl;  // Execute this statement as part of the data structure implementation.
    std::cout << "========================================" << std::endl;  // Execute this statement as part of the data structure implementation.

    return failed > 0 ? 1 : 0;  // Return the computed result to the caller.
}  // Close the current block scope.
