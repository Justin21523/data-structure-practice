"""Docstring start
雜湊表單元測試 / Hash Table Unit Tests

使用 pytest 測試框架
Using pytest testing framework
"""  # End of docstring

import pytest  # Import modules and symbols needed by this implementation.
from hash_table import HashTable, create_hash_table  # Import modules and symbols needed by this implementation.


class TestHashTableBasicOperations:  # Declare a class that encapsulates the data structure state and behavior.
    """基本操作測試 Basic Operations Tests"""

    def test_create_empty_hash_table(self):  # Define a function/method that implements one operation of this unit.
        """測試建立空的雜湊表 - Test creating empty hash table"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        assert ht.size == 0  # Assert an expected condition for this test case.
        assert ht.is_empty()  # Assert an expected condition for this test case.
        assert ht.capacity == 16  # 預設容量

    def test_create_with_custom_capacity(self):  # Define a function/method that implements one operation of this unit.
        """測試自訂容量 - Test custom capacity"""
        ht = HashTable(capacity=32)  # Create a new hash table instance to store key->value mappings.
        assert ht.capacity == 32  # Assert an expected condition for this test case.

    def test_invalid_capacity(self):  # Define a function/method that implements one operation of this unit.
        """測試無效容量 - Test invalid capacity"""
        with pytest.raises(ValueError):  # Execute this statement as part of the data structure implementation.
            HashTable(capacity=0)  # Create a new hash table instance to store key->value mappings.
        with pytest.raises(ValueError):  # Execute this statement as part of the data structure implementation.
            HashTable(capacity=-1)  # Create a new hash table instance to store key->value mappings.

    def test_insert_and_search(self):  # Define a function/method that implements one operation of this unit.
        """測試插入與搜尋 - Test insert and search"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("apple", 100)  # Insert or update the given key with the provided value.
        ht.insert("banana", 200)  # Insert or update the given key with the provided value.

        assert ht.search("apple") == 100  # Assert an expected condition for this test case.
        assert ht.search("banana") == 200  # Assert an expected condition for this test case.
        assert ht.search("cherry") is None  # Assert an expected condition for this test case.

    def test_insert_update_existing(self):  # Define a function/method that implements one operation of this unit.
        """測試更新已存在的鍵 - Test updating existing key"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("apple", 100)  # Insert or update the given key with the provided value.
        ht.insert("apple", 150)  # Insert or update the given key with the provided value.

        assert ht.search("apple") == 150  # Assert an expected condition for this test case.
        assert ht.size == 1  # 大小不應增加

    def test_delete(self):  # Define a function/method that implements one operation of this unit.
        """測試刪除操作 - Test delete operation"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("apple", 100)  # Insert or update the given key with the provided value.
        ht.insert("banana", 200)  # Insert or update the given key with the provided value.

        assert ht.delete("apple") is True  # Assert an expected condition for this test case.
        assert ht.search("apple") is None  # Assert an expected condition for this test case.
        assert ht.size == 1  # Assert an expected condition for this test case.

    def test_delete_nonexistent(self):  # Define a function/method that implements one operation of this unit.
        """測試刪除不存在的鍵 - Test deleting nonexistent key"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("apple", 100)  # Insert or update the given key with the provided value.

        assert ht.delete("banana") is False  # Assert an expected condition for this test case.
        assert ht.size == 1  # Assert an expected condition for this test case.

    def test_contains(self):  # Define a function/method that implements one operation of this unit.
        """測試 contains 方法 - Test contains method"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("apple", 100)  # Insert or update the given key with the provided value.

        assert ht.contains("apple") is True  # Assert an expected condition for this test case.
        assert ht.contains("banana") is False  # Assert an expected condition for this test case.


