"""Docstring start
碰撞處理單元測試 / Collision Resolution Unit Tests
"""  # End of docstring

import pytest  # Import modules and symbols needed by this implementation.
from chaining import ChainedHashTable  # Import modules and symbols needed by this implementation.
from open_addressing import OpenAddressingHashTable, ProbeMethod  # Import modules and symbols needed by this implementation.


class TestChainedHashTable:  # Declare a class that encapsulates the data structure state and behavior.
    """鏈結法測試 - Chaining tests"""

    def test_basic_operations(self):  # Define a function/method that implements one operation of this unit.
        """基本操作測試 / Basic operations test"""
        ht = ChainedHashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("b", 2)  # Insert or update the given key with the provided value.

        value, _ = ht.search("a")  # Look up the current value for the given key (None if the key is missing).
        assert value == 1  # Assert an expected condition for this test case.

        value, _ = ht.search("b")  # Look up the current value for the given key (None if the key is missing).
        assert value == 2  # Assert an expected condition for this test case.

    def test_collision_handling(self):  # Define a function/method that implements one operation of this unit.
        """碰撞處理測試 - 使用小容量強制碰撞 / Collision-handling test (force collisions with a small capacity)"""
        ht = ChainedHashTable(capacity=1)  # Create a new hash table instance to store key->value mappings.

        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("b", 2)  # Insert or update the given key with the provided value.
        ht.insert("c", 3)  # Insert or update the given key with the provided value.

        value, _ = ht.search("a")  # Look up the current value for the given key (None if the key is missing).
        assert value == 1  # Assert an expected condition for this test case.
        value, _ = ht.search("b")  # Look up the current value for the given key (None if the key is missing).
        assert value == 2  # Assert an expected condition for this test case.
        value, _ = ht.search("c")  # Look up the current value for the given key (None if the key is missing).
        assert value == 3  # Assert an expected condition for this test case.

    def test_update_existing(self):  # Define a function/method that implements one operation of this unit.
        """更新已存在的鍵 / Update an existing key"""
        ht = ChainedHashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("a", 100)  # Insert or update the given key with the provided value.

        value, _ = ht.search("a")  # Look up the current value for the given key (None if the key is missing).
        assert value == 100  # Assert an expected condition for this test case.
        assert len(ht) == 1  # Assert an expected condition for this test case.

    def test_delete(self):  # Define a function/method that implements one operation of this unit.
        """刪除測試 / Deletion test"""
        ht = ChainedHashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("b", 2)  # Insert or update the given key with the provided value.

        success, _ = ht.delete("a")  # Remove the given key from the table if it exists and report success.
        assert success  # Assert an expected condition for this test case.
        value, _ = ht.search("a")  # Look up the current value for the given key (None if the key is missing).
        assert value is None  # Assert an expected condition for this test case.

    def test_chain_lengths(self):  # Define a function/method that implements one operation of this unit.
        """鏈長度統計 / Chain-length statistics"""
        ht = ChainedHashTable(capacity=4)  # Create a new hash table instance to store key->value mappings.
        for i in range(10):  # Iterate over a range/collection to process each item in sequence.
            ht.insert(f"key{i}", i)  # Insert or update the given key with the provided value.

        lengths = ht.get_chain_lengths()  # Assign or update a variable that represents the current algorithm state.
        assert sum(lengths) == 10  # Assert an expected condition for this test case.
        assert ht.max_chain_length() >= 1  # Assert an expected condition for this test case.


