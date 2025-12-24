/**
 * 雜湊表鏈結法（Chaining）- C++ 實作
 * Hash Table with Chaining for Collision Resolution - C++ Implementation
 *
 * 使用鏈結串列處理碰撞，追蹤探測次數統計
 * Uses linked lists for collision handling with probe count tracking
 */

#ifndef CHAINING_HPP
#define CHAINING_HPP

#include <vector>
#include <list>
#include <utility>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>

/**
 * 鏈結雜湊表模板類別
 * Chained Hash Table template class
 *
 * @tparam K 鍵的型別（key type）
 * @tparam V 值的型別（value type）
 */
template <typename K, typename V>
class ChainedHashTable {
public:
    // 型別別名 - Type aliases
    using KeyType = K;
    using ValueType = V;
    using PairType = std::pair<K, V>;

    /**
     * 建構子：初始化雜湊表
     * Constructor: Initialize hash table
     *
     * @param capacity 桶的數量（number of buckets）
     */
    explicit ChainedHashTable(size_t capacity = DEFAULT_CAPACITY);

    /**
     * 解構子
     * Destructor
     */
    ~ChainedHashTable() = default;

    // ========== 基本操作 Basic Operations ==========

    /**
     * 插入鍵值對（若 key 已存在則更新）
     * Insert key-value pair (update if key exists)
     *
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *
     * @param key 鍵
     * @param value 值
     * @return 探測次數 - Number of probes performed
     */
    size_t insert(const K& key, const V& value);

    /**
     * 搜尋給定 key 的 value
     * Search for value associated with key
     *
     * @param key 要搜尋的鍵
     * @return 找到則回傳 value，否則回傳 std::nullopt
     *         Value if found, std::nullopt otherwise
     */
    std::optional<V> search(const K& key) const;

    /**
     * 搜尋並回傳探測次數
     * Search and return probe count
     *
     * @param key 要搜尋的鍵
     * @param probes 輸出參數：探測次數
     * @return 找到則回傳 value，否則回傳 std::nullopt
     */
    std::optional<V> search(const K& key, size_t& probes) const;

    /**
     * 刪除指定的鍵值對
     * Delete key-value pair
     *
     * @param key 要刪除的鍵
     * @return 刪除成功回傳 true，key 不存在回傳 false
     *         true if deleted, false if key not found
     */
    bool remove(const K& key);

    /**
     * 檢查 key 是否存在
     * Check if key exists
     *
     * @param key 要檢查的鍵
     * @return 存在回傳 true，否則回傳 false
     */
    bool contains(const K& key) const;

    // ========== 容量操作 Capacity Operations ==========

    /**
     * 回傳元素數量
     * Return number of elements
     */
    size_t size() const { return size_; }

    /**
     * 回傳桶的數量
     * Return number of buckets
     */
    size_t capacity() const { return capacity_; }

    /**
     * 檢查是否為空
     * Check if empty
     */
    bool empty() const { return size_ == 0; }

    /**
     * 計算負載因子 α = n / m
     * Calculate load factor (α = n / m)
     *
     * @return 負載因子
     */
    double loadFactor() const {
        return static_cast<double>(size_) / capacity_;
    }

    /**
     * 清空雜湊表
     * Clear all elements
     */
    void clear();

    // ========== 統計資訊 Statistics ==========

    /**
     * 取得最長鏈結長度
     * Get maximum chain length
     */
    size_t getMaxChainLength() const;

    /**
     * 取得平均鏈結長度
     * Get average chain length
     */
    double getAverageChainLength() const;

    /**
     * 取得總探測次數
     * Get total probe count
     */
    size_t getTotalProbes() const { return total_probes_; }

    /**
     * 重設探測計數器
     * Reset probe counter
     */
    void resetProbeCount() { total_probes_ = 0; }

private:
    // 桶的型別：每個桶是一個鏈結串列
    // Bucket type: each bucket is a linked list
    using Bucket = std::list<PairType>;

