/** Doc block start
 * 鏈結法雜湊表（Chaining Hash Table）- C 語言實作
 * 使用鏈結串列處理碰撞，包含鏈長統計 / Use linked list 處理 collision ，包含鏈長 count
 *(blank line)
 * Chaining Hash Table implementation with chain length statistics
 */  // End of block comment

#include "chaining.h"  // Execute this statement as part of the data structure implementation.
#include <stdlib.h>  // Execute this statement as part of the data structure implementation.
#include <string.h>  // Execute this statement as part of the data structure implementation.
#include <stdio.h>  // Execute this statement as part of the data structure implementation.

// ========== 私有函式 Private Functions ==========

/** Doc block start
 * 計算雜湊值（使用 djb2 演算法）
 * Compute hash value using djb2 algorithm
 *(blank line)
 * @param key 要雜湊的字串
 * @param capacity 桶的數量
 * @return 桶的索引
 */  // End of block comment
static size_t hash(const char* key, size_t capacity) {  // Compute a hash-based index so keys map into the table's storage.
    unsigned long hash = 5381;  // Assign or update a variable that represents the current algorithm state.
    int c;  // Execute this statement as part of the data structure implementation.

    while ((c = *key++)) {  // Repeat while the loop condition remains true.
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c;  // Compute a hash-based index so keys map into the table's storage.
    }  // Close the current block scope.

    return hash % capacity;  // Return the computed result to the caller.
}  // Close the current block scope.

/** Doc block start
 * 建立新的節點 / Create a new entry
 */  // End of block comment
static ChainingEntry* create_entry(const char* key, void* value) {  // Execute this statement as part of the data structure implementation.
    ChainingEntry* entry = malloc(sizeof(ChainingEntry));  // Allocate heap memory for a new struct/object instance.
    if (!entry) return NULL;  // Evaluate the condition and branch into the appropriate code path.

    // 複製 key 字串 - Copy key string
    entry->key = strdup(key);  // Duplicate the key string so the hash table owns its own copy.
    if (!entry->key) {  // Evaluate the condition and branch into the appropriate code path.
        free(entry);  // Free previously allocated heap memory to avoid leaks.
        return NULL;  // Return the computed result to the caller.
    }  // Close the current block scope.

    entry->value = value;  // Assign or update a variable that represents the current algorithm state.
    entry->next = NULL;  // Assign or update a variable that represents the current algorithm state.
    return entry;  // Return the computed result to the caller.
}  // Close the current block scope.

/** Doc block start
 * 釋放節點 / Free an entry
 */  // End of block comment
static void free_entry(ChainingEntry* entry) {  // Execute this statement as part of the data structure implementation.
    if (entry) {  // Evaluate the condition and branch into the appropriate code path.
        free(entry->key);  // Free previously allocated heap memory to avoid leaks.
        // 注意：不釋放 value，由呼叫者負責
        // Note: value is not freed, caller's responsibility
        free(entry);  // Free previously allocated heap memory to avoid leaks.
    }  // Close the current block scope.
}  // Close the current block scope.

/** Doc block start
 * 計算單一鏈結串列的長度 / Calculate length of a single chain
 */  // End of block comment