class TestOpenAddressingLinear:  # Declare a class that encapsulates the data structure state and behavior.
    """線性探測測試 - Linear probing tests"""

    def test_basic_operations(self):  # Define a function/method that implements one operation of this unit.
        """基本操作測試 / Basic operations test"""
        ht = OpenAddressingHashTable(probe_method=ProbeMethod.LINEAR)  # Create a new hash table instance to store key->value mappings.
        success, _ = ht.insert("a", 1)  # Insert or update the given key with the provided value.
        assert success  # Assert an expected condition for this test case.

        value, _ = ht.search("a")  # Look up the current value for the given key (None if the key is missing).
        assert value == 1  # Assert an expected condition for this test case.

    def test_collision_handling(self):  # Define a function/method that implements one operation of this unit.
        """碰撞處理測試 / Collision-handling test"""
        ht = OpenAddressingHashTable(capacity=7, probe_method=ProbeMethod.LINEAR)  # Create a new hash table instance to store key->value mappings.

        # 插入多個元素 / Insert 多個 elements
        for i in range(5):  # Iterate over a range/collection to process each item in sequence.
            success, _ = ht.insert(f"key{i}", i)  # Insert or update the given key with the provided value.
            assert success  # Assert an expected condition for this test case.

        # 全部可以找到 / All can be found
        for i in range(5):  # Iterate over a range/collection to process each item in sequence.
            value, _ = ht.search(f"key{i}")  # Look up the current value for the given key (None if the key is missing).
            assert value == i  # Assert an expected condition for this test case.

    def test_delete_with_tombstone(self):  # Define a function/method that implements one operation of this unit.
        """墓碑刪除測試 / Tombstone-deletion test"""
        ht = OpenAddressingHashTable(capacity=7, probe_method=ProbeMethod.LINEAR)  # Create a new hash table instance to store key->value mappings.

        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("b", 2)  # Insert or update the given key with the provided value.
        ht.insert("c", 3)  # Insert or update the given key with the provided value.

        # 刪除中間的元素 / Delete middle 的 elements
        success, _ = ht.delete("b")  # Remove the given key from the table if it exists and report success.
        assert success  # Assert an expected condition for this test case.

        # 後面的元素仍可找到 / Later 的 elements still 可 be found
        value, _ = ht.search("c")  # Look up the current value for the given key (None if the key is missing).
        assert value == 3  # Assert an expected condition for this test case.

        # 被刪除的找不到 / 被 delete 的找不到
        value, _ = ht.search("b")  # Look up the current value for the given key (None if the key is missing).
        assert value is None  # Assert an expected condition for this test case.

    def test_insert_at_tombstone(self):  # Define a function/method that implements one operation of this unit.
        """在墓碑位置插入 / Insert into a tombstone slot"""
        ht = OpenAddressingHashTable(capacity=7, probe_method=ProbeMethod.LINEAR)  # Create a new hash table instance to store key->value mappings.

        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("b", 2)  # Insert or update the given key with the provided value.
        ht.delete("a")  # Remove the given key from the table if it exists and report success.

        # 新元素可能會插入墓碑位置 / New elements may 會 insert tombstone 位置
        ht.insert("c", 3)  # Insert or update the given key with the provided value.
        value, _ = ht.search("c")  # Look up the current value for the given key (None if the key is missing).
        assert value == 3  # Assert an expected condition for this test case.

    def test_rehash(self):  # Define a function/method that implements one operation of this unit.
        """擴容測試 / Resize/Rehash test"""
        ht = OpenAddressingHashTable(capacity=4, probe_method=ProbeMethod.LINEAR)  # Create a new hash table instance to store key->value mappings.

        # 插入足夠多元素觸發擴容 / Insert 足夠多 elements 觸發 resize
        for i in range(5):  # Iterate over a range/collection to process each item in sequence.
            ht.insert(f"key{i}", i)  # Insert or update the given key with the provided value.

        assert ht.capacity > 4  # Assert an expected condition for this test case.

        # 所有元素仍可存取 / 所有 elements still 可存取
        for i in range(5):  # Iterate over a range/collection to process each item in sequence.
            value, _ = ht.search(f"key{i}")  # Look up the current value for the given key (None if the key is missing).
            assert value == i  # Assert an expected condition for this test case.


