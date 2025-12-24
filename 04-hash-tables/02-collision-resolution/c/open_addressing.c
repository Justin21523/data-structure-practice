/**
 * 開放定址法雜湊表（Open Addressing Hash Table）- C 語言實作
 * 支援線性探測、平方探測和雙重雜湊
 *
 * Open Addressing Hash Table with linear probing, quadratic probing, and double hashing
 */

#include "open_addressing.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ========== 私有函式 Private Functions ==========

/**
 * 主雜湊函式（使用 djb2 演算法）
 * Primary hash function using djb2 algorithm
 *
 * @param key 要雜湊的字串
 * @param capacity 容量
 * @return 雜湊值
 */
static size_t hash1(const char* key, size_t capacity) {
    unsigned long hash = 5381;
    int c;

    while ((c = *key++)) {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c;
    }

    return hash % capacity;
}

/**
 * 次雜湊函式（用於雙重雜湊）
 * Secondary hash function for double hashing
 *
 * @param key 要雜湊的字串
 * @param capacity 容量
 * @return 雜湊值（必須非零）
 */
static size_t hash2(const char* key, size_t capacity) {
    unsigned long hash = 0;
    int c;

    while ((c = *key++)) {
        hash = hash * 31 + c;
    }

    // 確保結果非零且為奇數（避免與偶數容量產生問題）
    // Ensure result is non-zero and odd (avoids issues with even capacity)
    size_t result = (hash % (capacity - 1)) + 1;
    return result | 1;  // 確保為奇數 - Ensure odd
}

/**
 * 計算探測位置
 * Calculate probe position
 *
 * @param ht 雜湊表指標
 * @param key 鍵
 * @param i 探測次數（從 0 開始）
 * @return 探測位置
 */
static size_t probe(OpenAddressingHashTable* ht, const char* key, size_t i) {
    size_t h1 = hash1(key, ht->capacity);

    switch (ht->probe_method) {
        case PROBE_LINEAR:
            // 線性探測: h(k, i) = (h1(k) + i) mod m
            // Linear probing: h(k, i) = (h1(k) + i) mod m
            return (h1 + i) % ht->capacity;

        case PROBE_QUADRATIC:
            // 平方探測: h(k, i) = (h1(k) + c1*i + c2*i²) mod m
            // Quadratic probing: h(k, i) = (h1(k) + c1*i + c2*i²) mod m
            // 使用 c1=1, c2=1
            return (h1 + i + i * i) % ht->capacity;

        case PROBE_DOUBLE_HASH:
            // 雙重雜湊: h(k, i) = (h1(k) + i*h2(k)) mod m
            // Double hashing: h(k, i) = (h1(k) + i*h2(k)) mod m
            {
                size_t h2 = hash2(key, ht->capacity);
                return (h1 + i * h2) % ht->capacity;
            }

        default:
            return h1;
    }
}

/**
 * 檢查條目是否為空（未使用）
 * Check if entry is empty (unused)
 */
static bool is_empty(OpenAddressingEntry* entry) {
    return entry->key == NULL && entry->value == NULL;
}

/**
 * 檢查條目是否為墓碑
 * Check if entry is a tombstone
 */
static bool is_tombstone(OpenAddressingEntry* entry) {
    return entry->key == NULL && entry->value == OAHT_TOMBSTONE;
}

/**
 * 檢查條目是否為有效元素
 * Check if entry is a valid element
 */
static bool is_valid(OpenAddressingEntry* entry) {
    return entry->key != NULL;
}

/**
 * 擴容並重新雜湊
 * Resize and rehash
 */
static bool rehash(OpenAddressingHashTable* ht) {
    size_t old_capacity = ht->capacity;
    OpenAddressingEntry* old_entries = ht->entries;

    // 容量加倍 - Double capacity
    size_t new_capacity = old_capacity * 2;
    OpenAddressingEntry* new_entries = calloc(new_capacity, sizeof(OpenAddressingEntry));
    if (!new_entries) return false;

    // 更新雜湊表 - Update hash table
    ht->entries = new_entries;
    ht->capacity = new_capacity;
    size_t old_size = ht->size;
    ht->size = 0;
    ht->tombstones = 0;

    // 重新插入所有有效元素 - Reinsert all valid elements
    for (size_t i = 0; i < old_capacity; i++) {
        OpenAddressingEntry* entry = &old_entries[i];
        if (is_valid(entry)) {
            oaht_insert(ht, entry->key, entry->value);
            free(entry->key);
        }
    }

    free(old_entries);
    return true;
}