    // ========== 私有成員 Private Members ==========
    std::vector<Bucket> buckets_;  // 桶陣列 - Array of buckets
    size_t capacity_;               // 桶的數量 - Number of buckets
    size_t size_;                   // 元素數量 - Number of elements
    size_t total_probes_;          // 總探測次數 - Total probe count
    std::hash<K> hasher_;          // 雜湊函數 - Hash function

    // ========== 常數 Constants ==========
    static constexpr size_t DEFAULT_CAPACITY = 16;

    // ========== 私有方法 Private Methods ==========

    /**
     * 計算雜湊索引
     * Compute hash index
     *
     * @param key 要雜湊的鍵
     * @return 桶的索引
     */
    size_t hash(const K& key) const {
        return hasher_(key) % capacity_;
    }
};

// ============================================================
// 實作部分 Implementation
// ============================================================

template <typename K, typename V>
ChainedHashTable<K, V>::ChainedHashTable(size_t capacity)
    : capacity_(capacity), size_(0), total_probes_(0) {
    if (capacity == 0) {
        throw std::invalid_argument(
            "容量必須為正整數 / Capacity must be positive");
    }
    buckets_.resize(capacity_);
}

template <typename K, typename V>
size_t ChainedHashTable<K, V>::insert(const K& key, const V& value) {
    // 計算雜湊索引 - Compute hash index
    size_t index = hash(key);
    Bucket& bucket = buckets_[index];
    size_t probes = 0;

    // 檢查 key 是否已存在 - Check if key exists
    for (auto& pair : bucket) {
        ++probes;
        if (pair.first == key) {
            pair.second = value;  // 更新 - Update existing
            total_probes_ += probes;
            return probes;
        }
    }

    // 新增鍵值對 - Add new key-value pair
    bucket.emplace_back(key, value);
    ++size_;
    ++probes;  // 插入操作算一次探測 - Insertion counts as one probe
    total_probes_ += probes;

    return probes;
}

template <typename K, typename V>
std::optional<V> ChainedHashTable<K, V>::search(const K& key) const {
    size_t probes;
    return search(key, probes);
}

template <typename K, typename V>
std::optional<V> ChainedHashTable<K, V>::search(const K& key, size_t& probes) const {
    // 計算雜湊索引 - Compute hash index
    size_t index = hash(key);
    const Bucket& bucket = buckets_[index];
    probes = 0;

    // 在桶中搜尋 - Search in bucket
    for (const auto& pair : bucket) {
        ++probes;
        if (pair.first == key) {
            return pair.second;
        }
    }
    return std::nullopt;
}

template <typename K, typename V>
bool ChainedHashTable<K, V>::remove(const K& key) {
    // 計算雜湊索引 - Compute hash index
    size_t index = hash(key);
    Bucket& bucket = buckets_[index];

    // 在桶中尋找並刪除 - Find and delete from bucket
    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
        if (it->first == key) {
            bucket.erase(it);
            --size_;
            return true;
        }
    }
    return false;
}

template <typename K, typename V>
bool ChainedHashTable<K, V>::contains(const K& key) const {
    return search(key).has_value();
}

template <typename K, typename V>
void ChainedHashTable<K, V>::clear() {
    for (auto& bucket : buckets_) {
        bucket.clear();
    }
    size_ = 0;
    total_probes_ = 0;
}

template <typename K, typename V>
size_t ChainedHashTable<K, V>::getMaxChainLength() const {
    size_t max_length = 0;
    // 遍歷所有桶找最長鏈結 - Traverse all buckets to find longest chain
    for (const auto& bucket : buckets_) {
        max_length = std::max(max_length, bucket.size());
    }
    return max_length;
}

template <typename K, typename V>
double ChainedHashTable<K, V>::getAverageChainLength() const {
    if (capacity_ == 0) return 0.0;

    size_t total_length = 0;
    size_t non_empty_buckets = 0;

    // 計算非空桶的總長度 - Calculate total length of non-empty buckets
    for (const auto& bucket : buckets_) {
        if (!bucket.empty()) {
            total_length += bucket.size();
            ++non_empty_buckets;
        }
    }

    if (non_empty_buckets == 0) return 0.0;
    return static_cast<double>(total_length) / non_empty_buckets;
}

#endif // CHAINING_HPP
