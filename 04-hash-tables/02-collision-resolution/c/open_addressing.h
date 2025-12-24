/** Doc block start
 * 開放定址法雜湊表（Open Addressing Hash Table）- C 語言實作
 * 支援線性探測、平方探測和雙重雜湊 / 支援 linear probing 、平方 probe 和 double hashing
 *(blank line)
 * Open Addressing Hash Table with linear probing, quadratic probing, and double hashing
 */  // End of block comment

#ifndef OPEN_ADDRESSING_H  // Execute this statement as part of the data structure implementation.
#define OPEN_ADDRESSING_H  // Execute this statement as part of the data structure implementation.

#include <stddef.h>  // Execute this statement as part of the data structure implementation.
#include <stdbool.h>  // Execute this statement as part of the data structure implementation.

// ========== 常數定義 Constants ==========

#define OAHT_DEFAULT_CAPACITY 16  // Execute this statement as part of the data structure implementation.
#define OAHT_MAX_LOAD_FACTOR 0.5  // 開放定址法需要較低的負載因子
#define OAHT_TOMBSTONE ((void*)0x1) // 墓碑標記 - Tombstone marker

// ========== 型別定義 Type Definitions ==========

/** Doc block start
 * 探測方法列舉 / Probing method enumeration
 */  // End of block comment
typedef enum ProbeMethod {  // Execute this statement as part of the data structure implementation.
    PROBE_LINEAR,       // 線性探測 - Linear probing
    PROBE_QUADRATIC,    // 平方探測 - Quadratic probing
    PROBE_DOUBLE_HASH   // 雙重雜湊 - Double hashing
} ProbeMethod;  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 鍵值對節點 / Key-value pair entry
 */  // End of block comment
typedef struct OpenAddressingEntry {  // Execute this statement as part of the data structure implementation.
    char* key;      // 鍵（使用字串）- Key (string)
    void* value;    // 值（通用指標）- Value (generic pointer)
                    // 注意：OAHT_TOMBSTONE 表示已刪除
                    // Note: OAHT_TOMBSTONE indicates deleted entry
} OpenAddressingEntry;  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 開放定址法雜湊表結構 / Open addressing hash table structure
 */  // End of block comment
typedef struct OpenAddressingHashTable {  // Execute this statement as part of the data structure implementation.
    OpenAddressingEntry* entries;   // 條目陣列 - Array of entries
    size_t capacity;                // 容量 - Capacity
    size_t size;                    // 元素數量（不含墓碑）- Number of elements (excluding tombstones)
    size_t tombstones;              // 墓碑數量 - Number of tombstones
    ProbeMethod probe_method;       // 探測方法 - Probing method
} OpenAddressingHashTable;  // Execute this statement as part of the data structure implementation.

// ========== 函式宣告 Function Declarations ==========

/** Doc block start
 * 建立開放定址法雜湊表 / Create a new open addressing hash table
 *(blank line)
 * @param capacity 容量（0 表示使用預設值）
 * @param method 探測方法
 * @return 新的雜湊表指標，失敗回傳 NULL
 */  // End of block comment
OpenAddressingHashTable* oaht_create(size_t capacity, ProbeMethod method);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 釋放開放定址法雜湊表 / Free open addressing hash table and all entries
 *(blank line)
 * @param ht 雜湊表指標
 */  // End of block comment
void oaht_free(OpenAddressingHashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 插入鍵值對（若 key 已存在則更新）
 * Insert key-value pair (update if key exists)
 *(blank line)
 * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
 *(blank line)
 * @param ht 雜湊表指標
 * @param key 鍵（字串，會被複製）
 * @param value 值指標（不可為 NULL 或 OAHT_TOMBSTONE）
 * @return 成功回傳 true，失敗回傳 false
 */  // End of block comment
bool oaht_insert(OpenAddressingHashTable* ht, const char* key, void* value);  // Execute this statement as part of the data structure implementation.

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
void* oaht_search(OpenAddressingHashTable* ht, const char* key);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 刪除指定的鍵值對（使用墓碑標記） / Delete key-value pair (using tombstone marker)
 *(blank line)
 * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
 *(blank line)
 * @param ht 雜湊表指標
 * @param key 要刪除的鍵
 * @return 刪除成功回傳 true，key 不存在回傳 false
 */  // End of block comment
bool oaht_delete(OpenAddressingHashTable* ht, const char* key);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 檢查 key 是否存在
 * Check if key exists
 *(blank line)
 * @param ht 雜湊表指標
 * @param key 要檢查的鍵
 * @return 存在回傳 true，否則回傳 false
 */  // End of block comment
bool oaht_contains(OpenAddressingHashTable* ht, const char* key);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 回傳元素數量（不含墓碑） / Return number of elements (excluding tombstones)
 */  // End of block comment
size_t oaht_size(OpenAddressingHashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 回傳容量 / Return capacity
 */  // End of block comment
size_t oaht_capacity(OpenAddressingHashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 回傳墓碑數量 / Return number of tombstones
 */  // End of block comment
size_t oaht_tombstone_count(OpenAddressingHashTable* ht);  // Handle tombstones so deletions do not break the probing/search sequence.

/** Doc block start
 * 檢查是否為空 / Check if empty
 */  // End of block comment
bool oaht_is_empty(OpenAddressingHashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 計算負載因子（包含墓碑） / Calculate load factor (including tombstones)
 */  // End of block comment
double oaht_load_factor(OpenAddressingHashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 清空雜湊表（移除所有元素和墓碑） / Clear all elements and tombstones
 */  // End of block comment
void oaht_clear(OpenAddressingHashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 取得探測方法名稱 / Get probing method name
 *(blank line)
 * @param method 探測方法
 * @return 方法名稱字串
 */  // End of block comment
const char* oaht_probe_method_name(ProbeMethod method);  // Advance or track the probing sequence used by open addressing.

/** Doc block start
 * 列印雜湊表（除錯用） / Print hash table (for debugging)
 */  // End of block comment
void oaht_print(OpenAddressingHashTable* ht);  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 列印統計資訊 / Print statistics
 */  // End of block comment
void oaht_print_stats(OpenAddressingHashTable* ht);  // Execute this statement as part of the data structure implementation.

#endif // OPEN_ADDRESSING_H