// ========== 公開函式 Public Functions ==========

OpenAddressingHashTable* oaht_create(size_t capacity, ProbeMethod method) {
    if (capacity == 0) {
        capacity = OAHT_DEFAULT_CAPACITY;
    }

    OpenAddressingHashTable* ht = malloc(sizeof(OpenAddressingHashTable));
    if (!ht) return NULL;

    ht->entries = calloc(capacity, sizeof(OpenAddressingEntry));
    if (!ht->entries) {
        free(ht);
        return NULL;
    }

    ht->capacity = capacity;
    ht->size = 0;
    ht->tombstones = 0;
    ht->probe_method = method;
    return ht;
}

void oaht_free(OpenAddressingHashTable* ht) {
    if (!ht) return;

    // 釋放所有 key 字串 - Free all key strings
    for (size_t i = 0; i < ht->capacity; i++) {
        if (is_valid(&ht->entries[i])) {
            free(ht->entries[i].key);
        }
    }

    free(ht->entries);
    free(ht);
}

bool oaht_insert(OpenAddressingHashTable* ht, const char* key, void* value) {
    if (!ht || !key || !value || value == OAHT_TOMBSTONE) return false;

    // 檢查是否需要擴容 - Check if rehashing needed
    double effective_load = (double)(ht->size + ht->tombstones) / ht->capacity;
    if (effective_load > OAHT_MAX_LOAD_FACTOR) {
        rehash(ht);
    }

    size_t first_tombstone = SIZE_MAX;

    // 探測找到插入位置 - Probe to find insertion position
    for (size_t i = 0; i < ht->capacity; i++) {
        size_t index = probe(ht, key, i);
        OpenAddressingEntry* entry = &ht->entries[index];

        if (is_empty(entry)) {
            // 找到空位 - Found empty slot
            if (first_tombstone != SIZE_MAX) {
                // 優先使用之前遇到的墓碑位置
                // Use previously encountered tombstone position
                index = first_tombstone;
                entry = &ht->entries[index];
                ht->tombstones--;
            }

            entry->key = strdup(key);
            if (!entry->key) return false;
            entry->value = value;
            ht->size++;
            return true;
        } else if (is_tombstone(entry)) {
            // 記錄第一個墓碑位置 - Record first tombstone position
            if (first_tombstone == SIZE_MAX) {
                first_tombstone = index;
            }
        } else if (strcmp(entry->key, key) == 0) {
            // 鍵已存在，更新值 - Key exists, update value
            entry->value = value;
            return true;
        }
    }

    // 表已滿（理論上不會發生，因為有擴容機制）
    // Table is full (should not happen due to rehashing)
    return false;
}

void* oaht_search(OpenAddressingHashTable* ht, const char* key) {
    if (!ht || !key) return NULL;

    // 探測搜尋 - Probe to search
    for (size_t i = 0; i < ht->capacity; i++) {
        size_t index = probe(ht, key, i);
        OpenAddressingEntry* entry = &ht->entries[index];

        if (is_empty(entry)) {
            // 遇到空位，搜尋失敗 - Found empty slot, search failed
            return NULL;
        } else if (is_valid(entry) && strcmp(entry->key, key) == 0) {
            // 找到 - Found
            return entry->value;
        }
        // 遇到墓碑或其他鍵，繼續探測 - Found tombstone or other key, continue probing
    }

    return NULL;
}

