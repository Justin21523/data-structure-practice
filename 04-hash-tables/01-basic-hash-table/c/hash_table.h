/**
 * 雜湊表（Hash Table）- C 語言實作
 * 使用鏈結法（chaining）處理碰撞
 *
 * Hash Table implementation using chaining for collision resolution
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdbool.h>

// ========== 常數定義 Constants ==========

#define HT_DEFAULT_CAPACITY 16
#define HT_MAX_LOAD_FACTOR 0.75

// ========== 型別定義 Type Definitions ==========

/**
 * 鍵值對節點
 * Key-value pair node for chaining
 */
typedef struct HashTableEntry {
    char* key;                      // 鍵（使用字串）- Key (string)
    void* value;                    // 值（通用指標）- Value (generic pointer)
    struct HashTableEntry* next;    // 下一個節點 - Next node
} HashTableEntry;

/**
 * 雜湊表結構
 * Hash table structure
 */
typedef struct HashTable {
    HashTableEntry** buckets;   // 桶陣列 - Array of buckets
    size_t capacity;            // 桶的數量 - Number of buckets
    size_t size;                // 元素數量 - Number of elements
} HashTable;

// ========== 函式宣告 Function Declarations ==========

/**
 * 建立雜湊表
 * Create a new hash table
 *
 * @param capacity 桶的數量（0 表示使用預設值）
 * @return 新的雜湊表指標，失敗回傳 NULL
 */
HashTable* ht_create(size_t capacity);

/**
 * 釋放雜湊表
 * Free hash table and all entries
 *
 * @param ht 雜湊表指標
 */
void ht_free(HashTable* ht);

/**
 * 插入鍵值對（若 key 已存在則更新）
 * Insert key-value pair (update if key exists)
 *
 * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
 *
 * @param ht 雜湊表指標
 * @param key 鍵（字串，會被複製）
 * @param value 值指標
 * @return 成功回傳 true，失敗回傳 false
 */
bool ht_insert(HashTable* ht, const char* key, void* value);

/**
 * 搜尋給定 key 的 value
 * Search for value associated with key
 *
 * @param ht 雜湊表指標
 * @param key 要搜尋的鍵
 * @return 找到則回傳 value 指標，否則回傳 NULL
 */
void* ht_search(HashTable* ht, const char* key);

/**
 * 刪除指定的鍵值對
 * Delete key-value pair
 *
 * @param ht 雜湊表指標
 * @param key 要刪除的鍵
 * @return 刪除成功回傳 true，key 不存在回傳 false
 */
bool ht_delete(HashTable* ht, const char* key);

/**
 * 檢查 key 是否存在
 * Check if key exists
 *
 * @param ht 雜湊表指標
 * @param key 要檢查的鍵
 * @return 存在回傳 true，否則回傳 false
 */
bool ht_contains(HashTable* ht, const char* key);

/**
 * 回傳元素數量
 * Return number of elements
 */
size_t ht_size(HashTable* ht);

/**
 * 回傳桶的數量
 * Return number of buckets
 */
size_t ht_capacity(HashTable* ht);

/**
 * 檢查是否為空
 * Check if empty
 */
bool ht_is_empty(HashTable* ht);

/**
 * 計算負載因子
 * Calculate load factor
 */
double ht_load_factor(HashTable* ht);

/**
 * 清空雜湊表（不釋放記憶體）
 * Clear all elements (keep memory)
 */
void ht_clear(HashTable* ht);

/**
 * 列印雜湊表（除錯用）
 * Print hash table (for debugging)
 */
void ht_print(HashTable* ht);

#endif // HASH_TABLE_H
