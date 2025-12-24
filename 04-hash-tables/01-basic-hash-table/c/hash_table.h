/** Doc block start
 * 雜湊表（Hash Table）- C 語言實作
 * 使用鏈結法（chaining）處理碰撞
 *(blank line)
 * Hash Table implementation using chaining for collision resolution
 */  // End of block comment

#ifndef HASH_TABLE_H  // Execute this statement as part of the data structure implementation.
#define HASH_TABLE_H  // Execute this statement as part of the data structure implementation.

#include <stddef.h>  // Execute this statement as part of the data structure implementation.
#include <stdbool.h>  // Execute this statement as part of the data structure implementation.

// ========== 常數定義 Constants ==========

#define HT_DEFAULT_CAPACITY 16  // Execute this statement as part of the data structure implementation.
#define HT_MAX_LOAD_FACTOR 0.75  // Execute this statement as part of the data structure implementation.

// ========== 型別定義 Type Definitions ==========

/** Doc block start
 * 鍵值對節點 / Key-value pair node for chaining
 */  // End of block comment
typedef struct HashTableEntry {  // Execute this statement as part of the data structure implementation.
    char* key;                      // 鍵（使用字串）- Key (string)
    void* value;                    // 值（通用指標）- Value (generic pointer)
    struct HashTableEntry* next;    // 下一個節點 - Next node
} HashTableEntry;  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 雜湊表結構 / Hash table structure
 */  // End of block comment
typedef struct HashTable {  // Execute this statement as part of the data structure implementation.
    HashTableEntry** buckets;   // 桶陣列 - Array of buckets
    size_t capacity;            // 桶的數量 - Number of buckets
    size_t size;                // 元素數量 - Number of elements
} HashTable;  // Execute this statement as part of the data structure implementation.

// ========== 函式宣告 Function Declarations ==========

/** Doc block start
 * 建立雜湊表 / Create a new hash table
 *(blank line)
 * @param capacity 桶的數量（0 表示使用預設值）
 * @return 新的雜湊表指標，失敗回傳 NULL
 */  // End of block comment
HashTable* ht_create(size_t capacity);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 釋放雜湊表 / Free hash table and all entries
 *(blank line)
 * @param ht 雜湊表指標
 */  // End of block comment
void ht_free(HashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 插入鍵值對（若 key 已存在則更新）
 * Insert key-value pair (update if key exists)
 *(blank line)
 * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
 *(blank line)
 * @param ht 雜湊表指標
 * @param key 鍵（字串，會被複製）
 * @param value 值指標
 * @return 成功回傳 true，失敗回傳 false
 */  // End of block comment
bool ht_insert(HashTable* ht, const char* key, void* value);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 搜尋給定 key 的 value
 * Search for value associated with key
 *(blank line)
 * @param ht 雜湊表指標
 * @param key 要搜尋的鍵
 * @return 找到則回傳 value 指標，否則回傳 NULL
 */  // End of block comment
void* ht_search(HashTable* ht, const char* key);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 刪除指定的鍵值對 / Delete key-value pair
 *(blank line)
 * @param ht 雜湊表指標
 * @param key 要刪除的鍵
 * @return 刪除成功回傳 true，key 不存在回傳 false
 */  // End of block comment
bool ht_delete(HashTable* ht, const char* key);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 檢查 key 是否存在
 * Check if key exists
 *(blank line)
 * @param ht 雜湊表指標
 * @param key 要檢查的鍵
 * @return 存在回傳 true，否則回傳 false
 */  // End of block comment
bool ht_contains(HashTable* ht, const char* key);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 回傳元素數量 / Return number of elements
 */  // End of block comment
size_t ht_size(HashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 回傳桶的數量 / Return number of buckets
 */  // End of block comment
size_t ht_capacity(HashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 檢查是否為空 / Check if empty
 */  // End of block comment
bool ht_is_empty(HashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 計算負載因子 / Calculate load factor
 */  // End of block comment
double ht_load_factor(HashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 清空雜湊表（不釋放記憶體） / Clear all elements (keep memory)
 */  // End of block comment
void ht_clear(HashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 列印雜湊表（除錯用） / Print hash table (for debugging)
 */  // End of block comment
void ht_print(HashTable* ht);  // Execute this statement as part of the data structure implementation.

#endif // HASH_TABLE_H
