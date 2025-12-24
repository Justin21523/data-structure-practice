/** Doc block start
 * 開放定址法雜湊表（Open Addressing Hash Table）- C 語言實作
 * 支援線性探測、平方探測和雙重雜湊 / 支援 linear probing 、平方 probe 和 double hashing
 *(blank line)
 * Open Addressing Hash Table with linear probing, quadratic probing, and double hashing
 */  // End of block comment

#include "open_addressing.h"  // Execute this statement as part of the data structure implementation.
#include <stdlib.h>  // Execute this statement as part of the data structure implementation.
#include <string.h>  // Execute this statement as part of the data structure implementation.
#include <stdio.h>  // Execute this statement as part of the data structure implementation.

// ========== 私有函式 Private Functions ==========

/** Doc block start
 * 主雜湊函式（使用 djb2 演算法）
 * Primary hash function using djb2 algorithm
 *(blank line)
 * @param key 要雜湊的字串
 * @param capacity 容量
 * @return 雜湊值
 */  // End of block comment
static size_t hash1(const char* key, size_t capacity) {  // Compute a hash-based index so keys map into the table's storage.
    unsigned long hash = 5381;  // Assign or update a variable that represents the current algorithm state.
    int c;  // Execute this statement as part of the data structure implementation.

    while ((c = *key++)) {  // Repeat while the loop condition remains true.
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c;  // Compute a hash-based index so keys map into the table's storage.
    }  // Close the current block scope.

    return hash % capacity;  // Return the computed result to the caller.
}  // Close the current block scope.

/** Doc block start
 * 次雜湊函式（用於雙重雜湊） / Secondary hash function for double hashing
 *(blank line)
 * @param key 要雜湊的字串
 * @param capacity 容量
 * @return 雜湊值（必須非零）
 */  // End of block comment
