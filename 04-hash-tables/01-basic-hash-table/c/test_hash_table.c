/** Doc block start
 * 雜湊表單元測試 / Hash Table Unit Tests
 */  // End of block comment

#include "hash_table.h"  // Execute this statement as part of the data structure implementation.
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

// ========== 基本操作測試 Basic Operations Tests ==========

TEST(test_create_empty_hash_table) {  // Compute a hash-based index so keys map into the table's storage.
    HashTable* ht = ht_create(0);  // Assign or update a variable that represents the current algorithm state.
    ASSERT(ht != NULL);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_size(ht) == 0);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_is_empty(ht));  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_capacity(ht) == HT_DEFAULT_CAPACITY);  // Execute this statement as part of the data structure implementation.
    ht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_create_with_custom_capacity) {  // Execute this statement as part of the data structure implementation.
    HashTable* ht = ht_create(32);  // Assign or update a variable that represents the current algorithm state.
    ASSERT(ht != NULL);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_capacity(ht) == 32);  // Execute this statement as part of the data structure implementation.
    ht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_insert_and_search) {  // Execute this statement as part of the data structure implementation.
    HashTable* ht = ht_create(0);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100, v2 = 200;  // Assign or update a variable that represents the current algorithm state.

    ASSERT(ht_insert(ht, "apple", &v1));  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_insert(ht, "banana", &v2));  // Execute this statement as part of the data structure implementation.

    ASSERT(ht_search(ht, "apple") == &v1);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_search(ht, "banana") == &v2);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_search(ht, "cherry") == NULL);  // Execute this statement as part of the data structure implementation.

    ht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_insert_update_existing) {  // Execute this statement as part of the data structure implementation.
    HashTable* ht = ht_create(0);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100, v2 = 150;  // Assign or update a variable that represents the current algorithm state.

    ht_insert(ht, "apple", &v1);  // Execute this statement as part of the data structure implementation.
    ht_insert(ht, "apple", &v2);  // Execute this statement as part of the data structure implementation.

    ASSERT(ht_search(ht, "apple") == &v2);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_size(ht) == 1);  // Execute this statement as part of the data structure implementation.

    ht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_delete) {  // Execute this statement as part of the data structure implementation.
    HashTable* ht = ht_create(0);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100, v2 = 200;  // Assign or update a variable that represents the current algorithm state.

    ht_insert(ht, "apple", &v1);  // Execute this statement as part of the data structure implementation.
    ht_insert(ht, "banana", &v2);  // Execute this statement as part of the data structure implementation.

    ASSERT(ht_delete(ht, "apple") == true);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_search(ht, "apple") == NULL);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_size(ht) == 1);  // Execute this statement as part of the data structure implementation.

    ht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_delete_nonexistent) {  // Execute this statement as part of the data structure implementation.
    HashTable* ht = ht_create(0);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100;  // Assign or update a variable that represents the current algorithm state.

    ht_insert(ht, "apple", &v1);  // Execute this statement as part of the data structure implementation.

    ASSERT(ht_delete(ht, "banana") == false);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_size(ht) == 1);  // Execute this statement as part of the data structure implementation.

    ht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_contains) {  // Execute this statement as part of the data structure implementation.
    HashTable* ht = ht_create(0);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100;  // Assign or update a variable that represents the current algorithm state.

    ht_insert(ht, "apple", &v1);  // Execute this statement as part of the data structure implementation.

    ASSERT(ht_contains(ht, "apple") == true);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_contains(ht, "banana") == false);  // Execute this statement as part of the data structure implementation.

    ht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 容量測試 Capacity Tests ==========

TEST(test_clear) {  // Execute this statement as part of the data structure implementation.
    HashTable* ht = ht_create(0);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 1, v2 = 2;  // Assign or update a variable that represents the current algorithm state.

    ht_insert(ht, "a", &v1);  // Execute this statement as part of the data structure implementation.
    ht_insert(ht, "b", &v2);  // Execute this statement as part of the data structure implementation.
    ht_clear(ht);  // Execute this statement as part of the data structure implementation.

    ASSERT(ht_is_empty(ht));  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_size(ht) == 0);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_search(ht, "a") == NULL);  // Execute this statement as part of the data structure implementation.

    ht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

