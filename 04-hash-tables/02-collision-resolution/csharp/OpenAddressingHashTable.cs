/** Doc block start
 * 開放定址法雜湊表（Open Addressing Hash Table）- C# 實作
 * 支援三種探測方法：線性探測、平方探測、雙重雜湊 / 支援三種 probe 方法： linear probing 、平方 probe 、 double hashing
 *(blank line)
 * Hash Table implementation using open addressing
 * Supports three probing methods: Linear, Quadratic, and Double Hashing
 */  // End of block comment

using System;  // Execute this statement as part of the data structure implementation.
using System.Collections;  // Execute this statement as part of the data structure implementation.
using System.Collections.Generic;  // Execute this statement as part of the data structure implementation.
using System.Text;  // Execute this statement as part of the data structure implementation.

namespace DataStructures  // Execute this statement as part of the data structure implementation.
{  // Open a new block scope.
    /// <summary>
    /// 探測方法列舉 / Probing method enumeration
    /// </summary>
    public enum ProbeMethod  // Execute this statement as part of the data structure implementation.
    {  // Open a new block scope.
        /// <summary>線性探測 - Linear probing</summary>
        Linear,  // Execute this statement as part of the data structure implementation.
        /// <summary>平方探測 - Quadratic probing</summary>
        Quadratic,  // Execute this statement as part of the data structure implementation.
        /// <summary>雙重雜湊 - Double hashing</summary>
        DoubleHash  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    /// <summary>
    /// 開放定址法雜湊表泛型類別 / Generic Open Addressing Hash Table class
    /// </summary>
    /// <typeparam name="TKey">鍵的型別（key type）</typeparam>
    /// <typeparam name="TValue">值的型別（value type）</typeparam>
    public class OpenAddressingHashTable<TKey, TValue> : IEnumerable<KeyValuePair<TKey, TValue>>  // Execute this statement as part of the data structure implementation.
    {  // Open a new block scope.
        // ========== 內部類別 Inner Classes ==========

        /// <summary>
        /// 鍵值對項目 / Key-value pair entry
        /// </summary>
        private class Entry  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            public TKey Key { get; set; }  // Execute this statement as part of the data structure implementation.
            public TValue Value { get; set; }  // Execute this statement as part of the data structure implementation.
            public bool IsDeleted { get; set; }  // 墓碑標記 - Tombstone marker

            public Entry(TKey key, TValue value)  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                Key = key;  // Assign or update a variable that represents the current algorithm state.
                Value = value;  // Assign or update a variable that represents the current algorithm state.
                IsDeleted = false;  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.
        }  // Close the current block scope.

        /// <summary>
        /// 探測統計資訊 / Probe statistics
        /// </summary>
        public class ProbeStatistics  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            /// <summary>總探測次數（Total number of probes）</summary>
            public int TotalProbes { get; set; }  // Execute this statement as part of the data structure implementation.

            /// <summary>操作次數（Number of operations）</summary>
            public int Operations { get; set; }  // Execute this statement as part of the data structure implementation.

            /// <summary>平均探測次數（Average probes per operation）</summary>
            public double AverageProbes => Operations > 0 ? (double)TotalProbes / Operations : 0;  // Assign or update a variable that represents the current algorithm state.

            /// <summary>最大探測次數（Maximum probes in a single operation）</summary>
            public int MaxProbes { get; set; }  // Execute this statement as part of the data structure implementation.

            /// <summary>
            /// 重設統計資訊 / Reset statistics
            /// </summary>
            public void Reset()  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                TotalProbes = 0;  // Assign or update a variable that represents the current algorithm state.
                Operations = 0;  // Assign or update a variable that represents the current algorithm state.
                MaxProbes = 0;  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.

            public override string ToString()  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                return $"Operations: {Operations}, Total Probes: {TotalProbes}, " +  // Return the computed result to the caller.
                       $"Avg: {AverageProbes:F2}, Max: {MaxProbes}";  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        }  // Close the current block scope.

