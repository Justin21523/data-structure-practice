/**
 * 雜湊表開放定址法（Open Addressing）- C++ 實作
 * Hash Table with Open Addressing for Collision Resolution - C++ Implementation
 *
 * 支援三種探測方法：線性探測、二次探測、雙重雜湊
 * Supports three probing methods: Linear, Quadratic, Double Hashing
 */

#ifndef OPEN_ADDRESSING_HPP
#define OPEN_ADDRESSING_HPP

#include <vector>
#include <optional>
#include <functional>
#include <stdexcept>
#include <string>

/**
 * 探測方法列舉
 * Probing method enumeration
 */
enum class ProbeMethod {
    LINEAR,       // 線性探測 - Linear probing: h(k, i) = (h(k) + i) % m
    QUADRATIC,    // 二次探測 - Quadratic probing: h(k, i) = (h(k) + c1*i + c2*i²) % m
    DOUBLE_HASH   // 雙重雜湊 - Double hashing: h(k, i) = (h1(k) + i*h2(k)) % m
};

/**
 * 開放定址雜湊表模板類別
 * Open Addressing Hash Table template class
 *
 * @tparam K 鍵的型別（key type）
 * @tparam V 值的型別（value type）
 */
template <typename K, typename V>
class OpenAddressingHashTable {
public:
    // 型別別名 - Type aliases
    using KeyType = K;
    using ValueType = V;

    /**
     * 建構子：初始化雜湊表
     * Constructor: Initialize hash table
     *
     * @param capacity 表的容量（table capacity）
     * @param method 探測方法（probing method）
     */
    explicit OpenAddressingHashTable(size_t capacity = DEFAULT_CAPACITY,
                                    ProbeMethod method = ProbeMethod::LINEAR);

    /**
     * 解構子
     * Destructor
     */
    ~OpenAddressingHashTable() = default;

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
     * @throws std::runtime_error 若表已滿
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
     * 刪除指定的鍵值對（使用墓碑標記）
     * Delete key-value pair (using tombstone marker)
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
     * 回傳元素數量（不包括墓碑）
     * Return number of elements (excluding tombstones)
     */
    size_t size() const { return size_; }

    /**
     * 回傳表的容量
     * Return table capacity
     */
    size_t capacity() const { return capacity_; }

    /**
     * 檢查是否為空
     * Check if empty
     */
    bool empty() const { return size_ == 0; }

    /**
     * 計算負載因子 α = (n + deleted) / m
     * Calculate load factor including tombstones
     *
     * @return 負載因子
     */
    double loadFactor() const {
        return static_cast<double>(size_ + deleted_count_) / capacity_;
    }

    /**
     * 清空雜湊表
     * Clear all elements
     */
    void clear();

    // ========== 統計資訊 Statistics ==========

    /**
     * 取得已刪除槽位數量（墓碑數量）
     * Get number of deleted slots (tombstones)
     */
    size_t getDeletedCount() const { return deleted_count_; }

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

    /**
     * 取得當前探測方法
     * Get current probing method
     */
    ProbeMethod getProbeMethod() const { return method_; }

private:
    // ========== 槽位狀態 Slot State ==========
    enum class SlotState {
        EMPTY,      // 空槽位 - Empty slot
        OCCUPIED,   // 佔用槽位 - Occupied slot
        DELETED     // 已刪除（墓碑）- Deleted (tombstone)
    };

    /**
     * 槽位結構
     * Slot structure
     */
    struct Slot {
        K key;
        V value;
        SlotState state;

        Slot() : state(SlotState::EMPTY) {}
    };

    // ========== 私有成員 Private Members ==========
    std::vector<Slot> table_;      // 雜湊表陣列 - Hash table array
    size_t capacity_;               // 表的容量 - Table capacity
    size_t size_;                   // 元素數量 - Number of elements
    size_t deleted_count_;          // 墓碑數量 - Number of tombstones
    size_t total_probes_;          // 總探測次數 - Total probe count
    ProbeMethod method_;            // 探測方法 - Probing method
    std::hash<K> hasher_;          // 雜湊函數 - Hash function

    // ========== 常數 Constants ==========
    static constexpr size_t DEFAULT_CAPACITY = 16;
    static constexpr double MAX_LOAD_FACTOR = 0.7;  // 開放定址法建議較低的負載因子

    // 二次探測係數 - Quadratic probing coefficients
    static constexpr size_t C1 = 1;
    static constexpr size_t C2 = 3;

    // ========== 私有方法 Private Methods ==========

    /**
     * 主雜湊函數 h1(k)
     * Primary hash function
     *
     * @param key 要雜湊的鍵
     * @return 雜湊值
     */
    size_t hash1(const K& key) const {
        return hasher_(key) % capacity_;
    }

    /**
     * 次雜湊函數 h2(k)（用於雙重雜湊）
     * Secondary hash function (for double hashing)
     *
     * @param key 要雜湊的鍵
     * @return 雜湊值（保證為奇數）
     */
    size_t hash2(const K& key) const {
        // 使用不同的質數以獲得不同的雜湊值
        // Use different prime to get different hash value
        size_t h = hasher_(key) % (capacity_ - 1) + 1;
        // 確保回傳奇數以避免與偶數表大小產生共因數
        // Ensure odd number to avoid common factors with even table sizes
        return (h % 2 == 0) ? h + 1 : h;
    }

