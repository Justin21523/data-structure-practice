/**
 * 雜湊表（Hash Table）- C# 實作
 * 使用鏈結法（chaining）處理碰撞
 *
 * Hash Table implementation using chaining for collision resolution
 */

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace DataStructures
{
    /// <summary>
    /// 雜湊表泛型類別
    /// Generic Hash Table class
    /// </summary>
    /// <typeparam name="TKey">鍵的型別（key type）</typeparam>
    /// <typeparam name="TValue">值的型別（value type）</typeparam>
    public class HashTable<TKey, TValue> : IEnumerable<KeyValuePair<TKey, TValue>>
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

        // ========== 常數 Constants ==========
        private const int DefaultCapacity = 16;
        private const double MaxLoadFactor = 0.75;

        // ========== 成員變數 Member Variables ==========
        private Entry[] _buckets;       // 桶陣列 - Array of buckets
        private int _capacity;          // 桶的數量 - Number of buckets
        private int _size;              // 元素數量 - Number of elements

        // ========== 建構子 Constructors ==========

        /// <summary>
        /// 使用預設容量建立雜湊表
        /// Create hash table with default capacity
        /// </summary>
        public HashTable() : this(DefaultCapacity) { }

        /// <summary>
        /// 使用指定容量建立雜湊表
        /// Create hash table with specified capacity
        /// </summary>
        /// <param name="capacity">桶的數量（number of buckets）</param>
        public HashTable(int capacity)
        {
            if (capacity <= 0)
            {
                throw new ArgumentException(
                    "容量必須為正整數 / Capacity must be positive", nameof(capacity));
            }
            _capacity = capacity;
            _size = 0;
            _buckets = new Entry[capacity];
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

            // 檢查 key 是否已存在 - Check if key exists
            while (current != null)
            {
                if (Equals(current.Key, key))
                {
                    current.Value = value;  // 更新 - Update existing
                    return;
                }
                current = current.Next;
            }

            // 新增節點到鏈結串列頭部 - Add new node at head
            var newEntry = new Entry(key, value)
            {
                Next = _buckets[index]
            };
            _buckets[index] = newEntry;
            _size++;

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

            while (current != null)
            {
                if (Equals(current.Key, key))
                {
                    value = current.Value;
                    return true;
                }
                current = current.Next;
            }

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

            while (current != null)
            {
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
                    return true;
                }
                prev = current;
                current = current.Next;
            }

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
            var sb = new StringBuilder("HashTable{");
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

    // ========== 範例程式 Example Program ==========

    public class HashTableDemo
    {
        public static void Main(string[] args)
        {
            Console.WriteLine("=== 雜湊表基本操作示範 Hash Table Basic Operations Demo ===\n");

            // 建立雜湊表 - Create hash table
            var ht = new HashTable<string, int>();

            // 插入操作 - Insert operations
            Console.WriteLine("插入鍵值對 Inserting key-value pairs:");
            ht.Insert("apple", 100);
            ht.Insert("banana", 200);
            ht.Insert("cherry", 300);
            ht["date"] = 400;  // 使用索引子
            Console.WriteLine($"  雜湊表: {ht}");
            Console.WriteLine($"  大小: {ht.Count}, 負載因子: {ht.LoadFactor:F3}\n");

            // 搜尋操作 - Search operations
            Console.WriteLine("搜尋操作 Search operations:");
            Console.WriteLine($"  Search('apple') = {ht.Search("apple")}");
            Console.WriteLine($"  ht['banana'] = {ht["banana"]}");
            Console.WriteLine($"  Contains('cherry') = {ht.Contains("cherry")}");
            Console.WriteLine($"  Contains('grape') = {ht.Contains("grape")}\n");

            // 更新操作 - Update operation
            Console.WriteLine("更新操作 Update operation:");
            ht["apple"] = 150;
            Console.WriteLine("  更新 apple 的值為 150");
            Console.WriteLine($"  ht['apple'] = {ht["apple"]}\n");

            // 刪除操作 - Delete operation
            Console.WriteLine("刪除操作 Delete operations:");
            ht.Delete("banana");
            Console.WriteLine($"  刪除 'banana' 後: {ht}");
            Console.WriteLine($"  大小: {ht.Count}\n");

            // 迭代操作 - Iteration
            Console.WriteLine("迭代操作 Iteration:");
            Console.WriteLine($"  所有鍵 Keys: {string.Join(", ", ht.Keys)}");
            Console.WriteLine($"  所有值 Values: {string.Join(", ", ht.Values)}");
        }
    }
}
