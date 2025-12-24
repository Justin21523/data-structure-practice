/**
 * 碰撞解決方法單元測試
 * Collision Resolution Unit Tests
 */

#include "chaining.h"
#include "open_addressing.h"
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

// ========== 鏈結法測試 Chaining Tests ==========

TEST(test_chaining_create) {
    ChainingHashTable* ht = cht_create(0);
    ASSERT(ht != NULL);
    ASSERT(cht_size(ht) == 0);
    ASSERT(cht_is_empty(ht));
    ASSERT(cht_capacity(ht) == CHT_DEFAULT_CAPACITY);
    cht_free(ht);
}

TEST(test_chaining_insert_and_search) {
    ChainingHashTable* ht = cht_create(0);
    int v1 = 100, v2 = 200, v3 = 300;

    ASSERT(cht_insert(ht, "apple", &v1));
    ASSERT(cht_insert(ht, "banana", &v2));
    ASSERT(cht_insert(ht, "cherry", &v3));

    ASSERT(cht_search(ht, "apple") == &v1);
    ASSERT(cht_search(ht, "banana") == &v2);
    ASSERT(cht_search(ht, "cherry") == &v3);
    ASSERT(cht_search(ht, "grape") == NULL);

    cht_free(ht);
}

TEST(test_chaining_update) {
    ChainingHashTable* ht = cht_create(0);
    int v1 = 100, v2 = 150;

    cht_insert(ht, "apple", &v1);
    cht_insert(ht, "apple", &v2);

    ASSERT(cht_search(ht, "apple") == &v2);
    ASSERT(cht_size(ht) == 1);

    cht_free(ht);
}

TEST(test_chaining_delete) {
    ChainingHashTable* ht = cht_create(0);
    int v1 = 100, v2 = 200;

    cht_insert(ht, "apple", &v1);
    cht_insert(ht, "banana", &v2);

    ASSERT(cht_delete(ht, "apple") == true);
    ASSERT(cht_search(ht, "apple") == NULL);
    ASSERT(cht_size(ht) == 1);
    ASSERT(cht_delete(ht, "grape") == false);

    cht_free(ht);
}

TEST(test_chaining_collision_handling) {
    // 使用容量 1 強制所有元素碰撞 - Use capacity 1 to force collisions
    ChainingHashTable* ht = cht_create(1);
    int v1 = 1, v2 = 2, v3 = 3, v4 = 4;

    cht_insert(ht, "a", &v1);
    cht_insert(ht, "b", &v2);
    cht_insert(ht, "c", &v3);
    cht_insert(ht, "d", &v4);

    ASSERT(cht_size(ht) == 4);
    ASSERT(cht_search(ht, "a") == &v1);
    ASSERT(cht_search(ht, "b") == &v2);
    ASSERT(cht_search(ht, "c") == &v3);
    ASSERT(cht_search(ht, "d") == &v4);

    cht_delete(ht, "b");
    ASSERT(cht_size(ht) == 3);
    ASSERT(cht_search(ht, "b") == NULL);
    ASSERT(cht_search(ht, "a") == &v1);
    ASSERT(cht_search(ht, "c") == &v3);

    cht_free(ht);
}

TEST(test_chaining_statistics) {
    ChainingHashTable* ht = cht_create(8);
    int values[10];
    ChainStats stats;

    // 插入一些元素 - Insert some elements
    for (int i = 0; i < 10; i++) {
        char key[10];
        sprintf(key, "key%d", i);
        values[i] = i * 100;
        cht_insert(ht, key, &values[i]);
    }

    cht_get_stats(ht, &stats);

    ASSERT(stats.max_length >= 1);
    ASSERT(stats.min_length >= 1);
    ASSERT(stats.avg_length > 0);
    ASSERT(stats.empty_buckets < ht->capacity);

    cht_free(ht);
}

TEST(test_chaining_clear) {
    ChainingHashTable* ht = cht_create(0);
    int v1 = 1, v2 = 2;

    cht_insert(ht, "a", &v1);
    cht_insert(ht, "b", &v2);
    cht_clear(ht);

    ASSERT(cht_is_empty(ht));
    ASSERT(cht_size(ht) == 0);
    ASSERT(cht_search(ht, "a") == NULL);

    cht_free(ht);
}

// ========== 開放定址法測試 - 線性探測 Open Addressing Tests - Linear Probing ==========

TEST(test_open_addressing_linear_create) {
    OpenAddressingHashTable* ht = oaht_create(0, PROBE_LINEAR);
    ASSERT(ht != NULL);
    ASSERT(oaht_size(ht) == 0);
    ASSERT(oaht_is_empty(ht));
    ASSERT(oaht_capacity(ht) == OAHT_DEFAULT_CAPACITY);
    ASSERT(ht->probe_method == PROBE_LINEAR);
    oaht_free(ht);
}

