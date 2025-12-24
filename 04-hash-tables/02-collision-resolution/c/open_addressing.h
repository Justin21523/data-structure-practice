/**
 * 開放定址法雜湊表（Open Addressing Hash Table）- C 語言實作
 * 支援線性探測、平方探測和雙重雜湊
 *
 * Open Addressing Hash Table with linear probing, quadratic probing, and double hashing
 */

#ifndef OPEN_ADDRESSING_H
#define OPEN_ADDRESSING_H

#include <stddef.h>
#include <stdbool.h>

// ========== 常數定義 Constants ==========

#define OAHT_DEFAULT_CAPACITY 16
#define OAHT_MAX_LOAD_FACTOR 0.5  // 開放定址法需要較低的負載因子
#define OAHT_TOMBSTONE ((void*)0x1) // 墓碑標記 - Tombstone marker

// ========== 型別定義 Type Definitions ==========

/**
 * 探測方法列舉
 * Probing method enumeration
 */
typedef enum ProbeMethod {
    PROBE_LINEAR,       // 線性探測 - Linear probing
    PROBE_QUADRATIC,    // 平方探測 - Quadratic probing
    PROBE_DOUBLE_HASH   // 雙重雜湊 - Double hashing
} ProbeMethod;

/**
 * 鍵值對節點
 * Key-value pair entry
 */
typedef struct OpenAddressingEntry {
    char* key;      // 鍵（使用字串）- Key (string)
    void* value;    // 值（通用指標）- Value (generic pointer)
                    // 注意：OAHT_TOMBSTONE 表示已刪除
                    // Note: OAHT_TOMBSTONE indicates deleted entry
} OpenAddressingEntry;

/**
 * 開放定址法雜湊表結構
 * Open addressing hash table structure
 */
typedef struct OpenAddressingHashTable {
    OpenAddressingEntry* entries;   // 條目陣列 - Array of entries
    size_t capacity;                // 容量 - Capacity
    size_t size;                    // 元素數量（不含墓碑）- Number of elements (excluding tombstones)
    size_t tombstones;              // 墓碑數量 - Number of tombstones
    ProbeMethod probe_method;       // 探測方法 - Probing method
} OpenAddressingHashTable;

// ========== 函式宣告 Function Declarations ==========

/**
 * 建立開放定址法雜湊表
 * Create a new open addressing hash table
 *
 * @param capacity 容量（0 表示使用預設值）
 * @param method 探測方法
 * @return 新的雜湊表指標，失敗回傳 NULL
 */
OpenAddressingHashTable* oaht_create(size_t capacity, ProbeMethod method);

/**
 * 釋放開放定址法雜湊表
 * Free open addressing hash table and all entries
 *
 * @param ht 雜湊表指標
 */
void oaht_free(OpenAddressingHashTable* ht);

/**
 * 插入鍵值對（若 key 已存在則更新）
 * Insert key-value pair (update if key exists)
 *
 * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
 *
 * @param ht 雜湊表指標
 * @param key 鍵（字串，會被複製）
 * @param value 值指標（不可為 NULL 或 OAHT_TOMBSTONE）
 * @return 成功回傳 true，失敗回傳 false
 */
bool oaht_insert(OpenAddressingHashTable* ht, const char* key, void* value);

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
void* oaht_search(OpenAddressingHashTable* ht, const char* key);

/**
 * 刪除指定的鍵值對（使用墓碑標記）
 * Delete key-value pair (using tombstone marker)
 *
 * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
 *
 * @param ht 雜湊表指標
 * @param key 要刪除的鍵
 * @return 刪除成功回傳 true，key 不存在回傳 false
 */
bool oaht_delete(OpenAddressingHashTable* ht, const char* key);

/**
 * 檢查 key 是否存在
 * Check if key exists
 *
 * @param ht 雜湊表指標
 * @param key 要檢查的鍵
 * @return 存在回傳 true，否則回傳 false
 */
bool oaht_contains(OpenAddressingHashTable* ht, const char* key);

/**
 * 回傳元素數量（不含墓碑）
 * Return number of elements (excluding tombstones)
 */
size_t oaht_size(OpenAddressingHashTable* ht);

/**
 * 回傳容量
 * Return capacity
 */
size_t oaht_capacity(OpenAddressingHashTable* ht);

/**
 * 回傳墓碑數量
 * Return number of tombstones
 */
size_t oaht_tombstone_count(OpenAddressingHashTable* ht);

/**
 * 檢查是否為空
 * Check if empty
 */
bool oaht_is_empty(OpenAddressingHashTable* ht);

/**
 * 計算負載因子（包含墓碑）
 * Calculate load factor (including tombstones)
 */
double oaht_load_factor(OpenAddressingHashTable* ht);

/**
 * 清空雜湊表（移除所有元素和墓碑）
 * Clear all elements and tombstones
 */
void oaht_clear(OpenAddressingHashTable* ht);

/**
 * 取得探測方法名稱
 * Get probing method name
 *
 * @param method 探測方法
 * @return 方法名稱字串
 */
const char* oaht_probe_method_name(ProbeMethod method);

/**
 * 列印雜湊表（除錯用）
 * Print hash table (for debugging)
 */
void oaht_print(OpenAddressingHashTable* ht);

/**
 * 列印統計資訊
 * Print statistics
 */
void oaht_print_stats(OpenAddressingHashTable* ht);

#endif // OPEN_ADDRESSING_H