static size_t hash2(const char* key, size_t capacity) {  // Compute a hash-based index so keys map into the table's storage.
    unsigned long hash = 0;  // Assign or update a variable that represents the current algorithm state.
    int c;  // Execute this statement as part of the data structure implementation.

    while ((c = *key++)) {  // Repeat while the loop condition remains true.
        hash = hash * 31 + c;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    // 確保結果非零且為奇數（避免與偶數容量產生問題） / Ensure result is non-zero and odd (avoids issues with even capacity)
    size_t result = (hash % (capacity - 1)) + 1;  // Compute a hash-based index so keys map into the table's storage.
    return result | 1;  // 確保為奇數 - Ensure odd
}  // Close the current block scope.

/** Doc block start
 * 計算探測位置 / Calculate probe position
 *(blank line)
 * @param ht 雜湊表指標
 * @param key 鍵
 * @param i 探測次數（從 0 開始）
 * @return 探測位置
 */  // End of block comment
static size_t probe(OpenAddressingHashTable* ht, const char* key, size_t i) {  // Advance or track the probing sequence used by open addressing.
    size_t h1 = hash1(key, ht->capacity);  // Compute a hash-based index so keys map into the table's storage.

    switch (ht->probe_method) {  // Advance or track the probing sequence used by open addressing.
        case PROBE_LINEAR:  // Execute this statement as part of the data structure implementation.
            // 線性探測: h(k, i) = (h1(k) + i) mod m
            // Linear probing: h(k, i) = (h1(k) + i) mod m
            return (h1 + i) % ht->capacity;  // Return the computed result to the caller.

        case PROBE_QUADRATIC:  // Execute this statement as part of the data structure implementation.
            // 平方探測: h(k, i) = (h1(k) + c1*i + c2*i²) mod m
            // Quadratic probing: h(k, i) = (h1(k) + c1*i + c2*i²) mod m
            // 使用 c1=1, c2=1
            return (h1 + i + i * i) % ht->capacity;  // Return the computed result to the caller.

        case PROBE_DOUBLE_HASH:  // Execute this statement as part of the data structure implementation.
            // 雙重雜湊: h(k, i) = (h1(k) + i*h2(k)) mod m
            // Double hashing: h(k, i) = (h1(k) + i*h2(k)) mod m
            {  // Open a new block scope.
                size_t h2 = hash2(key, ht->capacity);  // Compute a hash-based index so keys map into the table's storage.
                return (h1 + i * h2) % ht->capacity;  // Return the computed result to the caller.
            }  // Close the current block scope.

        default:  // Execute this statement as part of the data structure implementation.
            return h1;  // Return the computed result to the caller.
    }  // Close the current block scope.
}  // Close the current block scope.

/** Doc block start
 * 檢查條目是否為空（未使用） / Check if entry is empty (unused)
 */  // End of block comment
static bool is_empty(OpenAddressingEntry* entry) {  // Execute this statement as part of the data structure implementation.
    return entry->key == NULL && entry->value == NULL;  // Return the computed result to the caller.
}  // Close the current block scope.

/** Doc block start
 * 檢查條目是否為墓碑 / Check if entry is a tombstone
 */  // End of block comment
static bool is_tombstone(OpenAddressingEntry* entry) {  // Handle tombstones so deletions do not break the probing/search sequence.
    return entry->key == NULL && entry->value == OAHT_TOMBSTONE;  // Return the computed result to the caller.
}  // Close the current block scope.

/** Doc block start
 * 檢查條目是否為有效元素 / Check if entry is a valid element
 */  // End of block comment
static bool is_valid(OpenAddressingEntry* entry) {  // Execute this statement as part of the data structure implementation.
    return entry->key != NULL;  // Return the computed result to the caller.
}  // Close the current block scope.

/** Doc block start
 * 擴容並重新雜湊 / Resize and rehash
 */  // End of block comment
static bool rehash(OpenAddressingHashTable* ht) {  // Rehash entries into a larger table to keep operations near O(1) on average.
    size_t old_capacity = ht->capacity;  // Assign or update a variable that represents the current algorithm state.
    OpenAddressingEntry* old_entries = ht->entries;  // Assign or update a variable that represents the current algorithm state.

    // 容量加倍 - Double capacity
    size_t new_capacity = old_capacity * 2;  // Assign or update a variable that represents the current algorithm state.
    OpenAddressingEntry* new_entries = calloc(new_capacity, sizeof(OpenAddressingEntry));  // Allocate zero-initialized heap memory for arrays/struct storage.
    if (!new_entries) return false;  // Evaluate the condition and branch into the appropriate code path.

    // 更新雜湊表 - Update hash table
    ht->entries = new_entries;  // Assign or update a variable that represents the current algorithm state.
    ht->capacity = new_capacity;  // Assign or update a variable that represents the current algorithm state.
    size_t old_size = ht->size;  // Assign or update a variable that represents the current algorithm state.
    ht->size = 0;  // Assign or update a variable that represents the current algorithm state.
    ht->tombstones = 0;  // Handle tombstones so deletions do not break the probing/search sequence.

    // 重新插入所有有效元素 - Reinsert all valid elements
    for (size_t i = 0; i < old_capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        OpenAddressingEntry* entry = &old_entries[i];  // Assign or update a variable that represents the current algorithm state.
        if (is_valid(entry)) {  // Evaluate the condition and branch into the appropriate code path.
            oaht_insert(ht, entry->key, entry->value);  // Execute this statement as part of the data structure implementation.
            free(entry->key);  // Free previously allocated heap memory to avoid leaks.
        }  // Close the current block scope.
    }  // Close the current block scope.

    free(old_entries);  // Free previously allocated heap memory to avoid leaks.
    return true;  // Return the computed result to the caller.
}  // Close the current block scope.

// ========== 公開函式 Public Functions ==========

OpenAddressingHashTable* oaht_create(size_t capacity, ProbeMethod method) {  // Execute this statement as part of the data structure implementation.
    if (capacity == 0) {  // Evaluate the condition and branch into the appropriate code path.
        capacity = OAHT_DEFAULT_CAPACITY;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    OpenAddressingHashTable* ht = malloc(sizeof(OpenAddressingHashTable));  // Allocate heap memory for a new struct/object instance.
    if (!ht) return NULL;  // Evaluate the condition and branch into the appropriate code path.

    ht->entries = calloc(capacity, sizeof(OpenAddressingEntry));  // Allocate zero-initialized heap memory for arrays/struct storage.
    if (!ht->entries) {  // Evaluate the condition and branch into the appropriate code path.
        free(ht);  // Free previously allocated heap memory to avoid leaks.
        return NULL;  // Return the computed result to the caller.
    }  // Close the current block scope.

    ht->capacity = capacity;  // Assign or update a variable that represents the current algorithm state.
    ht->size = 0;  // Assign or update a variable that represents the current algorithm state.
    ht->tombstones = 0;  // Handle tombstones so deletions do not break the probing/search sequence.
    ht->probe_method = method;  // Advance or track the probing sequence used by open addressing.
    return ht;  // Return the computed result to the caller.
}  // Close the current block scope.

void oaht_free(OpenAddressingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    if (!ht) return;  // Evaluate the condition and branch into the appropriate code path.

    // 釋放所有 key 字串 - Free all key strings
    for (size_t i = 0; i < ht->capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        if (is_valid(&ht->entries[i])) {  // Evaluate the condition and branch into the appropriate code path.
            free(ht->entries[i].key);  // Free previously allocated heap memory to avoid leaks.
        }  // Close the current block scope.
    }  // Close the current block scope.

    free(ht->entries);  // Free previously allocated heap memory to avoid leaks.
    free(ht);  // Free previously allocated heap memory to avoid leaks.
}  // Close the current block scope.

bool oaht_insert(OpenAddressingHashTable* ht, const char* key, void* value) {  // Execute this statement as part of the data structure implementation.
    if (!ht || !key || !value || value == OAHT_TOMBSTONE) return false;  // Evaluate the condition and branch into the appropriate code path.

    // 檢查是否需要擴容 - Check if rehashing needed
    double effective_load = (double)(ht->size + ht->tombstones) / ht->capacity;  // Handle tombstones so deletions do not break the probing/search sequence.
    if (effective_load > OAHT_MAX_LOAD_FACTOR) {  // Evaluate the condition and branch into the appropriate code path.
        rehash(ht);  // Rehash entries into a larger table to keep operations near O(1) on average.
    }  // Close the current block scope.

    size_t first_tombstone = SIZE_MAX;  // Handle tombstones so deletions do not break the probing/search sequence.

    // 探測找到插入位置 - Probe to find insertion position
    for (size_t i = 0; i < ht->capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        size_t index = probe(ht, key, i);  // Advance or track the probing sequence used by open addressing.
        OpenAddressingEntry* entry = &ht->entries[index];  // Assign or update a variable that represents the current algorithm state.

        if (is_empty(entry)) {  // Evaluate the condition and branch into the appropriate code path.
            // 找到空位 - Found empty slot
            if (first_tombstone != SIZE_MAX) {  // Evaluate the condition and branch into the appropriate code path.
                // 優先使用之前遇到的墓碑位置 / Use previously encountered tombstone position
                index = first_tombstone;  // Handle tombstones so deletions do not break the probing/search sequence.
                entry = &ht->entries[index];  // Assign or update a variable that represents the current algorithm state.
                ht->tombstones--;  // Handle tombstones so deletions do not break the probing/search sequence.
            }  // Close the current block scope.

            entry->key = strdup(key);  // Duplicate the key string so the hash table owns its own copy.
            if (!entry->key) return false;  // Evaluate the condition and branch into the appropriate code path.
            entry->value = value;  // Assign or update a variable that represents the current algorithm state.
            ht->size++;  // Execute this statement as part of the data structure implementation.
            return true;  // Return the computed result to the caller.
        } else if (is_tombstone(entry)) {  // Handle tombstones so deletions do not break the probing/search sequence.
            // 記錄第一個墓碑位置 - Record first tombstone position
            if (first_tombstone == SIZE_MAX) {  // Evaluate the condition and branch into the appropriate code path.
                first_tombstone = index;  // Handle tombstones so deletions do not break the probing/search sequence.
            }  // Close the current block scope.
        } else if (strcmp(entry->key, key) == 0) {  // Execute this statement as part of the data structure implementation.
            // 鍵已存在，更新值 - Key exists, update value
            entry->value = value;  // Assign or update a variable that represents the current algorithm state.
            return true;  // Return the computed result to the caller.
        }  // Close the current block scope.
    }  // Close the current block scope.

    // 表已滿（理論上不會發生，因為有擴容機制） / Table is full (should not happen due to rehashing)
    return false;  // Return the computed result to the caller.
}  // Close the current block scope.

void* oaht_search(OpenAddressingHashTable* ht, const char* key) {  // Execute this statement as part of the data structure implementation.
    if (!ht || !key) return NULL;  // Evaluate the condition and branch into the appropriate code path.

    // 探測搜尋 - Probe to search
    for (size_t i = 0; i < ht->capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        size_t index = probe(ht, key, i);  // Advance or track the probing sequence used by open addressing.
        OpenAddressingEntry* entry = &ht->entries[index];  // Assign or update a variable that represents the current algorithm state.

        if (is_empty(entry)) {  // Evaluate the condition and branch into the appropriate code path.
            // 遇到空位，搜尋失敗 - Found empty slot, search failed
            return NULL;  // Return the computed result to the caller.
        } else if (is_valid(entry) && strcmp(entry->key, key) == 0) {  // Execute this statement as part of the data structure implementation.
            // 找到 - Found
            return entry->value;  // Return the computed result to the caller.
        }  // Close the current block scope.
        // 遇到墓碑或其他鍵，繼續探測 - Found tombstone or other key, continue probing
    }  // Close the current block scope.

    return NULL;  // Return the computed result to the caller.
}  // Close the current block scope.

bool oaht_delete(OpenAddressingHashTable* ht, const char* key) {  // Execute this statement as part of the data structure implementation.
    if (!ht || !key) return false;  // Evaluate the condition and branch into the appropriate code path.

    // 探測搜尋 - Probe to search
    for (size_t i = 0; i < ht->capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        size_t index = probe(ht, key, i);  // Advance or track the probing sequence used by open addressing.
        OpenAddressingEntry* entry = &ht->entries[index];  // Assign or update a variable that represents the current algorithm state.

        if (is_empty(entry)) {  // Evaluate the condition and branch into the appropriate code path.
            // 遇到空位，刪除失敗 - Found empty slot, deletion failed
            return false;  // Return the computed result to the caller.
        } else if (is_valid(entry) && strcmp(entry->key, key) == 0) {  // Execute this statement as part of the data structure implementation.
            // 找到，標記為墓碑 - Found, mark as tombstone
            free(entry->key);  // Free previously allocated heap memory to avoid leaks.
            entry->key = NULL;  // Assign or update a variable that represents the current algorithm state.
            entry->value = OAHT_TOMBSTONE;  // Assign or update a variable that represents the current algorithm state.
            ht->size--;  // Execute this statement as part of the data structure implementation.
            ht->tombstones++;  // Handle tombstones so deletions do not break the probing/search sequence.
            return true;  // Return the computed result to the caller.
        }  // Close the current block scope.
        // 遇到墓碑或其他鍵，繼續探測 - Found tombstone or other key, continue probing
    }  // Close the current block scope.

    return false;  // Return the computed result to the caller.
}  // Close the current block scope.

bool oaht_contains(OpenAddressingHashTable* ht, const char* key) {  // Execute this statement as part of the data structure implementation.
    return oaht_search(ht, key) != NULL;  // Return the computed result to the caller.
}  // Close the current block scope.

size_t oaht_size(OpenAddressingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    return ht ? ht->size : 0;  // Return the computed result to the caller.
}  // Close the current block scope.

size_t oaht_capacity(OpenAddressingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    return ht ? ht->capacity : 0;  // Return the computed result to the caller.
}  // Close the current block scope.

size_t oaht_tombstone_count(OpenAddressingHashTable* ht) {  // Handle tombstones so deletions do not break the probing/search sequence.
    return ht ? ht->tombstones : 0;  // Return the computed result to the caller.
}  // Close the current block scope.

bool oaht_is_empty(OpenAddressingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    return ht ? ht->size == 0 : true;  // Return the computed result to the caller.
}  // Close the current block scope.

double oaht_load_factor(OpenAddressingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    if (!ht || ht->capacity == 0) return 0.0;  // Evaluate the condition and branch into the appropriate code path.
    return (double)(ht->size + ht->tombstones) / ht->capacity;  // Return the computed result to the caller.
}  // Close the current block scope.

void oaht_clear(OpenAddressingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    if (!ht) return;  // Evaluate the condition and branch into the appropriate code path.

    for (size_t i = 0; i < ht->capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        OpenAddressingEntry* entry = &ht->entries[i];  // Assign or update a variable that represents the current algorithm state.
        if (is_valid(entry)) {  // Evaluate the condition and branch into the appropriate code path.
            free(entry->key);  // Free previously allocated heap memory to avoid leaks.
        }  // Close the current block scope.
        entry->key = NULL;  // Assign or update a variable that represents the current algorithm state.
        entry->value = NULL;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.
    ht->size = 0;  // Assign or update a variable that represents the current algorithm state.
    ht->tombstones = 0;  // Handle tombstones so deletions do not break the probing/search sequence.
}  // Close the current block scope.

const char* oaht_probe_method_name(ProbeMethod method) {  // Advance or track the probing sequence used by open addressing.
    switch (method) {  // Execute this statement as part of the data structure implementation.
        case PROBE_LINEAR:  // Execute this statement as part of the data structure implementation.
            return "Linear Probing 線性探測";  // Return the computed result to the caller.
        case PROBE_QUADRATIC:  // Execute this statement as part of the data structure implementation.
            return "Quadratic Probing 平方探測";  // Return the computed result to the caller.
        case PROBE_DOUBLE_HASH:  // Execute this statement as part of the data structure implementation.
            return "Double Hashing 雙重雜湊";  // Return the computed result to the caller.
        default:  // Execute this statement as part of the data structure implementation.
            return "Unknown 未知";  // Return the computed result to the caller.
    }  // Close the current block scope.
}  // Close the current block scope.

void oaht_print(OpenAddressingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    if (!ht) {  // Evaluate the condition and branch into the appropriate code path.
        printf("OpenAddressingHashTable: NULL\n");  // Execute this statement as part of the data structure implementation.
        return;  // Return the computed result to the caller.
    }  // Close the current block scope.

    printf("OpenAddressingHashTable (size=%zu, capacity=%zu, load=%.3f, method=%s):\n",  // Assign or update a variable that represents the current algorithm state.
           ht->size, ht->capacity, oaht_load_factor(ht),  // Execute this statement as part of the data structure implementation.
           oaht_probe_method_name(ht->probe_method));  // Advance or track the probing sequence used by open addressing.

    for (size_t i = 0; i < ht->capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        OpenAddressingEntry* entry = &ht->entries[i];  // Assign or update a variable that represents the current algorithm state.
        if (is_valid(entry)) {  // Evaluate the condition and branch into the appropriate code path.
            printf("  [%zu]: (%s, %p)\n", i, entry->key, entry->value);  // Execute this statement as part of the data structure implementation.
        } else if (is_tombstone(entry)) {  // Handle tombstones so deletions do not break the probing/search sequence.
            printf("  [%zu]: TOMBSTONE\n", i);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.
}  // Close the current block scope.

void oaht_print_stats(OpenAddressingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    if (!ht) {  // Evaluate the condition and branch into the appropriate code path.
        printf("OpenAddressingHashTable: NULL\n");  // Execute this statement as part of the data structure implementation.
        return;  // Return the computed result to the caller.
    }  // Close the current block scope.

    size_t empty_slots = 0;  // Assign or update a variable that represents the current algorithm state.
    for (size_t i = 0; i < ht->capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        if (is_empty(&ht->entries[i])) {  // Evaluate the condition and branch into the appropriate code path.
            empty_slots++;  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.

    printf("開放定址法雜湊表統計 Open Addressing Hash Table Statistics:\n");  // Execute this statement as part of the data structure implementation.
    printf("  探測方法 Probe Method: %s\n", oaht_probe_method_name(ht->probe_method));  // Advance or track the probing sequence used by open addressing.
    printf("  元素數量 Size: %zu\n", ht->size);  // Execute this statement as part of the data structure implementation.
    printf("  容量 Capacity: %zu\n", ht->capacity);  // Execute this statement as part of the data structure implementation.
    printf("  墓碑數量 Tombstones: %zu\n", ht->tombstones);  // Handle tombstones so deletions do not break the probing/search sequence.
    printf("  空槽數量 Empty Slots: %zu\n", empty_slots);  // Execute this statement as part of the data structure implementation.
    printf("  負載因子 Load Factor: %.3f\n", oaht_load_factor(ht));  // Execute this statement as part of the data structure implementation.
    printf("  實際負載 Actual Load: %.3f (不含墓碑 excluding tombstones)\n",  // Handle tombstones so deletions do not break the probing/search sequence.
           ht->capacity > 0 ? (double)ht->size / ht->capacity : 0.0);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.
