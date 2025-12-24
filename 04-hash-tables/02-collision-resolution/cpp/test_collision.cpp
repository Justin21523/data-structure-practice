/**
 * 碰撞解決方法單元測試
 * Collision Resolution Unit Tests
 *
 * 使用簡單的測試框架
 * Using simple testing framework
 */

#include <iostream>
#include <string>
#include <cassert>
#include <stdexcept>
#include "Chaining.hpp"
#include "OpenAddressing.hpp"

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

// ========== 鏈結法測試 Chaining Tests ==========

TEST(test_chaining_create_empty) {
    ChainedHashTable<std::string, int> ht;
    assert(ht.size() == 0);
    assert(ht.empty());
    assert(ht.capacity() == 16);  // 預設容量
}

TEST(test_chaining_insert_and_search) {
    ChainedHashTable<std::string, int> ht;
    ht.insert("apple", 100);
    ht.insert("banana", 200);
    ht.insert("cherry", 300);

    assert(ht.search("apple").value() == 100);
    assert(ht.search("banana").value() == 200);
    assert(ht.search("cherry").value() == 300);
    assert(!ht.search("date").has_value());
}

TEST(test_chaining_update_existing) {
    ChainedHashTable<std::string, int> ht;
    ht.insert("apple", 100);
    ht.insert("apple", 150);

    assert(ht.search("apple").value() == 150);
    assert(ht.size() == 1);  // 大小不應增加
}

TEST(test_chaining_remove) {
    ChainedHashTable<std::string, int> ht;
    ht.insert("apple", 100);
    ht.insert("banana", 200);

    assert(ht.remove("apple") == true);
    assert(!ht.search("apple").has_value());
    assert(ht.size() == 1);

    assert(ht.remove("cherry") == false);  // 不存在的鍵
}

TEST(test_chaining_load_factor) {
    ChainedHashTable<std::string, int> ht(10);
    ht.insert("a", 1);
    ht.insert("b", 2);
    ht.insert("c", 3);

    assert(ht.loadFactor() == 0.3);
}

TEST(test_chaining_probe_count) {
    ChainedHashTable<std::string, int> ht(1);  // 強制碰撞

    size_t probes1 = ht.insert("a", 1);
    size_t probes2 = ht.insert("b", 2);
    size_t probes3 = ht.insert("c", 3);

    // 第一次插入應該是 1 次探測
    assert(probes1 == 1);
    // 後續插入會有更多探測（需要遍歷鏈結）
    assert(probes2 >= 1);
    assert(probes3 >= 1);

    // 測試搜尋的探測次數
    size_t search_probes;
    ht.search("c", search_probes);
    assert(search_probes >= 1);
}

TEST(test_chaining_statistics) {
    ChainedHashTable<std::string, int> ht(4);
    ht.insert("a", 1);
    ht.insert("b", 2);
    ht.insert("c", 3);
    ht.insert("d", 4);

    // 測試統計功能 - Test statistics
    assert(ht.getMaxChainLength() >= 1);
    assert(ht.getAverageChainLength() >= 1.0);
    assert(ht.getTotalProbes() >= 4);
}

TEST(test_chaining_clear) {
    ChainedHashTable<std::string, int> ht;
    ht.insert("a", 1);
    ht.insert("b", 2);
    ht.clear();

    assert(ht.empty());
    assert(ht.size() == 0);
    assert(ht.getTotalProbes() == 0);
}

// ========== 開放定址法 - 線性探測測試 Open Addressing - Linear Probing Tests ==========

TEST(test_linear_create_empty) {
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::LINEAR);
    assert(ht.size() == 0);
    assert(ht.empty());
    assert(ht.capacity() == 16);
}

TEST(test_linear_insert_and_search) {
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::LINEAR);
    ht.insert("apple", 100);
    ht.insert("banana", 200);
    ht.insert("cherry", 300);

    assert(ht.search("apple").value() == 100);
    assert(ht.search("banana").value() == 200);
    assert(ht.search("cherry").value() == 300);
    assert(!ht.search("date").has_value());
}

TEST(test_linear_update_existing) {
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::LINEAR);
    ht.insert("apple", 100);
    ht.insert("apple", 150);

    assert(ht.search("apple").value() == 150);
    assert(ht.size() == 1);
}

