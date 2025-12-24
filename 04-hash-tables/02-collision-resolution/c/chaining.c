/**
 * 鏈結法雜湊表（Chaining Hash Table）- C 語言實作
 * 使用鏈結串列處理碰撞，包含鏈長統計
 *
 * Chaining Hash Table implementation with chain length statistics
 */

#include "chaining.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ========== 私有函式 Private Functions ==========

/**
 * 計算雜湊值（使用 djb2 演算法）
 * Compute hash value using djb2 algorithm
 *
 * @param key 要雜湊的字串
 * @param capacity 桶的數量
 * @return 桶的索引
 */
static size_t hash(const char* key, size_t capacity) {
    unsigned long hash = 5381;
    int c;

    while ((c = *key++)) {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c;
    }

    return hash % capacity;
}

/**
 * 建立新的節點
 * Create a new entry
 */
static ChainingEntry* create_entry(const char* key, void* value) {
    ChainingEntry* entry = malloc(sizeof(ChainingEntry));
    if (!entry) return NULL;

    // 複製 key 字串 - Copy key string
    entry->key = strdup(key);
    if (!entry->key) {
        free(entry);
        return NULL;
    }

    entry->value = value;
    entry->next = NULL;
    return entry;
}

/**
 * 釋放節點
 * Free an entry
 */
static void free_entry(ChainingEntry* entry) {
    if (entry) {
        free(entry->key);
        // 注意：不釋放 value，由呼叫者負責
        // Note: value is not freed, caller's responsibility
        free(entry);
    }
}

/**
 * 計算單一鏈結串列的長度
 * Calculate length of a single chain
 */
static size_t chain_length(ChainingEntry* entry) {
    size_t length = 0;
    while (entry) {
        length++;
        entry = entry->next;
    }
    return length;
}

/**
 * 擴容並重新雜湊
 * Resize and rehash
 */
static bool rehash(ChainingHashTable* ht) {
    size_t old_capacity = ht->capacity;
    ChainingEntry** old_buckets = ht->buckets;

    // 容量加倍 - Double capacity
    size_t new_capacity = old_capacity * 2;
    ChainingEntry** new_buckets = calloc(new_capacity, sizeof(ChainingEntry*));
    if (!new_buckets) return false;

    // 更新雜湊表 - Update hash table
    ht->buckets = new_buckets;
    ht->capacity = new_capacity;
    ht->size = 0;

    // 重新插入所有元素 - Reinsert all elements
    for (size_t i = 0; i < old_capacity; i++) {
        ChainingEntry* entry = old_buckets[i];
        while (entry) {
            ChainingEntry* next = entry->next;
            cht_insert(ht, entry->key, entry->value);
            free(entry->key);
            free(entry);
            entry = next;
        }
    }

    free(old_buckets);
    return true;
}

// ========== 公開函式 Public Functions ==========

ChainingHashTable* cht_create(size_t capacity) {
    if (capacity == 0) {
        capacity = CHT_DEFAULT_CAPACITY;
    }

    ChainingHashTable* ht = malloc(sizeof(ChainingHashTable));
    if (!ht) return NULL;

    ht->buckets = calloc(capacity, sizeof(ChainingEntry*));
    if (!ht->buckets) {
        free(ht);
        return NULL;
    }

    ht->capacity = capacity;
    ht->size = 0;
    return ht;
}

void cht_free(ChainingHashTable* ht) {
    if (!ht) return;

    // 釋放所有節點 - Free all entries
    for (size_t i = 0; i < ht->capacity; i++) {
        ChainingEntry* entry = ht->buckets[i];
        while (entry) {
            ChainingEntry* next = entry->next;
            free_entry(entry);
            entry = next;
        }
    }

    free(ht->buckets);
    free(ht);
}

bool cht_insert(ChainingHashTable* ht, const char* key, void* value) {
    if (!ht || !key) return false;

    size_t index = hash(key, ht->capacity);
    ChainingEntry* entry = ht->buckets[index];

    // 檢查 key 是否已存在 - Check if key exists
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;  // 更新 - Update existing
            return true;
        }
        entry = entry->next;
    }

    // 建立新節點 - Create new entry
    ChainingEntry* new_entry = create_entry(key, value);
    if (!new_entry) return false;

    // 插入到鏈結串列頭部 - Insert at head
    new_entry->next = ht->buckets[index];
    ht->buckets[index] = new_entry;
    ht->size++;

    // 檢查是否需要擴容 - Check if rehashing needed
    if (cht_load_factor(ht) > CHT_MAX_LOAD_FACTOR) {
        rehash(ht);
    }

    return true;
}

