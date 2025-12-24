/** Doc block start
 * 雜湊表開放定址法（Open Addressing）- C++ 實作
 * Hash Table with Open Addressing for Collision Resolution - C++ Implementation
 *(blank line)
 * 支援三種探測方法：線性探測、二次探測、雙重雜湊 / Supports three probing methods: Linear, Quadratic, Double Hashing
 */  // End of block comment

#ifndef OPEN_ADDRESSING_HPP  // Execute this statement as part of the data structure implementation.
#define OPEN_ADDRESSING_HPP  // Execute this statement as part of the data structure implementation.

#include <vector>  // Execute this statement as part of the data structure implementation.
#include <optional>  // Execute this statement as part of the data structure implementation.
#include <functional>  // Execute this statement as part of the data structure implementation.
#include <stdexcept>  // Execute this statement as part of the data structure implementation.
#include <string>  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 探測方法列舉 / Probing method enumeration
 */  // End of block comment
enum class ProbeMethod {  // Execute this statement as part of the data structure implementation.
    LINEAR,       // 線性探測 - Linear probing: h(k, i) = (h(k) + i) % m
    QUADRATIC,    // 二次探測 - Quadratic probing: h(k, i) = (h(k) + c1*i + c2*i²) % m
    DOUBLE_HASH   // 雙重雜湊 - Double hashing: h(k, i) = (h1(k) + i*h2(k)) % m
};  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 開放定址雜湊表模板類別 / Open Addressing Hash Table template class
 *(blank line)
 * @tparam K 鍵的型別（key type）
 * @tparam V 值的型別（value type）
 */  // End of block comment
template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
class OpenAddressingHashTable {  // Execute this statement as part of the data structure implementation.
public:  // Execute this statement as part of the data structure implementation.
    // 型別別名 - Type aliases
    using KeyType = K;  // Assign or update a variable that represents the current algorithm state.
    using ValueType = V;  // Assign or update a variable that represents the current algorithm state.

    /** Doc block start
     * 建構子：初始化雜湊表 / Constructor: Initialize hash table
     *(blank line)
     * @param capacity 表的容量（table capacity）
     * @param method 探測方法（probing method）
     */  // End of block comment
    explicit OpenAddressingHashTable(size_t capacity = DEFAULT_CAPACITY,  // Assign or update a variable that represents the current algorithm state.
                                    ProbeMethod method = ProbeMethod::LINEAR);  // Assign or update a variable that represents the current algorithm state.

    /** Doc block start
     * 解構子 / Destructor
     */  // End of block comment
    ~OpenAddressingHashTable() = default;  // Assign or update a variable that represents the current algorithm state.

    // ========== 基本操作 Basic Operations ==========

    /** Doc block start
     * 插入鍵值對（若 key 已存在則更新）
     * Insert key-value pair (update if key exists)
     *(blank line)
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *(blank line)
     * @param key 鍵
     * @param value 值
     * @return 探測次數 - Number of probes performed
     * @throws std::runtime_error 若表已滿
     */  // End of block comment
    size_t insert(const K& key, const V& value);  // Execute this statement as part of the data structure implementation.

    /** Doc block start
     * 搜尋給定 key 的 value
     * Search for value associated with key
     *(blank line)
     * @param key 要搜尋的鍵
     * @return 找到則回傳 value，否則回傳 std::nullopt
     *         Value if found, std::nullopt otherwise
     */  // End of block comment
    std::optional<V> search(const K& key) const;  // Execute this statement as part of the data structure implementation.

    /** Doc block start
     * 搜尋並回傳探測次數 / Search and return probe count
     *(blank line)
     * @param key 要搜尋的鍵
     * @param probes 輸出參數：探測次數
     * @return 找到則回傳 value，否則回傳 std::nullopt
     */  // End of block comment
    std::optional<V> search(const K& key, size_t& probes) const;  // Advance or track the probing sequence used by open addressing.

