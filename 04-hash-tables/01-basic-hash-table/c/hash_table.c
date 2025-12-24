/** Doc block start
 * 雜湊表（Hash Table）- C 語言實作
 * 使用鏈結法（chaining）處理碰撞
 *(blank line)
 * Hash Table implementation using chaining for collision resolution
 */  // End of block comment

#include "hash_table.h"  // Execute this statement as part of the data structure implementation.
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
 * strdup 的可攜替代（避免依賴 POSIX strdup） / Portable strdup replacement (avoid relying on POSIX strdup)
 */  // End of block comment
static char* ht_strdup(const char* src) {  // Execute this statement as part of the data structure implementation.
    if (!src) {  // Evaluate the condition and branch into the appropriate code path.
        return NULL;  // Return the computed result to the caller.
    }  // Close the current block scope.

    size_t len = strlen(src) + 1;  // Assign or update a variable that represents the current algorithm state.
    char* copy = malloc(len);  // Allocate heap memory for a new struct/object instance.
    if (!copy) {  // Evaluate the condition and branch into the appropriate code path.
        return NULL;  // Return the computed result to the caller.
    }  // Close the current block scope.

    memcpy(copy, src, len);  // Execute this statement as part of the data structure implementation.
    return copy;  // Return the computed result to the caller.
}  // Close the current block scope.

/** Doc block start
 * 建立新的節點 / Create a new entry
 */  // End of block comment
