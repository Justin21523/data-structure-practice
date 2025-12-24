/**
 * 雜湊表（Hash Table）- C++ 實作
 * 使用鏈結法（chaining）處理碰撞
 *
 * Hash Table implementation using chaining for collision resolution
 */

#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <vector>
#include <list>
#include <utility>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>

/**
 * 雜湊表模板類別
 * Hash Table template class
 *
 * @tparam K 鍵的型別（key type）
 * @tparam V 值的型別（value type）
 */
template <typename K, typename V>
class HashTable {
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
    explicit HashTable(size_t capacity = DEFAULT_CAPACITY);

    /**
     * 解構子
     * Destructor
     */
    ~HashTable() = default;

    // ========== 基本操作 Basic Operations ==========

    /**
     * 插入鍵值對（若 key 已存在則更新）
     * Insert key-value pair (update if key exists)
     *
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *
     * @param key 鍵
     * @param value 值
     */
    void insert(const K& key, const V& value);

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
     * Calculate load factor
     */
    double loadFactor() const {
        return static_cast<double>(size_) / capacity_;
    }

    /**
     * 清空雜湊表
     * Clear all elements
     */
    void clear();

    // ========== 存取操作 Access Operations ==========

    /**
     * 使用 [] 運算子存取
     * Access using [] operator
     *
     * 若 key 不存在則插入預設值
     * If key doesn't exist, inserts default value
     *
     * @param key 鍵
     * @return 值的參考
     */
    V& operator[](const K& key);

    /**
     * 取得 value（若不存在則拋出例外）
     * Get value (throws if not found)
     *
     * @param key 鍵
     * @return 值的參考
     * @throws std::out_of_range 若 key 不存在
     */
    V& at(const K& key);
    const V& at(const K& key) const;

    // ========== 迭代器支援 Iterator Support ==========

    /**
     * 迭代器類別
     * Iterator class for traversing all key-value pairs
     */
    class Iterator {
    public:
        using BucketIterator = typename std::list<PairType>::iterator;
        using BucketsIterator = typename std::vector<std::list<PairType>>::iterator;

        Iterator(BucketsIterator bucketsBegin, BucketsIterator bucketsEnd,
                 BucketIterator current)
            : bucketsIt_(bucketsBegin), bucketsEnd_(bucketsEnd), current_(current) {
            // 找到第一個非空桶 - Find first non-empty bucket
            advanceToValid();
        }

        PairType& operator*() { return *current_; }
        PairType* operator->() { return &(*current_); }

        Iterator& operator++() {
            ++current_;
            advanceToValid();
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return bucketsIt_ == other.bucketsIt_ &&
                   (bucketsIt_ == bucketsEnd_ || current_ == other.current_);
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

    private:
        void advanceToValid() {
            while (bucketsIt_ != bucketsEnd_ && current_ == bucketsIt_->end()) {
                ++bucketsIt_;
                if (bucketsIt_ != bucketsEnd_) {
                    current_ = bucketsIt_->begin();
                }
            }
        }

        BucketsIterator bucketsIt_;
        BucketsIterator bucketsEnd_;
        BucketIterator current_;
    };

    Iterator begin() {
        if (buckets_.empty()) {
            return end();
        }
        return Iterator(buckets_.begin(), buckets_.end(), buckets_.front().begin());
    }

    Iterator end() {
        return Iterator(buckets_.end(), buckets_.end(),
                       buckets_.empty() ? typename std::list<PairType>::iterator()
                                       : buckets_.back().end());
    }

private:
    // 桶的型別：每個桶是一個鏈結串列
    // Bucket type: each bucket is a linked list
    using Bucket = std::list<PairType>;

    // ========== 私有成員 Private Members ==========
    std::vector<Bucket> buckets_;  // 桶陣列 - Array of buckets
    size_t capacity_;               // 桶的數量 - Number of buckets
    size_t size_;                   // 元素數量 - Number of elements
    std::hash<K> hasher_;          // 雜湊函數 - Hash function

    // ========== 常數 Constants ==========
    static constexpr size_t DEFAULT_CAPACITY = 16;
    static constexpr double MAX_LOAD_FACTOR = 0.75;

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

    /**
     * 擴容並重新雜湊
     * Resize and rehash
     */
    void rehash();
};

// ============================================================
// 實作部分 Implementation
// ============================================================

template <typename K, typename V>
HashTable<K, V>::HashTable(size_t capacity)
    : capacity_(capacity), size_(0) {
    if (capacity == 0) {
        throw std::invalid_argument(
            "容量必須為正整數 / Capacity must be positive");
    }
    buckets_.resize(capacity_);
}

template <typename K, typename V>
void HashTable<K, V>::insert(const K& key, const V& value) {
    size_t index = hash(key);
    Bucket& bucket = buckets_[index];

    // 檢查 key 是否已存在 - Check if key exists
    for (auto& pair : bucket) {
        if (pair.first == key) {
            pair.second = value;  // 更新 - Update existing
            return;
        }
    }

    // 新增鍵值對 - Add new key-value pair
    bucket.emplace_back(key, value);
    ++size_;

    // 檢查是否需要擴容 - Check if rehashing needed
    if (loadFactor() > MAX_LOAD_FACTOR) {
        rehash();
    }
}

template <typename K, typename V>
std::optional<V> HashTable<K, V>::search(const K& key) const {
    size_t index = hash(key);
    const Bucket& bucket = buckets_[index];

    // 在桶中搜尋 - Search in bucket
    for (const auto& pair : bucket) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    return std::nullopt;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K& key) {
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
bool HashTable<K, V>::contains(const K& key) const {
    return search(key).has_value();
}

template <typename K, typename V>
void HashTable<K, V>::clear() {
    for (auto& bucket : buckets_) {
        bucket.clear();
    }
    size_ = 0;
}

template <typename K, typename V>
V& HashTable<K, V>::operator[](const K& key) {
    size_t index = hash(key);
    Bucket& bucket = buckets_[index];

    // 搜尋現有的鍵 - Search for existing key
    for (auto& pair : bucket) {
        if (pair.first == key) {
            return pair.second;
        }
    }

    // 不存在則插入預設值 - Insert default value if not found
    bucket.emplace_back(key, V{});
    ++size_;

    if (loadFactor() > MAX_LOAD_FACTOR) {
        rehash();
        // rehash 後需要重新找到元素 - Need to find element again after rehash
        return (*this)[key];
    }

    return bucket.back().second;
}

template <typename K, typename V>
V& HashTable<K, V>::at(const K& key) {
    size_t index = hash(key);
    Bucket& bucket = buckets_[index];

    for (auto& pair : bucket) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    throw std::out_of_range("Key not found in hash table");
}

template <typename K, typename V>
const V& HashTable<K, V>::at(const K& key) const {
    size_t index = hash(key);
    const Bucket& bucket = buckets_[index];

    for (const auto& pair : bucket) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    throw std::out_of_range("Key not found in hash table");
}

template <typename K, typename V>
void HashTable<K, V>::rehash() {
    // 儲存舊的桶 - Store old buckets
    std::vector<Bucket> oldBuckets = std::move(buckets_);

    // 容量加倍 - Double capacity
    capacity_ *= 2;
    buckets_.clear();
    buckets_.resize(capacity_);
    size_ = 0;

    // 重新插入所有元素 - Reinsert all elements
    for (const auto& bucket : oldBuckets) {
        for (const auto& pair : bucket) {
            insert(pair.first, pair.second);
        }
    }
}

#endif // HASH_TABLE_HPP
