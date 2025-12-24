/** Doc block start
 * 碰撞解決方法單元測試 / Collision Resolution Unit Tests
 */  // End of block comment

#include "chaining.h"  // Execute this statement as part of the data structure implementation.
#include "open_addressing.h"  // Execute this statement as part of the data structure implementation.
#include <stdio.h>  // Execute this statement as part of the data structure implementation.
#include <stdlib.h>  // Execute this statement as part of the data structure implementation.
#include <string.h>  // Execute this statement as part of the data structure implementation.
#include <assert.h>  // Execute this statement as part of the data structure implementation.

// 測試計數器 - Test counters
static int tests_passed = 0;  // Assign or update a variable that represents the current algorithm state.
static int tests_failed = 0;  // Assign or update a variable that represents the current algorithm state.

// 測試巨集 - Test macros
#define TEST(name) static void name(void)  // Execute this statement as part of the data structure implementation.
#define RUN_TEST(name) do { \
    printf("測試 Testing: %s... ", #name); \
    name(); \
    printf("通過 PASSED\n"); \
    tests_passed++; \
} while(0)  // Execute this statement as part of the data structure implementation.

#define ASSERT(condition) do { \
    if (!(condition)) { \
        printf("失敗 FAILED at line %d: %s\n", __LINE__, #condition); \
        tests_failed++; \
        return; \
    } \
} while(0)  // Execute this statement as part of the data structure implementation.

// ========== 鏈結法測試 Chaining Tests ==========