class TestHashTableSpecialCases:  # Declare a class that encapsulates the data structure state and behavior.
    """特殊情況測試 Special Cases Tests"""

    def test_insert_none_value(self):  # Define a function/method that implements one operation of this unit.
        """測試插入 None 值 - Test inserting None value"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("key", None)  # Insert or update the given key with the provided value.

        assert ht.search("key") is None  # Assert an expected condition for this test case.
        assert ht.contains("key") is True  # Assert an expected condition for this test case.

    def test_various_key_types(self):  # Define a function/method that implements one operation of this unit.
        """測試不同的鍵類型 - Test various key types"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("string_key", 1)  # Insert or update the given key with the provided value.
        ht.insert(123, 2)  # Insert or update the given key with the provided value.
        ht.insert((1, 2, 3), 3)  # 元組可以作為鍵

        assert ht.search("string_key") == 1  # Assert an expected condition for this test case.
        assert ht.search(123) == 2  # Assert an expected condition for this test case.
        assert ht.search((1, 2, 3)) == 3  # Assert an expected condition for this test case.

    def test_clear(self):  # Define a function/method that implements one operation of this unit.
        """測試清空操作 - Test clear operation"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("b", 2)  # Insert or update the given key with the provided value.
        ht.clear()  # Execute this statement as part of the data structure implementation.

        assert ht.is_empty()  # Assert an expected condition for this test case.
        assert ht.size == 0  # Assert an expected condition for this test case.
        assert ht.search("a") is None  # Assert an expected condition for this test case.


class TestHashTableRehashing:  # Declare a class that encapsulates the data structure state and behavior.
    """擴容測試 Rehashing Tests"""

    def test_rehash_on_load_factor(self):  # Define a function/method that implements one operation of this unit.
        """測試負載因子觸發擴容 - Test rehashing on load factor threshold"""
        # 使用小容量以快速觸發擴容 / Use 小 capacity 以快速觸發 resize
        ht = HashTable(capacity=4)  # Create a new hash table instance to store key->value mappings.

        # 插入足夠多的元素使負載因子超過 0.75 / Insert 足夠多的 elements 使 load factor 超過 0.75
        for i in range(4):  # Iterate over a range/collection to process each item in sequence.
            ht.insert(f"key{i}", i)  # Insert or update the given key with the provided value.

        # 應該已經擴容 / Should 已經 resize
        assert ht.capacity > 4  # Assert an expected condition for this test case.

        # 所有元素應該仍然可以存取 / 所有 elements should still 然 can 存取
        for i in range(4):  # Iterate over a range/collection to process each item in sequence.
            assert ht.search(f"key{i}") == i  # Assert an expected condition for this test case.

    def test_load_factor_calculation(self):  # Define a function/method that implements one operation of this unit.
        """測試負載因子計算 - Test load factor calculation"""
        ht = HashTable(capacity=10)  # Create a new hash table instance to store key->value mappings.
        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("b", 2)  # Insert or update the given key with the provided value.
        ht.insert("c", 3)  # Insert or update the given key with the provided value.

        assert ht.load_factor == 0.3  # Assert an expected condition for this test case.


class TestHashTableIterators:  # Declare a class that encapsulates the data structure state and behavior.
    """迭代器測試 Iterator Tests"""

    def test_keys_iterator(self):  # Define a function/method that implements one operation of this unit.
        """測試 keys 迭代器 - Test keys iterator"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("b", 2)  # Insert or update the given key with the provided value.
        ht.insert("c", 3)  # Insert or update the given key with the provided value.

        keys = set(ht.keys())  # Assign or update a variable that represents the current algorithm state.
        assert keys == {"a", "b", "c"}  # Assert an expected condition for this test case.

    def test_values_iterator(self):  # Define a function/method that implements one operation of this unit.
        """測試 values 迭代器 - Test values iterator"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("b", 2)  # Insert or update the given key with the provided value.
        ht.insert("c", 3)  # Insert or update the given key with the provided value.

        values = set(ht.values())  # Assign or update a variable that represents the current algorithm state.
        assert values == {1, 2, 3}  # Assert an expected condition for this test case.

    def test_items_iterator(self):  # Define a function/method that implements one operation of this unit.
        """測試 items 迭代器 - Test items iterator"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("b", 2)  # Insert or update the given key with the provided value.

        items = set(ht.items())  # Iterate over all stored (key, value) pairs in the hash table.
        assert items == {("a", 1), ("b", 2)}  # Assert an expected condition for this test case.