TEST(test_load_factor) {  // Execute this statement as part of the data structure implementation.
    HashTable* ht = ht_create(10);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 1, v2 = 2, v3 = 3;  // Assign or update a variable that represents the current algorithm state.

    ht_insert(ht, "a", &v1);  // Execute this statement as part of the data structure implementation.
    ht_insert(ht, "b", &v2);  // Execute this statement as part of the data structure implementation.
    ht_insert(ht, "c", &v3);  // Execute this statement as part of the data structure implementation.

    double lf = ht_load_factor(ht);  // Assign or update a variable that represents the current algorithm state.
    ASSERT(lf > 0.29 && lf < 0.31);  // 約 0.3

    ht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 擴容測試 Rehashing Tests ==========

TEST(test_rehash_on_load_factor) {  // Rehash entries into a larger table to keep operations near O(1) on average.
    HashTable* ht = ht_create(4);  // Assign or update a variable that represents the current algorithm state.
    int values[4] = {0, 1, 2, 3};  // Assign or update a variable that represents the current algorithm state.

    for (int i = 0; i < 4; i++) {  // Iterate over a range/collection to process each item in sequence.
        char key[10];  // Execute this statement as part of the data structure implementation.
        sprintf(key, "key%d", i);  // Execute this statement as part of the data structure implementation.
        ht_insert(ht, key, &values[i]);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    // 應該已經擴容 - Should have rehashed
    ASSERT(ht_capacity(ht) > 4);  // Execute this statement as part of the data structure implementation.

    // 所有元素應該仍然可以存取 - All elements should still be accessible
    for (int i = 0; i < 4; i++) {  // Iterate over a range/collection to process each item in sequence.
        char key[10];  // Execute this statement as part of the data structure implementation.
        sprintf(key, "key%d", i);  // Execute this statement as part of the data structure implementation.
        ASSERT(ht_search(ht, key) == &values[i]);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    ht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 碰撞測試 Collision Tests ==========

TEST(test_multiple_items_same_bucket) {  // Access or update the bucket storage used to hold entries or chains.
    // 使用容量 1 強制所有元素碰撞 / Use capacity 1 強制所有 elements collision
    HashTable* ht = ht_create(1);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 1, v2 = 2, v3 = 3;  // Assign or update a variable that represents the current algorithm state.

    ht_insert(ht, "a", &v1);  // Execute this statement as part of the data structure implementation.
    ht_insert(ht, "b", &v2);  // Execute this statement as part of the data structure implementation.
    ht_insert(ht, "c", &v3);  // Execute this statement as part of the data structure implementation.

    ASSERT(ht_search(ht, "a") == &v1);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_search(ht, "b") == &v2);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_search(ht, "c") == &v3);  // Execute this statement as part of the data structure implementation.

    ht_delete(ht, "b");  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_search(ht, "b") == NULL);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_search(ht, "a") == &v1);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_search(ht, "c") == &v3);  // Execute this statement as part of the data structure implementation.

    ht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== NULL 參數測試 NULL Parameter Tests ==========

TEST(test_null_parameters) {  // Execute this statement as part of the data structure implementation.
    HashTable* ht = ht_create(0);  // Assign or update a variable that represents the current algorithm state.
    int v1 = 100;  // Assign or update a variable that represents the current algorithm state.

    // 應該安全處理 NULL - Should handle NULL safely
    ASSERT(ht_insert(NULL, "key", &v1) == false);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_insert(ht, NULL, &v1) == false);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_search(NULL, "key") == NULL);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_search(ht, NULL) == NULL);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_delete(NULL, "key") == false);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_delete(ht, NULL) == false);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_contains(NULL, "key") == false);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_size(NULL) == 0);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_capacity(NULL) == 0);  // Execute this statement as part of the data structure implementation.
    ASSERT(ht_is_empty(NULL) == true);  // Execute this statement as part of the data structure implementation.

    ht_free(ht);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 主函式 Main Function ==========

int main(void) {  // Execute this statement as part of the data structure implementation.
    printf("=== 雜湊表單元測試 Hash Table Unit Tests ===\n\n");  // Execute this statement as part of the data structure implementation.

    // 基本操作測試 - Basic operations tests
    RUN_TEST(test_create_empty_hash_table);  // Compute a hash-based index so keys map into the table's storage.
    RUN_TEST(test_create_with_custom_capacity);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_insert_and_search);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_insert_update_existing);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_delete);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_delete_nonexistent);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_contains);  // Execute this statement as part of the data structure implementation.

    // 容量測試 - Capacity tests
    RUN_TEST(test_clear);  // Execute this statement as part of the data structure implementation.
    RUN_TEST(test_load_factor);  // Execute this statement as part of the data structure implementation.

    // 擴容測試 - Rehashing tests
    RUN_TEST(test_rehash_on_load_factor);  // Rehash entries into a larger table to keep operations near O(1) on average.

    // 碰撞測試 - Collision tests
    RUN_TEST(test_multiple_items_same_bucket);  // Access or update the bucket storage used to hold entries or chains.

    // NULL 參數測試 - NULL parameter tests
    RUN_TEST(test_null_parameters);  // Execute this statement as part of the data structure implementation.

    // 結果摘要 - Results summary
    printf("\n========================================\n");  // Execute this statement as part of the data structure implementation.
    printf("測試結果 Test Results: %d 通過 passed, %d 失敗 failed\n",  // Execute this statement as part of the data structure implementation.
           tests_passed, tests_failed);  // Execute this statement as part of the data structure implementation.
    printf("========================================\n");  // Execute this statement as part of the data structure implementation.

    return tests_failed > 0 ? 1 : 0;  // Return the computed result to the caller.
}  // Close the current block scope.