static size_t chain_length(ChainingEntry* entry) {  // Execute this statement as part of the data structure implementation.
    size_t length = 0;  // Assign or update a variable that represents the current algorithm state.
    while (entry) {  // Repeat while the loop condition remains true.
        length++;  // Execute this statement as part of the data structure implementation.
        entry = entry->next;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.
    return length;  // Return the computed result to the caller.
}  // Close the current block scope.

/** Doc block start
 * 擴容並重新雜湊 / Resize and rehash
 */  // End of block comment
static bool rehash(ChainingHashTable* ht) {  // Rehash entries into a larger table to keep operations near O(1) on average.
    size_t old_capacity = ht->capacity;  // Assign or update a variable that represents the current algorithm state.
    ChainingEntry** old_buckets = ht->buckets;  // Access or update the bucket storage used to hold entries or chains.

    // 容量加倍 - Double capacity
    size_t new_capacity = old_capacity * 2;  // Assign or update a variable that represents the current algorithm state.
    ChainingEntry** new_buckets = calloc(new_capacity, sizeof(ChainingEntry*));  // Access or update the bucket storage used to hold entries or chains.
    if (!new_buckets) return false;  // Evaluate the condition and branch into the appropriate code path.

    // 更新雜湊表 - Update hash table
    ht->buckets = new_buckets;  // Access or update the bucket storage used to hold entries or chains.
    ht->capacity = new_capacity;  // Assign or update a variable that represents the current algorithm state.
    ht->size = 0;  // Assign or update a variable that represents the current algorithm state.

    // 重新插入所有元素 - Reinsert all elements
    for (size_t i = 0; i < old_capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        ChainingEntry* entry = old_buckets[i];  // Access or update the bucket storage used to hold entries or chains.
        while (entry) {  // Repeat while the loop condition remains true.
            ChainingEntry* next = entry->next;  // Assign or update a variable that represents the current algorithm state.
            cht_insert(ht, entry->key, entry->value);  // Execute this statement as part of the data structure implementation.
            free(entry->key);  // Free previously allocated heap memory to avoid leaks.
            free(entry);  // Free previously allocated heap memory to avoid leaks.
            entry = next;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.
    }  // Close the current block scope.

    free(old_buckets);  // Access or update the bucket storage used to hold entries or chains.
    return true;  // Return the computed result to the caller.
}  // Close the current block scope.

// ========== 公開函式 Public Functions ==========

ChainingHashTable* cht_create(size_t capacity) {  // Execute this statement as part of the data structure implementation.
    if (capacity == 0) {  // Evaluate the condition and branch into the appropriate code path.
        capacity = CHT_DEFAULT_CAPACITY;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    ChainingHashTable* ht = malloc(sizeof(ChainingHashTable));  // Allocate heap memory for a new struct/object instance.
    if (!ht) return NULL;  // Evaluate the condition and branch into the appropriate code path.

    ht->buckets = calloc(capacity, sizeof(ChainingEntry*));  // Access or update the bucket storage used to hold entries or chains.
    if (!ht->buckets) {  // Evaluate the condition and branch into the appropriate code path.
        free(ht);  // Free previously allocated heap memory to avoid leaks.
        return NULL;  // Return the computed result to the caller.
    }  // Close the current block scope.

    ht->capacity = capacity;  // Assign or update a variable that represents the current algorithm state.
    ht->size = 0;  // Assign or update a variable that represents the current algorithm state.
    return ht;  // Return the computed result to the caller.
}  // Close the current block scope.

void cht_free(ChainingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    if (!ht) return;  // Evaluate the condition and branch into the appropriate code path.

    // 釋放所有節點 - Free all entries
    for (size_t i = 0; i < ht->capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        ChainingEntry* entry = ht->buckets[i];  // Access or update the bucket storage used to hold entries or chains.
        while (entry) {  // Repeat while the loop condition remains true.
            ChainingEntry* next = entry->next;  // Assign or update a variable that represents the current algorithm state.
            free_entry(entry);  // Execute this statement as part of the data structure implementation.
            entry = next;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.
    }  // Close the current block scope.

    free(ht->buckets);  // Access or update the bucket storage used to hold entries or chains.
    free(ht);  // Free previously allocated heap memory to avoid leaks.
}  // Close the current block scope.

bool cht_insert(ChainingHashTable* ht, const char* key, void* value) {  // Execute this statement as part of the data structure implementation.
    if (!ht || !key) return false;  // Evaluate the condition and branch into the appropriate code path.

    size_t index = hash(key, ht->capacity);  // Compute a hash-based index so keys map into the table's storage.
    ChainingEntry* entry = ht->buckets[index];  // Access or update the bucket storage used to hold entries or chains.

    // 檢查 key 是否已存在 - Check if key exists
    while (entry) {  // Repeat while the loop condition remains true.
        if (strcmp(entry->key, key) == 0) {  // Evaluate the condition and branch into the appropriate code path.
            entry->value = value;  // 更新 - Update existing
            return true;  // Return the computed result to the caller.
        }  // Close the current block scope.
        entry = entry->next;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    // 建立新節點 - Create new entry
    ChainingEntry* new_entry = create_entry(key, value);  // Assign or update a variable that represents the current algorithm state.
    if (!new_entry) return false;  // Evaluate the condition and branch into the appropriate code path.

    // 插入到鏈結串列頭部 - Insert at head
    new_entry->next = ht->buckets[index];  // Access or update the bucket storage used to hold entries or chains.
    ht->buckets[index] = new_entry;  // Access or update the bucket storage used to hold entries or chains.
    ht->size++;  // Execute this statement as part of the data structure implementation.

    // 檢查是否需要擴容 - Check if rehashing needed
    if (cht_load_factor(ht) > CHT_MAX_LOAD_FACTOR) {  // Evaluate the condition and branch into the appropriate code path.
        rehash(ht);  // Rehash entries into a larger table to keep operations near O(1) on average.
    }  // Close the current block scope.

    return true;  // Return the computed result to the caller.
}  // Close the current block scope.

void* cht_search(ChainingHashTable* ht, const char* key) {  // Execute this statement as part of the data structure implementation.
    if (!ht || !key) return NULL;  // Evaluate the condition and branch into the appropriate code path.

    size_t index = hash(key, ht->capacity);  // Compute a hash-based index so keys map into the table's storage.
    ChainingEntry* entry = ht->buckets[index];  // Access or update the bucket storage used to hold entries or chains.

    while (entry) {  // Repeat while the loop condition remains true.
        if (strcmp(entry->key, key) == 0) {  // Evaluate the condition and branch into the appropriate code path.
            return entry->value;  // Return the computed result to the caller.
        }  // Close the current block scope.
        entry = entry->next;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    return NULL;  // Return the computed result to the caller.
}  // Close the current block scope.

bool cht_delete(ChainingHashTable* ht, const char* key) {  // Execute this statement as part of the data structure implementation.
    if (!ht || !key) return false;  // Evaluate the condition and branch into the appropriate code path.

    size_t index = hash(key, ht->capacity);  // Compute a hash-based index so keys map into the table's storage.
    ChainingEntry* entry = ht->buckets[index];  // Access or update the bucket storage used to hold entries or chains.
    ChainingEntry* prev = NULL;  // Assign or update a variable that represents the current algorithm state.

    while (entry) {  // Repeat while the loop condition remains true.
        if (strcmp(entry->key, key) == 0) {  // Evaluate the condition and branch into the appropriate code path.
            if (prev) {  // Evaluate the condition and branch into the appropriate code path.
                prev->next = entry->next;  // Assign or update a variable that represents the current algorithm state.
            } else {  // Handle the alternative branch when the condition is false.
                ht->buckets[index] = entry->next;  // Access or update the bucket storage used to hold entries or chains.
            }  // Close the current block scope.
            free_entry(entry);  // Execute this statement as part of the data structure implementation.
            ht->size--;  // Execute this statement as part of the data structure implementation.
            return true;  // Return the computed result to the caller.
        }  // Close the current block scope.
        prev = entry;  // Assign or update a variable that represents the current algorithm state.
        entry = entry->next;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    return false;  // Return the computed result to the caller.
}  // Close the current block scope.

bool cht_contains(ChainingHashTable* ht, const char* key) {  // Execute this statement as part of the data structure implementation.
    return cht_search(ht, key) != NULL;  // Return the computed result to the caller.
}  // Close the current block scope.

size_t cht_size(ChainingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    return ht ? ht->size : 0;  // Return the computed result to the caller.
}  // Close the current block scope.

size_t cht_capacity(ChainingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    return ht ? ht->capacity : 0;  // Return the computed result to the caller.
}  // Close the current block scope.

bool cht_is_empty(ChainingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    return ht ? ht->size == 0 : true;  // Return the computed result to the caller.
}  // Close the current block scope.

double cht_load_factor(ChainingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    if (!ht || ht->capacity == 0) return 0.0;  // Evaluate the condition and branch into the appropriate code path.
    return (double)ht->size / ht->capacity;  // Return the computed result to the caller.
}  // Close the current block scope.

void cht_clear(ChainingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    if (!ht) return;  // Evaluate the condition and branch into the appropriate code path.

    for (size_t i = 0; i < ht->capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        ChainingEntry* entry = ht->buckets[i];  // Access or update the bucket storage used to hold entries or chains.
        while (entry) {  // Repeat while the loop condition remains true.
            ChainingEntry* next = entry->next;  // Assign or update a variable that represents the current algorithm state.
            free_entry(entry);  // Execute this statement as part of the data structure implementation.
            entry = next;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.
        ht->buckets[i] = NULL;  // Access or update the bucket storage used to hold entries or chains.
    }  // Close the current block scope.
    ht->size = 0;  // Assign or update a variable that represents the current algorithm state.
}  // Close the current block scope.

void cht_get_stats(ChainingHashTable* ht, ChainStats* stats) {  // Execute this statement as part of the data structure implementation.
    if (!ht || !stats) return;  // Evaluate the condition and branch into the appropriate code path.

    stats->min_length = SIZE_MAX;  // Assign or update a variable that represents the current algorithm state.
    stats->max_length = 0;  // Assign or update a variable that represents the current algorithm state.
    stats->avg_length = 0.0;  // Assign or update a variable that represents the current algorithm state.
    stats->empty_buckets = 0;  // Access or update the bucket storage used to hold entries or chains.

    size_t total_length = 0;  // Assign or update a variable that represents the current algorithm state.
    size_t non_empty_buckets = 0;  // Access or update the bucket storage used to hold entries or chains.

    // 遍歷所有桶 - Iterate through all buckets
    for (size_t i = 0; i < ht->capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        size_t length = chain_length(ht->buckets[i]);  // Access or update the bucket storage used to hold entries or chains.

        if (length == 0) {  // Evaluate the condition and branch into the appropriate code path.
            stats->empty_buckets++;  // Access or update the bucket storage used to hold entries or chains.
        } else {  // Handle the alternative branch when the condition is false.
            non_empty_buckets++;  // Access or update the bucket storage used to hold entries or chains.
            total_length += length;  // Assign or update a variable that represents the current algorithm state.
            if (length < stats->min_length) {  // Evaluate the condition and branch into the appropriate code path.
                stats->min_length = length;  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.
            if (length > stats->max_length) {  // Evaluate the condition and branch into the appropriate code path.
                stats->max_length = length;  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.
        }  // Close the current block scope.
    }  // Close the current block scope.

    // 計算平均鏈長 - Calculate average chain length
    if (non_empty_buckets > 0) {  // Evaluate the condition and branch into the appropriate code path.
        stats->avg_length = (double)total_length / non_empty_buckets;  // Access or update the bucket storage used to hold entries or chains.
    }  // Close the current block scope.

    // 如果沒有非空桶，將最小長度設為 0 / If no non-empty buckets, set min_length to 0
    if (stats->min_length == SIZE_MAX) {  // Evaluate the condition and branch into the appropriate code path.
        stats->min_length = 0;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.
}  // Close the current block scope.

void cht_print_stats(ChainingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    if (!ht) {  // Evaluate the condition and branch into the appropriate code path.
        printf("ChainingHashTable: NULL\n");  // Execute this statement as part of the data structure implementation.
        return;  // Return the computed result to the caller.
    }  // Close the current block scope.

    ChainStats stats;  // Execute this statement as part of the data structure implementation.
    cht_get_stats(ht, &stats);  // Execute this statement as part of the data structure implementation.

    printf("鏈結法雜湊表統計 Chaining Hash Table Statistics:\n");  // Execute this statement as part of the data structure implementation.
    printf("  元素數量 Size: %zu\n", ht->size);  // Execute this statement as part of the data structure implementation.
    printf("  桶數量 Capacity: %zu\n", ht->capacity);  // Execute this statement as part of the data structure implementation.
    printf("  負載因子 Load Factor: %.3f\n", cht_load_factor(ht));  // Execute this statement as part of the data structure implementation.
    printf("  空桶數量 Empty Buckets: %zu (%.1f%%)\n",  // Execute this statement as part of the data structure implementation.
           stats.empty_buckets,  // Access or update the bucket storage used to hold entries or chains.
           100.0 * stats.empty_buckets / ht->capacity);  // Access or update the bucket storage used to hold entries or chains.
    printf("  最短鏈長 Min Chain: %zu\n", stats.min_length);  // Execute this statement as part of the data structure implementation.
    printf("  最長鏈長 Max Chain: %zu\n", stats.max_length);  // Execute this statement as part of the data structure implementation.
    printf("  平均鏈長 Avg Chain: %.2f\n", stats.avg_length);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

void cht_print(ChainingHashTable* ht) {  // Execute this statement as part of the data structure implementation.
    if (!ht) {  // Evaluate the condition and branch into the appropriate code path.
        printf("ChainingHashTable: NULL\n");  // Execute this statement as part of the data structure implementation.
        return;  // Return the computed result to the caller.
    }  // Close the current block scope.

    printf("ChainingHashTable (size=%zu, capacity=%zu, load=%.3f):\n",  // Assign or update a variable that represents the current algorithm state.
           ht->size, ht->capacity, cht_load_factor(ht));  // Execute this statement as part of the data structure implementation.

    for (size_t i = 0; i < ht->capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        ChainingEntry* entry = ht->buckets[i];  // Access or update the bucket storage used to hold entries or chains.
        if (entry) {  // Evaluate the condition and branch into the appropriate code path.
            printf("  [%zu]: ", i);  // Execute this statement as part of the data structure implementation.
            while (entry) {  // Repeat while the loop condition remains true.
                printf("(%s, %p)", entry->key, entry->value);  // Execute this statement as part of the data structure implementation.
                if (entry->next) printf(" -> ");  // Evaluate the condition and branch into the appropriate code path.
                entry = entry->next;  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.
            printf("\n");  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.
}  // Close the current block scope.
