"""
雜湊表單元測試
Hash Table Unit Tests

使用 pytest 測試框架
Using pytest testing framework
"""

import pytest
from hash_table import HashTable, create_hash_table


class TestHashTableBasicOperations:
    """基本操作測試 Basic Operations Tests"""

    def test_create_empty_hash_table(self):
        """測試建立空的雜湊表 - Test creating empty hash table"""
        ht = HashTable()
        assert ht.size == 0
        assert ht.is_empty()
        assert ht.capacity == 16  # 預設容量

    def test_create_with_custom_capacity(self):
        """測試自訂容量 - Test custom capacity"""
        ht = HashTable(capacity=32)
        assert ht.capacity == 32

    def test_invalid_capacity(self):
        """測試無效容量 - Test invalid capacity"""
        with pytest.raises(ValueError):
            HashTable(capacity=0)
        with pytest.raises(ValueError):
            HashTable(capacity=-1)

    def test_insert_and_search(self):
        """測試插入與搜尋 - Test insert and search"""
        ht = HashTable()
        ht.insert("apple", 100)
        ht.insert("banana", 200)

        assert ht.search("apple") == 100
        assert ht.search("banana") == 200
        assert ht.search("cherry") is None

    def test_insert_update_existing(self):
        """測試更新已存在的鍵 - Test updating existing key"""
        ht = HashTable()
        ht.insert("apple", 100)
        ht.insert("apple", 150)

        assert ht.search("apple") == 150
        assert ht.size == 1  # 大小不應增加

    def test_delete(self):
        """測試刪除操作 - Test delete operation"""
        ht = HashTable()
        ht.insert("apple", 100)
        ht.insert("banana", 200)

        assert ht.delete("apple") is True
        assert ht.search("apple") is None
        assert ht.size == 1

    def test_delete_nonexistent(self):
        """測試刪除不存在的鍵 - Test deleting nonexistent key"""
        ht = HashTable()
        ht.insert("apple", 100)

        assert ht.delete("banana") is False
        assert ht.size == 1

    def test_contains(self):
        """測試 contains 方法 - Test contains method"""
        ht = HashTable()
        ht.insert("apple", 100)

        assert ht.contains("apple") is True
        assert ht.contains("banana") is False


class TestHashTableSpecialCases:
    """特殊情況測試 Special Cases Tests"""

    def test_insert_none_value(self):
        """測試插入 None 值 - Test inserting None value"""
        ht = HashTable()
        ht.insert("key", None)

        assert ht.search("key") is None
        assert ht.contains("key") is True

    def test_various_key_types(self):
        """測試不同的鍵類型 - Test various key types"""
        ht = HashTable()
        ht.insert("string_key", 1)
        ht.insert(123, 2)
        ht.insert((1, 2, 3), 3)  # 元組可以作為鍵

        assert ht.search("string_key") == 1
        assert ht.search(123) == 2
        assert ht.search((1, 2, 3)) == 3

    def test_clear(self):
        """測試清空操作 - Test clear operation"""
        ht = HashTable()
        ht.insert("a", 1)
        ht.insert("b", 2)
        ht.clear()

        assert ht.is_empty()
        assert ht.size == 0
        assert ht.search("a") is None


class TestHashTableRehashing:
    """擴容測試 Rehashing Tests"""

    def test_rehash_on_load_factor(self):
        """測試負載因子觸發擴容 - Test rehashing on load factor threshold"""
        # 使用小容量以快速觸發擴容
        ht = HashTable(capacity=4)

        # 插入足夠多的元素使負載因子超過 0.75
        for i in range(4):
            ht.insert(f"key{i}", i)

        # 應該已經擴容
        assert ht.capacity > 4

        # 所有元素應該仍然可以存取
        for i in range(4):
            assert ht.search(f"key{i}") == i

    def test_load_factor_calculation(self):
        """測試負載因子計算 - Test load factor calculation"""
        ht = HashTable(capacity=10)
        ht.insert("a", 1)
        ht.insert("b", 2)
        ht.insert("c", 3)

        assert ht.load_factor == 0.3