class TestHashTableMagicMethods:  # Declare a class that encapsulates the data structure state and behavior.
    """魔術方法測試 Magic Methods Tests"""

    def test_len(self):  # Define a function/method that implements one operation of this unit.
        """測試 len() 函數 - Test len() function"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        assert len(ht) == 0  # Assert an expected condition for this test case.

        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("b", 2)  # Insert or update the given key with the provided value.
        assert len(ht) == 2  # Assert an expected condition for this test case.

    def test_in_operator(self):  # Define a function/method that implements one operation of this unit.
        """測試 'in' 運算子 - Test 'in' operator"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("apple", 100)  # Insert or update the given key with the provided value.

        assert "apple" in ht  # Assert an expected condition for this test case.
        assert "banana" not in ht  # Assert an expected condition for this test case.

    def test_getitem(self):  # Define a function/method that implements one operation of this unit.
        """測試 [] 取值語法 - Test [] access syntax"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("apple", 100)  # Insert or update the given key with the provided value.

        assert ht["apple"] == 100  # Assert an expected condition for this test case.

    def test_getitem_key_error(self):  # Define a function/method that implements one operation of this unit.
        """測試 [] 取值不存在的鍵 - Test [] access with nonexistent key"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.

        with pytest.raises(KeyError):  # Execute this statement as part of the data structure implementation.
            _ = ht["nonexistent"]  # Assign or update a variable that represents the current algorithm state.

    def test_setitem(self):  # Define a function/method that implements one operation of this unit.
        """測試 [] 設值語法 - Test [] assignment syntax"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht["apple"] = 100  # Assign or update a variable that represents the current algorithm state.

        assert ht.search("apple") == 100  # Assert an expected condition for this test case.

    def test_delitem(self):  # Define a function/method that implements one operation of this unit.
        """測試 del 語法 - Test del syntax"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht["apple"] = 100  # Assign or update a variable that represents the current algorithm state.

        del ht["apple"]  # Execute this statement as part of the data structure implementation.
        assert "apple" not in ht  # Assert an expected condition for this test case.

    def test_delitem_key_error(self):  # Define a function/method that implements one operation of this unit.
        """測試 del 不存在的鍵 - Test del with nonexistent key"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.

        with pytest.raises(KeyError):  # Execute this statement as part of the data structure implementation.
            del ht["nonexistent"]  # Execute this statement as part of the data structure implementation.

    def test_iter(self):  # Define a function/method that implements one operation of this unit.
        """測試迭代 - Test iteration"""
        ht = HashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("b", 2)  # Insert or update the given key with the provided value.

        keys = set(ht)  # Assign or update a variable that represents the current algorithm state.
        assert keys == {"a", "b"}  # Assert an expected condition for this test case.


class TestCreateHashTableFunction:  # Declare a class that encapsulates the data structure state and behavior.
    """便利函式測試 Convenience Function Tests"""

    def test_create_with_initial_data(self):  # Define a function/method that implements one operation of this unit.
        """測試使用初始資料建立 - Test creation with initial data"""
        data = {"a": 1, "b": 2, "c": 3}  # Assign or update a variable that represents the current algorithm state.
        ht = create_hash_table(data)  # Compute a hash-based index so keys map into the table's storage.

        assert ht.size == 3  # Assert an expected condition for this test case.
        assert ht.search("a") == 1  # Assert an expected condition for this test case.
        assert ht.search("b") == 2  # Assert an expected condition for this test case.
        assert ht.search("c") == 3  # Assert an expected condition for this test case.

    def test_create_empty(self):  # Define a function/method that implements one operation of this unit.
        """測試建立空雜湊表 - Test creating empty hash table"""
        ht = create_hash_table()  # Compute a hash-based index so keys map into the table's storage.
        assert ht.is_empty()  # Assert an expected condition for this test case.


class TestHashTableCollisions:  # Declare a class that encapsulates the data structure state and behavior.
    """碰撞處理測試 Collision Handling Tests"""

    def test_multiple_items_same_bucket(self):  # Define a function/method that implements one operation of this unit.
        """Docstring start
        測試多個元素在同一桶中 / Test multiple items in same bucket

        注意：由於 Python hash() 的特性，我們無法直接控制碰撞，
        但可以透過小容量來增加碰撞機率 / 但 can 透過小 capacity 來增加 collision 機率
        """  # End of docstring
        ht = HashTable(capacity=1)  # 只有一個桶，所有元素必定碰撞

        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("b", 2)  # Insert or update the given key with the provided value.
        ht.insert("c", 3)  # Insert or update the given key with the provided value.

        assert ht.search("a") == 1  # Assert an expected condition for this test case.
        assert ht.search("b") == 2  # Assert an expected condition for this test case.
        assert ht.search("c") == 3  # Assert an expected condition for this test case.

        ht.delete("b")  # Remove the given key from the table if it exists and report success.
        assert ht.search("b") is None  # Assert an expected condition for this test case.
        assert ht.search("a") == 1  # Assert an expected condition for this test case.
        assert ht.search("c") == 3  # Assert an expected condition for this test case.


# ========== 執行測試 Run Tests ==========

if __name__ == "__main__":  # Evaluate the condition and branch into the appropriate code path.
    pytest.main([__file__, "-v"])  # Execute this statement as part of the data structure implementation.
