/** Doc block start
 * 鏈結法雜湊表（Chained Hash Table）- C# 實作
 * 使用鏈結法（separate chaining）處理碰撞，並追蹤探測統計資訊
 *(blank line)
 * Hash Table implementation using separate chaining for collision resolution
 * with probe statistics tracking
 */  // End of block comment

using System;  // Execute this statement as part of the data structure implementation.
using System.Collections;  // Execute this statement as part of the data structure implementation.
using System.Collections.Generic;  // Execute this statement as part of the data structure implementation.
using System.Text;  // Execute this statement as part of the data structure implementation.

namespace DataStructures  // Execute this statement as part of the data structure implementation.
{  // Open a new block scope.
    /// <summary>
    /// 鏈結法雜湊表泛型類別 / Generic Chained Hash Table class
    /// </summary>
    /// <typeparam name="TKey">鍵的型別（key type）</typeparam>
    /// <typeparam name="TValue">值的型別（value type）</typeparam>
    public class ChainedHashTable<TKey, TValue> : IEnumerable<KeyValuePair<TKey, TValue>>  // Execute this statement as part of the data structure implementation.
    {  // Open a new block scope.
        // ========== 內部類別 Inner Classes ==========

        /// <summary>
        /// 鍵值對節點 / Key-value pair node for chaining
        /// </summary>
        private class Entry  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            public TKey Key { get; set; }  // Execute this statement as part of the data structure implementation.
            public TValue Value { get; set; }  // Execute this statement as part of the data structure implementation.
            public Entry Next { get; set; }  // Execute this statement as part of the data structure implementation.

            public Entry(TKey key, TValue value)  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                Key = key;  // Assign or update a variable that represents the current algorithm state.
                Value = value;  // Assign or update a variable that represents the current algorithm state.
                Next = null;  // Assign or update a variable that represents the current algorithm state.
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
        private Entry[] _buckets;              // 桶陣列 - Array of buckets
        private int _capacity;                 // 桶的數量 - Number of buckets
        private int _size;                     // 元素數量 - Number of elements
        private ProbeStatistics _insertStats;  // 插入統計 - Insert statistics
        private ProbeStatistics _searchStats;  // 搜尋統計 - Search statistics
        private ProbeStatistics _deleteStats;  // 刪除統計 - Delete statistics

        // ========== 建構子 Constructors ==========

        /// <summary>
        /// 使用預設容量建立雜湊表 / Create hash table with default capacity
        /// </summary>
        public ChainedHashTable() : this(DefaultCapacity) { }  // Execute this statement as part of the data structure implementation.

        /// <summary>
        /// 使用指定容量建立雜湊表 / Create hash table with specified capacity
        /// </summary>
        /// <param name="capacity">桶的數量（number of buckets）</param>
        public ChainedHashTable(int capacity)  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            if (capacity <= 0)  // Evaluate the condition and branch into the appropriate code path.
            {  // Open a new block scope.
                throw new ArgumentException(  // Throw an exception to signal an invalid argument or operation.
                    "容量必須為正整數 / Capacity must be positive", nameof(capacity));  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
            _capacity = capacity;  // Assign or update a variable that represents the current algorithm state.
            _size = 0;  // Assign or update a variable that represents the current algorithm state.
            _buckets = new Entry[capacity];  // Access or update the bucket storage used to hold entries or chains.
            _insertStats = new ProbeStatistics();  // Assign or update a variable that represents the current algorithm state.
            _searchStats = new ProbeStatistics();  // Assign or update a variable that represents the current algorithm state.
            _deleteStats = new ProbeStatistics();  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.

        // ========== 屬性 Properties ==========

        /// <summary>
        /// 元素數量 - Number of elements
        /// </summary>
        public int Count => _size;  // Assign or update a variable that represents the current algorithm state.

        /// <summary>
        /// 桶的數量 - Number of buckets
        /// </summary>
        public int Capacity => _capacity;  // Assign or update a variable that represents the current algorithm state.

        /// <summary>
        /// 是否為空 - Is empty
        /// </summary>
        public bool IsEmpty => _size == 0;  // Execute this statement as part of the data structure implementation.

        /// <summary>
        /// 負載因子 - Load factor
        /// </summary>
        public double LoadFactor => (double)_size / _capacity;  // Assign or update a variable that represents the current algorithm state.

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
        /// 計算雜湊索引 / Compute hash index
        /// </summary>
        private int Hash(TKey key)  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            if (key == null) return 0;  // Evaluate the condition and branch into the appropriate code path.
            // 確保回傳正數 - Ensure positive value
            return Math.Abs(key.GetHashCode()) % _capacity;  // Return the computed result to the caller.
        }  // Close the current block scope.

