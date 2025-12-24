/**
 * 鏈結法雜湊表（Chained Hash Table）- C# 實作
 * 使用鏈結法（separate chaining）處理碰撞，並追蹤探測統計資訊
 *
 * Hash Table implementation using separate chaining for collision resolution
 * with probe statistics tracking
 */

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace DataStructures
{
    /// <summary>
    /// 鏈結法雜湊表泛型類別
    /// Generic Chained Hash Table class
    /// </summary>
    /// <typeparam name="TKey">鍵的型別（key type）</typeparam>
    /// <typeparam name="TValue">值的型別（value type）</typeparam>
    public class ChainedHashTable<TKey, TValue> : IEnumerable<KeyValuePair<TKey, TValue>>
    {
        // ========== 內部類別 Inner Classes ==========

        /// <summary>
        /// 鍵值對節點
        /// Key-value pair node for chaining
        /// </summary>
        private class Entry
        {
            public TKey Key { get; set; }
            public TValue Value { get; set; }
            public Entry Next { get; set; }

            public Entry(TKey key, TValue value)
            {
                Key = key;
                Value = value;
                Next = null;
            }
        }

        /// <summary>
        /// 探測統計資訊
        /// Probe statistics
        /// </summary>
        public class ProbeStatistics
        {
            /// <summary>總探測次數（Total number of probes）</summary>
            public int TotalProbes { get; set; }

            /// <summary>操作次數（Number of operations）</summary>
            public int Operations { get; set; }

            /// <summary>平均探測次數（Average probes per operation）</summary>
            public double AverageProbes => Operations > 0 ? (double)TotalProbes / Operations : 0;

            /// <summary>最大探測次數（Maximum probes in a single operation）</summary>
            public int MaxProbes { get; set; }

            /// <summary>
            /// 重設統計資訊
            /// Reset statistics
            /// </summary>
            public void Reset()
            {
                TotalProbes = 0;
                Operations = 0;
                MaxProbes = 0;
            }

            public override string ToString()
            {
                return $"Operations: {Operations}, Total Probes: {TotalProbes}, " +
                       $"Avg: {AverageProbes:F2}, Max: {MaxProbes}";
            }
        }

        // ========== 常數 Constants ==========
        private const int DefaultCapacity = 16;
        private const double MaxLoadFactor = 0.75;

        // ========== 成員變數 Member Variables ==========
        private Entry[] _buckets;              // 桶陣列 - Array of buckets
        private int _capacity;                 // 桶的數量 - Number of buckets
        private int _size;                     // 元素數量 - Number of elements
        private ProbeStatistics _insertStats;  // 插入統計 - Insert statistics
        private ProbeStatistics _searchStats;  // 搜尋統計 - Search statistics
        private ProbeStatistics _deleteStats;  // 刪除統計 - Delete statistics

        // ========== 建構子 Constructors ==========

        /// <summary>
        /// 使用預設容量建立雜湊表
        /// Create hash table with default capacity
        /// </summary>
        public ChainedHashTable() : this(DefaultCapacity) { }

        /// <summary>
        /// 使用指定容量建立雜湊表
        /// Create hash table with specified capacity
        /// </summary>
        /// <param name="capacity">桶的數量（number of buckets）</param>
        public ChainedHashTable(int capacity)
        {
            if (capacity <= 0)
            {
                throw new ArgumentException(
                    "容量必須為正整數 / Capacity must be positive", nameof(capacity));
            }
            _capacity = capacity;
            _size = 0;
            _buckets = new Entry[capacity];
            _insertStats = new ProbeStatistics();
            _searchStats = new ProbeStatistics();
            _deleteStats = new ProbeStatistics();
        }

        // ========== 屬性 Properties ==========

        /// <summary>
        /// 元素數量 - Number of elements
        /// </summary>
        public int Count => _size;

        /// <summary>
        /// 桶的數量 - Number of buckets
        /// </summary>
        public int Capacity => _capacity;

        /// <summary>
        /// 是否為空 - Is empty
        /// </summary>
        public bool IsEmpty => _size == 0;

        /// <summary>
        /// 負載因子 - Load factor
        /// </summary>
        public double LoadFactor => (double)_size / _capacity;

        /// <summary>
        /// 插入操作統計資訊 - Insert operation statistics
        /// </summary>
        public ProbeStatistics InsertStatistics => _insertStats;

        /// <summary>
        /// 搜尋操作統計資訊 - Search operation statistics
        /// </summary>
        public ProbeStatistics SearchStatistics => _searchStats;

        /// <summary>
        /// 刪除操作統計資訊 - Delete operation statistics
        /// </summary>
        public ProbeStatistics DeleteStatistics => _deleteStats;

        // ========== 索引子 Indexer ==========

        /// <summary>
        /// 使用 [] 運算子存取
        /// Access using [] operator
        /// </summary>
        public TValue this[TKey key]
        {
            get
            {
                if (TryGetValue(key, out TValue value))
                {
                    return value;
                }
                throw new KeyNotFoundException($"Key not found: {key}");
            }
            set => Insert(key, value);
        }

        // ========== 私有方法 Private Methods ==========

        /// <summary>
        /// 計算雜湊索引
        /// Compute hash index
        /// </summary>
        private int Hash(TKey key)
        {
            if (key == null) return 0;
            // 確保回傳正數 - Ensure positive value
            return Math.Abs(key.GetHashCode()) % _capacity;
        }

        /// <summary>
        /// 擴容並重新雜湊
        /// Resize and rehash
        /// </summary>
        private void Rehash()
        {
            var oldBuckets = _buckets;
            _capacity *= 2;
            _buckets = new Entry[_capacity];
            _size = 0;

            // 暫時停止統計以避免計入重新雜湊的操作
            // Temporarily disable statistics to exclude rehashing operations
            var tempInsert = _insertStats;
            _insertStats = new ProbeStatistics();

            // 重新插入所有元素 - Reinsert all elements
            foreach (var head in oldBuckets)
            {
                var current = head;
                while (current != null)
                {
                    Insert(current.Key, current.Value);
                    current = current.Next;
                }
            }

            // 恢復統計資訊 - Restore statistics
            _insertStats = tempInsert;
        }

        // ========== 公開方法 Public Methods ==========

        /// <summary>
        /// 插入鍵值對（若 key 已存在則更新）
        /// Insert key-value pair (update if key exists)
        ///
        /// 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
        /// </summary>
        public void Insert(TKey key, TValue value)
        {
            int index = Hash(key);
            var current = _buckets[index];
            int probes = 1; // 第一次探測 - First probe

            // 檢查 key 是否已存在 - Check if key exists
            while (current != null)
            {
                if (Equals(current.Key, key))
                {
                    current.Value = value;  // 更新 - Update existing
                    _insertStats.Operations++;
                    _insertStats.TotalProbes += probes;
                    _insertStats.MaxProbes = Math.Max(_insertStats.MaxProbes, probes);
                    return;
                }
                current = current.Next;
                probes++;
            }

            // 新增節點到鏈結串列頭部 - Add new node at head
            var newEntry = new Entry(key, value)
            {
                Next = _buckets[index]
            };
            _buckets[index] = newEntry;
            _size++;

            // 更新統計資訊 - Update statistics
            _insertStats.Operations++;
            _insertStats.TotalProbes += probes;
            _insertStats.MaxProbes = Math.Max(_insertStats.MaxProbes, probes);

            // 檢查是否需要擴容 - Check if rehashing needed
            if (LoadFactor > MaxLoadFactor)
            {
                Rehash();
            }
        }

        /// <summary>
        /// 搜尋給定 key 的 value
        /// Search for value associated with key
        /// </summary>
        /// <returns>找到則回傳 value，否則回傳 default</returns>
        public TValue Search(TKey key)
        {
            TryGetValue(key, out TValue value);
            return value;
        }

        /// <summary>
        /// 嘗試取得給定 key 的 value
        /// Try to get value associated with key
        /// </summary>
        public bool TryGetValue(TKey key, out TValue value)
        {
            int index = Hash(key);
            var current = _buckets[index];
            int probes = 0;

            while (current != null)
            {
                probes++;
                if (Equals(current.Key, key))
                {
                    value = current.Value;
                    _searchStats.Operations++;
                    _searchStats.TotalProbes += probes;
                    _searchStats.MaxProbes = Math.Max(_searchStats.MaxProbes, probes);
                    return true;
                }
                current = current.Next;
            }

            // 未找到也要記錄統計 - Record statistics even when not found
            _searchStats.Operations++;
            _searchStats.TotalProbes += probes;
            _searchStats.MaxProbes = Math.Max(_searchStats.MaxProbes, probes);

            value = default;
            return false;
        }

        /// <summary>
        /// 刪除指定的鍵值對
        /// Delete key-value pair
        /// </summary>
        /// <returns>刪除成功回傳 true，key 不存在回傳 false</returns>
        public bool Delete(TKey key)
        {
            int index = Hash(key);
            var current = _buckets[index];
            Entry prev = null;
            int probes = 0;

            while (current != null)
            {
                probes++;
                if (Equals(current.Key, key))
                {
                    if (prev == null)
                    {
                        _buckets[index] = current.Next;
                    }
                    else
                    {
                        prev.Next = current.Next;
                    }
                    _size--;

                    // 更新統計資訊 - Update statistics
                    _deleteStats.Operations++;
                    _deleteStats.TotalProbes += probes;
                    _deleteStats.MaxProbes = Math.Max(_deleteStats.MaxProbes, probes);
                    return true;
                }
                prev = current;
                current = current.Next;
            }

            // 未找到也要記錄統計 - Record statistics even when not found
            _deleteStats.Operations++;
            _deleteStats.TotalProbes += probes;
            _deleteStats.MaxProbes = Math.Max(_deleteStats.MaxProbes, probes);
            return false;
        }

        /// <summary>
        /// 檢查 key 是否存在
        /// Check if key exists
        /// </summary>
        public bool Contains(TKey key)
        {
            return TryGetValue(key, out _);
        }

        /// <summary>
        /// 清空雜湊表
        /// Clear all elements
        /// </summary>
        public void Clear()
        {
            _buckets = new Entry[_capacity];
            _size = 0;
        }

        /// <summary>
        /// 重設所有統計資訊
        /// Reset all statistics
        /// </summary>
        public void ResetStatistics()
        {
            _insertStats.Reset();
            _searchStats.Reset();
            _deleteStats.Reset();
        }

        /// <summary>
        /// 取得鏈結長度分布
        /// Get chain length distribution
        /// </summary>
        /// <returns>每個桶的鏈結長度（chain length for each bucket）</returns>
        public int[] GetChainLengths()
        {
            int[] lengths = new int[_capacity];
            for (int i = 0; i < _capacity; i++)
            {
                int length = 0;
                var current = _buckets[i];
                while (current != null)
                {
                    length++;
                    current = current.Next;
                }
                lengths[i] = length;
            }
            return lengths;
        }

        /// <summary>
        /// 取得最長鏈結長度
        /// Get maximum chain length
        /// </summary>
        public int GetMaxChainLength()
        {
            int maxLength = 0;
            foreach (var length in GetChainLengths())
            {
                maxLength = Math.Max(maxLength, length);
            }
            return maxLength;
        }

        /// <summary>
        /// 取得平均鏈結長度
        /// Get average chain length
        /// </summary>
        public double GetAverageChainLength()
        {
            if (_size == 0) return 0;
            return (double)_size / _capacity;
        }

        /// <summary>
        /// 取得所有鍵
        /// Get all keys
        /// </summary>
        public IEnumerable<TKey> Keys
        {
            get
            {
                foreach (var head in _buckets)
                {
                    var current = head;
                    while (current != null)
                    {
                        yield return current.Key;
                        current = current.Next;
                    }
                }
            }
        }

        /// <summary>
        /// 取得所有值
        /// Get all values
        /// </summary>
        public IEnumerable<TValue> Values
        {
            get
            {
                foreach (var head in _buckets)
                {
                    var current = head;
                    while (current != null)
                    {
                        yield return current.Value;
                        current = current.Next;
                    }
                }
            }
        }

        // ========== IEnumerable 實作 ==========

        public IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator()
        {
            foreach (var head in _buckets)
            {
                var current = head;
                while (current != null)
                {
                    yield return new KeyValuePair<TKey, TValue>(current.Key, current.Value);
                    current = current.Next;
                }
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        /// <summary>
        /// 字串表示
        /// String representation
        /// </summary>
        public override string ToString()
        {
            var sb = new StringBuilder("ChainedHashTable{");
            bool first = true;

            foreach (var kvp in this)
            {
                if (!first) sb.Append(", ");
                sb.Append($"{kvp.Key}={kvp.Value}");
                first = false;
            }

            sb.Append("}");
            return sb.ToString();
        }
    }
}