    /** Doc block start
     * 刪除指定的鍵值對（使用墓碑標記） / Delete key-value pair (using tombstone marker)
     *(blank line)
     * @param key 要刪除的鍵
     * @return 刪除成功回傳 true，key 不存在回傳 false
     *         true if deleted, false if key not found
     */  // End of block comment
    bool remove(const K& key);  // Execute this statement as part of the data structure implementation.

    /** Doc block start
     * 檢查 key 是否存在
     * Check if key exists
     *(blank line)
     * @param key 要檢查的鍵
     * @return 存在回傳 true，否則回傳 false
     */  // End of block comment
    bool contains(const K& key) const;  // Execute this statement as part of the data structure implementation.

    // ========== 容量操作 Capacity Operations ==========

    /** Doc block start
     * 回傳元素數量（不包括墓碑） / Return number of elements (excluding tombstones)
     */  // End of block comment
    size_t size() const { return size_; }  // Execute this statement as part of the data structure implementation.

    /** Doc block start
     * 回傳表的容量 / Return table capacity
     */  // End of block comment
    size_t capacity() const { return capacity_; }  // Execute this statement as part of the data structure implementation.

    /** Doc block start
     * 檢查是否為空 / Check if empty
     */  // End of block comment
    bool empty() const { return size_ == 0; }  // Execute this statement as part of the data structure implementation.