TEST(test_linear_remove_with_tombstone) {
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::LINEAR);
    ht.insert("apple", 100);
    ht.insert("banana", 200);

    assert(ht.remove("apple") == true);
    assert(!ht.search("apple").has_value());
    assert(ht.size() == 1);
    assert(ht.getDeletedCount() == 1);  // 應該有一個墓碑

    // 刪除後仍能找到其他鍵
    assert(ht.search("banana").value() == 200);
}

TEST(test_linear_probe_count) {
    OpenAddressingHashTable<int, int> ht(10, ProbeMethod::LINEAR);

    // 插入會產生碰撞的鍵（假設它們雜湊到相同位置）
    size_t probes1 = ht.insert(0, 100);
    size_t probes2 = ht.insert(10, 200);  // 可能碰撞
    size_t probes3 = ht.insert(20, 300);  // 可能碰撞

    // 探測次數應該遞增
    assert(probes1 >= 1);
    assert(probes2 >= 1);
    assert(probes3 >= 1);
}

TEST(test_linear_load_factor) {
    OpenAddressingHashTable<std::string, int> ht(10, ProbeMethod::LINEAR);
    ht.insert("a", 1);
    ht.insert("b", 2);
    ht.insert("c", 3);

    double lf = ht.loadFactor();
    assert(lf == 0.3);
}

TEST(test_linear_table_full) {
    OpenAddressingHashTable<int, int> ht(5, ProbeMethod::LINEAR);

    // 插入直到接近負載因子限制（0.7）
    ht.insert(0, 0);
    ht.insert(1, 1);
    ht.insert(2, 2);

    // 第4次插入應該會超過負載因子限制
    bool threw = false;
    try {
        ht.insert(3, 3);
        ht.insert(4, 4);  // 這應該拋出例外
    } catch (const std::runtime_error&) {
        threw = true;
    }
    assert(threw);
}

// ========== 開放定址法 - 二次探測測試 Open Addressing - Quadratic Probing Tests ==========

TEST(test_quadratic_insert_and_search) {
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::QUADRATIC);
    ht.insert("apple", 100);
    ht.insert("banana", 200);
    ht.insert("cherry", 300);

    assert(ht.search("apple").value() == 100);
    assert(ht.search("banana").value() == 200);
    assert(ht.search("cherry").value() == 300);
}

TEST(test_quadratic_remove) {
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::QUADRATIC);
    ht.insert("apple", 100);
    ht.insert("banana", 200);

    assert(ht.remove("apple") == true);
    assert(!ht.search("apple").has_value());
    assert(ht.search("banana").value() == 200);
}

TEST(test_quadratic_probe_method) {
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::QUADRATIC);
    assert(ht.getProbeMethod() == ProbeMethod::QUADRATIC);
}

// ========== 開放定址法 - 雙重雜湊測試 Open Addressing - Double Hashing Tests ==========

TEST(test_double_hash_insert_and_search) {
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::DOUBLE_HASH);
    ht.insert("apple", 100);
    ht.insert("banana", 200);
    ht.insert("cherry", 300);

    assert(ht.search("apple").value() == 100);
    assert(ht.search("banana").value() == 200);
    assert(ht.search("cherry").value() == 300);
}

TEST(test_double_hash_remove) {
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::DOUBLE_HASH);
    ht.insert("apple", 100);
    ht.insert("banana", 200);

    assert(ht.remove("apple") == true);
    assert(!ht.search("apple").has_value());
    assert(ht.search("banana").value() == 200);
}

TEST(test_double_hash_probe_method) {
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::DOUBLE_HASH);
    assert(ht.getProbeMethod() == ProbeMethod::DOUBLE_HASH);
}

TEST(test_double_hash_collisions) {
    OpenAddressingHashTable<int, int> ht(10, ProbeMethod::DOUBLE_HASH);

    // 插入可能碰撞的鍵
    ht.insert(0, 100);
    ht.insert(10, 200);
    ht.insert(20, 300);

    assert(ht.search(0).value() == 100);
    assert(ht.search(10).value() == 200);
    assert(ht.search(20).value() == 300);
}