TEST(test_open_addressing_linear_insert_and_search) {
    OpenAddressingHashTable* ht = oaht_create(0, PROBE_LINEAR);
    int v1 = 100, v2 = 200, v3 = 300;

    ASSERT(oaht_insert(ht, "apple", &v1));
    ASSERT(oaht_insert(ht, "banana", &v2));
    ASSERT(oaht_insert(ht, "cherry", &v3));

    ASSERT(oaht_search(ht, "apple") == &v1);
    ASSERT(oaht_search(ht, "banana") == &v2);
    ASSERT(oaht_search(ht, "cherry") == &v3);
    ASSERT(oaht_search(ht, "grape") == NULL);

    oaht_free(ht);
}

TEST(test_open_addressing_linear_update) {
    OpenAddressingHashTable* ht = oaht_create(0, PROBE_LINEAR);
    int v1 = 100, v2 = 150;

    oaht_insert(ht, "apple", &v1);
    oaht_insert(ht, "apple", &v2);

    ASSERT(oaht_search(ht, "apple") == &v2);
    ASSERT(oaht_size(ht) == 1);

    oaht_free(ht);
}

TEST(test_open_addressing_linear_delete) {
    OpenAddressingHashTable* ht = oaht_create(0, PROBE_LINEAR);
    int v1 = 100, v2 = 200;

    oaht_insert(ht, "apple", &v1);
    oaht_insert(ht, "banana", &v2);

    ASSERT(oaht_delete(ht, "apple") == true);
    ASSERT(oaht_search(ht, "apple") == NULL);
    ASSERT(oaht_size(ht) == 1);
    ASSERT(oaht_tombstone_count(ht) == 1);
    ASSERT(oaht_delete(ht, "grape") == false);

    oaht_free(ht);
}

TEST(test_open_addressing_linear_tombstone_reuse) {
    OpenAddressingHashTable* ht = oaht_create(16, PROBE_LINEAR);
    int v1 = 100, v2 = 200;

    oaht_insert(ht, "apple", &v1);
    oaht_delete(ht, "apple");

    ASSERT(oaht_tombstone_count(ht) == 1);

    // 重新插入應該重用墓碑位置 - Reinsertion should reuse tombstone
    oaht_insert(ht, "banana", &v2);
    ASSERT(oaht_size(ht) == 1);
    ASSERT(oaht_tombstone_count(ht) == 0);

    oaht_free(ht);
}

TEST(test_open_addressing_linear_collision) {
    OpenAddressingHashTable* ht = oaht_create(8, PROBE_LINEAR);
    int values[5];

    // 插入多個元素，可能產生碰撞 - Insert multiple elements, may cause collisions
    for (int i = 0; i < 5; i++) {
        char key[10];
        sprintf(key, "key%d", i);
        values[i] = i * 100;
        ASSERT(oaht_insert(ht, key, &values[i]));
    }

    ASSERT(oaht_size(ht) == 5);

    // 驗證所有元素都能找到 - Verify all elements can be found
    for (int i = 0; i < 5; i++) {
        char key[10];
        sprintf(key, "key%d", i);
        ASSERT(oaht_search(ht, key) == &values[i]);
    }

    oaht_free(ht);
}

// ========== 開放定址法測試 - 平方探測 Open Addressing Tests - Quadratic Probing ==========

TEST(test_open_addressing_quadratic_basic) {
    OpenAddressingHashTable* ht = oaht_create(0, PROBE_QUADRATIC);
    int v1 = 100, v2 = 200;

    ASSERT(oaht_insert(ht, "apple", &v1));
    ASSERT(oaht_insert(ht, "banana", &v2));

    ASSERT(oaht_search(ht, "apple") == &v1);
    ASSERT(oaht_search(ht, "banana") == &v2);

    ASSERT(oaht_delete(ht, "apple"));
    ASSERT(oaht_search(ht, "apple") == NULL);
    ASSERT(oaht_search(ht, "banana") == &v2);

    oaht_free(ht);
}

TEST(test_open_addressing_quadratic_collision) {
    OpenAddressingHashTable* ht = oaht_create(8, PROBE_QUADRATIC);
    int values[5];

    for (int i = 0; i < 5; i++) {
        char key[10];
        sprintf(key, "test%d", i);
        values[i] = i;
        ASSERT(oaht_insert(ht, key, &values[i]));
    }

    for (int i = 0; i < 5; i++) {
        char key[10];
        sprintf(key, "test%d", i);
        ASSERT(oaht_search(ht, key) == &values[i]);
    }

    oaht_free(ht);
}

// ========== 開放定址法測試 - 雙重雜湊 Open Addressing Tests - Double Hashing ==========

TEST(test_open_addressing_double_hash_basic) {
    OpenAddressingHashTable* ht = oaht_create(0, PROBE_DOUBLE_HASH);
    int v1 = 100, v2 = 200;

    ASSERT(oaht_insert(ht, "apple", &v1));
    ASSERT(oaht_insert(ht, "banana", &v2));

    ASSERT(oaht_search(ht, "apple") == &v1);
    ASSERT(oaht_search(ht, "banana") == &v2);

    ASSERT(oaht_delete(ht, "apple"));
    ASSERT(oaht_search(ht, "apple") == NULL);

    oaht_free(ht);
}