    /**
     * 探測函數：根據探測方法計算第 i 次探測的索引
     * Probe function: compute i-th probe index based on probing method
     *
     * @param key 鍵
     * @param i 探測次數（0, 1, 2, ...）
     * @return 探測索引
     */
    size_t probe(const K& key, size_t i) const {
        size_t h = hash1(key);

        switch (method_) {
            case ProbeMethod::LINEAR:
                // 線性探測 - Linear probing: h(k, i) = (h(k) + i) % m
                return (h + i) % capacity_;

            case ProbeMethod::QUADRATIC:
                // 二次探測 - Quadratic probing: h(k, i) = (h(k) + c1*i + c2*i²) % m
                return (h + C1 * i + C2 * i * i) % capacity_;

            case ProbeMethod::DOUBLE_HASH:
                // 雙重雜湊 - Double hashing: h(k, i) = (h1(k) + i*h2(k)) % m
                return (h + i * hash2(key)) % capacity_;

            default:
                return h;
        }
    }

    /**
     * 尋找鍵的位置
     * Find position of key
     *
     * @param key 要尋找的鍵
     * @param probes 輸出參數：探測次數
     * @return 若找到回傳索引，否則回傳 std::nullopt
     */
    std::optional<size_t> findSlot(const K& key, size_t& probes) const {
        probes = 0;

        // 探測直到找到或遇到空槽位 - Probe until found or empty slot
        for (size_t i = 0; i < capacity_; ++i) {
            size_t index = probe(key, i);
            ++probes;

            const Slot& slot = table_[index];

            if (slot.state == SlotState::EMPTY) {
                // 遇到空槽位，鍵不存在 - Empty slot, key doesn't exist
                return std::nullopt;
            }

            if (slot.state == SlotState::OCCUPIED && slot.key == key) {
                // 找到鍵 - Key found
                return index;
            }

            // 若是墓碑，繼續探測 - If tombstone, continue probing
        }

        // 表已滿且未找到 - Table full and not found
        return std::nullopt;
    }

    /**
     * 尋找插入位置
     * Find position for insertion
     *
     * @param key 要插入的鍵
     * @param probes 輸出參數：探測次數
     * @return 可插入的索引，若表已滿則回傳 std::nullopt
     */
    std::optional<size_t> findInsertSlot(const K& key, size_t& probes) const {
        probes = 0;
        std::optional<size_t> first_deleted;

        // 探測尋找空槽位或現有鍵 - Probe to find empty slot or existing key
        for (size_t i = 0; i < capacity_; ++i) {
            size_t index = probe(key, i);
            ++probes;

            const Slot& slot = table_[index];

            if (slot.state == SlotState::EMPTY) {
                // 找到空槽位 - Found empty slot
                // 若之前遇到墓碑，使用墓碑位置；否則使用此位置
                // If tombstone seen before, use it; otherwise use this slot
                return first_deleted.has_value() ? first_deleted : index;
            }

            if (slot.state == SlotState::DELETED && !first_deleted.has_value()) {
                // 記錄第一個墓碑位置 - Record first tombstone position
                first_deleted = index;
            }

            if (slot.state == SlotState::OCCUPIED && slot.key == key) {
                // 鍵已存在 - Key already exists
                return index;
            }
        }

        // 表已滿 - Table full
        return first_deleted;
    }
};

// ============================================================
// 實作部分 Implementation
// ============================================================

template <typename K, typename V>
OpenAddressingHashTable<K, V>::OpenAddressingHashTable(size_t capacity, ProbeMethod method)
    : capacity_(capacity), size_(0), deleted_count_(0), total_probes_(0), method_(method) {
    if (capacity == 0) {
        throw std::invalid_argument(
            "容量必須為正整數 / Capacity must be positive");
    }
    table_.resize(capacity_);
}

template <typename K, typename V>
size_t OpenAddressingHashTable<K, V>::insert(const K& key, const V& value) {
    // 檢查負載因子 - Check load factor
    if (loadFactor() >= MAX_LOAD_FACTOR) {
        throw std::runtime_error(
            "雜湊表負載過高 / Hash table load factor too high");
    }

    size_t probes = 0;
    auto slot_index = findInsertSlot(key, probes);

    if (!slot_index.has_value()) {
        throw std::runtime_error("雜湊表已滿 / Hash table is full");
    }

    Slot& slot = table_[slot_index.value()];

    if (slot.state == SlotState::OCCUPIED) {
        // 更新現有鍵 - Update existing key
        slot.value = value;
    } else {
        // 插入新鍵 - Insert new key
        if (slot.state == SlotState::DELETED) {
            --deleted_count_;
        }
        slot.key = key;
        slot.value = value;
        slot.state = SlotState::OCCUPIED;
        ++size_;
    }

    total_probes_ += probes;
    return probes;
}

template <typename K, typename V>
std::optional<V> OpenAddressingHashTable<K, V>::search(const K& key) const {
    size_t probes;
    return search(key, probes);
}

template <typename K, typename V>
std::optional<V> OpenAddressingHashTable<K, V>::search(const K& key, size_t& probes) const {
    auto slot_index = findSlot(key, probes);

    if (slot_index.has_value()) {
        return table_[slot_index.value()].value;
    }

    return std::nullopt;
}

template <typename K, typename V>
bool OpenAddressingHashTable<K, V>::remove(const K& key) {
    size_t probes = 0;
    auto slot_index = findSlot(key, probes);

    if (!slot_index.has_value()) {
        return false;
    }

    // 使用墓碑標記刪除 - Delete using tombstone marker
    Slot& slot = table_[slot_index.value()];
    slot.state = SlotState::DELETED;
    --size_;
    ++deleted_count_;

    return true;
}

template <typename K, typename V>
bool OpenAddressingHashTable<K, V>::contains(const K& key) const {
    return search(key).has_value();
}

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::clear() {
    for (auto& slot : table_) {
        slot.state = SlotState::EMPTY;
    }
    size_ = 0;
    deleted_count_ = 0;
    total_probes_ = 0;
}

#endif // OPEN_ADDRESSING_HPP
