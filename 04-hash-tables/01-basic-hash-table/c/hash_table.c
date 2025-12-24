/**
 * 雜湊表（Hash Table）- C 語言實作
 * 使用鏈結法（chaining）處理碰撞
 *
 * Hash Table implementation using chaining for collision resolution
 */

#include "hash_table.h"
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
static HashTableEntry* create_entry(const char* key, void* value) {
    HashTableEntry* entry = malloc(sizeof(HashTableEntry));
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
static void free_entry(HashTableEntry* entry) {
    if (entry) {
        free(entry->key);
        // 注意：不釋放 value，由呼叫者負責
        // Note: value is not freed, caller's responsibility
        free(entry);
    }
}

/**
 * 擴容並重新雜湊
 * Resize and rehash
 */
static bool rehash(HashTable* ht) {
    size_t old_capacity = ht->capacity;
    HashTableEntry** old_buckets = ht->buckets;

    // 容量加倍 - Double capacity
    size_t new_capacity = old_capacity * 2;
    HashTableEntry** new_buckets = calloc(new_capacity, sizeof(HashTableEntry*));
    if (!new_buckets) return false;

    // 更新雜湊表 - Update hash table
    ht->buckets = new_buckets;
    ht->capacity = new_capacity;
    ht->size = 0;

    // 重新插入所有元素 - Reinsert all elements
    for (size_t i = 0; i < old_capacity; i++) {
        HashTableEntry* entry = old_buckets[i];
        while (entry) {
            HashTableEntry* next = entry->next;
            ht_insert(ht, entry->key, entry->value);
            free(entry->key);
            free(entry);
            entry = next;
        }
    }

    free(old_buckets);
    return true;
}

// ========== 公開函式 Public Functions ==========

HashTable* ht_create(size_t capacity) {
    if (capacity == 0) {
        capacity = HT_DEFAULT_CAPACITY;
    }

    HashTable* ht = malloc(sizeof(HashTable));
    if (!ht) return NULL;

    ht->buckets = calloc(capacity, sizeof(HashTableEntry*));
    if (!ht->buckets) {
        free(ht);
        return NULL;
    }

    ht->capacity = capacity;
    ht->size = 0;
    return ht;
}

void ht_free(HashTable* ht) {
    if (!ht) return;

    // 釋放所有節點 - Free all entries
    for (size_t i = 0; i < ht->capacity; i++) {
        HashTableEntry* entry = ht->buckets[i];
        while (entry) {
            HashTableEntry* next = entry->next;
            free_entry(entry);
            entry = next;
        }
    }

    free(ht->buckets);
    free(ht);
}

bool ht_insert(HashTable* ht, const char* key, void* value) {
    if (!ht || !key) return false;

    size_t index = hash(key, ht->capacity);
    HashTableEntry* entry = ht->buckets[index];

    // 檢查 key 是否已存在 - Check if key exists
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;  // 更新 - Update existing
            return true;
        }
        entry = entry->next;
    }

    // 建立新節點 - Create new entry
    HashTableEntry* new_entry = create_entry(key, value);
    if (!new_entry) return false;

    // 插入到鏈結串列頭部 - Insert at head
    new_entry->next = ht->buckets[index];
    ht->buckets[index] = new_entry;
    ht->size++;

    // 檢查是否需要擴容 - Check if rehashing needed
    if (ht_load_factor(ht) > HT_MAX_LOAD_FACTOR) {
        rehash(ht);
    }

    return true;
}

void* ht_search(HashTable* ht, const char* key) {
    if (!ht || !key) return NULL;

    size_t index = hash(key, ht->capacity);
    HashTableEntry* entry = ht->buckets[index];

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL;
}

bool ht_delete(HashTable* ht, const char* key) {
    if (!ht || !key) return false;

    size_t index = hash(key, ht->capacity);
    HashTableEntry* entry = ht->buckets[index];
    HashTableEntry* prev = NULL;

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

bool ht_contains(HashTable* ht, const char* key) {
    return ht_search(ht, key) != NULL;
}

size_t ht_size(HashTable* ht) {
    return ht ? ht->size : 0;
}

size_t ht_capacity(HashTable* ht) {
    return ht ? ht->capacity : 0;
}

bool ht_is_empty(HashTable* ht) {
    return ht ? ht->size == 0 : true;
}

double ht_load_factor(HashTable* ht) {
    if (!ht || ht->capacity == 0) return 0.0;
    return (double)ht->size / ht->capacity;
}

void ht_clear(HashTable* ht) {
    if (!ht) return;

    for (size_t i = 0; i < ht->capacity; i++) {
        HashTableEntry* entry = ht->buckets[i];
        while (entry) {
            HashTableEntry* next = entry->next;
            free_entry(entry);
            entry = next;
        }
        ht->buckets[i] = NULL;
    }
    ht->size = 0;
}

void ht_print(HashTable* ht) {
    if (!ht) {
        printf("HashTable: NULL\n");
        return;
    }

    printf("HashTable (size=%zu, capacity=%zu, load=%.3f):\n",
           ht->size, ht->capacity, ht_load_factor(ht));

    for (size_t i = 0; i < ht->capacity; i++) {
        HashTableEntry* entry = ht->buckets[i];
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

// ========== 範例主程式 Example Main ==========

#ifdef HASH_TABLE_MAIN

int main(void) {
    printf("=== 雜湊表基本操作示範 Hash Table Basic Operations Demo ===\n\n");

    // 建立雜湊表 - Create hash table
    HashTable* ht = ht_create(0);
    if (!ht) {
        fprintf(stderr, "無法建立雜湊表 / Failed to create hash table\n");
        return 1;
    }

    // 插入操作 - Insert operations
    printf("插入鍵值對 Inserting key-value pairs:\n");
    int values[] = {100, 200, 300, 400};
    ht_insert(ht, "apple", &values[0]);
    ht_insert(ht, "banana", &values[1]);
    ht_insert(ht, "cherry", &values[2]);
    ht_insert(ht, "date", &values[3]);

    printf("  大小: %zu, 負載因子: %.3f\n\n",
           ht_size(ht), ht_load_factor(ht));

    // 搜尋操作 - Search operations
    printf("搜尋操作 Search operations:\n");
    int* result = (int*)ht_search(ht, "apple");
    printf("  search('apple') = %d\n", result ? *result : -1);

    result = (int*)ht_search(ht, "grape");
    printf("  search('grape') = %s\n", result ? "found" : "NULL");

    printf("  contains('cherry') = %s\n\n",
           ht_contains(ht, "cherry") ? "true" : "false");

    // 更新操作 - Update operation
    printf("更新操作 Update operation:\n");
    int new_value = 150;
    ht_insert(ht, "apple", &new_value);
    result = (int*)ht_search(ht, "apple");
    printf("  更新 apple 的值為 %d\n\n", result ? *result : -1);

    // 刪除操作 - Delete operation
    printf("刪除操作 Delete operations:\n");
    ht_delete(ht, "banana");
    printf("  刪除 'banana' 後:\n");
    printf("  大小: %zu\n", ht_size(ht));
    printf("  contains('banana') = %s\n\n",
           ht_contains(ht, "banana") ? "true" : "false");

    // 列印雜湊表 - Print hash table
    printf("雜湊表內容 Hash table contents:\n");
    ht_print(ht);

    // 釋放記憶體 - Free memory
    ht_free(ht);

    return 0;
}

#endif // HASH_TABLE_MAIN