        /// <summary>
        /// 擴容並重新雜湊 / Resize and rehash
        /// </summary>
        private void Rehash()  // Compute a hash-based index so keys map into the table's storage.
        {  // Open a new block scope.
            var oldBuckets = _buckets;  // Access or update the bucket storage used to hold entries or chains.
            _capacity *= 2;  // Assign or update a variable that represents the current algorithm state.
            _buckets = new Entry[_capacity];  // Access or update the bucket storage used to hold entries or chains.
            _size = 0;  // Assign or update a variable that represents the current algorithm state.

            // 暫時停止統計以避免計入重新雜湊的操作 / Temporarily disable statistics to exclude rehashing operations
            var tempInsert = _insertStats;  // Assign or update a variable that represents the current algorithm state.
            _insertStats = new ProbeStatistics();  // Assign or update a variable that represents the current algorithm state.

            // 重新插入所有元素 - Reinsert all elements
            foreach (var head in oldBuckets)  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                var current = head;  // Assign or update a variable that represents the current algorithm state.
                while (current != null)  // Repeat while the loop condition remains true.
                {  // Open a new block scope.
                    Insert(current.Key, current.Value);  // Execute this statement as part of the data structure implementation.
                    current = current.Next;  // Assign or update a variable that represents the current algorithm state.
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
            int index = Hash(key);  // Assign or update a variable that represents the current algorithm state.
            var current = _buckets[index];  // Access or update the bucket storage used to hold entries or chains.
            int probes = 1; // 第一次探測 - First probe

            // 檢查 key 是否已存在 - Check if key exists
            while (current != null)  // Repeat while the loop condition remains true.
            {  // Open a new block scope.
                if (Equals(current.Key, key))  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    current.Value = value;  // 更新 - Update existing
                    _insertStats.Operations++;  // Execute this statement as part of the data structure implementation.
                    _insertStats.TotalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                    _insertStats.MaxProbes = Math.Max(_insertStats.MaxProbes, probes);  // Advance or track the probing sequence used by open addressing.
                    return;  // Return the computed result to the caller.
                }  // Close the current block scope.
                current = current.Next;  // Assign or update a variable that represents the current algorithm state.
                probes++;  // Advance or track the probing sequence used by open addressing.
            }  // Close the current block scope.

            // 新增節點到鏈結串列頭部 - Add new node at head
            var newEntry = new Entry(key, value)  // Assign or update a variable that represents the current algorithm state.
            {  // Open a new block scope.
                Next = _buckets[index]  // Access or update the bucket storage used to hold entries or chains.
            };  // Execute this statement as part of the data structure implementation.
            _buckets[index] = newEntry;  // Access or update the bucket storage used to hold entries or chains.
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
            int index = Hash(key);  // Assign or update a variable that represents the current algorithm state.
            var current = _buckets[index];  // Access or update the bucket storage used to hold entries or chains.
            int probes = 0;  // Advance or track the probing sequence used by open addressing.

            while (current != null)  // Repeat while the loop condition remains true.
            {  // Open a new block scope.
                probes++;  // Advance or track the probing sequence used by open addressing.
                if (Equals(current.Key, key))  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    value = current.Value;  // Assign or update a variable that represents the current algorithm state.
                    _searchStats.Operations++;  // Execute this statement as part of the data structure implementation.
                    _searchStats.TotalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                    _searchStats.MaxProbes = Math.Max(_searchStats.MaxProbes, probes);  // Advance or track the probing sequence used by open addressing.
                    return true;  // Return the computed result to the caller.
                }  // Close the current block scope.
                current = current.Next;  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.

            // 未找到也要記錄統計 - Record statistics even when not found
            _searchStats.Operations++;  // Execute this statement as part of the data structure implementation.
            _searchStats.TotalProbes += probes;  // Advance or track the probing sequence used by open addressing.
            _searchStats.MaxProbes = Math.Max(_searchStats.MaxProbes, probes);  // Advance or track the probing sequence used by open addressing.

            value = default;  // Assign or update a variable that represents the current algorithm state.
            return false;  // Return the computed result to the caller.
        }  // Close the current block scope.

        /// <summary>
        /// 刪除指定的鍵值對 / Delete key-value pair
        /// </summary>
        /// <returns>刪除成功回傳 true，key 不存在回傳 false</returns>
        public bool Delete(TKey key)  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            int index = Hash(key);  // Assign or update a variable that represents the current algorithm state.
            var current = _buckets[index];  // Access or update the bucket storage used to hold entries or chains.
            Entry prev = null;  // Assign or update a variable that represents the current algorithm state.
            int probes = 0;  // Advance or track the probing sequence used by open addressing.