TEST(test_chaining_create) {  // Execute this statement as part of the data structure implementation.
    ChainingHashTable* ht = cht_create(0);  // Assign or update a variable that represents the current algorithm state.
    ASSERT(ht != NULL);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_size(ht) == 0);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_is_empty(ht));  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_capacity(ht) == CHT_DEFAULT_CAPACITY);  // Execute this statement as part of the data structure implementation.
    cht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_chaining_insert_and_search) {  // Execute this statement as part of the data structure implementation.
    ChainingHashTable* ht = cht_create(0);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100, v2 = 200, v3 = 300;  // Assign or update a variable that represents the current algorithm state.

    ASSERT(cht_insert(ht, "apple", &v1));  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_insert(ht, "banana", &v2));  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_insert(ht, "cherry", &v3));  // Execute this statement as part of the data structure implementation.

    ASSERT(cht_search(ht, "apple") == &v1);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_search(ht, "banana") == &v2);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_search(ht, "cherry") == &v3);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_search(ht, "grape") == NULL);  // Execute this statement as part of the data structure implementation.

    cht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_chaining_update) {  // Execute this statement as part of the data structure implementation.
    ChainingHashTable* ht = cht_create(0);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100, v2 = 150;  // Assign or update a variable that represents the current algorithm state.

    cht_insert(ht, "apple", &v1);  // Execute this statement as part of the data structure implementation.
    cht_insert(ht, "apple", &v2);  // Execute this statement as part of the data structure implementation.

    ASSERT(cht_search(ht, "apple") == &v2);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_size(ht) == 1);  // Execute this statement as part of the data structure implementation.

    cht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_chaining_delete) {  // Execute this statement as part of the data structure implementation.
    ChainingHashTable* ht = cht_create(0);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100, v2 = 200;  // Assign or update a variable that represents the current algorithm state.

    cht_insert(ht, "apple", &v1);  // Execute this statement as part of the data structure implementation.
    cht_insert(ht, "banana", &v2);  // Execute this statement as part of the data structure implementation.

    ASSERT(cht_delete(ht, "apple") == true);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_search(ht, "apple") == NULL);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_size(ht) == 1);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_delete(ht, "grape") == false);  // Execute this statement as part of the data structure implementation.

    cht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_chaining_collision_handling) {  // Execute this statement as part of the data structure implementation.
    // 使用容量 1 強制所有元素碰撞 - Use capacity 1 to force collisions
    ChainingHashTable* ht = cht_create(1);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 1, v2 = 2, v3 = 3, v4 = 4;  // Assign or update a variable that represents the current algorithm state.

    cht_insert(ht, "a", &v1);  // Execute this statement as part of the data structure implementation.
    cht_insert(ht, "b", &v2);  // Execute this statement as part of the data structure implementation.
    cht_insert(ht, "c", &v3);  // Execute this statement as part of the data structure implementation.
    cht_insert(ht, "d", &v4);  // Execute this statement as part of the data structure implementation.

    ASSERT(cht_size(ht) == 4);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_search(ht, "a") == &v1);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_search(ht, "b") == &v2);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_search(ht, "c") == &v3);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_search(ht, "d") == &v4);  // Execute this statement as part of the data structure implementation.

    cht_delete(ht, "b");  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_size(ht) == 3);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_search(ht, "b") == NULL);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_search(ht, "a") == &v1);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_search(ht, "c") == &v3);  // Execute this statement as part of the data structure implementation.

    cht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_chaining_statistics) {  // Execute this statement as part of the data structure implementation.
    ChainingHashTable* ht = cht_create(8);  // Assign or update a variable that represents the current algorithm state.
    int values[10];  // Execute this statement as part of the data structure implementation.
    ChainStats stats;  // Execute this statement as part of the data structure implementation.

    // 插入一些元素 - Insert some elements
    for (int i = 0; i < 10; i++) {  // Iterate over a range/collection to process each item in sequence.
        char key[10];  // Execute this statement as part of the data structure implementation.
        sprintf(key, "key%d", i);  // Execute this statement as part of the data structure implementation.
        values[i] = i * 100;  // Assign or update a variable that represents the current algorithm state.
        cht_insert(ht, key, &values[i]);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    cht_get_stats(ht, &stats);  // Execute this statement as part of the data structure implementation.

    ASSERT(stats.max_length >= 1);  // Execute this statement as part of the data structure implementation.
    ASSERT(stats.min_length >= 1);  // Execute this statement as part of the data structure implementation.
    ASSERT(stats.avg_length > 0);  // Execute this statement as part of the data structure implementation.
    ASSERT(stats.empty_buckets < ht->capacity);  // Access or update the bucket storage used to hold entries or chains.

    cht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_chaining_clear) {  // Execute this statement as part of the data structure implementation.
    ChainingHashTable* ht = cht_create(0);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 1, v2 = 2;  // Assign or update a variable that represents the current algorithm state.

    cht_insert(ht, "a", &v1);  // Execute this statement as part of the data structure implementation.
    cht_insert(ht, "b", &v2);  // Execute this statement as part of the data structure implementation.
    cht_clear(ht);  // Execute this statement as part of the data structure implementation.

    ASSERT(cht_is_empty(ht));  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_size(ht) == 0);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_search(ht, "a") == NULL);  // Execute this statement as part of the data structure implementation.

    cht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 開放定址法測試 - 線性探測 Open Addressing Tests - Linear Probing ==========

