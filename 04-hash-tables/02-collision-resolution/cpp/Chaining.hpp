/** Doc block start
 * 雜湊表鏈結法（Chaining）- C++ 實作
 * Hash Table with Chaining for Collision Resolution - C++ Implementation
 *(blank line)
 * 使用鏈結串列處理碰撞，追蹤探測次數統計 / Uses linked lists for collision handling with probe count tracking
 */  // End of block comment

#ifndef CHAINING_HPP  // Execute this statement as part of the data structure implementation.
#define CHAINING_HPP  // Execute this statement as part of the data structure implementation.

#include <vector>  // Execute this statement as part of the data structure implementation.
#include <list>  // Execute this statement as part of the data structure implementation.
#include <utility>  // Execute this statement as part of the data structure implementation.
#include <functional>  // Execute this statement as part of the data structure implementation.
#include <optional>  // Execute this statement as part of the data structure implementation.
#include <stdexcept>  // Execute this statement as part of the data structure implementation.
#include <string>  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 鏈結雜湊表模板類別 / Chained Hash Table template class
 *(blank line)
 * @tparam K 鍵的型別（key type）
 * @tparam V 值的型別（value type）
 */  // End of block comment
template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
class ChainedHashTable {  // Execute this statement as part of the data structure implementation.
public:  // Execute this statement as part of the data structure implementation.
    // 型別別名 - Type aliases
    using KeyType = K;  // Assign or update a variable that represents the current algorithm state.
    using ValueType = V;  // Assign or update a variable that represents the current algorithm state.
    using PairType = std::pair<K, V>;  // Assign or update a variable that represents the current algorithm state.

    /** Doc block start
     * 建構子：初始化雜湊表 / Constructor: Initialize hash table
     *(blank line)
     * @param capacity 桶的數量（number of buckets）
     */  // End of block comment
    explicit ChainedHashTable(size_t capacity = DEFAULT_CAPACITY);  // Assign or update a variable that represents the current algorithm state.

    /** Doc block start
     * 解構子 / Destructor
     */  // End of block comment
    ~ChainedHashTable() = default;  // Assign or update a variable that represents the current algorithm state.

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
     * 刪除指定的鍵值對 / Delete key-value pair
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
     * 回傳元素數量 / Return number of elements
     */  // End of block comment
    size_t size() const { return size_; }  // Execute this statement as part of the data structure implementation.

    /** Doc block start
     * 回傳桶的數量 / Return number of buckets
     */  // End of block comment
    size_t capacity() const { return capacity_; }  // Execute this statement as part of the data structure implementation.

    /** Doc block start
     * 檢查是否為空 / Check if empty
     */  // End of block comment
    bool empty() const { return size_ == 0; }  // Execute this statement as part of the data structure implementation.

