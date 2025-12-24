/** Doc block start
 * 雜湊表（Hash Table）- C++ 實作
 * 使用鏈結法（chaining）處理碰撞
 *(blank line)
 * Hash Table implementation using chaining for collision resolution
 */  // End of block comment

#ifndef HASH_TABLE_HPP  // Execute this statement as part of the data structure implementation.
#define HASH_TABLE_HPP  // Execute this statement as part of the data structure implementation.

#include <vector>  // Execute this statement as part of the data structure implementation.
#include <list>  // Execute this statement as part of the data structure implementation.
#include <utility>  // Execute this statement as part of the data structure implementation.
#include <functional>  // Execute this statement as part of the data structure implementation.
#include <optional>  // Execute this statement as part of the data structure implementation.
#include <stdexcept>  // Execute this statement as part of the data structure implementation.
#include <string>  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 雜湊表模板類別 / Hash Table template class
 *(blank line)
 * @tparam K 鍵的型別（key type）
 * @tparam V 值的型別（value type）
 */  // End of block comment
template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
class HashTable {  // Execute this statement as part of the data structure implementation.
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
    explicit HashTable(size_t capacity = DEFAULT_CAPACITY);  // Assign or update a variable that represents the current algorithm state.

    /** Doc block start
     * 解構子 / Destructor
     */  // End of block comment
    ~HashTable() = default;  // Assign or update a variable that represents the current algorithm state.

    // ========== 基本操作 Basic Operations ==========