void* cht_search(ChainingHashTable* ht, const char* key) {
    if (!ht || !key) return NULL;

    size_t index = hash(key, ht->capacity);
    ChainingEntry* entry = ht->buckets[index];

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL;
}

bool cht_delete(ChainingHashTable* ht, const char* key) {
    if (!ht || !key) return false;

    size_t index = hash(key, ht->capacity);
    ChainingEntry* entry = ht->buckets[index];
    ChainingEntry* prev = NULL;

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                ht->buckets[index] = entry->next;
            }
            free_entry(entry);
            ht->size--;
            return true;
        }
        prev = entry;
        entry = entry->next;
    }

    return false;
}

bool cht_contains(ChainingHashTable* ht, const char* key) {
    return cht_search(ht, key) != NULL;
}

size_t cht_size(ChainingHashTable* ht) {
    return ht ? ht->size : 0;
}

size_t cht_capacity(ChainingHashTable* ht) {
    return ht ? ht->capacity : 0;
}

bool cht_is_empty(ChainingHashTable* ht) {
    return ht ? ht->size == 0 : true;
}

double cht_load_factor(ChainingHashTable* ht) {
    if (!ht || ht->capacity == 0) return 0.0;
    return (double)ht->size / ht->capacity;
}

void cht_clear(ChainingHashTable* ht) {
    if (!ht) return;

    for (size_t i = 0; i < ht->capacity; i++) {
        ChainingEntry* entry = ht->buckets[i];
        while (entry) {
            ChainingEntry* next = entry->next;
            free_entry(entry);
            entry = next;
        }
        ht->buckets[i] = NULL;
    }
    ht->size = 0;
}

void cht_get_stats(ChainingHashTable* ht, ChainStats* stats) {
    if (!ht || !stats) return;

    stats->min_length = SIZE_MAX;
    stats->max_length = 0;
    stats->avg_length = 0.0;
    stats->empty_buckets = 0;

    size_t total_length = 0;
    size_t non_empty_buckets = 0;

    // 遍歷所有桶 - Iterate through all buckets
    for (size_t i = 0; i < ht->capacity; i++) {
        size_t length = chain_length(ht->buckets[i]);

        if (length == 0) {
            stats->empty_buckets++;
        } else {
            non_empty_buckets++;
            total_length += length;
            if (length < stats->min_length) {
                stats->min_length = length;
            }
            if (length > stats->max_length) {
                stats->max_length = length;
            }
        }
    }

    // 計算平均鏈長 - Calculate average chain length
    if (non_empty_buckets > 0) {
        stats->avg_length = (double)total_length / non_empty_buckets;
    }

    // 如果沒有非空桶，將最小長度設為 0
    // If no non-empty buckets, set min_length to 0
    if (stats->min_length == SIZE_MAX) {
        stats->min_length = 0;
    }
}

void cht_print_stats(ChainingHashTable* ht) {
    if (!ht) {
        printf("ChainingHashTable: NULL\n");
        return;
    }

    ChainStats stats;
    cht_get_stats(ht, &stats);

    printf("鏈結法雜湊表統計 Chaining Hash Table Statistics:\n");
    printf("  元素數量 Size: %zu\n", ht->size);
    printf("  桶數量 Capacity: %zu\n", ht->capacity);
    printf("  負載因子 Load Factor: %.3f\n", cht_load_factor(ht));
    printf("  空桶數量 Empty Buckets: %zu (%.1f%%)\n",
           stats.empty_buckets,
           100.0 * stats.empty_buckets / ht->capacity);
    printf("  最短鏈長 Min Chain: %zu\n", stats.min_length);
    printf("  最長鏈長 Max Chain: %zu\n", stats.max_length);
    printf("  平均鏈長 Avg Chain: %.2f\n", stats.avg_length);
}

void cht_print(ChainingHashTable* ht) {
    if (!ht) {
        printf("ChainingHashTable: NULL\n");
        return;
    }

    printf("ChainingHashTable (size=%zu, capacity=%zu, load=%.3f):\n",
           ht->size, ht->capacity, cht_load_factor(ht));

    for (size_t i = 0; i < ht->capacity; i++) {
        ChainingEntry* entry = ht->buckets[i];
        if (entry) {
            printf("  [%zu]: ", i);
            while (entry) {
                printf("(%s, %p)", entry->key, entry->value);
                if (entry->next) printf(" -> ");
                entry = entry->next;
            }
            printf("\n");
        }
    }
}