    /** Doc block start
     * 計算負載因子 α = n / m
     * Calculate load factor (α = n / m)
     *(blank line)
     * @return 負載因子
     */  // End of block comment
    double loadFactor() const {  // Execute this statement as part of the data structure implementation.
        return static_cast<double>(size_) / capacity_;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 清空雜湊表 / Clear all elements
     */  // End of block comment
    void clear();  // Execute this statement as part of the data structure implementation.

    // ========== 統計資訊 Statistics ==========

    /** Doc block start
     * 取得最長鏈結長度 / Get maximum chain length
     */  // End of block comment
    size_t getMaxChainLength() const;  // Execute this statement as part of the data structure implementation.

    /** Doc block start
     * 取得平均鏈結長度 / Get average chain length
     */  // End of block comment
    double getAverageChainLength() const;  // Execute this statement as part of the data structure implementation.

    /** Doc block start
     * 取得總探測次數 / Get total probe count
     */  // End of block comment
    size_t getTotalProbes() const { return total_probes_; }  // Advance or track the probing sequence used by open addressing.

    /** Doc block start
     * 重設探測計數器 / Reset probe counter
     */  // End of block comment
    void resetProbeCount() { total_probes_ = 0; }  // Advance or track the probing sequence used by open addressing.

private:  // Execute this statement as part of the data structure implementation.
    // 桶的型別：每個桶是一個鏈結串列 / Bucket type: each bucket is a linked list
    using Bucket = std::list<PairType>;  // Assign or update a variable that represents the current algorithm state.

    // ========== 私有成員 Private Members ==========
    std::vector<Bucket> buckets_;  // 桶陣列 - Array of buckets
    size_t capacity_;               // 桶的數量 - Number of buckets
    size_t size_;                   // 元素數量 - Number of elements
    size_t total_probes_;          // 總探測次數 - Total probe count
    std::hash<K> hasher_;          // 雜湊函數 - Hash function

    // ========== 常數 Constants ==========
    static constexpr size_t DEFAULT_CAPACITY = 16;  // Assign or update a variable that represents the current algorithm state.

    // ========== 私有方法 Private Methods ==========

    /** Doc block start
     * 計算雜湊索引 / Compute hash index
     *(blank line)
     * @param key 要雜湊的鍵
     * @return 桶的索引
     */  // End of block comment
    size_t hash(const K& key) const {  // Compute a hash-based index so keys map into the table's storage.
        return hasher_(key) % capacity_;  // Return the computed result to the caller.
    }  // Close the current block scope.
};  // Execute this statement as part of the data structure implementation.

// ============================================================
// 實作部分 Implementation
// ============================================================

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
ChainedHashTable<K, V>::ChainedHashTable(size_t capacity)  // Execute this statement as part of the data structure implementation.
    : capacity_(capacity), size_(0), total_probes_(0) {  // Advance or track the probing sequence used by open addressing.
    if (capacity == 0) {  // Evaluate the condition and branch into the appropriate code path.
        throw std::invalid_argument(  // Throw an exception to signal an invalid argument or operation.
            "容量必須為正整數 / Capacity must be positive");  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.
    buckets_.resize(capacity_);  // Access or update the bucket storage used to hold entries or chains.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
size_t ChainedHashTable<K, V>::insert(const K& key, const V& value) {  // Execute this statement as part of the data structure implementation.
    // 計算雜湊索引 - Compute hash index
    size_t index = hash(key);  // Compute a hash-based index so keys map into the table's storage.
    Bucket& bucket = buckets_[index];  // Access or update the bucket storage used to hold entries or chains.
    size_t probes = 0;  // Advance or track the probing sequence used by open addressing.

    // 檢查 key 是否已存在 - Check if key exists
    for (auto& pair : bucket) {  // Iterate over a range/collection to process each item in sequence.
        ++probes;  // Advance or track the probing sequence used by open addressing.
        if (pair.first == key) {  // Evaluate the condition and branch into the appropriate code path.
            pair.second = value;  // 更新 - Update existing
            total_probes_ += probes;  // Advance or track the probing sequence used by open addressing.
            return probes;  // Return the computed result to the caller.
        }  // Close the current block scope.
    }  // Close the current block scope.

    // 新增鍵值對 - Add new key-value pair
    bucket.emplace_back(key, value);  // Access or update the bucket storage used to hold entries or chains.
    ++size_;  // Execute this statement as part of the data structure implementation.
    ++probes;  // 插入操作算一次探測 - Insertion counts as one probe
    total_probes_ += probes;  // Advance or track the probing sequence used by open addressing.

    return probes;  // Return the computed result to the caller.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
std::optional<V> ChainedHashTable<K, V>::search(const K& key) const {  // Execute this statement as part of the data structure implementation.
    size_t probes;  // Advance or track the probing sequence used by open addressing.
    return search(key, probes);  // Return the computed result to the caller.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
std::optional<V> ChainedHashTable<K, V>::search(const K& key, size_t& probes) const {  // Advance or track the probing sequence used by open addressing.
    // 計算雜湊索引 - Compute hash index
    size_t index = hash(key);  // Compute a hash-based index so keys map into the table's storage.
    const Bucket& bucket = buckets_[index];  // Access or update the bucket storage used to hold entries or chains.
    probes = 0;  // Advance or track the probing sequence used by open addressing.

    // 在桶中搜尋 - Search in bucket
    for (const auto& pair : bucket) {  // Iterate over a range/collection to process each item in sequence.
        ++probes;  // Advance or track the probing sequence used by open addressing.
        if (pair.first == key) {  // Evaluate the condition and branch into the appropriate code path.
            return pair.second;  // Return the computed result to the caller.
        }  // Close the current block scope.
    }  // Close the current block scope.
    return std::nullopt;  // Return the computed result to the caller.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
bool ChainedHashTable<K, V>::remove(const K& key) {  // Execute this statement as part of the data structure implementation.
    // 計算雜湊索引 - Compute hash index
    size_t index = hash(key);  // Compute a hash-based index so keys map into the table's storage.
    Bucket& bucket = buckets_[index];  // Access or update the bucket storage used to hold entries or chains.

    // 在桶中尋找並刪除 - Find and delete from bucket
    for (auto it = bucket.begin(); it != bucket.end(); ++it) {  // Iterate over a range/collection to process each item in sequence.
        if (it->first == key) {  // Evaluate the condition and branch into the appropriate code path.
            bucket.erase(it);  // Access or update the bucket storage used to hold entries or chains.
            --size_;  // Execute this statement as part of the data structure implementation.
            return true;  // Return the computed result to the caller.
        }  // Close the current block scope.
    }  // Close the current block scope.
    return false;  // Return the computed result to the caller.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
bool ChainedHashTable<K, V>::contains(const K& key) const {  // Execute this statement as part of the data structure implementation.
    return search(key).has_value();  // Return the computed result to the caller.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
void ChainedHashTable<K, V>::clear() {  // Execute this statement as part of the data structure implementation.
    for (auto& bucket : buckets_) {  // Iterate over a range/collection to process each item in sequence.
        bucket.clear();  // Access or update the bucket storage used to hold entries or chains.
    }  // Close the current block scope.
    size_ = 0;  // Assign or update a variable that represents the current algorithm state.
    total_probes_ = 0;  // Advance or track the probing sequence used by open addressing.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
size_t ChainedHashTable<K, V>::getMaxChainLength() const {  // Execute this statement as part of the data structure implementation.
    size_t max_length = 0;  // Assign or update a variable that represents the current algorithm state.
    // 遍歷所有桶找最長鏈結 - Traverse all buckets to find longest chain
    for (const auto& bucket : buckets_) {  // Iterate over a range/collection to process each item in sequence.
        max_length = std::max(max_length, bucket.size());  // Access or update the bucket storage used to hold entries or chains.
    }  // Close the current block scope.
    return max_length;  // Return the computed result to the caller.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
double ChainedHashTable<K, V>::getAverageChainLength() const {  // Execute this statement as part of the data structure implementation.
    if (capacity_ == 0) return 0.0;  // Evaluate the condition and branch into the appropriate code path.

    size_t total_length = 0;  // Assign or update a variable that represents the current algorithm state.
    size_t non_empty_buckets = 0;  // Access or update the bucket storage used to hold entries or chains.

    // 計算非空桶的總長度 - Calculate total length of non-empty buckets
    for (const auto& bucket : buckets_) {  // Iterate over a range/collection to process each item in sequence.
        if (!bucket.empty()) {  // Evaluate the condition and branch into the appropriate code path.
            total_length += bucket.size();  // Access or update the bucket storage used to hold entries or chains.
            ++non_empty_buckets;  // Access or update the bucket storage used to hold entries or chains.
        }  // Close the current block scope.
    }  // Close the current block scope.

    if (non_empty_buckets == 0) return 0.0;  // Evaluate the condition and branch into the appropriate code path.
    return static_cast<double>(total_length) / non_empty_buckets;  // Return the computed result to the caller.
}  // Close the current block scope.

#endif // CHAINING_HPP