    /** Doc block start
     * 插入鍵值對（若 key 已存在則更新）
     * Insert key-value pair (update if key exists)
     *(blank line)
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *(blank line)
     * @param key 鍵
     * @param value 值
     */  // End of block comment
    void insert(const K& key, const V& value);  // Execute this statement as part of the data structure implementation.

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
     * Calculate load factor
     */  // End of block comment
    double loadFactor() const {  // Execute this statement as part of the data structure implementation.
        return static_cast<double>(size_) / capacity_;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 清空雜湊表 / Clear all elements
     */  // End of block comment
    void clear();  // Execute this statement as part of the data structure implementation.

    // ========== 存取操作 Access Operations ==========

    /** Doc block start
     * 使用 [] 運算子存取 / Access using [] operator
     *(blank line)
     * 若 key 不存在則插入預設值
     * If key doesn't exist, inserts default value
     *(blank line)
     * @param key 鍵
     * @return 值的參考
     */  // End of block comment
    V& operator[](const K& key);  // Execute this statement as part of the data structure implementation.

    /** Doc block start
     * 取得 value（若不存在則拋出例外）
     * Get value (throws if not found)
     *(blank line)
     * @param key 鍵
     * @return 值的參考
     * @throws std::out_of_range 若 key 不存在
     */  // End of block comment
    V& at(const K& key);  // Execute this statement as part of the data structure implementation.
    const V& at(const K& key) const;  // Execute this statement as part of the data structure implementation.

    // ========== 迭代器支援 Iterator Support ==========

    /** Doc block start
     * 迭代器類別 / Iterator class for traversing all key-value pairs
     */  // End of block comment
    class Iterator {  // Execute this statement as part of the data structure implementation.
    public:  // Execute this statement as part of the data structure implementation.
        using BucketIterator = typename std::list<PairType>::iterator;  // Assign or update a variable that represents the current algorithm state.
        using BucketsIterator = typename std::vector<std::list<PairType>>::iterator;  // Assign or update a variable that represents the current algorithm state.

        Iterator(BucketsIterator bucketsBegin, BucketsIterator bucketsEnd,  // Access or update the bucket storage used to hold entries or chains.
                 BucketIterator current)  // Execute this statement as part of the data structure implementation.
            : bucketsIt_(bucketsBegin), bucketsEnd_(bucketsEnd), current_(current) {  // Access or update the bucket storage used to hold entries or chains.
            // 找到第一個非空桶 - Find first non-empty bucket
            advanceToValid();  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        PairType& operator*() { return *current_; }  // Execute this statement as part of the data structure implementation.
        PairType* operator->() { return &(*current_); }  // Execute this statement as part of the data structure implementation.

        Iterator& operator++() {  // Execute this statement as part of the data structure implementation.
            ++current_;  // Execute this statement as part of the data structure implementation.
            advanceToValid();  // Execute this statement as part of the data structure implementation.
            return *this;  // Return the computed result to the caller.
        }  // Close the current block scope.

        bool operator==(const Iterator& other) const {  // Execute this statement as part of the data structure implementation.
            return bucketsIt_ == other.bucketsIt_ &&  // Return the computed result to the caller.
                   (bucketsIt_ == bucketsEnd_ || current_ == other.current_);  // Access or update the bucket storage used to hold entries or chains.
        }  // Close the current block scope.

        bool operator!=(const Iterator& other) const {  // Execute this statement as part of the data structure implementation.
            return !(*this == other);  // Return the computed result to the caller.
        }  // Close the current block scope.

    private:  // Execute this statement as part of the data structure implementation.
        void advanceToValid() {  // Execute this statement as part of the data structure implementation.
            while (bucketsIt_ != bucketsEnd_ && current_ == bucketsIt_->end()) {  // Repeat while the loop condition remains true.
                ++bucketsIt_;  // Access or update the bucket storage used to hold entries or chains.
                if (bucketsIt_ != bucketsEnd_) {  // Evaluate the condition and branch into the appropriate code path.
                    current_ = bucketsIt_->begin();  // Access or update the bucket storage used to hold entries or chains.
                }  // Close the current block scope.
            }  // Close the current block scope.
        }  // Close the current block scope.

        BucketsIterator bucketsIt_;  // Access or update the bucket storage used to hold entries or chains.
        BucketsIterator bucketsEnd_;  // Access or update the bucket storage used to hold entries or chains.
        BucketIterator current_;  // Execute this statement as part of the data structure implementation.
    };  // Execute this statement as part of the data structure implementation.

    Iterator begin() {  // Execute this statement as part of the data structure implementation.
        if (buckets_.empty()) {  // Evaluate the condition and branch into the appropriate code path.
            return end();  // Return the computed result to the caller.
        }  // Close the current block scope.
        return Iterator(buckets_.begin(), buckets_.end(), buckets_.front().begin());  // Return the computed result to the caller.
    }  // Close the current block scope.

    Iterator end() {  // Execute this statement as part of the data structure implementation.
        return Iterator(buckets_.end(), buckets_.end(),  // Return the computed result to the caller.
                       buckets_.empty() ? typename std::list<PairType>::iterator()  // Access or update the bucket storage used to hold entries or chains.
                                       : buckets_.back().end());  // Access or update the bucket storage used to hold entries or chains.
    }  // Close the current block scope.

private:  // Execute this statement as part of the data structure implementation.
    // 桶的型別：每個桶是一個鏈結串列 / Bucket type: each bucket is a linked list
    using Bucket = std::list<PairType>;  // Assign or update a variable that represents the current algorithm state.

    // ========== 私有成員 Private Members ==========
    std::vector<Bucket> buckets_;  // 桶陣列 - Array of buckets
    size_t capacity_;               // 桶的數量 - Number of buckets
    size_t size_;                   // 元素數量 - Number of elements
    std::hash<K> hasher_;          // 雜湊函數 - Hash function

    // ========== 常數 Constants ==========
    static constexpr size_t DEFAULT_CAPACITY = 16;  // Assign or update a variable that represents the current algorithm state.
    static constexpr double MAX_LOAD_FACTOR = 0.75;  // Assign or update a variable that represents the current algorithm state.

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

    /** Doc block start
     * 擴容並重新雜湊 / Resize and rehash
     */  // End of block comment
    void rehash();  // Rehash entries into a larger table to keep operations near O(1) on average.
};  // Execute this statement as part of the data structure implementation.

// ============================================================
// 實作部分 Implementation
// ============================================================

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
HashTable<K, V>::HashTable(size_t capacity)  // Execute this statement as part of the data structure implementation.
    : capacity_(capacity), size_(0) {  // Execute this statement as part of the data structure implementation.
    if (capacity == 0) {  // Evaluate the condition and branch into the appropriate code path.
        throw std::invalid_argument(  // Throw an exception to signal an invalid argument or operation.
            "容量必須為正整數 / Capacity must be positive");  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.
    buckets_.resize(capacity_);  // Access or update the bucket storage used to hold entries or chains.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
void HashTable<K, V>::insert(const K& key, const V& value) {  // Execute this statement as part of the data structure implementation.
    size_t index = hash(key);  // Compute a hash-based index so keys map into the table's storage.
    Bucket& bucket = buckets_[index];  // Access or update the bucket storage used to hold entries or chains.

    // 檢查 key 是否已存在 - Check if key exists
    for (auto& pair : bucket) {  // Iterate over a range/collection to process each item in sequence.
        if (pair.first == key) {  // Evaluate the condition and branch into the appropriate code path.
            pair.second = value;  // 更新 - Update existing
            return;  // Return the computed result to the caller.
        }  // Close the current block scope.
    }  // Close the current block scope.

    // 新增鍵值對 - Add new key-value pair
    bucket.emplace_back(key, value);  // Access or update the bucket storage used to hold entries or chains.
    ++size_;  // Execute this statement as part of the data structure implementation.

    // 檢查是否需要擴容 - Check if rehashing needed
    if (loadFactor() > MAX_LOAD_FACTOR) {  // Evaluate the condition and branch into the appropriate code path.
        rehash();  // Rehash entries into a larger table to keep operations near O(1) on average.
    }  // Close the current block scope.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
std::optional<V> HashTable<K, V>::search(const K& key) const {  // Execute this statement as part of the data structure implementation.
    size_t index = hash(key);  // Compute a hash-based index so keys map into the table's storage.
    const Bucket& bucket = buckets_[index];  // Access or update the bucket storage used to hold entries or chains.

    // 在桶中搜尋 - Search in bucket
    for (const auto& pair : bucket) {  // Iterate over a range/collection to process each item in sequence.
        if (pair.first == key) {  // Evaluate the condition and branch into the appropriate code path.
            return pair.second;  // Return the computed result to the caller.
        }  // Close the current block scope.
    }  // Close the current block scope.
    return std::nullopt;  // Return the computed result to the caller.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
bool HashTable<K, V>::remove(const K& key) {  // Execute this statement as part of the data structure implementation.
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
bool HashTable<K, V>::contains(const K& key) const {  // Execute this statement as part of the data structure implementation.
    return search(key).has_value();  // Return the computed result to the caller.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
void HashTable<K, V>::clear() {  // Execute this statement as part of the data structure implementation.
    for (auto& bucket : buckets_) {  // Iterate over a range/collection to process each item in sequence.
        bucket.clear();  // Access or update the bucket storage used to hold entries or chains.
    }  // Close the current block scope.
    size_ = 0;  // Assign or update a variable that represents the current algorithm state.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
V& HashTable<K, V>::operator[](const K& key) {  // Execute this statement as part of the data structure implementation.
    size_t index = hash(key);  // Compute a hash-based index so keys map into the table's storage.
    Bucket& bucket = buckets_[index];  // Access or update the bucket storage used to hold entries or chains.

    // 搜尋現有的鍵 - Search for existing key
    for (auto& pair : bucket) {  // Iterate over a range/collection to process each item in sequence.
        if (pair.first == key) {  // Evaluate the condition and branch into the appropriate code path.
            return pair.second;  // Return the computed result to the caller.
        }  // Close the current block scope.
    }  // Close the current block scope.

    // 不存在則插入預設值 - Insert default value if not found
    bucket.emplace_back(key, V{});  // Access or update the bucket storage used to hold entries or chains.
    ++size_;  // Execute this statement as part of the data structure implementation.

    if (loadFactor() > MAX_LOAD_FACTOR) {  // Evaluate the condition and branch into the appropriate code path.
        rehash();  // Rehash entries into a larger table to keep operations near O(1) on average.
        // rehash 後需要重新找到元素 - Need to find element again after rehash
        return (*this)[key];  // Return the computed result to the caller.
    }  // Close the current block scope.

    return bucket.back().second;  // Return the computed result to the caller.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
V& HashTable<K, V>::at(const K& key) {  // Execute this statement as part of the data structure implementation.
    size_t index = hash(key);  // Compute a hash-based index so keys map into the table's storage.
    Bucket& bucket = buckets_[index];  // Access or update the bucket storage used to hold entries or chains.

    for (auto& pair : bucket) {  // Iterate over a range/collection to process each item in sequence.
        if (pair.first == key) {  // Evaluate the condition and branch into the appropriate code path.
            return pair.second;  // Return the computed result to the caller.
        }  // Close the current block scope.
    }  // Close the current block scope.
    throw std::out_of_range("Key not found in hash table");  // Throw an exception to signal an invalid argument or operation.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
const V& HashTable<K, V>::at(const K& key) const {  // Execute this statement as part of the data structure implementation.
    size_t index = hash(key);  // Compute a hash-based index so keys map into the table's storage.
    const Bucket& bucket = buckets_[index];  // Access or update the bucket storage used to hold entries or chains.

    for (const auto& pair : bucket) {  // Iterate over a range/collection to process each item in sequence.
        if (pair.first == key) {  // Evaluate the condition and branch into the appropriate code path.
            return pair.second;  // Return the computed result to the caller.
        }  // Close the current block scope.
    }  // Close the current block scope.
    throw std::out_of_range("Key not found in hash table");  // Throw an exception to signal an invalid argument or operation.
}  // Close the current block scope.

template <typename K, typename V>  // Execute this statement as part of the data structure implementation.
void HashTable<K, V>::rehash() {  // Rehash entries into a larger table to keep operations near O(1) on average.
    // 儲存舊的桶 - Store old buckets
    std::vector<Bucket> oldBuckets = std::move(buckets_);  // Access or update the bucket storage used to hold entries or chains.

    // 容量加倍 - Double capacity
    capacity_ *= 2;  // Assign or update a variable that represents the current algorithm state.
    buckets_.clear();  // Access or update the bucket storage used to hold entries or chains.
    buckets_.resize(capacity_);  // Access or update the bucket storage used to hold entries or chains.
    size_ = 0;  // Assign or update a variable that represents the current algorithm state.

    // 重新插入所有元素 - Reinsert all elements
    for (const auto& bucket : oldBuckets) {  // Iterate over a range/collection to process each item in sequence.
        for (const auto& pair : bucket) {  // Iterate over a range/collection to process each item in sequence.
            insert(pair.first, pair.second);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.
}  // Close the current block scope.

#endif // HASH_TABLE_HPP