class TestHashTableIterators:
    """迭代器測試 Iterator Tests"""

    def test_keys_iterator(self):
        """測試 keys 迭代器 - Test keys iterator"""
        ht = HashTable()
        ht.insert("a", 1)
        ht.insert("b", 2)
        ht.insert("c", 3)

        keys = set(ht.keys())
        assert keys == {"a", "b", "c"}

    def test_values_iterator(self):
        """測試 values 迭代器 - Test values iterator"""
        ht = HashTable()
        ht.insert("a", 1)
        ht.insert("b", 2)
        ht.insert("c", 3)

        values = set(ht.values())
        assert values == {1, 2, 3}

    def test_items_iterator(self):
        """測試 items 迭代器 - Test items iterator"""
        ht = HashTable()
        ht.insert("a", 1)
        ht.insert("b", 2)

        items = set(ht.items())
        assert items == {("a", 1), ("b", 2)}


class TestHashTableMagicMethods:
    """魔術方法測試 Magic Methods Tests"""

    def test_len(self):
        """測試 len() 函數 - Test len() function"""
        ht = HashTable()
        assert len(ht) == 0

        ht.insert("a", 1)
        ht.insert("b", 2)
        assert len(ht) == 2

    def test_in_operator(self):
        """測試 'in' 運算子 - Test 'in' operator"""
        ht = HashTable()
        ht.insert("apple", 100)

        assert "apple" in ht
        assert "banana" not in ht

    def test_getitem(self):
        """測試 [] 取值語法 - Test [] access syntax"""
        ht = HashTable()
        ht.insert("apple", 100)

        assert ht["apple"] == 100

    def test_getitem_key_error(self):
        """測試 [] 取值不存在的鍵 - Test [] access with nonexistent key"""
        ht = HashTable()

        with pytest.raises(KeyError):
            _ = ht["nonexistent"]

    def test_setitem(self):
        """測試 [] 設值語法 - Test [] assignment syntax"""
        ht = HashTable()
        ht["apple"] = 100

        assert ht.search("apple") == 100

    def test_delitem(self):
        """測試 del 語法 - Test del syntax"""
        ht = HashTable()
        ht["apple"] = 100

        del ht["apple"]
        assert "apple" not in ht

    def test_delitem_key_error(self):
        """測試 del 不存在的鍵 - Test del with nonexistent key"""
        ht = HashTable()

        with pytest.raises(KeyError):
            del ht["nonexistent"]

    def test_iter(self):
        """測試迭代 - Test iteration"""
        ht = HashTable()
        ht.insert("a", 1)
        ht.insert("b", 2)

        keys = set(ht)
        assert keys == {"a", "b"}


class TestCreateHashTableFunction:
    """便利函式測試 Convenience Function Tests"""

    def test_create_with_initial_data(self):
        """測試使用初始資料建立 - Test creation with initial data"""
        data = {"a": 1, "b": 2, "c": 3}
        ht = create_hash_table(data)

        assert ht.size == 3
        assert ht.search("a") == 1
        assert ht.search("b") == 2
        assert ht.search("c") == 3

    def test_create_empty(self):
        """測試建立空雜湊表 - Test creating empty hash table"""
        ht = create_hash_table()
        assert ht.is_empty()


class TestHashTableCollisions:
    """碰撞處理測試 Collision Handling Tests"""

    def test_multiple_items_same_bucket(self):
        """
        測試多個元素在同一桶中
        Test multiple items in same bucket

        注意：由於 Python hash() 的特性，我們無法直接控制碰撞，
        但可以透過小容量來增加碰撞機率
        """
        ht = HashTable(capacity=1)  # 只有一個桶，所有元素必定碰撞

        ht.insert("a", 1)
        ht.insert("b", 2)
        ht.insert("c", 3)

        assert ht.search("a") == 1
        assert ht.search("b") == 2
        assert ht.search("c") == 3

        ht.delete("b")
        assert ht.search("b") is None
        assert ht.search("a") == 1
        assert ht.search("c") == 3


# ========== 執行測試 Run Tests ==========

if __name__ == "__main__":
    pytest.main([__file__, "-v"])
