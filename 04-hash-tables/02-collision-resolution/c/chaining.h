/** Doc block start
 * 鏈結法雜湊表（Chaining Hash Table）- C 語言實作
 * 使用鏈結串列處理碰撞，包含鏈長統計 / Use linked list 處理 collision ，包含鏈長 count
 *(blank line)
 * Chaining Hash Table implementation with chain length statistics
 */  // End of block comment

#ifndef CHAINING_H  // Execute this statement as part of the data structure implementation.
#define CHAINING_H  // Execute this statement as part of the data structure implementation.

#include <stddef.h>  // Execute this statement as part of the data structure implementation.
#include <stdbool.h>  // Execute this statement as part of the data structure implementation.

// ========== 常數定義 Constants ==========

#define CHT_DEFAULT_CAPACITY 16  // Execute this statement as part of the data structure implementation.
#define CHT_MAX_LOAD_FACTOR 0.75  // Execute this statement as part of the data structure implementation.

// ========== 型別定義 Type Definitions ==========

/** Doc block start
 * 鍵值對節點 / Key-value pair node for chaining
 */  // End of block comment
typedef struct ChainingEntry {  // Execute this statement as part of the data structure implementation.
    char* key;                      // 鍵（使用字串）- Key (string)
    void* value;                    // 值（通用指標）- Value (generic pointer)
    struct ChainingEntry* next;     // 下一個節點 - Next node
} ChainingEntry;  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 鏈結法雜湊表結構 / Chaining hash table structure
 */  // End of block comment
typedef struct ChainingHashTable {  // Execute this statement as part of the data structure implementation.
    ChainingEntry** buckets;    // 桶陣列 - Array of buckets
    size_t capacity;            // 桶的數量 - Number of buckets
    size_t size;                // 元素數量 - Number of elements
} ChainingHashTable;  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 鏈長統計資訊 / Chain length statistics
 */  // End of block comment
typedef struct ChainStats {  // Execute this statement as part of the data structure implementation.
    size_t min_length;      // 最短鏈長 - Minimum chain length
    size_t max_length;      // 最長鏈長 - Maximum chain length
    double avg_length;      // 平均鏈長 - Average chain length
    size_t empty_buckets;   // 空桶數量 - Number of empty buckets
} ChainStats;  // Execute this statement as part of the data structure implementation.

// ========== 函式宣告 Function Declarations ==========

/** Doc block start
 * 建立鏈結法雜湊表 / Create a new chaining hash table
 *(blank line)
 * @param capacity 桶的數量（0 表示使用預設值）
 * @return 新的雜湊表指標，失敗回傳 NULL
 */  // End of block comment
ChainingHashTable* cht_create(size_t capacity);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 釋放鏈結法雜湊表 / Free chaining hash table and all entries
 *(blank line)
 * @param ht 雜湊表指標
 */  // End of block comment
void cht_free(ChainingHashTable* ht);  // Execute this statement as part of the data structure implementation.

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
bool cht_insert(ChainingHashTable* ht, const char* key, void* value);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 搜尋給定 key 的 value
 * Search for value associated with key
 *(blank line)
 * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
 *(blank line)
 * @param ht 雜湊表指標
 * @param key 要搜尋的鍵
 * @return 找到則回傳 value 指標，否則回傳 NULL
 */  // End of block comment
void* cht_search(ChainingHashTable* ht, const char* key);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 刪除指定的鍵值對 / Delete key-value pair
 *(blank line)
 * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
 *(blank line)
 * @param ht 雜湊表指標
 * @param key 要刪除的鍵
 * @return 刪除成功回傳 true，key 不存在回傳 false
 */  // End of block comment
bool cht_delete(ChainingHashTable* ht, const char* key);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 檢查 key 是否存在
 * Check if key exists
 *(blank line)
 * @param ht 雜湊表指標
 * @param key 要檢查的鍵
 * @return 存在回傳 true，否則回傳 false
 */  // End of block comment
bool cht_contains(ChainingHashTable* ht, const char* key);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 回傳元素數量 / Return number of elements
 */  // End of block comment
size_t cht_size(ChainingHashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 回傳桶的數量 / Return number of buckets
 */  // End of block comment
size_t cht_capacity(ChainingHashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 檢查是否為空 / Check if empty
 */  // End of block comment
bool cht_is_empty(ChainingHashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 計算負載因子 / Calculate load factor
 */  // End of block comment
double cht_load_factor(ChainingHashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 清空雜湊表（不釋放記憶體） / Clear all elements (keep memory)
 */  // End of block comment
void cht_clear(ChainingHashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 取得鏈長統計資訊 / Get chain length statistics
 *(blank line)
 * @param ht 雜湊表指標
 * @param stats 用於儲存統計資訊的結構
 */  // End of block comment
void cht_get_stats(ChainingHashTable* ht, ChainStats* stats);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 列印鏈長統計 / Print chain length statistics
 *(blank line)
 * @param ht 雜湊表指標
 */  // End of block comment
void cht_print_stats(ChainingHashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 列印雜湊表（除錯用） / Print hash table (for debugging)
 */  // End of block comment
void cht_print(ChainingHashTable* ht);  // Execute this statement as part of the data structure implementation.

#endif // CHAINING_H
