/**
 * 雜湊表單元測試
 * Hash Table Unit Tests
 */

#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// 測試計數器 - Test counters
static int tests_passed = 0;
static int tests_failed = 0;

// 測試巨集 - Test macros
#define TEST(name) static void name(void)
#define RUN_TEST(name) do { \
    printf("測試 Testing: %s... ", #name); \
    name(); \
    printf("通過 PASSED\n"); \
    tests_passed++; \
} while(0)

#define ASSERT(condition) do { \
    if (!(condition)) { \
        printf("失敗 FAILED at line %d: %s\n", __LINE__, #condition); \
        tests_failed++; \
        return; \
    } \
} while(0)

// ========== 基本操作測試 Basic Operations Tests ==========

TEST(test_create_empty_hash_table) {
    HashTable* ht = ht_create(0);
    ASSERT(ht != NULL);
    ASSERT(ht_size(ht) == 0);
    ASSERT(ht_is_empty(ht));
    ASSERT(ht_capacity(ht) == HT_DEFAULT_CAPACITY);
    ht_free(ht);
}

TEST(test_create_with_custom_capacity) {
    HashTable* ht = ht_create(32);
    ASSERT(ht != NULL);
    ASSERT(ht_capacity(ht) == 32);
    ht_free(ht);
}

TEST(test_insert_and_search) {
    HashTable* ht = ht_create(0);
    int v1 = 100, v2 = 200;

    ASSERT(ht_insert(ht, "apple", &v1));
    ASSERT(ht_insert(ht, "banana", &v2));

    ASSERT(ht_search(ht, "apple") == &v1);
    ASSERT(ht_search(ht, "banana") == &v2);
    ASSERT(ht_search(ht, "cherry") == NULL);

    ht_free(ht);
}

TEST(test_insert_update_existing) {
    HashTable* ht = ht_create(0);
    int v1 = 100, v2 = 150;

    ht_insert(ht, "apple", &v1);
    ht_insert(ht, "apple", &v2);

    ASSERT(ht_search(ht, "apple") == &v2);
    ASSERT(ht_size(ht) == 1);

    ht_free(ht);
}

TEST(test_delete) {
    HashTable* ht = ht_create(0);
    int v1 = 100, v2 = 200;

    ht_insert(ht, "apple", &v1);
    ht_insert(ht, "banana", &v2);

    ASSERT(ht_delete(ht, "apple") == true);
    ASSERT(ht_search(ht, "apple") == NULL);
    ASSERT(ht_size(ht) == 1);

    ht_free(ht);
}

TEST(test_delete_nonexistent) {
    HashTable* ht = ht_create(0);
    int v1 = 100;

    ht_insert(ht, "apple", &v1);

    ASSERT(ht_delete(ht, "banana") == false);
    ASSERT(ht_size(ht) == 1);

    ht_free(ht);
}

TEST(test_contains) {
    HashTable* ht = ht_create(0);
    int v1 = 100;

    ht_insert(ht, "apple", &v1);

    ASSERT(ht_contains(ht, "apple") == true);
    ASSERT(ht_contains(ht, "banana") == false);

    ht_free(ht);
}

// ========== 容量測試 Capacity Tests ==========

TEST(test_clear) {
    HashTable* ht = ht_create(0);
    int v1 = 1, v2 = 2;

    ht_insert(ht, "a", &v1);
    ht_insert(ht, "b", &v2);
    ht_clear(ht);

    ASSERT(ht_is_empty(ht));
    ASSERT(ht_size(ht) == 0);
    ASSERT(ht_search(ht, "a") == NULL);

    ht_free(ht);
}

TEST(test_load_factor) {
    HashTable* ht = ht_create(10);
    int v1 = 1, v2 = 2, v3 = 3;

    ht_insert(ht, "a", &v1);
    ht_insert(ht, "b", &v2);
    ht_insert(ht, "c", &v3);

    double lf = ht_load_factor(ht);
    ASSERT(lf > 0.29 && lf < 0.31);  // 約 0.3

    ht_free(ht);
}

// ========== 擴容測試 Rehashing Tests ==========

TEST(test_rehash_on_load_factor) {
    HashTable* ht = ht_create(4);
    int values[4] = {0, 1, 2, 3};

    for (int i = 0; i < 4; i++) {
        char key[10];
        sprintf(key, "key%d", i);
        ht_insert(ht, key, &values[i]);
    }

    // 應該已經擴容 - Should have rehashed
    ASSERT(ht_capacity(ht) > 4);

    // 所有元素應該仍然可以存取 - All elements should still be accessible
    for (int i = 0; i < 4; i++) {
        char key[10];
        sprintf(key, "key%d", i);
        ASSERT(ht_search(ht, key) == &values[i]);
    }

    ht_free(ht);
}

// ========== 碰撞測試 Collision Tests ==========

TEST(test_multiple_items_same_bucket) {
    // 使用容量 1 強制所有元素碰撞
    HashTable* ht = ht_create(1);
    int v1 = 1, v2 = 2, v3 = 3;

    ht_insert(ht, "a", &v1);
    ht_insert(ht, "b", &v2);
    ht_insert(ht, "c", &v3);

    ASSERT(ht_search(ht, "a") == &v1);
    ASSERT(ht_search(ht, "b") == &v2);
    ASSERT(ht_search(ht, "c") == &v3);

    ht_delete(ht, "b");
    ASSERT(ht_search(ht, "b") == NULL);
    ASSERT(ht_search(ht, "a") == &v1);
    ASSERT(ht_search(ht, "c") == &v3);

    ht_free(ht);
}

// ========== NULL 參數測試 NULL Parameter Tests ==========

TEST(test_null_parameters) {
    HashTable* ht = ht_create(0);
    int v1 = 100;

    // 應該安全處理 NULL - Should handle NULL safely
    ASSERT(ht_insert(NULL, "key", &v1) == false);
    ASSERT(ht_insert(ht, NULL, &v1) == false);
    ASSERT(ht_search(NULL, "key") == NULL);
    ASSERT(ht_search(ht, NULL) == NULL);
    ASSERT(ht_delete(NULL, "key") == false);
    ASSERT(ht_delete(ht, NULL) == false);
    ASSERT(ht_contains(NULL, "key") == false);
    ASSERT(ht_size(NULL) == 0);
    ASSERT(ht_capacity(NULL) == 0);
    ASSERT(ht_is_empty(NULL) == true);

    ht_free(ht);
}

// ========== 主函式 Main Function ==========

int main(void) {
    printf("=== 雜湊表單元測試 Hash Table Unit Tests ===\n\n");

    // 基本操作測試 - Basic operations tests
    RUN_TEST(test_create_empty_hash_table);
    RUN_TEST(test_create_with_custom_capacity);
    RUN_TEST(test_insert_and_search);
    RUN_TEST(test_insert_update_existing);
    RUN_TEST(test_delete);
    RUN_TEST(test_delete_nonexistent);
    RUN_TEST(test_contains);

    // 容量測試 - Capacity tests
    RUN_TEST(test_clear);
    RUN_TEST(test_load_factor);

    // 擴容測試 - Rehashing tests
    RUN_TEST(test_rehash_on_load_factor);

    // 碰撞測試 - Collision tests
    RUN_TEST(test_multiple_items_same_bucket);

    // NULL 參數測試 - NULL parameter tests
    RUN_TEST(test_null_parameters);

    // 結果摘要 - Results summary
    printf("\n========================================\n");
    printf("測試結果 Test Results: %d 通過 passed, %d 失敗 failed\n",
           tests_passed, tests_failed);
    printf("========================================\n");

    return tests_failed > 0 ? 1 : 0;
}