class TestOpenAddressingQuadratic:  # Declare a class that encapsulates the data structure state and behavior.
    """二次探測測試 - Quadratic probing tests"""

    def test_basic_operations(self):  # Define a function/method that implements one operation of this unit.
        """基本操作測試 / Basic operations test"""
        ht = OpenAddressingHashTable(capacity=11, probe_method=ProbeMethod.QUADRATIC)  # Create a new hash table instance to store key->value mappings.

        for i in range(5):  # Iterate over a range/collection to process each item in sequence.
            success, _ = ht.insert(f"key{i}", i)  # Insert or update the given key with the provided value.
            assert success  # Assert an expected condition for this test case.

        for i in range(5):  # Iterate over a range/collection to process each item in sequence.
            value, _ = ht.search(f"key{i}")  # Look up the current value for the given key (None if the key is missing).
            assert value == i  # Assert an expected condition for this test case.


class TestOpenAddressingDoubleHash:  # Declare a class that encapsulates the data structure state and behavior.
    """雙重雜湊測試 - Double hashing tests"""

    def test_basic_operations(self):  # Define a function/method that implements one operation of this unit.
        """基本操作測試 / Basic operations test"""
        ht = OpenAddressingHashTable(capacity=11, probe_method=ProbeMethod.DOUBLE_HASH)  # Create a new hash table instance to store key->value mappings.

        for i in range(5):  # Iterate over a range/collection to process each item in sequence.
            success, _ = ht.insert(f"key{i}", i)  # Insert or update the given key with the provided value.
            assert success  # Assert an expected condition for this test case.

        for i in range(5):  # Iterate over a range/collection to process each item in sequence.
            value, _ = ht.search(f"key{i}")  # Look up the current value for the given key (None if the key is missing).
            assert value == i  # Assert an expected condition for this test case.

    def test_distribution(self):  # Define a function/method that implements one operation of this unit.
        """分布測試 - 雙重雜湊應該有較好的分布 / Distribution test (double hashing should distribute better)"""
        ht = OpenAddressingHashTable(capacity=23, probe_method=ProbeMethod.DOUBLE_HASH)  # Create a new hash table instance to store key->value mappings.

        for i in range(15):  # Iterate over a range/collection to process each item in sequence.
            ht.insert(f"key{i}", i)  # Insert or update the given key with the provided value.

        # 應該全部成功且效能合理 / Should all 成功且效能合理
        assert len(ht) == 15  # Assert an expected condition for this test case.
        assert ht.average_probes() < 3  # 合理的探測次數


class TestCompareProbeStatistics:  # Declare a class that encapsulates the data structure state and behavior.
    """探測統計比較測試 / Probe-statistics comparison test"""

    def test_average_probes_tracking(self):  # Define a function/method that implements one operation of this unit.
        """平均探測次數追蹤 / Track average probe count"""
        ht = ChainedHashTable(capacity=4)  # Create a new hash table instance to store key->value mappings.

        ht.insert("a", 1)  # Insert or update the given key with the provided value.
        ht.insert("b", 2)  # Insert or update the given key with the provided value.
        ht.insert("c", 3)  # Insert or update the given key with the provided value.

        avg = ht.average_probes()  # Advance or track the probing sequence used by open addressing.
        assert avg > 0  # Assert an expected condition for this test case.

    def test_probe_count_returned(self):  # Define a function/method that implements one operation of this unit.
        """探測次數回傳 / Return probe count"""
        ht = OpenAddressingHashTable(capacity=11, probe_method=ProbeMethod.LINEAR)  # Create a new hash table instance to store key->value mappings.

        _, probes = ht.insert("a", 1)  # Insert or update the given key with the provided value.
        assert probes >= 1  # Assert an expected condition for this test case.

        _, probes = ht.search("a")  # Look up the current value for the given key (None if the key is missing).
        assert probes >= 1  # Assert an expected condition for this test case.


if __name__ == "__main__":  # Evaluate the condition and branch into the appropriate code path.
    pytest.main([__file__, "-v"])  # Execute this statement as part of the data structure implementation.