static HashTableEntry* create_entry(const char* key, void* value) {  // Execute this statement as part of the data structure implementation.
    HashTableEntry* entry = malloc(sizeof(HashTableEntry));  // Allocate heap memory for a new struct/object instance.
    if (!entry) return NULL;  // Evaluate the condition and branch into the appropriate code path.

    // 複製 key 字串 - Copy key string
    entry->key = ht_strdup(key);  // Duplicate the key string so the hash table owns its own copy.
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
static void free_entry(HashTableEntry* entry) {  // Execute this statement as part of the data structure implementation.
    if (entry) {  // Evaluate the condition and branch into the appropriate code path.
        free(entry->key);  // Free previously allocated heap memory to avoid leaks.
        // 注意：不釋放 value，由呼叫者負責
        // Note: value is not freed, caller's responsibility
        free(entry);  // Free previously allocated heap memory to avoid leaks.
    }  // Close the current block scope.
}  // Close the current block scope.

/** Doc block start
 * 擴容並重新雜湊 / Resize and rehash
 */  // End of block comment
static bool rehash(HashTable* ht) {  // Rehash entries into a larger table to keep operations near O(1) on average.
    size_t old_capacity = ht->capacity;  // Assign or update a variable that represents the current algorithm state.
    HashTableEntry** old_buckets = ht->buckets;  // Access or update the bucket storage used to hold entries or chains.

    // 容量加倍 - Double capacity
    size_t new_capacity = old_capacity * 2;  // Assign or update a variable that represents the current algorithm state.
    HashTableEntry** new_buckets = calloc(new_capacity, sizeof(HashTableEntry*));  // Access or update the bucket storage used to hold entries or chains.
    if (!new_buckets) return false;  // Evaluate the condition and branch into the appropriate code path.

    // 更新雜湊表 - Update hash table
    ht->buckets = new_buckets;  // Access or update the bucket storage used to hold entries or chains.
    ht->capacity = new_capacity;  // Assign or update a variable that represents the current algorithm state.
    ht->size = 0;  // Assign or update a variable that represents the current algorithm state.

    // 重新插入所有元素 - Reinsert all elements
    for (size_t i = 0; i < old_capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        HashTableEntry* entry = old_buckets[i];  // Access or update the bucket storage used to hold entries or chains.
        while (entry) {  // Repeat while the loop condition remains true.
            HashTableEntry* next = entry->next;  // Assign or update a variable that represents the current algorithm state.
            ht_insert(ht, entry->key, entry->value);  // Execute this statement as part of the data structure implementation.
            free(entry->key);  // Free previously allocated heap memory to avoid leaks.
            free(entry);  // Free previously allocated heap memory to avoid leaks.
            entry = next;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.
    }  // Close the current block scope.

    free(old_buckets);  // Access or update the bucket storage used to hold entries or chains.
    return true;  // Return the computed result to the caller.
}  // Close the current block scope.

// ========== 公開函式 Public Functions ==========

HashTable* ht_create(size_t capacity) {  // Execute this statement as part of the data structure implementation.
    if (capacity == 0) {  // Evaluate the condition and branch into the appropriate code path.
        capacity = HT_DEFAULT_CAPACITY;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    HashTable* ht = malloc(sizeof(HashTable));  // Allocate heap memory for a new struct/object instance.
    if (!ht) return NULL;  // Evaluate the condition and branch into the appropriate code path.

    ht->buckets = calloc(capacity, sizeof(HashTableEntry*));  // Access or update the bucket storage used to hold entries or chains.
    if (!ht->buckets) {  // Evaluate the condition and branch into the appropriate code path.
        free(ht);  // Free previously allocated heap memory to avoid leaks.
        return NULL;  // Return the computed result to the caller.
    }  // Close the current block scope.

    ht->capacity = capacity;  // Assign or update a variable that represents the current algorithm state.
    ht->size = 0;  // Assign or update a variable that represents the current algorithm state.
    return ht;  // Return the computed result to the caller.
}  // Close the current block scope.

void ht_free(HashTable* ht) {  // Execute this statement as part of the data structure implementation.
    if (!ht) return;  // Evaluate the condition and branch into the appropriate code path.

    // 釋放所有節點 - Free all entries
    for (size_t i = 0; i < ht->capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        HashTableEntry* entry = ht->buckets[i];  // Access or update the bucket storage used to hold entries or chains.
        while (entry) {  // Repeat while the loop condition remains true.
            HashTableEntry* next = entry->next;  // Assign or update a variable that represents the current algorithm state.
            free_entry(entry);  // Execute this statement as part of the data structure implementation.
            entry = next;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.
    }  // Close the current block scope.

    free(ht->buckets);  // Access or update the bucket storage used to hold entries or chains.
    free(ht);  // Free previously allocated heap memory to avoid leaks.
}  // Close the current block scope.

bool ht_insert(HashTable* ht, const char* key, void* value) {  // Execute this statement as part of the data structure implementation.
    if (!ht || !key) return false;  // Evaluate the condition and branch into the appropriate code path.

    size_t index = hash(key, ht->capacity);  // Compute a hash-based index so keys map into the table's storage.
    HashTableEntry* entry = ht->buckets[index];  // Access or update the bucket storage used to hold entries or chains.

    // 檢查 key 是否已存在 - Check if key exists
    while (entry) {  // Repeat while the loop condition remains true.
        if (strcmp(entry->key, key) == 0) {  // Evaluate the condition and branch into the appropriate code path.
            entry->value = value;  // 更新 - Update existing
            return true;  // Return the computed result to the caller.
        }  // Close the current block scope.
        entry = entry->next;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    // 建立新節點 - Create new entry
    HashTableEntry* new_entry = create_entry(key, value);  // Assign or update a variable that represents the current algorithm state.
    if (!new_entry) return false;  // Evaluate the condition and branch into the appropriate code path.

    // 插入到鏈結串列頭部 - Insert at head
    new_entry->next = ht->buckets[index];  // Access or update the bucket storage used to hold entries or chains.
    ht->buckets[index] = new_entry;  // Access or update the bucket storage used to hold entries or chains.
    ht->size++;  // Execute this statement as part of the data structure implementation.

    // 檢查是否需要擴容 - Check if rehashing needed
    if (ht_load_factor(ht) > HT_MAX_LOAD_FACTOR) {  // Evaluate the condition and branch into the appropriate code path.
        rehash(ht);  // Rehash entries into a larger table to keep operations near O(1) on average.
    }  // Close the current block scope.

    return true;  // Return the computed result to the caller.
}  // Close the current block scope.

void* ht_search(HashTable* ht, const char* key) {  // Execute this statement as part of the data structure implementation.
    if (!ht || !key) return NULL;  // Evaluate the condition and branch into the appropriate code path.

    size_t index = hash(key, ht->capacity);  // Compute a hash-based index so keys map into the table's storage.
    HashTableEntry* entry = ht->buckets[index];  // Access or update the bucket storage used to hold entries or chains.

    while (entry) {  // Repeat while the loop condition remains true.
        if (strcmp(entry->key, key) == 0) {  // Evaluate the condition and branch into the appropriate code path.
            return entry->value;  // Return the computed result to the caller.
        }  // Close the current block scope.
        entry = entry->next;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    return NULL;  // Return the computed result to the caller.
}  // Close the current block scope.

bool ht_delete(HashTable* ht, const char* key) {  // Execute this statement as part of the data structure implementation.
    if (!ht || !key) return false;  // Evaluate the condition and branch into the appropriate code path.

    size_t index = hash(key, ht->capacity);  // Compute a hash-based index so keys map into the table's storage.
    HashTableEntry* entry = ht->buckets[index];  // Access or update the bucket storage used to hold entries or chains.
    HashTableEntry* prev = NULL;  // Assign or update a variable that represents the current algorithm state.

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

bool ht_contains(HashTable* ht, const char* key) {  // Execute this statement as part of the data structure implementation.
    return ht_search(ht, key) != NULL;  // Return the computed result to the caller.
}  // Close the current block scope.

size_t ht_size(HashTable* ht) {  // Execute this statement as part of the data structure implementation.
    return ht ? ht->size : 0;  // Return the computed result to the caller.
}  // Close the current block scope.

size_t ht_capacity(HashTable* ht) {  // Execute this statement as part of the data structure implementation.
    return ht ? ht->capacity : 0;  // Return the computed result to the caller.
}  // Close the current block scope.

bool ht_is_empty(HashTable* ht) {  // Execute this statement as part of the data structure implementation.
    return ht ? ht->size == 0 : true;  // Return the computed result to the caller.
}  // Close the current block scope.

double ht_load_factor(HashTable* ht) {  // Execute this statement as part of the data structure implementation.
    if (!ht || ht->capacity == 0) return 0.0;  // Evaluate the condition and branch into the appropriate code path.
    return (double)ht->size / ht->capacity;  // Return the computed result to the caller.
}  // Close the current block scope.

void ht_clear(HashTable* ht) {  // Execute this statement as part of the data structure implementation.
    if (!ht) return;  // Evaluate the condition and branch into the appropriate code path.

    for (size_t i = 0; i < ht->capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        HashTableEntry* entry = ht->buckets[i];  // Access or update the bucket storage used to hold entries or chains.
        while (entry) {  // Repeat while the loop condition remains true.
            HashTableEntry* next = entry->next;  // Assign or update a variable that represents the current algorithm state.
            free_entry(entry);  // Execute this statement as part of the data structure implementation.
            entry = next;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.
        ht->buckets[i] = NULL;  // Access or update the bucket storage used to hold entries or chains.
    }  // Close the current block scope.
    ht->size = 0;  // Assign or update a variable that represents the current algorithm state.
}  // Close the current block scope.

void ht_print(HashTable* ht) {  // Execute this statement as part of the data structure implementation.
    if (!ht) {  // Evaluate the condition and branch into the appropriate code path.
        printf("HashTable: NULL\n");  // Execute this statement as part of the data structure implementation.
        return;  // Return the computed result to the caller.
    }  // Close the current block scope.

    printf("HashTable (size=%zu, capacity=%zu, load=%.3f):\n",  // Assign or update a variable that represents the current algorithm state.
           ht->size, ht->capacity, ht_load_factor(ht));  // Execute this statement as part of the data structure implementation.

    for (size_t i = 0; i < ht->capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
        HashTableEntry* entry = ht->buckets[i];  // Access or update the bucket storage used to hold entries or chains.
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

// ========== 範例主程式 Example Main ==========

#ifdef HASH_TABLE_MAIN  // Execute this statement as part of the data structure implementation.

int main(void) {  // Execute this statement as part of the data structure implementation.
    printf("=== 雜湊表基本操作示範 Hash Table Basic Operations Demo ===\n\n");  // Execute this statement as part of the data structure implementation.

    // 建立雜湊表 - Create hash table
    HashTable* ht = ht_create(0);  // Assign or update a variable that represents the current algorithm state.
    if (!ht) {  // Evaluate the condition and branch into the appropriate code path.
        fprintf(stderr, "無法建立雜湊表 / Failed to create hash table\n");  // Compute a hash-based index so keys map into the table's storage.
        return 1;  // Return the computed result to the caller.
    }  // Close the current block scope.

    // 插入操作 - Insert operations
    printf("插入鍵值對 Inserting key-value pairs:\n");  // Execute this statement as part of the data structure implementation.
    int values[] = {100, 200, 300, 400};  // Assign or update a variable that represents the current algorithm state.
    ht_insert(ht, "apple", &values[0]);  // Execute this statement as part of the data structure implementation.
    ht_insert(ht, "banana", &values[1]);  // Execute this statement as part of the data structure implementation.
    ht_insert(ht, "cherry", &values[2]);  // Execute this statement as part of the data structure implementation.
    ht_insert(ht, "date", &values[3]);  // Execute this statement as part of the data structure implementation.

    printf("  大小: %zu, 負載因子: %.3f\n\n",  // Execute this statement as part of the data structure implementation.
           ht_size(ht), ht_load_factor(ht));  // Execute this statement as part of the data structure implementation.

    // 搜尋操作 - Search operations
    printf("搜尋操作 Search operations:\n");  // Execute this statement as part of the data structure implementation.
    int* result = (int*)ht_search(ht, "apple");  // Assign or update a variable that represents the current algorithm state.
    printf("  search('apple') = %d\n", result ? *result : -1);  // Assign or update a variable that represents the current algorithm state.

    result = (int*)ht_search(ht, "grape");  // Assign or update a variable that represents the current algorithm state.
    printf("  search('grape') = %s\n", result ? "found" : "NULL");  // Assign or update a variable that represents the current algorithm state.

    printf("  contains('cherry') = %s\n\n",  // Assign or update a variable that represents the current algorithm state.
           ht_contains(ht, "cherry") ? "true" : "false");  // Execute this statement as part of the data structure implementation.

    // 更新操作 - Update operation
    printf("更新操作 Update operation:\n");  // Execute this statement as part of the data structure implementation.
    int new_value = 150;  // Assign or update a variable that represents the current algorithm state.
    ht_insert(ht, "apple", &new_value);  // Execute this statement as part of the data structure implementation.
    result = (int*)ht_search(ht, "apple");  // Assign or update a variable that represents the current algorithm state.
    printf("  更新 apple 的值為 %d\n\n", result ? *result : -1);  // Execute this statement as part of the data structure implementation.

    // 刪除操作 - Delete operation
    printf("刪除操作 Delete operations:\n");  // Execute this statement as part of the data structure implementation.
    ht_delete(ht, "banana");  // Execute this statement as part of the data structure implementation.
    printf("  刪除 'banana' 後:\n");  // Execute this statement as part of the data structure implementation.
    printf("  大小: %zu\n", ht_size(ht));  // Execute this statement as part of the data structure implementation.
    printf("  contains('banana') = %s\n\n",  // Assign or update a variable that represents the current algorithm state.
           ht_contains(ht, "banana") ? "true" : "false");  // Execute this statement as part of the data structure implementation.

    // 列印雜湊表 - Print hash table
    printf("雜湊表內容 Hash table contents:\n");  // Execute this statement as part of the data structure implementation.
    ht_print(ht);  // Execute this statement as part of the data structure implementation.

    // 釋放記憶體 - Free memory
    ht_free(ht);  // Execute this statement as part of the data structure implementation.

    return 0;  // Return the computed result to the caller.
}  // Close the current block scope.

#endif // HASH_TABLE_MAIN