        // ========== 常數 Constants ==========
        private const int DefaultCapacity = 16;  // Assign or update a variable that represents the current algorithm state.
        private const double MaxLoadFactor = 0.75;  // Assign or update a variable that represents the current algorithm state.

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
        /// 使用預設容量和線性探測建立雜湊表 / Create hash table with default capacity and linear probing
        /// </summary>
        public OpenAddressingHashTable() : this(DefaultCapacity, ProbeMethod.Linear) { }  // Execute this statement as part of the data structure implementation.

        /// <summary>
        /// 使用指定容量和探測方法建立雜湊表 / Create hash table with specified capacity and probing method
        /// </summary>
        /// <param name="capacity">表格大小（table size）</param>
        /// <param name="probeMethod">探測方法（probing method）</param>
        public OpenAddressingHashTable(int capacity, ProbeMethod probeMethod = ProbeMethod.Linear)  // Advance or track the probing sequence used by open addressing.
        {  // Open a new block scope.
            if (capacity <= 0)  // Evaluate the condition and branch into the appropriate code path.
            {  // Open a new block scope.
                throw new ArgumentException(  // Throw an exception to signal an invalid argument or operation.
                    "容量必須為正整數 / Capacity must be positive", nameof(capacity));  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
            _capacity = capacity;  // Assign or update a variable that represents the current algorithm state.
            _size = 0;  // Assign or update a variable that represents the current algorithm state.
            _deletedCount = 0;  // Handle tombstones so deletions do not break the probing/search sequence.
            _table = new Entry[capacity];  // Assign or update a variable that represents the current algorithm state.
            _probeMethod = probeMethod;  // Advance or track the probing sequence used by open addressing.
            _insertStats = new ProbeStatistics();  // Assign or update a variable that represents the current algorithm state.
            _searchStats = new ProbeStatistics();  // Assign or update a variable that represents the current algorithm state.
            _deleteStats = new ProbeStatistics();  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.

        // ========== 屬性 Properties ==========

        /// <summary>
        /// 元素數量（不含已刪除） - Number of active elements
        /// </summary>
        public int Count => _size;  // Assign or update a variable that represents the current algorithm state.

        /// <summary>
        /// 表格大小 - Table size
        /// </summary>
        public int Capacity => _capacity;  // Assign or update a variable that represents the current algorithm state.

        /// <summary>
        /// 是否為空 - Is empty
        /// </summary>
        public bool IsEmpty => _size == 0;  // Execute this statement as part of the data structure implementation.

        /// <summary>
        /// 負載因子（包含已刪除元素） - Load factor (including deleted elements)
        /// </summary>
        public double LoadFactor => (double)(_size + _deletedCount) / _capacity;  // Handle tombstones so deletions do not break the probing/search sequence.

        /// <summary>
        /// 探測方法 - Probing method
        /// </summary>
        public ProbeMethod Method => _probeMethod;  // Advance or track the probing sequence used by open addressing.

        /// <summary>
        /// 插入操作統計資訊 - Insert operation statistics
        /// </summary>
        public ProbeStatistics InsertStatistics => _insertStats;  // Assign or update a variable that represents the current algorithm state.

        /// <summary>
        /// 搜尋操作統計資訊 - Search operation statistics
        /// </summary>
        public ProbeStatistics SearchStatistics => _searchStats;  // Assign or update a variable that represents the current algorithm state.

        /// <summary>
        /// 刪除操作統計資訊 - Delete operation statistics
        /// </summary>
        public ProbeStatistics DeleteStatistics => _deleteStats;  // Assign or update a variable that represents the current algorithm state.

        // ========== 索引子 Indexer ==========

        /// <summary>
        /// 使用 [] 運算子存取 / Access using [] operator
        /// </summary>
        public TValue this[TKey key]  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            get  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                if (TryGetValue(key, out TValue value))  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    return value;  // Return the computed result to the caller.
                }  // Close the current block scope.
                throw new KeyNotFoundException($"Key not found: {key}");  // Throw an exception to signal an invalid argument or operation.
            }  // Close the current block scope.
            set => Insert(key, value);  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.

        // ========== 私有方法 Private Methods ==========