TEST(test_open_addressing_linear_create) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable* ht = oaht_create(0, PROBE_LINEAR);  // Assign or update a variable that represents the current algorithm state.
    ASSERT(ht != NULL);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_size(ht) == 0);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_is_empty(ht));  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_capacity(ht) == OAHT_DEFAULT_CAPACITY);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht->probe_method == PROBE_LINEAR);  // Advance or track the probing sequence used by open addressing.
    oaht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_open_addressing_linear_insert_and_search) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable* ht = oaht_create(0, PROBE_LINEAR);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100, v2 = 200, v3 = 300;  // Assign or update a variable that represents the current algorithm state.

    ASSERT(oaht_insert(ht, "apple", &v1));  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_insert(ht, "banana", &v2));  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_insert(ht, "cherry", &v3));  // Execute this statement as part of the data structure implementation.

    ASSERT(oaht_search(ht, "apple") == &v1);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_search(ht, "banana") == &v2);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_search(ht, "cherry") == &v3);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_search(ht, "grape") == NULL);  // Execute this statement as part of the data structure implementation.

    oaht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_open_addressing_linear_update) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable* ht = oaht_create(0, PROBE_LINEAR);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100, v2 = 150;  // Assign or update a variable that represents the current algorithm state.

    oaht_insert(ht, "apple", &v1);  // Execute this statement as part of the data structure implementation.
    oaht_insert(ht, "apple", &v2);  // Execute this statement as part of the data structure implementation.

    ASSERT(oaht_search(ht, "apple") == &v2);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_size(ht) == 1);  // Execute this statement as part of the data structure implementation.

    oaht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_open_addressing_linear_delete) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable* ht = oaht_create(0, PROBE_LINEAR);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100, v2 = 200;  // Assign or update a variable that represents the current algorithm state.

    oaht_insert(ht, "apple", &v1);  // Execute this statement as part of the data structure implementation.
    oaht_insert(ht, "banana", &v2);  // Execute this statement as part of the data structure implementation.

    ASSERT(oaht_delete(ht, "apple") == true);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_search(ht, "apple") == NULL);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_size(ht) == 1);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_tombstone_count(ht) == 1);  // Handle tombstones so deletions do not break the probing/search sequence.
    ASSERT(oaht_delete(ht, "grape") == false);  // Execute this statement as part of the data structure implementation.

    oaht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_open_addressing_linear_tombstone_reuse) {  // Handle tombstones so deletions do not break the probing/search sequence.
    OpenAddressingHashTable* ht = oaht_create(16, PROBE_LINEAR);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100, v2 = 200;  // Assign or update a variable that represents the current algorithm state.

    oaht_insert(ht, "apple", &v1);  // Execute this statement as part of the data structure implementation.
    oaht_delete(ht, "apple");  // Execute this statement as part of the data structure implementation.

    ASSERT(oaht_tombstone_count(ht) == 1);  // Handle tombstones so deletions do not break the probing/search sequence.

    // 重新插入應該重用墓碑位置 - Reinsertion should reuse tombstone
    oaht_insert(ht, "banana", &v2);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_size(ht) == 1);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_tombstone_count(ht) == 0);  // Handle tombstones so deletions do not break the probing/search sequence.

    oaht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_open_addressing_linear_collision) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable* ht = oaht_create(8, PROBE_LINEAR);  // Assign or update a variable that represents the current algorithm state.
    int values[5];  // Execute this statement as part of the data structure implementation.

    // 插入多個元素，可能產生碰撞 - Insert multiple elements, may cause collisions
    for (int i = 0; i < 5; i++) {  // Iterate over a range/collection to process each item in sequence.
        char key[10];  // Execute this statement as part of the data structure implementation.
        sprintf(key, "key%d", i);  // Execute this statement as part of the data structure implementation.
        values[i] = i * 100;  // Assign or update a variable that represents the current algorithm state.
        ASSERT(oaht_insert(ht, key, &values[i]));  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    ASSERT(oaht_size(ht) == 5);  // Execute this statement as part of the data structure implementation.

    // 驗證所有元素都能找到 - Verify all elements can be found
    for (int i = 0; i < 5; i++) {  // Iterate over a range/collection to process each item in sequence.
        char key[10];  // Execute this statement as part of the data structure implementation.
        sprintf(key, "key%d", i);  // Execute this statement as part of the data structure implementation.
        ASSERT(oaht_search(ht, key) == &values[i]);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    oaht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 開放定址法測試 - 平方探測 Open Addressing Tests - Quadratic Probing ==========

TEST(test_open_addressing_quadratic_basic) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable* ht = oaht_create(0, PROBE_QUADRATIC);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100, v2 = 200;  // Assign or update a variable that represents the current algorithm state.

    ASSERT(oaht_insert(ht, "apple", &v1));  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_insert(ht, "banana", &v2));  // Execute this statement as part of the data structure implementation.

    ASSERT(oaht_search(ht, "apple") == &v1);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_search(ht, "banana") == &v2);  // Execute this statement as part of the data structure implementation.

    ASSERT(oaht_delete(ht, "apple"));  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_search(ht, "apple") == NULL);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_search(ht, "banana") == &v2);  // Execute this statement as part of the data structure implementation.

    oaht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_open_addressing_quadratic_collision) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable* ht = oaht_create(8, PROBE_QUADRATIC);  // Assign or update a variable that represents the current algorithm state.
    int values[5];  // Execute this statement as part of the data structure implementation.

    for (int i = 0; i < 5; i++) {  // Iterate over a range/collection to process each item in sequence.
        char key[10];  // Execute this statement as part of the data structure implementation.
        sprintf(key, "test%d", i);  // Execute this statement as part of the data structure implementation.
        values[i] = i;  // Assign or update a variable that represents the current algorithm state.
        ASSERT(oaht_insert(ht, key, &values[i]));  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    for (int i = 0; i < 5; i++) {  // Iterate over a range/collection to process each item in sequence.
        char key[10];  // Execute this statement as part of the data structure implementation.
        sprintf(key, "test%d", i);  // Execute this statement as part of the data structure implementation.
        ASSERT(oaht_search(ht, key) == &values[i]);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    oaht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 開放定址法測試 - 雙重雜湊 Open Addressing Tests - Double Hashing ==========

TEST(test_open_addressing_double_hash_basic) {  // Compute a hash-based index so keys map into the table's storage.
    OpenAddressingHashTable* ht = oaht_create(0, PROBE_DOUBLE_HASH);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100, v2 = 200;  // Assign or update a variable that represents the current algorithm state.

    ASSERT(oaht_insert(ht, "apple", &v1));  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_insert(ht, "banana", &v2));  // Execute this statement as part of the data structure implementation.

    ASSERT(oaht_search(ht, "apple") == &v1);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_search(ht, "banana") == &v2);  // Execute this statement as part of the data structure implementation.

    ASSERT(oaht_delete(ht, "apple"));  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_search(ht, "apple") == NULL);  // Execute this statement as part of the data structure implementation.

    oaht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_open_addressing_double_hash_collision) {  // Compute a hash-based index so keys map into the table's storage.
    OpenAddressingHashTable* ht = oaht_create(16, PROBE_DOUBLE_HASH);  // Assign or update a variable that represents the current algorithm state.
    int values[8];  // Execute this statement as part of the data structure implementation.

    for (int i = 0; i < 8; i++) {  // Iterate over a range/collection to process each item in sequence.
        char key[10];  // Execute this statement as part of the data structure implementation.
        sprintf(key, "item%d", i);  // Execute this statement as part of the data structure implementation.
        values[i] = i * 10;  // Assign or update a variable that represents the current algorithm state.
        ASSERT(oaht_insert(ht, key, &values[i]));  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    for (int i = 0; i < 8; i++) {  // Iterate over a range/collection to process each item in sequence.
        char key[10];  // Execute this statement as part of the data structure implementation.
        sprintf(key, "item%d", i);  // Execute this statement as part of the data structure implementation.
        ASSERT(oaht_search(ht, key) == &values[i]);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    oaht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 通用測試 General Tests ==========

TEST(test_open_addressing_clear) {  // Execute this statement as part of the data structure implementation.
    OpenAddressingHashTable* ht = oaht_create(0, PROBE_LINEAR);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 1, v2 = 2;  // Assign or update a variable that represents the current algorithm state.

    oaht_insert(ht, "a", &v1);  // Execute this statement as part of the data structure implementation.
    oaht_insert(ht, "b", &v2);  // Execute this statement as part of the data structure implementation.
    oaht_clear(ht);  // Execute this statement as part of the data structure implementation.

    ASSERT(oaht_is_empty(ht));  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_size(ht) == 0);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_tombstone_count(ht) == 0);  // Handle tombstones so deletions do not break the probing/search sequence.
    ASSERT(oaht_search(ht, "a") == NULL);  // Execute this statement as part of the data structure implementation.

    oaht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_open_addressing_rehash) {  // Rehash entries into a larger table to keep operations near O(1) on average.
    OpenAddressingHashTable* ht = oaht_create(4, PROBE_LINEAR);  // Assign or update a variable that represents the current algorithm state.
    int values[3];  // Execute this statement as part of the data structure implementation.

    // 插入元素觸發擴容 - Insert elements to trigger rehash
    for (int i = 0; i < 3; i++) {  // Iterate over a range/collection to process each item in sequence.
        char key[10];  // Execute this statement as part of the data structure implementation.
        sprintf(key, "k%d", i);  // Execute this statement as part of the data structure implementation.
        values[i] = i;  // Assign or update a variable that represents the current algorithm state.
        oaht_insert(ht, key, &values[i]);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    // 應該已經擴容 - Should have rehashed
    ASSERT(oaht_capacity(ht) > 4);  // Execute this statement as part of the data structure implementation.

    // 所有元素應該仍然可以存取 - All elements should still be accessible
    for (int i = 0; i < 3; i++) {  // Iterate over a range/collection to process each item in sequence.
        char key[10];  // Execute this statement as part of the data structure implementation.
        sprintf(key, "k%d", i);  // Execute this statement as part of the data structure implementation.
        ASSERT(oaht_search(ht, key) == &values[i]);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    oaht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_null_parameters) {  // Execute this statement as part of the data structure implementation.
    // 測試 NULL 參數的安全處理 - Test safe handling of NULL parameters
    int v1 = 100;  // Assign or update a variable that represents the current algorithm state.

    // Chaining
    ASSERT(cht_insert(NULL, "key", &v1) == false);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_insert(cht_create(0), NULL, &v1) == false);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_search(NULL, "key") == NULL);  // Execute this statement as part of the data structure implementation.
    ASSERT(cht_delete(NULL, "key") == false);  // Execute this statement as part of the data structure implementation.

    // Open Addressing
    ASSERT(oaht_insert(NULL, "key", &v1) == false);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_insert(oaht_create(0, PROBE_LINEAR), NULL, &v1) == false);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_insert(oaht_create(0, PROBE_LINEAR), "key", NULL) == false);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_search(NULL, "key") == NULL);  // Execute this statement as part of the data structure implementation.
    ASSERT(oaht_delete(NULL, "key") == false);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 主函式 Main Function ==========

