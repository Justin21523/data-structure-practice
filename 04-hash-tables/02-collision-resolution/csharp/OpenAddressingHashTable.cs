/**
 * 開放定址法雜湊表（Open Addressing Hash Table）- C# 實作
 * 支援三種探測方法：線性探測、平方探測、雙重雜湊
 *
 * Hash Table implementation using open addressing
 * Supports three probing methods: Linear, Quadratic, and Double Hashing
 */

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace DataStructures
{
    /// <summary>
    /// 探測方法列舉
    /// Probing method enumeration
    /// </summary>
    public enum ProbeMethod
    {
        /// <summary>線性探測 - Linear probing</summary>
        Linear,
        /// <summary>平方探測 - Quadratic probing</summary>
        Quadratic,
        /// <summary>雙重雜湊 - Double hashing</summary>
        DoubleHash
    }

    /// <summary>
    /// 開放定址法雜湊表泛型類別
    /// Generic Open Addressing Hash Table class
    /// </summary>
    /// <typeparam name="TKey">鍵的型別（key type）</typeparam>
    /// <typeparam name="TValue">值的型別（value type）</typeparam>
    public class OpenAddressingHashTable<TKey, TValue> : IEnumerable<KeyValuePair<TKey, TValue>>
    {
        // ========== 內部類別 Inner Classes ==========

        /// <summary>
        /// 鍵值對項目
        /// Key-value pair entry
        /// </summary>
        private class Entry
        {
            public TKey Key { get; set; }
            public TValue Value { get; set; }
            public bool IsDeleted { get; set; }  // 墓碑標記 - Tombstone marker

            public Entry(TKey key, TValue value)
            {
                Key = key;
                Value = value;
                IsDeleted = false;
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
        private Entry[] _table;                // 雜湊表陣列 - Hash table array
        private int _capacity;                 // 表格大小 - Table size
        private int _size;                     // 元素數量（不含已刪除）- Number of active elements
        private int _deletedCount;             // 已刪除元素數量 - Number of deleted elements
        private ProbeMethod _probeMethod;      // 探測方法 - Probing method
        private ProbeStatistics _insertStats;  // 插入統計 - Insert statistics
        private ProbeStatistics _searchStats;  // 搜尋統計 - Search statistics
        private ProbeStatistics _deleteStats;  // 刪除統計 - Delete statistics

        // ========== 建構子 Constructors ==========

        /// <summary>
        /// 使用預設容量和線性探測建立雜湊表
        /// Create hash table with default capacity and linear probing
        /// </summary>
        public OpenAddressingHashTable() : this(DefaultCapacity, ProbeMethod.Linear) { }

        /// <summary>
        /// 使用指定容量和探測方法建立雜湊表
        /// Create hash table with specified capacity and probing method
        /// </summary>
        /// <param name="capacity">表格大小（table size）</param>
        /// <param name="probeMethod">探測方法（probing method）</param>
        public OpenAddressingHashTable(int capacity, ProbeMethod probeMethod = ProbeMethod.Linear)
        {
            if (capacity <= 0)
            {
                throw new ArgumentException(
                    "容量必須為正整數 / Capacity must be positive", nameof(capacity));
            }
            _capacity = capacity;
            _size = 0;
            _deletedCount = 0;
            _table = new Entry[capacity];
            _probeMethod = probeMethod;
            _insertStats = new ProbeStatistics();
            _searchStats = new ProbeStatistics();
            _deleteStats = new ProbeStatistics();
        }

        // ========== 屬性 Properties ==========

        /// <summary>
        /// 元素數量（不含已刪除） - Number of active elements
        /// </summary>
        public int Count => _size;

        /// <summary>
        /// 表格大小 - Table size
        /// </summary>
        public int Capacity => _capacity;

        /// <summary>
        /// 是否為空 - Is empty
        /// </summary>
        public bool IsEmpty => _size == 0;

        /// <summary>
        /// 負載因子（包含已刪除元素） - Load factor (including deleted elements)
        /// </summary>
        public double LoadFactor => (double)(_size + _deletedCount) / _capacity;

        /// <summary>
        /// 探測方法 - Probing method
        /// </summary>
        public ProbeMethod Method => _probeMethod;

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
        /// 計算主要雜湊值
        /// Compute primary hash value
        /// </summary>
        private int Hash1(TKey key)
        {
            if (key == null) return 0;
            return Math.Abs(key.GetHashCode()) % _capacity;
        }

        /// <summary>
        /// 計算次要雜湊值（用於雙重雜湊）
        /// Compute secondary hash value (for double hashing)
        /// </summary>
        private int Hash2(TKey key)
        {
            if (key == null) return 1;
            // 使用質數 7 避免與主雜湊值重疊
            // Use prime number 7 to avoid overlap with primary hash
            int hash = Math.Abs(key.GetHashCode());
            return 7 - (hash % 7);
        }

        /// <summary>
        /// 計算探測序列的下一個索引
        /// Compute next index in probe sequence
        /// </summary>
        /// <param name="key">鍵（key）</param>
        /// <param name="i">探測次數（probe attempt number）</param>
        /// <returns>下一個探測索引（next probe index）</returns>
        private int Probe(TKey key, int i)
        {
            int h1 = Hash1(key);

            switch (_probeMethod)
            {
                case ProbeMethod.Linear:
                    // 線性探測: h(k, i) = (h1(k) + i) mod m
                    // Linear probing: h(k, i) = (h1(k) + i) mod m
                    return (h1 + i) % _capacity;

                case ProbeMethod.Quadratic:
                    // 平方探測: h(k, i) = (h1(k) + c1*i + c2*i^2) mod m
                    // Quadratic probing: h(k, i) = (h1(k) + c1*i + c2*i^2) mod m
                    // 使用 c1=1, c2=1
                    return (h1 + i + i * i) % _capacity;

                case ProbeMethod.DoubleHash:
                    // 雙重雜湊: h(k, i) = (h1(k) + i*h2(k)) mod m
                    // Double hashing: h(k, i) = (h1(k) + i*h2(k)) mod m
                    int h2 = Hash2(key);
                    return (h1 + i * h2) % _capacity;

                default:
                    return h1;
            }
        }

        /// <summary>
        /// 擴容並重新雜湊
        /// Resize and rehash
        /// </summary>
        private void Rehash()
        {
            var oldTable = _table;
            _capacity *= 2;
            _table = new Entry[_capacity];
            _size = 0;
            _deletedCount = 0;

            // 暫時停止統計以避免計入重新雜湊的操作
            // Temporarily disable statistics to exclude rehashing operations
            var tempInsert = _insertStats;
            _insertStats = new ProbeStatistics();

            // 重新插入所有元素 - Reinsert all active elements
            foreach (var entry in oldTable)
            {
                if (entry != null && !entry.IsDeleted)
                {
                    Insert(entry.Key, entry.Value);
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
            int probes = 0;
            int firstDeletedIndex = -1;

            for (int i = 0; i < _capacity; i++)
            {
                probes++;
                int index = Probe(key, i);
                var entry = _table[index];

                // 找到空位 - Found empty slot
                if (entry == null)
                {
                    // 如果之前遇到已刪除的位置，使用該位置
                    // If we encountered a deleted slot earlier, use that
                    if (firstDeletedIndex != -1)
                    {
                        _table[firstDeletedIndex] = new Entry(key, value);
                        _deletedCount--;
                    }
                    else
                    {
                        _table[index] = new Entry(key, value);
                    }
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
                    return;
                }

                // 找到已刪除的位置 - Found deleted slot
                if (entry.IsDeleted && firstDeletedIndex == -1)
                {
                    firstDeletedIndex = index;
                }

                // 找到相同的鍵，更新值 - Found same key, update value
                if (!entry.IsDeleted && Equals(entry.Key, key))
                {
                    entry.Value = value;
                    _insertStats.Operations++;
                    _insertStats.TotalProbes += probes;
                    _insertStats.MaxProbes = Math.Max(_insertStats.MaxProbes, probes);
                    return;
                }
            }

            // 表格已滿，觸發擴容 - Table is full, trigger rehashing
            Rehash();
            Insert(key, value);
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
            int probes = 0;

            for (int i = 0; i < _capacity; i++)
            {
                probes++;
                int index = Probe(key, i);
                var entry = _table[index];

                // 找到空位，表示 key 不存在 - Found empty slot, key doesn't exist
                if (entry == null)
                {
                    _searchStats.Operations++;
                    _searchStats.TotalProbes += probes;
                    _searchStats.MaxProbes = Math.Max(_searchStats.MaxProbes, probes);
                    value = default;
                    return false;
                }

                // 找到相同的鍵且未被刪除 - Found same key and not deleted
                if (!entry.IsDeleted && Equals(entry.Key, key))
                {
                    _searchStats.Operations++;
                    _searchStats.TotalProbes += probes;
                    _searchStats.MaxProbes = Math.Max(_searchStats.MaxProbes, probes);
                    value = entry.Value;
                    return true;
                }

                // 繼續探測（已刪除或不同的鍵） - Continue probing (deleted or different key)
            }

            // 探測整個表格都沒找到 - Probed entire table without finding
            _searchStats.Operations++;
            _searchStats.TotalProbes += probes;
            _searchStats.MaxProbes = Math.Max(_searchStats.MaxProbes, probes);
            value = default;
            return false;
        }

        /// <summary>
        /// 刪除指定的鍵值對（使用墓碑標記）
        /// Delete key-value pair (using tombstone marker)
        /// </summary>
        /// <returns>刪除成功回傳 true，key 不存在回傳 false</returns>
        public bool Delete(TKey key)
        {
            int probes = 0;

            for (int i = 0; i < _capacity; i++)
            {
                probes++;
                int index = Probe(key, i);
                var entry = _table[index];

                // 找到空位，表示 key 不存在 - Found empty slot, key doesn't exist
                if (entry == null)
                {
                    _deleteStats.Operations++;
                    _deleteStats.TotalProbes += probes;
                    _deleteStats.MaxProbes = Math.Max(_deleteStats.MaxProbes, probes);
                    return false;
                }

                // 找到相同的鍵且未被刪除 - Found same key and not deleted
                if (!entry.IsDeleted && Equals(entry.Key, key))
                {
                    entry.IsDeleted = true;  // 設置墓碑標記 - Set tombstone marker
                    _size--;
                    _deletedCount++;

                    _deleteStats.Operations++;
                    _deleteStats.TotalProbes += probes;
                    _deleteStats.MaxProbes = Math.Max(_deleteStats.MaxProbes, probes);
                    return true;
                }

                // 繼續探測 - Continue probing
            }

            // 探測整個表格都沒找到 - Probed entire table without finding
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
            _table = new Entry[_capacity];
            _size = 0;
            _deletedCount = 0;
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
        /// 取得已刪除元素數量
        /// Get number of deleted elements
        /// </summary>
        public int GetDeletedCount()
        {
            return _deletedCount;
        }

        /// <summary>
        /// 取得所有鍵
        /// Get all keys
        /// </summary>
        public IEnumerable<TKey> Keys
        {
            get
            {
                foreach (var entry in _table)
                {
                    if (entry != null && !entry.IsDeleted)
                    {
                        yield return entry.Key;
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
                foreach (var entry in _table)
                {
                    if (entry != null && !entry.IsDeleted)
                    {
                        yield return entry.Value;
                    }
                }
            }
        }

        // ========== IEnumerable 實作 ==========

        public IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator()
        {
            foreach (var entry in _table)
            {
                if (entry != null && !entry.IsDeleted)
                {
                    yield return new KeyValuePair<TKey, TValue>(entry.Key, entry.Value);
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
            var sb = new StringBuilder($"OpenAddressingHashTable[{_probeMethod}]{{");
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
