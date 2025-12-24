/**
 * 鏈結法雜湊表（Chaining Hash Table）- C 語言實作
 * 使用鏈結串列處理碰撞，包含鏈長統計
 *
 * Chaining Hash Table implementation with chain length statistics
 */

#ifndef CHAINING_H
#define CHAINING_H

#include <stddef.h>
#include <stdbool.h>

// ========== 常數定義 Constants ==========

#define CHT_DEFAULT_CAPACITY 16
#define CHT_MAX_LOAD_FACTOR 0.75

// ========== 型別定義 Type Definitions ==========

/**
 * 鍵值對節點
 * Key-value pair node for chaining
 */
typedef struct ChainingEntry {
    char* key;                      // 鍵（使用字串）- Key (string)
    void* value;                    // 值（通用指標）- Value (generic pointer)
    struct ChainingEntry* next;     // 下一個節點 - Next node
} ChainingEntry;

/**
 * 鏈結法雜湊表結構
 * Chaining hash table structure
 */
typedef struct ChainingHashTable {
    ChainingEntry** buckets;    // 桶陣列 - Array of buckets
    size_t capacity;            // 桶的數量 - Number of buckets
    size_t size;                // 元素數量 - Number of elements
} ChainingHashTable;

/**
 * 鏈長統計資訊
 * Chain length statistics
 */
typedef struct ChainStats {
    size_t min_length;      // 最短鏈長 - Minimum chain length
    size_t max_length;      // 最長鏈長 - Maximum chain length
    double avg_length;      // 平均鏈長 - Average chain length
    size_t empty_buckets;   // 空桶數量 - Number of empty buckets
} ChainStats;

// ========== 函式宣告 Function Declarations ==========

/**
 * 建立鏈結法雜湊表
 * Create a new chaining hash table
 *
 * @param capacity 桶的數量（0 表示使用預設值）
 * @return 新的雜湊表指標，失敗回傳 NULL
 */
ChainingHashTable* cht_create(size_t capacity);

/**
 * 釋放鏈結法雜湊表
 * Free chaining hash table and all entries
 *
 * @param ht 雜湊表指標
 */
void cht_free(ChainingHashTable* ht);

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
bool cht_insert(ChainingHashTable* ht, const char* key, void* value);

/**
 * 搜尋給定 key 的 value
 * Search for value associated with key
 *
 * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
 *
 * @param ht 雜湊表指標
 * @param key 要搜尋的鍵
 * @return 找到則回傳 value 指標，否則回傳 NULL
 */
void* cht_search(ChainingHashTable* ht, const char* key);

/**
 * 刪除指定的鍵值對
 * Delete key-value pair
 *
 * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
 *
 * @param ht 雜湊表指標
 * @param key 要刪除的鍵
 * @return 刪除成功回傳 true，key 不存在回傳 false
 */
bool cht_delete(ChainingHashTable* ht, const char* key);

/**
 * 檢查 key 是否存在
 * Check if key exists
 *
 * @param ht 雜湊表指標
 * @param key 要檢查的鍵
 * @return 存在回傳 true，否則回傳 false
 */
bool cht_contains(ChainingHashTable* ht, const char* key);

/**
 * 回傳元素數量
 * Return number of elements
 */
size_t cht_size(ChainingHashTable* ht);

/**
 * 回傳桶的數量
 * Return number of buckets
 */
size_t cht_capacity(ChainingHashTable* ht);

/**
 * 檢查是否為空
 * Check if empty
 */
bool cht_is_empty(ChainingHashTable* ht);

/**
 * 計算負載因子
 * Calculate load factor
 */
double cht_load_factor(ChainingHashTable* ht);

/**
 * 清空雜湊表（不釋放記憶體）
 * Clear all elements (keep memory)
 */
void cht_clear(ChainingHashTable* ht);

/**
 * 取得鏈長統計資訊
 * Get chain length statistics
 *
 * @param ht 雜湊表指標
 * @param stats 用於儲存統計資訊的結構
 */
void cht_get_stats(ChainingHashTable* ht, ChainStats* stats);

/**
 * 列印鏈長統計
 * Print chain length statistics
 *
 * @param ht 雜湊表指標
 */
void cht_print_stats(ChainingHashTable* ht);

/**
 * 列印雜湊表（除錯用）
 * Print hash table (for debugging)
 */
void cht_print(ChainingHashTable* ht);

#endif // CHAINING_H
