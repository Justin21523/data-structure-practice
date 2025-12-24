/**
 * 雜湊表單元測試
 * Hash Table Unit Tests
 *
 * 使用 xUnit 測試框架
 * Using xUnit testing framework
 */

using System;
using System.Collections.Generic;
using System.Linq;
using Xunit;

namespace DataStructures.Tests
{
    public class HashTableTests
    {
        // ========== 基本操作測試 Basic Operations Tests ==========

        [Fact]
        public void CreateEmptyHashTable_ShouldHaveZeroSize()
        {
            // 測試建立空的雜湊表 - Test creating empty hash table
            var ht = new HashTable<string, int>();

            Assert.Equal(0, ht.Count);
            Assert.True(ht.IsEmpty);
            Assert.Equal(16, ht.Capacity); // 預設容量
        }

        [Fact]
        public void CreateWithCustomCapacity_ShouldUseSpecifiedCapacity()
        {
            // 測試使用自訂容量 - Test custom capacity
            var ht = new HashTable<string, int>(32);

            Assert.Equal(32, ht.Capacity);
        }

        [Fact]
        public void CreateWithInvalidCapacity_ShouldThrowException()
        {
            // 測試無效容量 - Test invalid capacity
            Assert.Throws<ArgumentException>(() => new HashTable<string, int>(0));
            Assert.Throws<ArgumentException>(() => new HashTable<string, int>(-1));
        }

        [Fact]
        public void InsertAndSearch_ShouldReturnCorrectValues()
        {
            // 測試插入與搜尋 - Test insert and search
            var ht = new HashTable<string, int>();
            ht.Insert("apple", 100);
            ht.Insert("banana", 200);

            Assert.Equal(100, ht.Search("apple"));
            Assert.Equal(200, ht.Search("banana"));
            Assert.Equal(default(int), ht.Search("cherry"));
        }

        [Fact]
        public void InsertExistingKey_ShouldUpdateValue()
        {
            // 測試更新已存在的鍵 - Test updating existing key
            var ht = new HashTable<string, int>();
            ht.Insert("apple", 100);
            ht.Insert("apple", 150);

            Assert.Equal(150, ht.Search("apple"));
            Assert.Equal(1, ht.Count); // 大小不應增加
        }

        [Fact]
        public void Delete_ShouldRemoveElement()
        {
            // 測試刪除操作 - Test delete operation
            var ht = new HashTable<string, int>();
            ht.Insert("apple", 100);
            ht.Insert("banana", 200);

            Assert.True(ht.Delete("apple"));
            Assert.Equal(default(int), ht.Search("apple"));
            Assert.Equal(1, ht.Count);
        }

        [Fact]
        public void DeleteNonexistent_ShouldReturnFalse()
        {
            // 測試刪除不存在的鍵 - Test deleting nonexistent key
            var ht = new HashTable<string, int>();
            ht.Insert("apple", 100);

            Assert.False(ht.Delete("banana"));
            Assert.Equal(1, ht.Count);
        }

        [Fact]
        public void Contains_ShouldReturnCorrectResult()
        {
            // 測試 Contains 方法 - Test Contains method
            var ht = new HashTable<string, int>();
            ht.Insert("apple", 100);

            Assert.True(ht.Contains("apple"));
            Assert.False(ht.Contains("banana"));
        }

        // ========== 特殊情況測試 Special Cases Tests ==========

        [Fact]
        public void InsertNullKey_ShouldWork()
        {
            // 測試 null 鍵 - Test null key
            var ht = new HashTable<string, int>();
            ht.Insert(null, 100);

            Assert.Equal(100, ht.Search(null));
            Assert.True(ht.Contains(null));
        }

        [Fact]
        public void Clear_ShouldRemoveAllElements()
        {
            // 測試清空操作 - Test clear operation
            var ht = new HashTable<string, int>();
            ht.Insert("a", 1);
            ht.Insert("b", 2);
            ht.Clear();

            Assert.True(ht.IsEmpty);
            Assert.Equal(0, ht.Count);
            Assert.Equal(default(int), ht.Search("a"));
        }

        // ========== 擴容測試 Rehashing Tests ==========