    /** Doc block start
     * 計算負載因子 α = (n + deleted) / m
     * Calculate load factor including tombstones
     *(blank line)
     * @return 負載因子
     */  // End of block comment
    double loadFactor() const {  // Execute this statement as part of the data structure implementation.
        return static_cast<double>(size_ + deleted_count_) / capacity_;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 清空雜湊表 / Clear all elements
     */  // End of block comment
    void clear();  // Execute this statement as part of the data structure implementation.

    // ========== 統計資訊 Statistics ==========

    /** Doc block start
     * 取得已刪除槽位數量（墓碑數量） / Get number of deleted slots (tombstones)
     */  // End of block comment
    size_t getDeletedCount() const { return deleted_count_; }  // Handle tombstones so deletions do not break the probing/search sequence.

    /** Doc block start
     * 取得總探測次數 / Get total probe count
     */  // End of block comment
    size_t getTotalProbes() const { return total_probes_; }  // Advance or track the probing sequence used by open addressing.

    /** Doc block start
     * 重設探測計數器 / Reset probe counter
     */  // End of block comment
    void resetProbeCount() { total_probes_ = 0; }  // Advance or track the probing sequence used by open addressing.

    /** Doc block start
     * 取得當前探測方法 / Get current probing method
     */  // End of block comment
    ProbeMethod getProbeMethod() const { return method_; }  // Execute this statement as part of the data structure implementation.

private:  // Execute this statement as part of the data structure implementation.
    // ========== 槽位狀態 Slot State ==========
    enum class SlotState {  // Execute this statement as part of the data structure implementation.
        EMPTY,      // 空槽位 - Empty slot
        OCCUPIED,   // 佔用槽位 - Occupied slot
        DELETED     // 已刪除（墓碑）- Deleted (tombstone)
    };  // Execute this statement as part of the data structure implementation.

    /** Doc block start
     * 槽位結構 / Slot structure
     */  // End of block comment
    struct Slot {  // Execute this statement as part of the data structure implementation.
        K key;  // Execute this statement as part of the data structure implementation.
        V value;  // Execute this statement as part of the data structure implementation.
        SlotState state;  // Execute this statement as part of the data structure implementation.

        Slot() : state(SlotState::EMPTY) {}  // Execute this statement as part of the data structure implementation.
    };  // Execute this statement as part of the data structure implementation.

    // ========== 私有成員 Private Members ==========
    std::vector<Slot> table_;      // 雜湊表陣列 - Hash table array
    size_t capacity_;               // 表的容量 - Table capacity
    size_t size_;                   // 元素數量 - Number of elements
    size_t deleted_count_;          // 墓碑數量 - Number of tombstones
    size_t total_probes_;          // 總探測次數 - Total probe count
    ProbeMethod method_;            // 探測方法 - Probing method
    std::hash<K> hasher_;          // 雜湊函數 - Hash function

    // ========== 常數 Constants ==========
    static constexpr size_t DEFAULT_CAPACITY = 16;  // Assign or update a variable that represents the current algorithm state.
    static constexpr double MAX_LOAD_FACTOR = 0.7;  // 開放定址法建議較低的負載因子

    // 二次探測係數 - Quadratic probing coefficients
    static constexpr size_t C1 = 1;  // Assign or update a variable that represents the current algorithm state.
    static constexpr size_t C2 = 3;  // Assign or update a variable that represents the current algorithm state.

    // ========== 私有方法 Private Methods ==========

    /** Doc block start
     * 主雜湊函數 h1(k)
     * Primary hash function
     *(blank line)
     * @param key 要雜湊的鍵
     * @return 雜湊值
     */  // End of block comment
    size_t hash1(const K& key) const {  // Compute a hash-based index so keys map into the table's storage.
        return hasher_(key) % capacity_;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 次雜湊函數 h2(k)（用於雙重雜湊）
     * Secondary hash function (for double hashing)
     *(blank line)
     * @param key 要雜湊的鍵
     * @return 雜湊值（保證為奇數）
     */  // End of block comment
    size_t hash2(const K& key) const {  // Compute a hash-based index so keys map into the table's storage.
        // 使用不同的質數以獲得不同的雜湊值 / Use different prime to get different hash value
        size_t h = hasher_(key) % (capacity_ - 1) + 1;  // Compute a hash-based index so keys map into the table's storage.
        // 確保回傳奇數以避免與偶數表大小產生共因數 / Ensure odd number to avoid common factors with even table sizes
        return (h % 2 == 0) ? h + 1 : h;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 探測函數：根據探測方法計算第 i 次探測的索引
     * Probe function: compute i-th probe index based on probing method
     *(blank line)
     * @param key 鍵
     * @param i 探測次數（0, 1, 2, ...）
     * @return 探測索引
     */  // End of block comment
    size_t probe(const K& key, size_t i) const {  // Advance or track the probing sequence used by open addressing.
        size_t h = hash1(key);  // Compute a hash-based index so keys map into the table's storage.

        switch (method_) {  // Execute this statement as part of the data structure implementation.
            case ProbeMethod::LINEAR:  // Execute this statement as part of the data structure implementation.
                // 線性探測 - Linear probing: h(k, i) = (h(k) + i) % m
                return (h + i) % capacity_;  // Return the computed result to the caller.

            case ProbeMethod::QUADRATIC:  // Execute this statement as part of the data structure implementation.
                // 二次探測 - Quadratic probing: h(k, i) = (h(k) + c1*i + c2*i²) % m
                return (h + C1 * i + C2 * i * i) % capacity_;  // Return the computed result to the caller.

            case ProbeMethod::DOUBLE_HASH:  // Execute this statement as part of the data structure implementation.
                // 雙重雜湊 - Double hashing: h(k, i) = (h1(k) + i*h2(k)) % m
                return (h + i * hash2(key)) % capacity_;  // Return the computed result to the caller.

            default:  // Execute this statement as part of the data structure implementation.
                return h;  // Return the computed result to the caller.
        }  // Close the current block scope.
    }  // Close the current block scope.

    /** Doc block start
     * 尋找鍵的位置 / Find position of key
     *(blank line)
     * @param key 要尋找的鍵
     * @param probes 輸出參數：探測次數
     * @return 若找到回傳索引，否則回傳 std::nullopt
     */  // End of block comment
    std::optional<size_t> findSlot(const K& key, size_t& probes) const {  // Advance or track the probing sequence used by open addressing.
        probes = 0;  // Advance or track the probing sequence used by open addressing.

        // 探測直到找到或遇到空槽位 - Probe until found or empty slot
        for (size_t i = 0; i < capacity_; ++i) {  // Iterate over a range/collection to process each item in sequence.
            size_t index = probe(key, i);  // Advance or track the probing sequence used by open addressing.
            ++probes;  // Advance or track the probing sequence used by open addressing.

            const Slot& slot = table_[index];  // Assign or update a variable that represents the current algorithm state.

            if (slot.state == SlotState::EMPTY) {  // Evaluate the condition and branch into the appropriate code path.
                // 遇到空槽位，鍵不存在 - Empty slot, key doesn't exist
                return std::nullopt;  // Return the computed result to the caller.
            }  // Close the current block scope.

            if (slot.state == SlotState::OCCUPIED && slot.key == key) {  // Evaluate the condition and branch into the appropriate code path.
                // 找到鍵 - Key found
                return index;  // Return the computed result to the caller.
            }  // Close the current block scope.

            // 若是墓碑，繼續探測 - If tombstone, continue probing
        }  // Close the current block scope.

        // 表已滿且未找到 - Table full and not found
        return std::nullopt;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 尋找插入位置 / Find position for insertion
     *(blank line)
     * @param key 要插入的鍵
     * @param probes 輸出參數：探測次數
     * @return 可插入的索引，若表已滿則回傳 std::nullopt
     */  // End of block comment
    std::optional<size_t> findInsertSlot(const K& key, size_t& probes) const {  // Advance or track the probing sequence used by open addressing.
        probes = 0;  // Advance or track the probing sequence used by open addressing.
        std::optional<size_t> first_deleted;  // Handle tombstones so deletions do not break the probing/search sequence.

        // 探測尋找空槽位或現有鍵 - Probe to find empty slot or existing key
        for (size_t i = 0; i < capacity_; ++i) {  // Iterate over a range/collection to process each item in sequence.
            size_t index = probe(key, i);  // Advance or track the probing sequence used by open addressing.
            ++probes;  // Advance or track the probing sequence used by open addressing.

            const Slot& slot = table_[index];  // Assign or update a variable that represents the current algorithm state.

            if (slot.state == SlotState::EMPTY) {  // Evaluate the condition and branch into the appropriate code path.
                // 找到空槽位 - Found empty slot
                // 若之前遇到墓碑，使用墓碑位置；否則使用此位置 / If tombstone seen before, use it; otherwise use this slot
                return first_deleted.has_value() ? first_deleted : index;  // Return the computed result to the caller.
            }  // Close the current block scope.

            if (slot.state == SlotState::DELETED && !first_deleted.has_value()) {  // Evaluate the condition and branch into the appropriate code path.
                // 記錄第一個墓碑位置 - Record first tombstone position
                first_deleted = index;  // Handle tombstones so deletions do not break the probing/search sequence.
            }  // Close the current block scope.

            if (slot.state == SlotState::OCCUPIED && slot.key == key) {  // Evaluate the condition and branch into the appropriate code path.
                // 鍵已存在 - Key already exists
                return index;  // Return the computed result to the caller.
            }  // Close the current block scope.
        }  // Close the current block scope.

        // 表已滿 - Table full
        return first_deleted;  // Return the computed result to the caller.
    }  // Close the current block scope.
};  // Execute this statement as part of the data structure implementation.

// ============================================================
// 實作部分 Implementation
// ============================================================

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
OpenAddressingHashTable<K, V>::OpenAddressingHashTable(size_t capacity, ProbeMethod method)  // Execute this statement as part of the data structure implementation.
    : capacity_(capacity), size_(0), deleted_count_(0), total_probes_(0), method_(method) {  // Handle tombstones so deletions do not break the probing/search sequence.
    if (capacity == 0) {  // Evaluate the condition and branch into the appropriate code path.
        throw std::invalid_argument(  // Throw an exception to signal an invalid argument or operation.
            "容量必須為正整數 / Capacity must be positive");  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.
    table_.resize(capacity_);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
size_t OpenAddressingHashTable<K, V>::insert(const K& key, const V& value) {  // Execute this statement as part of the data structure implementation.
    // 檢查負載因子 - Check load factor
    if (loadFactor() >= MAX_LOAD_FACTOR) {  // Evaluate the condition and branch into the appropriate code path.
        throw std::runtime_error(  // Throw an exception to signal an invalid argument or operation.
            "雜湊表負載過高 / Hash table load factor too high");  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    size_t probes = 0;  // Advance or track the probing sequence used by open addressing.
    auto slot_index = findInsertSlot(key, probes);  // Advance or track the probing sequence used by open addressing.

    if (!slot_index.has_value()) {  // Evaluate the condition and branch into the appropriate code path.
        throw std::runtime_error("雜湊表已滿 / Hash table is full");  // Throw an exception to signal an invalid argument or operation.
    }  // Close the current block scope.

    Slot& slot = table_[slot_index.value()];  // Assign or update a variable that represents the current algorithm state.

    if (slot.state == SlotState::OCCUPIED) {  // Evaluate the condition and branch into the appropriate code path.
        // 更新現有鍵 - Update existing key
        slot.value = value;  // Assign or update a variable that represents the current algorithm state.
    } else {  // Handle the alternative branch when the condition is false.
        // 插入新鍵 - Insert new key
        if (slot.state == SlotState::DELETED) {  // Evaluate the condition and branch into the appropriate code path.
            --deleted_count_;  // Handle tombstones so deletions do not break the probing/search sequence.
        }  // Close the current block scope.
        slot.key = key;  // Assign or update a variable that represents the current algorithm state.
        slot.value = value;  // Assign or update a variable that represents the current algorithm state.
        slot.state = SlotState::OCCUPIED;  // Assign or update a variable that represents the current algorithm state.
        ++size_;  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    total_probes_ += probes;  // Advance or track the probing sequence used by open addressing.
    return probes;  // Return the computed result to the caller.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
std::optional<V> OpenAddressingHashTable<K, V>::search(const K& key) const {  // Execute this statement as part of the data structure implementation.
    size_t probes;  // Advance or track the probing sequence used by open addressing.
    return search(key, probes);  // Return the computed result to the caller.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
std::optional<V> OpenAddressingHashTable<K, V>::search(const K& key, size_t& probes) const {  // Advance or track the probing sequence used by open addressing.
    auto slot_index = findSlot(key, probes);  // Advance or track the probing sequence used by open addressing.

    if (slot_index.has_value()) {  // Evaluate the condition and branch into the appropriate code path.
        return table_[slot_index.value()].value;  // Return the computed result to the caller.
    }  // Close the current block scope.

    return std::nullopt;  // Return the computed result to the caller.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
bool OpenAddressingHashTable<K, V>::remove(const K& key) {  // Execute this statement as part of the data structure implementation.
    size_t probes = 0;  // Advance or track the probing sequence used by open addressing.
    auto slot_index = findSlot(key, probes);  // Advance or track the probing sequence used by open addressing.

    if (!slot_index.has_value()) {  // Evaluate the condition and branch into the appropriate code path.
        return false;  // Return the computed result to the caller.
    }  // Close the current block scope.

    // 使用墓碑標記刪除 - Delete using tombstone marker
    Slot& slot = table_[slot_index.value()];  // Assign or update a variable that represents the current algorithm state.
    slot.state = SlotState::DELETED;  // Handle tombstones so deletions do not break the probing/search sequence.
    --size_;  // Execute this statement as part of the data structure implementation.
    ++deleted_count_;  // Handle tombstones so deletions do not break the probing/search sequence.

    return true;  // Return the computed result to the caller.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
bool OpenAddressingHashTable<K, V>::contains(const K& key) const {  // Execute this statement as part of the data structure implementation.
    return search(key).has_value();  // Return the computed result to the caller.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
void OpenAddressingHashTable<K, V>::clear() {  // Execute this statement as part of the data structure implementation.
    for (auto& slot : table_) {  // Iterate over a range/collection to process each item in sequence.
        slot.state = SlotState::EMPTY;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.
    size_ = 0;  // Assign or update a variable that represents the current algorithm state.
    deleted_count_ = 0;  // Handle tombstones so deletions do not break the probing/search sequence.
    total_probes_ = 0;  // Advance or track the probing sequence used by open addressing.
}  // Close the current block scope.

#endif // OPEN_ADDRESSING_HPP