bool oaht_delete(OpenAddressingHashTable* ht, const char* key) {
    if (!ht || !key) return false;

    // 探測搜尋 - Probe to search
    for (size_t i = 0; i < ht->capacity; i++) {
        size_t index = probe(ht, key, i);
        OpenAddressingEntry* entry = &ht->entries[index];

        if (is_empty(entry)) {
            // 遇到空位，刪除失敗 - Found empty slot, deletion failed
            return false;
        } else if (is_valid(entry) && strcmp(entry->key, key) == 0) {
            // 找到，標記為墓碑 - Found, mark as tombstone
            free(entry->key);
            entry->key = NULL;
            entry->value = OAHT_TOMBSTONE;
            ht->size--;
            ht->tombstones++;
            return true;
        }
        // 遇到墓碑或其他鍵，繼續探測 - Found tombstone or other key, continue probing
    }

    return false;
}

bool oaht_contains(OpenAddressingHashTable* ht, const char* key) {
    return oaht_search(ht, key) != NULL;
}

size_t oaht_size(OpenAddressingHashTable* ht) {
    return ht ? ht->size : 0;
}

size_t oaht_capacity(OpenAddressingHashTable* ht) {
    return ht ? ht->capacity : 0;
}

size_t oaht_tombstone_count(OpenAddressingHashTable* ht) {
    return ht ? ht->tombstones : 0;
}

bool oaht_is_empty(OpenAddressingHashTable* ht) {
    return ht ? ht->size == 0 : true;
}

double oaht_load_factor(OpenAddressingHashTable* ht) {
    if (!ht || ht->capacity == 0) return 0.0;
    return (double)(ht->size + ht->tombstones) / ht->capacity;
}

void oaht_clear(OpenAddressingHashTable* ht) {
    if (!ht) return;

    for (size_t i = 0; i < ht->capacity; i++) {
        OpenAddressingEntry* entry = &ht->entries[i];
        if (is_valid(entry)) {
            free(entry->key);
        }
        entry->key = NULL;
        entry->value = NULL;
    }
    ht->size = 0;
    ht->tombstones = 0;
}

const char* oaht_probe_method_name(ProbeMethod method) {
    switch (method) {
        case PROBE_LINEAR:
            return "Linear Probing 線性探測";
        case PROBE_QUADRATIC:
            return "Quadratic Probing 平方探測";
        case PROBE_DOUBLE_HASH:
            return "Double Hashing 雙重雜湊";
        default:
            return "Unknown 未知";
    }
}

void oaht_print(OpenAddressingHashTable* ht) {
    if (!ht) {
        printf("OpenAddressingHashTable: NULL\n");
        return;
    }

    printf("OpenAddressingHashTable (size=%zu, capacity=%zu, load=%.3f, method=%s):\n",
           ht->size, ht->capacity, oaht_load_factor(ht),
           oaht_probe_method_name(ht->probe_method));

    for (size_t i = 0; i < ht->capacity; i++) {
        OpenAddressingEntry* entry = &ht->entries[i];
        if (is_valid(entry)) {
            printf("  [%zu]: (%s, %p)\n", i, entry->key, entry->value);
        } else if (is_tombstone(entry)) {
            printf("  [%zu]: TOMBSTONE\n", i);
        }
    }
}

void oaht_print_stats(OpenAddressingHashTable* ht) {
    if (!ht) {
        printf("OpenAddressingHashTable: NULL\n");
        return;
    }

    size_t empty_slots = 0;
    for (size_t i = 0; i < ht->capacity; i++) {
        if (is_empty(&ht->entries[i])) {
            empty_slots++;
        }
    }

    printf("開放定址法雜湊表統計 Open Addressing Hash Table Statistics:\n");
    printf("  探測方法 Probe Method: %s\n", oaht_probe_method_name(ht->probe_method));
    printf("  元素數量 Size: %zu\n", ht->size);
    printf("  容量 Capacity: %zu\n", ht->capacity);
    printf("  墓碑數量 Tombstones: %zu\n", ht->tombstones);
    printf("  空槽數量 Empty Slots: %zu\n", empty_slots);
    printf("  負載因子 Load Factor: %.3f\n", oaht_load_factor(ht));
    printf("  實際負載 Actual Load: %.3f (不含墓碑 excluding tombstones)\n",
           ht->capacity > 0 ? (double)ht->size / ht->capacity : 0.0);
}
