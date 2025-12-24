/** Doc block start
 * 雜湊表單元測試 / Hash Table Unit Tests
 *(blank line)
 * 使用 xUnit 測試框架
 * Using xUnit testing framework
 */  // End of block comment

using System;  // Execute this statement as part of the data structure implementation.
using System.Collections.Generic;  // Execute this statement as part of the data structure implementation.
using System.Linq;  // Execute this statement as part of the data structure implementation.
using Xunit;  // Execute this statement as part of the data structure implementation.

namespace DataStructures.Tests  // Execute this statement as part of the data structure implementation.
{  // Open a new block scope.
    public class HashTableTests  // Execute this statement as part of the data structure implementation.
    {  // Open a new block scope.
        // ========== 基本操作測試 Basic Operations Tests ==========

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void CreateEmptyHashTable_ShouldHaveZeroSize()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試建立空的雜湊表 - Test creating empty hash table
            var ht = new HashTable<string, int>();  // Assign or update a variable that represents the current algorithm state.

            Assert.Equal(0, ht.Count);  // Execute this statement as part of the data structure implementation.
            Assert.True(ht.IsEmpty);  // Execute this statement as part of the data structure implementation.
            Assert.Equal(16, ht.Capacity); // 預設容量
        }  // Close the current block scope.

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void CreateWithCustomCapacity_ShouldUseSpecifiedCapacity()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試使用自訂容量 - Test custom capacity
            var ht = new HashTable<string, int>(32);  // Assign or update a variable that represents the current algorithm state.

            Assert.Equal(32, ht.Capacity);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void CreateWithInvalidCapacity_ShouldThrowException()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試無效容量 - Test invalid capacity
            Assert.Throws<ArgumentException>(() => new HashTable<string, int>(0));  // Assign or update a variable that represents the current algorithm state.
            Assert.Throws<ArgumentException>(() => new HashTable<string, int>(-1));  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void InsertAndSearch_ShouldReturnCorrectValues()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試插入與搜尋 - Test insert and search
            var ht = new HashTable<string, int>();  // Assign or update a variable that represents the current algorithm state.
            ht.Insert("apple", 100);  // Execute this statement as part of the data structure implementation.
            ht.Insert("banana", 200);  // Execute this statement as part of the data structure implementation.

            Assert.Equal(100, ht.Search("apple"));  // Execute this statement as part of the data structure implementation.
            Assert.Equal(200, ht.Search("banana"));  // Execute this statement as part of the data structure implementation.
            Assert.Equal(default(int), ht.Search("cherry"));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void InsertExistingKey_ShouldUpdateValue()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試更新已存在的鍵 - Test updating existing key
            var ht = new HashTable<string, int>();  // Assign or update a variable that represents the current algorithm state.
            ht.Insert("apple", 100);  // Execute this statement as part of the data structure implementation.
            ht.Insert("apple", 150);  // Execute this statement as part of the data structure implementation.