            while (current != null)  // Repeat while the loop condition remains true.
            {  // Open a new block scope.
                probes++;  // Advance or track the probing sequence used by open addressing.
                if (Equals(current.Key, key))  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    if (prev == null)  // Evaluate the condition and branch into the appropriate code path.
                    {  // Open a new block scope.
                        _buckets[index] = current.Next;  // Access or update the bucket storage used to hold entries or chains.
                    }  // Close the current block scope.
                    else  // Handle the alternative branch when the condition is false.
                    {  // Open a new block scope.
                        prev.Next = current.Next;  // Assign or update a variable that represents the current algorithm state.
                    }  // Close the current block scope.
                    _size--;  // Execute this statement as part of the data structure implementation.

                    // 更新統計資訊 - Update statistics
                    _deleteStats.Operations++;  // Execute this statement as part of the data structure implementation.
                    _deleteStats.TotalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                    _deleteStats.MaxProbes = Math.Max(_deleteStats.MaxProbes, probes);  // Advance or track the probing sequence used by open addressing.
                    return true;  // Return the computed result to the caller.
                }  // Close the current block scope.
                prev = current;  // Assign or update a variable that represents the current algorithm state.
                current = current.Next;  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.

            // 未找到也要記錄統計 - Record statistics even when not found
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
            _buckets = new Entry[_capacity];  // Access or update the bucket storage used to hold entries or chains.
            _size = 0;  // Assign or update a variable that represents the current algorithm state.
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
        /// 取得鏈結長度分布 / Get chain length distribution
        /// </summary>
        /// <returns>每個桶的鏈結長度（chain length for each bucket）</returns>
        public int[] GetChainLengths()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            int[] lengths = new int[_capacity];  // Assign or update a variable that represents the current algorithm state.
            for (int i = 0; i < _capacity; i++)  // Iterate over a range/collection to process each item in sequence.
            {  // Open a new block scope.
                int length = 0;  // Assign or update a variable that represents the current algorithm state.
                var current = _buckets[i];  // Access or update the bucket storage used to hold entries or chains.
                while (current != null)  // Repeat while the loop condition remains true.
                {  // Open a new block scope.
                    length++;  // Execute this statement as part of the data structure implementation.
                    current = current.Next;  // Assign or update a variable that represents the current algorithm state.
                }  // Close the current block scope.
                lengths[i] = length;  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.
            return lengths;  // Return the computed result to the caller.
        }  // Close the current block scope.

        /// <summary>
        /// 取得最長鏈結長度 / Get maximum chain length
        /// </summary>
        public int GetMaxChainLength()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            int maxLength = 0;  // Assign or update a variable that represents the current algorithm state.
            foreach (var length in GetChainLengths())  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                maxLength = Math.Max(maxLength, length);  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.
            return maxLength;  // Return the computed result to the caller.
        }  // Close the current block scope.

        /// <summary>
        /// 取得平均鏈結長度 / Get average chain length
        /// </summary>
        public double GetAverageChainLength()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            if (_size == 0) return 0;  // Evaluate the condition and branch into the appropriate code path.
            return (double)_size / _capacity;  // Return the computed result to the caller.
        }  // Close the current block scope.

        /// <summary>
        /// 取得所有鍵 / Get all keys
        /// </summary>
        public IEnumerable<TKey> Keys  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            get  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                foreach (var head in _buckets)  // Access or update the bucket storage used to hold entries or chains.
                {  // Open a new block scope.
                    var current = head;  // Assign or update a variable that represents the current algorithm state.
                    while (current != null)  // Repeat while the loop condition remains true.
                    {  // Open a new block scope.
                        yield return current.Key;  // Execute this statement as part of the data structure implementation.
                        current = current.Next;  // Assign or update a variable that represents the current algorithm state.
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
                foreach (var head in _buckets)  // Access or update the bucket storage used to hold entries or chains.
                {  // Open a new block scope.
                    var current = head;  // Assign or update a variable that represents the current algorithm state.
                    while (current != null)  // Repeat while the loop condition remains true.
                    {  // Open a new block scope.
                        yield return current.Value;  // Execute this statement as part of the data structure implementation.
                        current = current.Next;  // Assign or update a variable that represents the current algorithm state.
                    }  // Close the current block scope.
                }  // Close the current block scope.
            }  // Close the current block scope.
        }  // Close the current block scope.

        // ========== IEnumerable 實作 ==========

        public IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            foreach (var head in _buckets)  // Access or update the bucket storage used to hold entries or chains.
            {  // Open a new block scope.
                var current = head;  // Assign or update a variable that represents the current algorithm state.
                while (current != null)  // Repeat while the loop condition remains true.
                {  // Open a new block scope.
                    yield return new KeyValuePair<TKey, TValue>(current.Key, current.Value);  // Execute this statement as part of the data structure implementation.
                    current = current.Next;  // Assign or update a variable that represents the current algorithm state.
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
            var sb = new StringBuilder("ChainedHashTable{");  // Assign or update a variable that represents the current algorithm state.
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