TEST(test_open_addressing_double_hash_collision) {
    OpenAddressingHashTable* ht = oaht_create(16, PROBE_DOUBLE_HASH);
    int values[8];

    for (int i = 0; i < 8; i++) {
        char key[10];
        sprintf(key, "item%d", i);
        values[i] = i * 10;
        ASSERT(oaht_insert(ht, key, &values[i]));
    }

    for (int i = 0; i < 8; i++) {
        char key[10];
        sprintf(key, "item%d", i);
        ASSERT(oaht_search(ht, key) == &values[i]);
    }

    oaht_free(ht);
}

// ========== 通用測試 General Tests ==========

TEST(test_open_addressing_clear) {
    OpenAddressingHashTable* ht = oaht_create(0, PROBE_LINEAR);
    int v1 = 1, v2 = 2;

    oaht_insert(ht, "a", &v1);
    oaht_insert(ht, "b", &v2);
    oaht_clear(ht);

    ASSERT(oaht_is_empty(ht));
    ASSERT(oaht_size(ht) == 0);
    ASSERT(oaht_tombstone_count(ht) == 0);
    ASSERT(oaht_search(ht, "a") == NULL);

    oaht_free(ht);
}

TEST(test_open_addressing_rehash) {
    OpenAddressingHashTable* ht = oaht_create(4, PROBE_LINEAR);
    int values[3];

    // 插入元素觸發擴容 - Insert elements to trigger rehash
    for (int i = 0; i < 3; i++) {
        char key[10];
        sprintf(key, "k%d", i);
        values[i] = i;
        oaht_insert(ht, key, &values[i]);
    }

    // 應該已經擴容 - Should have rehashed
    ASSERT(oaht_capacity(ht) > 4);

    // 所有元素應該仍然可以存取 - All elements should still be accessible
    for (int i = 0; i < 3; i++) {
        char key[10];
        sprintf(key, "k%d", i);
        ASSERT(oaht_search(ht, key) == &values[i]);
    }

    oaht_free(ht);
}

TEST(test_null_parameters) {
    // 測試 NULL 參數的安全處理 - Test safe handling of NULL parameters
    int v1 = 100;

    // Chaining
    ASSERT(cht_insert(NULL, "key", &v1) == false);
    ASSERT(cht_insert(cht_create(0), NULL, &v1) == false);
    ASSERT(cht_search(NULL, "key") == NULL);
    ASSERT(cht_delete(NULL, "key") == false);

    // Open Addressing
    ASSERT(oaht_insert(NULL, "key", &v1) == false);
    ASSERT(oaht_insert(oaht_create(0, PROBE_LINEAR), NULL, &v1) == false);
    ASSERT(oaht_insert(oaht_create(0, PROBE_LINEAR), "key", NULL) == false);
    ASSERT(oaht_search(NULL, "key") == NULL);
    ASSERT(oaht_delete(NULL, "key") == false);
}

// ========== 主函式 Main Function ==========

int main(void) {
    printf("=== 碰撞解決方法單元測試 Collision Resolution Unit Tests ===\n\n");

    printf("--- 鏈結法測試 Chaining Tests ---\n");
    RUN_TEST(test_chaining_create);
    RUN_TEST(test_chaining_insert_and_search);
    RUN_TEST(test_chaining_update);
    RUN_TEST(test_chaining_delete);
    RUN_TEST(test_chaining_collision_handling);
    RUN_TEST(test_chaining_statistics);
    RUN_TEST(test_chaining_clear);

    printf("\n--- 開放定址法測試 - 線性探測 Open Addressing Tests - Linear Probing ---\n");
    RUN_TEST(test_open_addressing_linear_create);
    RUN_TEST(test_open_addressing_linear_insert_and_search);
    RUN_TEST(test_open_addressing_linear_update);
    RUN_TEST(test_open_addressing_linear_delete);
    RUN_TEST(test_open_addressing_linear_tombstone_reuse);
    RUN_TEST(test_open_addressing_linear_collision);

    printf("\n--- 開放定址法測試 - 平方探測 Open Addressing Tests - Quadratic Probing ---\n");
    RUN_TEST(test_open_addressing_quadratic_basic);
    RUN_TEST(test_open_addressing_quadratic_collision);

    printf("\n--- 開放定址法測試 - 雙重雜湊 Open Addressing Tests - Double Hashing ---\n");
    RUN_TEST(test_open_addressing_double_hash_basic);
    RUN_TEST(test_open_addressing_double_hash_collision);

    printf("\n--- 通用測試 General Tests ---\n");
    RUN_TEST(test_open_addressing_clear);
    RUN_TEST(test_open_addressing_rehash);
    RUN_TEST(test_null_parameters);

    // 結果摘要 - Results summary
    printf("\n========================================\n");
    printf("測試結果 Test Results: %d 通過 passed, %d 失敗 failed\n",
           tests_passed, tests_failed);
    printf("========================================\n");

    return tests_failed > 0 ? 1 : 0;
}