            Assert.Equal(150, ht.Search("apple"));  // Execute this statement as part of the data structure implementation.
            Assert.Equal(1, ht.Count); // 大小不應增加
        }  // Close the current block scope.

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void Delete_ShouldRemoveElement()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試刪除操作 - Test delete operation
            var ht = new HashTable<string, int>();  // Assign or update a variable that represents the current algorithm state.
            ht.Insert("apple", 100);  // Execute this statement as part of the data structure implementation.
            ht.Insert("banana", 200);  // Execute this statement as part of the data structure implementation.

            Assert.True(ht.Delete("apple"));  // Execute this statement as part of the data structure implementation.
            Assert.Equal(default(int), ht.Search("apple"));  // Execute this statement as part of the data structure implementation.
            Assert.Equal(1, ht.Count);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void DeleteNonexistent_ShouldReturnFalse()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試刪除不存在的鍵 - Test deleting nonexistent key
            var ht = new HashTable<string, int>();  // Assign or update a variable that represents the current algorithm state.
            ht.Insert("apple", 100);  // Execute this statement as part of the data structure implementation.

            Assert.False(ht.Delete("banana"));  // Execute this statement as part of the data structure implementation.
            Assert.Equal(1, ht.Count);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void Contains_ShouldReturnCorrectResult()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試 Contains 方法 - Test Contains method
            var ht = new HashTable<string, int>();  // Assign or update a variable that represents the current algorithm state.
            ht.Insert("apple", 100);  // Execute this statement as part of the data structure implementation.

            Assert.True(ht.Contains("apple"));  // Execute this statement as part of the data structure implementation.
            Assert.False(ht.Contains("banana"));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        // ========== 特殊情況測試 Special Cases Tests ==========

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void InsertNullKey_ShouldWork()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試 null 鍵 - Test null key
            var ht = new HashTable<string, int>();  // Assign or update a variable that represents the current algorithm state.
            ht.Insert(null, 100);  // Execute this statement as part of the data structure implementation.

            Assert.Equal(100, ht.Search(null));  // Execute this statement as part of the data structure implementation.
            Assert.True(ht.Contains(null));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void Clear_ShouldRemoveAllElements()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試清空操作 - Test clear operation
            var ht = new HashTable<string, int>();  // Assign or update a variable that represents the current algorithm state.
            ht.Insert("a", 1);  // Execute this statement as part of the data structure implementation.
            ht.Insert("b", 2);  // Execute this statement as part of the data structure implementation.
            ht.Clear();  // Execute this statement as part of the data structure implementation.

            Assert.True(ht.IsEmpty);  // Execute this statement as part of the data structure implementation.
            Assert.Equal(0, ht.Count);  // Execute this statement as part of the data structure implementation.
            Assert.Equal(default(int), ht.Search("a"));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        // ========== 擴容測試 Rehashing Tests ==========

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void Rehash_ShouldOccurOnHighLoadFactor()  // Compute a hash-based index so keys map into the table's storage.
        {  // Open a new block scope.
            // 測試負載因子觸發擴容 - Test rehashing on load factor threshold
            var ht = new HashTable<string, int>(4);  // Assign or update a variable that represents the current algorithm state.

            for (int i = 0; i < 4; i++)  // Iterate over a range/collection to process each item in sequence.
            {  // Open a new block scope.
                ht.Insert($"key{i}", i);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.

            // 應該已經擴容 - Should have rehashed
            Assert.True(ht.Capacity > 4);  // Execute this statement as part of the data structure implementation.

            // 所有元素應該仍然可以存取 - All elements should still be accessible
            for (int i = 0; i < 4; i++)  // Iterate over a range/collection to process each item in sequence.
            {  // Open a new block scope.
                Assert.Equal(i, ht.Search($"key{i}"));  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        }  // Close the current block scope.

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void LoadFactor_ShouldCalculateCorrectly()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試負載因子計算 - Test load factor calculation
            var ht = new HashTable<string, int>(10);  // Assign or update a variable that represents the current algorithm state.
            ht.Insert("a", 1);  // Execute this statement as part of the data structure implementation.
            ht.Insert("b", 2);  // Execute this statement as part of the data structure implementation.
            ht.Insert("c", 3);  // Execute this statement as part of the data structure implementation.

            Assert.Equal(0.3, ht.LoadFactor, 3);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        // ========== 索引子測試 Indexer Tests ==========

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void Indexer_ShouldGetAndSetValues()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試索引子 - Test indexer
            var ht = new HashTable<string, int>();  // Assign or update a variable that represents the current algorithm state.
            ht["apple"] = 100;  // Assign or update a variable that represents the current algorithm state.

            Assert.Equal(100, ht["apple"]);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void Indexer_ShouldThrowOnMissingKey()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試取得不存在的鍵 - Test getting nonexistent key
            var ht = new HashTable<string, int>();  // Assign or update a variable that represents the current algorithm state.

            Assert.Throws<KeyNotFoundException>(() => _ = ht["nonexistent"]);  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.

        // ========== 迭代器測試 Iterator Tests ==========

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void Keys_ShouldReturnAllKeys()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試 Keys 屬性 - Test Keys property
            var ht = new HashTable<string, int>();  // Assign or update a variable that represents the current algorithm state.
            ht.Insert("a", 1);  // Execute this statement as part of the data structure implementation.
            ht.Insert("b", 2);  // Execute this statement as part of the data structure implementation.
            ht.Insert("c", 3);  // Execute this statement as part of the data structure implementation.

            var keys = ht.Keys.ToHashSet();  // Assign or update a variable that represents the current algorithm state.
            Assert.Equal(new HashSet<string> { "a", "b", "c" }, keys);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void Values_ShouldReturnAllValues()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試 Values 屬性 - Test Values property
            var ht = new HashTable<string, int>();  // Assign or update a variable that represents the current algorithm state.
            ht.Insert("a", 1);  // Execute this statement as part of the data structure implementation.
            ht.Insert("b", 2);  // Execute this statement as part of the data structure implementation.
            ht.Insert("c", 3);  // Execute this statement as part of the data structure implementation.

            var values = ht.Values.ToHashSet();  // Assign or update a variable that represents the current algorithm state.
            Assert.Equal(new HashSet<int> { 1, 2, 3 }, values);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void Enumeration_ShouldIterateAllElements()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試迭代 - Test enumeration
            var ht = new HashTable<string, int>();  // Assign or update a variable that represents the current algorithm state.
            ht.Insert("a", 1);  // Execute this statement as part of the data structure implementation.
            ht.Insert("b", 2);  // Execute this statement as part of the data structure implementation.
            ht.Insert("c", 3);  // Execute this statement as part of the data structure implementation.

            int count = 0;  // Assign or update a variable that represents the current algorithm state.
            foreach (var kvp in ht)  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                count++;  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.

            Assert.Equal(3, count);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        // ========== 碰撞測試 Collision Tests ==========

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void MultipleItemsSameBucket_ShouldWorkCorrectly()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試多個元素在同一桶 - Test multiple items in same bucket
            var ht = new HashTable<string, int>(1); // 強制所有元素碰撞

            ht.Insert("a", 1);  // Execute this statement as part of the data structure implementation.
            ht.Insert("b", 2);  // Execute this statement as part of the data structure implementation.
            ht.Insert("c", 3);  // Execute this statement as part of the data structure implementation.

            Assert.Equal(1, ht.Search("a"));  // Execute this statement as part of the data structure implementation.
            Assert.Equal(2, ht.Search("b"));  // Execute this statement as part of the data structure implementation.
            Assert.Equal(3, ht.Search("c"));  // Execute this statement as part of the data structure implementation.

            ht.Delete("b");  // Execute this statement as part of the data structure implementation.
            Assert.Equal(default(int), ht.Search("b"));  // Execute this statement as part of the data structure implementation.
            Assert.Equal(1, ht.Search("a"));  // Execute this statement as part of the data structure implementation.
            Assert.Equal(3, ht.Search("c"));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        // ========== 不同鍵類型測試 Different Key Types Tests ==========

        [Fact]  // Execute this statement as part of the data structure implementation.
        public void IntegerKey_ShouldWorkCorrectly()  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            // 測試整數作為鍵 - Test integer as key
            var ht = new HashTable<int, string>();  // Assign or update a variable that represents the current algorithm state.
            ht.Insert(1, "one");  // Execute this statement as part of the data structure implementation.
            ht.Insert(2, "two");  // Execute this statement as part of the data structure implementation.
            ht.Insert(3, "three");  // Execute this statement as part of the data structure implementation.

            Assert.Equal("one", ht.Search(1));  // Execute this statement as part of the data structure implementation.
            Assert.Equal("two", ht.Search(2));  // Execute this statement as part of the data structure implementation.
            Assert.Equal("three", ht.Search(3));  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.
}  // Close the current block scope.