        [Fact]
        public void Rehash_ShouldOccurOnHighLoadFactor()
        {
            // 測試負載因子觸發擴容 - Test rehashing on load factor threshold
            var ht = new HashTable<string, int>(4);

            for (int i = 0; i < 4; i++)
            {
                ht.Insert($"key{i}", i);
            }

            // 應該已經擴容 - Should have rehashed
            Assert.True(ht.Capacity > 4);

            // 所有元素應該仍然可以存取 - All elements should still be accessible
            for (int i = 0; i < 4; i++)
            {
                Assert.Equal(i, ht.Search($"key{i}"));
            }
        }

        [Fact]
        public void LoadFactor_ShouldCalculateCorrectly()
        {
            // 測試負載因子計算 - Test load factor calculation
            var ht = new HashTable<string, int>(10);
            ht.Insert("a", 1);
            ht.Insert("b", 2);
            ht.Insert("c", 3);

            Assert.Equal(0.3, ht.LoadFactor, 3);
        }

        // ========== 索引子測試 Indexer Tests ==========

        [Fact]
        public void Indexer_ShouldGetAndSetValues()
        {
            // 測試索引子 - Test indexer
            var ht = new HashTable<string, int>();
            ht["apple"] = 100;

            Assert.Equal(100, ht["apple"]);
        }

        [Fact]
        public void Indexer_ShouldThrowOnMissingKey()
        {
            // 測試取得不存在的鍵 - Test getting nonexistent key
            var ht = new HashTable<string, int>();

            Assert.Throws<KeyNotFoundException>(() => _ = ht["nonexistent"]);
        }

        // ========== 迭代器測試 Iterator Tests ==========

        [Fact]
        public void Keys_ShouldReturnAllKeys()
        {
            // 測試 Keys 屬性 - Test Keys property
            var ht = new HashTable<string, int>();
            ht.Insert("a", 1);
            ht.Insert("b", 2);
            ht.Insert("c", 3);

            var keys = ht.Keys.ToHashSet();
            Assert.Equal(new HashSet<string> { "a", "b", "c" }, keys);
        }

        [Fact]
        public void Values_ShouldReturnAllValues()
        {
            // 測試 Values 屬性 - Test Values property
            var ht = new HashTable<string, int>();
            ht.Insert("a", 1);
            ht.Insert("b", 2);
            ht.Insert("c", 3);

            var values = ht.Values.ToHashSet();
            Assert.Equal(new HashSet<int> { 1, 2, 3 }, values);
        }

        [Fact]
        public void Enumeration_ShouldIterateAllElements()
        {
            // 測試迭代 - Test enumeration
            var ht = new HashTable<string, int>();
            ht.Insert("a", 1);
            ht.Insert("b", 2);
            ht.Insert("c", 3);

            int count = 0;
            foreach (var kvp in ht)
            {
                count++;
            }

            Assert.Equal(3, count);
        }

        // ========== 碰撞測試 Collision Tests ==========

        [Fact]
        public void MultipleItemsSameBucket_ShouldWorkCorrectly()
        {
            // 測試多個元素在同一桶 - Test multiple items in same bucket
            var ht = new HashTable<string, int>(1); // 強制所有元素碰撞

            ht.Insert("a", 1);
            ht.Insert("b", 2);
            ht.Insert("c", 3);

            Assert.Equal(1, ht.Search("a"));
            Assert.Equal(2, ht.Search("b"));
            Assert.Equal(3, ht.Search("c"));

            ht.Delete("b");
            Assert.Equal(default(int), ht.Search("b"));
            Assert.Equal(1, ht.Search("a"));
            Assert.Equal(3, ht.Search("c"));
        }

        // ========== 不同鍵類型測試 Different Key Types Tests ==========

        [Fact]
        public void IntegerKey_ShouldWorkCorrectly()
        {
            // 測試整數作為鍵 - Test integer as key
            var ht = new HashTable<int, string>();
            ht.Insert(1, "one");
            ht.Insert(2, "two");
            ht.Insert(3, "three");

            Assert.Equal("one", ht.Search(1));
            Assert.Equal("two", ht.Search(2));
            Assert.Equal("three", ht.Search(3));
        }
    }
}