        /// <summary>
        /// 計算主要雜湊值 / Compute primary hash value
        /// </summary>
        private int Hash1(TKey key)  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            if (key == null) return 0;  // Evaluate the condition and branch into the appropriate code path.
            return Math.Abs(key.GetHashCode()) % _capacity;  // Return the computed result to the caller.
        }  // Close the current block scope.

        /// <summary>
        /// 計算次要雜湊值（用於雙重雜湊） / Compute secondary hash value (for double hashing)
        /// </summary>
        private int Hash2(TKey key)  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            if (key == null) return 1;  // Evaluate the condition and branch into the appropriate code path.
            // 使用質數 7 避免與主雜湊值重疊 / Use prime number 7 to avoid overlap with primary hash
            int hash = Math.Abs(key.GetHashCode());  // Compute a hash-based index so keys map into the table's storage.
            return 7 - (hash % 7);  // Return the computed result to the caller.
        }  // Close the current block scope.

        /// <summary>
        /// 計算探測序列的下一個索引 / Compute next index in probe sequence
        /// </summary>
        /// <param name="key">鍵（key）</param>
        /// <param name="i">探測次數（probe attempt number）</param>
        /// <returns>下一個探測索引（next probe index）</returns>
        private int Probe(TKey key, int i)  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            int h1 = Hash1(key);  // Assign or update a variable that represents the current algorithm state.

            switch (_probeMethod)  // Advance or track the probing sequence used by open addressing.
            {  // Open a new block scope.
                case ProbeMethod.Linear:  // Execute this statement as part of the data structure implementation.
                    // 線性探測: h(k, i) = (h1(k) + i) mod m
                    // Linear probing: h(k, i) = (h1(k) + i) mod m
                    return (h1 + i) % _capacity;  // Return the computed result to the caller.

                case ProbeMethod.Quadratic:  // Execute this statement as part of the data structure implementation.
                    // 平方探測: h(k, i) = (h1(k) + c1*i + c2*i^2) mod m
                    // Quadratic probing: h(k, i) = (h1(k) + c1*i + c2*i^2) mod m
                    // 使用 c1=1, c2=1
                    return (h1 + i + i * i) % _capacity;  // Return the computed result to the caller.

                case ProbeMethod.DoubleHash:  // Execute this statement as part of the data structure implementation.
                    // 雙重雜湊: h(k, i) = (h1(k) + i*h2(k)) mod m
                    // Double hashing: h(k, i) = (h1(k) + i*h2(k)) mod m
                    int h2 = Hash2(key);  // Assign or update a variable that represents the current algorithm state.
                    return (h1 + i * h2) % _capacity;  // Return the computed result to the caller.

                default:  // Execute this statement as part of the data structure implementation.
                    return h1;  // Return the computed result to the caller.
            }  // Close the current block scope.
        }  // Close the current block scope.

        /// <summary>
        /// 擴容並重新雜湊 / Resize and rehash
        /// </summary>
        private void Rehash()  // Compute a hash-based index so keys map into the table's storage.
        {  // Open a new block scope.
            var oldTable = _table;  // Assign or update a variable that represents the current algorithm state.
            _capacity *= 2;  // Assign or update a variable that represents the current algorithm state.
            _table = new Entry[_capacity];  // Assign or update a variable that represents the current algorithm state.
            _size = 0;  // Assign or update a variable that represents the current algorithm state.
            _deletedCount = 0;  // Handle tombstones so deletions do not break the probing/search sequence.

            // 暫時停止統計以避免計入重新雜湊的操作 / Temporarily disable statistics to exclude rehashing operations
            var tempInsert = _insertStats;  // Assign or update a variable that represents the current algorithm state.
            _insertStats = new ProbeStatistics();  // Assign or update a variable that represents the current algorithm state.

            // 重新插入所有元素 - Reinsert all active elements
            foreach (var entry in oldTable)  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                if (entry != null && !entry.IsDeleted)  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    Insert(entry.Key, entry.Value);  // Execute this statement as part of the data structure implementation.
                }  // Close the current block scope.
            }  // Close the current block scope.

            // 恢復統計資訊 - Restore statistics
            _insertStats = tempInsert;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.

        // ========== 公開方法 Public Methods ==========

        /// <summary>
        /// 插入鍵值對（若 key 已存在則更新）
        /// Insert key-value pair (update if key exists)
        ///
        /// 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
        /// </summary>
        public void Insert(TKey key, TValue value)  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            int probes = 0;  // Advance or track the probing sequence used by open addressing.
            int firstDeletedIndex = -1;  // Assign or update a variable that represents the current algorithm state.

            for (int i = 0; i < _capacity; i++)  // Iterate over a range/collection to process each item in sequence.
            {  // Open a new block scope.
                probes++;  // Advance or track the probing sequence used by open addressing.
                int index = Probe(key, i);  // Assign or update a variable that represents the current algorithm state.
                var entry = _table[index];  // Assign or update a variable that represents the current algorithm state.

                // 找到空位 - Found empty slot
                if (entry == null)  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    // 如果之前遇到已刪除的位置，使用該位置 / If we encountered a deleted slot earlier, use that
                    if (firstDeletedIndex != -1)  // Evaluate the condition and branch into the appropriate code path.
                    {  // Open a new block scope.
                        _table[firstDeletedIndex] = new Entry(key, value);  // Assign or update a variable that represents the current algorithm state.
                        _deletedCount--;  // Handle tombstones so deletions do not break the probing/search sequence.
                    }  // Close the current block scope.
                    else  // Handle the alternative branch when the condition is false.
                    {  // Open a new block scope.
                        _table[index] = new Entry(key, value);  // Assign or update a variable that represents the current algorithm state.
                    }  // Close the current block scope.
                    _size++;  // Execute this statement as part of the data structure implementation.

                    // 更新統計資訊 - Update statistics
                    _insertStats.Operations++;  // Execute this statement as part of the data structure implementation.
                    _insertStats.TotalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                    _insertStats.MaxProbes = Math.Max(_insertStats.MaxProbes, probes);  // Advance or track the probing sequence used by open addressing.

                    // 檢查是否需要擴容 - Check if rehashing needed
                    if (LoadFactor > MaxLoadFactor)  // Evaluate the condition and branch into the appropriate code path.
                    {  // Open a new block scope.
                        Rehash();  // Compute a hash-based index so keys map into the table's storage.
                    }  // Close the current block scope.
                    return;  // Return the computed result to the caller.
                }  // Close the current block scope.

                // 找到已刪除的位置 - Found deleted slot
                if (entry.IsDeleted && firstDeletedIndex == -1)  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    firstDeletedIndex = index;  // Assign or update a variable that represents the current algorithm state.
                }  // Close the current block scope.

                // 找到相同的鍵，更新值 - Found same key, update value
                if (!entry.IsDeleted && Equals(entry.Key, key))  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    entry.Value = value;  // Assign or update a variable that represents the current algorithm state.
                    _insertStats.Operations++;  // Execute this statement as part of the data structure implementation.
                    _insertStats.TotalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                    _insertStats.MaxProbes = Math.Max(_insertStats.MaxProbes, probes);  // Advance or track the probing sequence used by open addressing.
                    return;  // Return the computed result to the caller.
                }  // Close the current block scope.
            }  // Close the current block scope.

            // 表格已滿，觸發擴容 - Table is full, trigger rehashing
            Rehash();  // Compute a hash-based index so keys map into the table's storage.
            Insert(key, value);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        /// <summary>
        /// 搜尋給定 key 的 value
        /// Search for value associated with key
        /// </summary>
        /// <returns>找到則回傳 value，否則回傳 default</returns>
        public TValue Search(TKey key)  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            TryGetValue(key, out TValue value);  // Execute this statement as part of the data structure implementation.
            return value;  // Return the computed result to the caller.
        }  // Close the current block scope.

        /// <summary>
        /// 嘗試取得給定 key 的 value
        /// Try to get value associated with key
        /// </summary>
        public bool TryGetValue(TKey key, out TValue value)  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            int probes = 0;  // Advance or track the probing sequence used by open addressing.

            for (int i = 0; i < _capacity; i++)  // Iterate over a range/collection to process each item in sequence.
            {  // Open a new block scope.
                probes++;  // Advance or track the probing sequence used by open addressing.
                int index = Probe(key, i);  // Assign or update a variable that represents the current algorithm state.
                var entry = _table[index];  // Assign or update a variable that represents the current algorithm state.

                // 找到空位，表示 key 不存在 - Found empty slot, key doesn't exist
                if (entry == null)  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    _searchStats.Operations++;  // Execute this statement as part of the data structure implementation.
                    _searchStats.TotalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                    _searchStats.MaxProbes = Math.Max(_searchStats.MaxProbes, probes);  // Advance or track the probing sequence used by open addressing.
                    value = default;  // Assign or update a variable that represents the current algorithm state.
                    return false;  // Return the computed result to the caller.
                }  // Close the current block scope.

                // 找到相同的鍵且未被刪除 - Found same key and not deleted
                if (!entry.IsDeleted && Equals(entry.Key, key))  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    _searchStats.Operations++;  // Execute this statement as part of the data structure implementation.
                    _searchStats.TotalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                    _searchStats.MaxProbes = Math.Max(_searchStats.MaxProbes, probes);  // Advance or track the probing sequence used by open addressing.
                    value = entry.Value;  // Assign or update a variable that represents the current algorithm state.
                    return true;  // Return the computed result to the caller.
                }  // Close the current block scope.

                // 繼續探測（已刪除或不同的鍵） - Continue probing (deleted or different key)
            }  // Close the current block scope.

            // 探測整個表格都沒找到 - Probed entire table without finding
            _searchStats.Operations++;  // Execute this statement as part of the data structure implementation.
            _searchStats.TotalProbes += probes;  // Advance or track the probing sequence used by open addressing.
            _searchStats.MaxProbes = Math.Max(_searchStats.MaxProbes, probes);  // Advance or track the probing sequence used by open addressing.
            value = default;  // Assign or update a variable that represents the current algorithm state.
            return false;  // Return the computed result to the caller.
        }  // Close the current block scope.

        /// <summary>
        /// 刪除指定的鍵值對（使用墓碑標記） / Delete key-value pair (using tombstone marker)
        /// </summary>
        /// <returns>刪除成功回傳 true，key 不存在回傳 false</returns>
        public bool Delete(TKey key)  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            int probes = 0;  // Advance or track the probing sequence used by open addressing.

            for (int i = 0; i < _capacity; i++)  // Iterate over a range/collection to process each item in sequence.
            {  // Open a new block scope.
                probes++;  // Advance or track the probing sequence used by open addressing.
                int index = Probe(key, i);  // Assign or update a variable that represents the current algorithm state.
                var entry = _table[index];  // Assign or update a variable that represents the current algorithm state.

                // 找到空位，表示 key 不存在 - Found empty slot, key doesn't exist
                if (entry == null)  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    _deleteStats.Operations++;  // Execute this statement as part of the data structure implementation.
                    _deleteStats.TotalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                    _deleteStats.MaxProbes = Math.Max(_deleteStats.MaxProbes, probes);  // Advance or track the probing sequence used by open addressing.
                    return false;  // Return the computed result to the caller.
                }  // Close the current block scope.

                // 找到相同的鍵且未被刪除 - Found same key and not deleted
                if (!entry.IsDeleted && Equals(entry.Key, key))  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    entry.IsDeleted = true;  // 設置墓碑標記 - Set tombstone marker
                    _size--;  // Execute this statement as part of the data structure implementation.
                    _deletedCount++;  // Handle tombstones so deletions do not break the probing/search sequence.

                    _deleteStats.Operations++;  // Execute this statement as part of the data structure implementation.
                    _deleteStats.TotalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                    _deleteStats.MaxProbes = Math.Max(_deleteStats.MaxProbes, probes);  // Advance or track the probing sequence used by open addressing.
                    return true;  // Return the computed result to the caller.
                }  // Close the current block scope.

                // 繼續探測 - Continue probing
            }  // Close the current block scope.

            // 探測整個表格都沒找到 - Probed entire table without finding
            _deleteStats.Operations++;  // Execute this statement as part of the data structure implementation.
            _deleteStats.TotalProbes += probes;  // Advance or track the probing sequence used by open addressing.
            _deleteStats.MaxProbes = Math.Max(_deleteStats.MaxProbes, probes);  // Advance or track the probing sequence used by open addressing.
            return false;  // Return the computed result to the caller.
        }  // Close the current block scope.

        /// <summary>
        /// 檢查 key 是否存在
        /// Check if key exists
        /// </summary>
        public bool Contains(TKey key)  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            return TryGetValue(key, out _);  // Return the computed result to the caller.
        }  // Close the current block scope.

        /// <summary>
        /// 清空雜湊表 / Clear all elements
        /// </summary>
        public void Clear()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            _table = new Entry[_capacity];  // Assign or update a variable that represents the current algorithm state.
            _size = 0;  // Assign or update a variable that represents the current algorithm state.
            _deletedCount = 0;  // Handle tombstones so deletions do not break the probing/search sequence.
        }  // Close the current block scope.

        /// <summary>
        /// 重設所有統計資訊 / Reset all statistics
        /// </summary>
        public void ResetStatistics()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            _insertStats.Reset();  // Execute this statement as part of the data structure implementation.
            _searchStats.Reset();  // Execute this statement as part of the data structure implementation.
            _deleteStats.Reset();  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        /// <summary>
        /// 取得已刪除元素數量 / Get number of deleted elements
        /// </summary>
        public int GetDeletedCount()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            return _deletedCount;  // Return the computed result to the caller.
        }  // Close the current block scope.

        /// <summary>
        /// 取得所有鍵 / Get all keys
        /// </summary>
        public IEnumerable<TKey> Keys  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            get  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                foreach (var entry in _table)  // Execute this statement as part of the data structure implementation.
                {  // Open a new block scope.
                    if (entry != null && !entry.IsDeleted)  // Evaluate the condition and branch into the appropriate code path.
                    {  // Open a new block scope.
                        yield return entry.Key;  // Execute this statement as part of the data structure implementation.
                    }  // Close the current block scope.
                }  // Close the current block scope.
            }  // Close the current block scope.
        }  // Close the current block scope.

        /// <summary>
        /// 取得所有值 / Get all values
        /// </summary>
        public IEnumerable<TValue> Values  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            get  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                foreach (var entry in _table)  // Execute this statement as part of the data structure implementation.
                {  // Open a new block scope.
                    if (entry != null && !entry.IsDeleted)  // Evaluate the condition and branch into the appropriate code path.
                    {  // Open a new block scope.
                        yield return entry.Value;  // Execute this statement as part of the data structure implementation.
                    }  // Close the current block scope.
                }  // Close the current block scope.
            }  // Close the current block scope.
        }  // Close the current block scope.

        // ========== IEnumerable 實作 ==========

        public IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            foreach (var entry in _table)  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                if (entry != null && !entry.IsDeleted)  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    yield return new KeyValuePair<TKey, TValue>(entry.Key, entry.Value);  // Execute this statement as part of the data structure implementation.
                }  // Close the current block scope.
            }  // Close the current block scope.
        }  // Close the current block scope.

        IEnumerator IEnumerable.GetEnumerator()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            return GetEnumerator();  // Return the computed result to the caller.
        }  // Close the current block scope.

        /// <summary>
        /// 字串表示 / String representation
        /// </summary>
        public override string ToString()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            var sb = new StringBuilder($"OpenAddressingHashTable[{_probeMethod}]{{");  // Advance or track the probing sequence used by open addressing.
            bool first = true;  // Assign or update a variable that represents the current algorithm state.

            foreach (var kvp in this)  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                if (!first) sb.Append(", ");  // Evaluate the condition and branch into the appropriate code path.
                sb.Append($"{kvp.Key}={kvp.Value}");  // Assign or update a variable that represents the current algorithm state.
                first = false;  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.

            sb.Append("}");  // Execute this statement as part of the data structure implementation.
            return sb.ToString();  // Return the computed result to the caller.
        }  // Close the current block scope.
    }  // Close the current block scope.
}  // Close the current block scope.