// ========== 通用測試 General Tests ==========

TEST(test_open_addressing_clear) {
    OpenAddressingHashTable<std::string, int> ht(16, ProbeMethod::LINEAR);
    ht.insert("a", 1);
    ht.insert("b", 2);
    ht.remove("a");

    assert(ht.getDeletedCount() == 1);

    ht.clear();

    assert(ht.empty());
    assert(ht.size() == 0);
    assert(ht.getDeletedCount() == 0);
    assert(ht.getTotalProbes() == 0);
}

TEST(test_open_addressing_statistics) {
    OpenAddressingHashTable<int, int> ht(10, ProbeMethod::LINEAR);

    ht.resetProbeCount();
    ht.insert(1, 10);
    ht.insert(2, 20);

    assert(ht.getTotalProbes() >= 2);

    ht.resetProbeCount();
    assert(ht.getTotalProbes() == 0);
}

TEST(test_int_keys_chaining) {
    ChainedHashTable<int, std::string> ht;
    ht.insert(1, "one");
    ht.insert(2, "two");
    ht.insert(3, "three");

    assert(ht.search(1).value() == "one");
    assert(ht.search(2).value() == "two");
    assert(ht.search(3).value() == "three");
}

TEST(test_int_keys_open_addressing) {
    OpenAddressingHashTable<int, std::string> ht(16, ProbeMethod::LINEAR);
    ht.insert(1, "one");
    ht.insert(2, "two");
    ht.insert(3, "three");

    assert(ht.search(1).value() == "one");
    assert(ht.search(2).value() == "two");
    assert(ht.search(3).value() == "three");
}

// ========== 主函式 Main Function ==========

int main() {
    std::cout << "=== 碰撞解決方法單元測試 Collision Resolution Unit Tests ===" << std::endl
              << std::endl;

    std::cout << "--- 鏈結法測試 Chaining Tests ---" << std::endl;
    RUN_TEST(test_chaining_create_empty);
    RUN_TEST(test_chaining_insert_and_search);
    RUN_TEST(test_chaining_update_existing);
    RUN_TEST(test_chaining_remove);
    RUN_TEST(test_chaining_load_factor);
    RUN_TEST(test_chaining_probe_count);
    RUN_TEST(test_chaining_statistics);
    RUN_TEST(test_chaining_clear);

    std::cout << std::endl;
    std::cout << "--- 開放定址法 - 線性探測測試 Open Addressing - Linear Probing Tests ---" << std::endl;
    RUN_TEST(test_linear_create_empty);
    RUN_TEST(test_linear_insert_and_search);
    RUN_TEST(test_linear_update_existing);
    RUN_TEST(test_linear_remove_with_tombstone);
    RUN_TEST(test_linear_probe_count);
    RUN_TEST(test_linear_load_factor);
    RUN_TEST(test_linear_table_full);

    std::cout << std::endl;
    std::cout << "--- 開放定址法 - 二次探測測試 Open Addressing - Quadratic Probing Tests ---" << std::endl;
    RUN_TEST(test_quadratic_insert_and_search);
    RUN_TEST(test_quadratic_remove);
    RUN_TEST(test_quadratic_probe_method);

    std::cout << std::endl;
    std::cout << "--- 開放定址法 - 雙重雜湊測試 Open Addressing - Double Hashing Tests ---" << std::endl;
    RUN_TEST(test_double_hash_insert_and_search);
    RUN_TEST(test_double_hash_remove);
    RUN_TEST(test_double_hash_probe_method);
    RUN_TEST(test_double_hash_collisions);

    std::cout << std::endl;
    std::cout << "--- 通用測試 General Tests ---" << std::endl;
    RUN_TEST(test_open_addressing_clear);
    RUN_TEST(test_open_addressing_statistics);
    RUN_TEST(test_int_keys_chaining);
    RUN_TEST(test_int_keys_open_addressing);

    // 結果摘要 - Results summary
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "測試結果 Test Results: " << passed << " 通過 passed, "
              << failed << " 失敗 failed" << std::endl;
    std::cout << "========================================" << std::endl;

    return failed > 0 ? 1 : 0;
}