int main(void) {  // Execute this statement as part of the data structure implementation.
    printf("=== 碰撞解決方法單元測試 Collision Resolution Unit Tests ===\n\n");  // Execute this statement as part of the data structure implementation.

    printf("--- 鏈結法測試 Chaining Tests ---\n");  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_chaining_create);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_chaining_insert_and_search);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_chaining_update);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_chaining_delete);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_chaining_collision_handling);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_chaining_statistics);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_chaining_clear);  // Execute this statement as part of the data structure implementation.

    printf("\n--- 開放定址法測試 - 線性探測 Open Addressing Tests - Linear Probing ---\n");  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_open_addressing_linear_create);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_open_addressing_linear_insert_and_search);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_open_addressing_linear_update);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_open_addressing_linear_delete);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_open_addressing_linear_tombstone_reuse);  // Handle tombstones so deletions do not break the probing/search sequence.
    RUN_TEST(test_open_addressing_linear_collision);  // Execute this statement as part of the data structure implementation.

    printf("\n--- 開放定址法測試 - 平方探測 Open Addressing Tests - Quadratic Probing ---\n");  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_open_addressing_quadratic_basic);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_open_addressing_quadratic_collision);  // Execute this statement as part of the data structure implementation.

    printf("\n--- 開放定址法測試 - 雙重雜湊 Open Addressing Tests - Double Hashing ---\n");  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_open_addressing_double_hash_basic);  // Compute a hash-based index so keys map into the table's storage.
    RUN_TEST(test_open_addressing_double_hash_collision);  // Compute a hash-based index so keys map into the table's storage.

    printf("\n--- 通用測試 General Tests ---\n");  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_open_addressing_clear);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_open_addressing_rehash);  // Rehash entries into a larger table to keep operations near O(1) on average.
    RUN_TEST(test_null_parameters);  // Execute this statement as part of the data structure implementation.

    // 結果摘要 - Results summary
    printf("\n========================================\n");  // Execute this statement as part of the data structure implementation.
    printf("測試結果 Test Results: %d 通過 passed, %d 失敗 failed\n",  // Execute this statement as part of the data structure implementation.
           tests_passed, tests_failed);  // Execute this statement as part of the data structure implementation.
    printf("========================================\n");  // Execute this statement as part of the data structure implementation.

    return tests_failed > 0 ? 1 : 0;  // Return the computed result to the caller.
}  // Close the current block scope.
