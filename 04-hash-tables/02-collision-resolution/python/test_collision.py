"""
碰撞處理單元測試
Collision Resolution Unit Tests
"""

import pytest
from chaining import ChainedHashTable
from open_addressing import OpenAddressingHashTable, ProbeMethod


class TestChainedHashTable:
    """鏈結法測試 - Chaining tests"""

    def test_basic_operations(self):
        """基本操作測試"""
        ht = ChainedHashTable()
        ht.insert("a", 1)
        ht.insert("b", 2)

        value, _ = ht.search("a")
        assert value == 1

        value, _ = ht.search("b")
        assert value == 2

    def test_collision_handling(self):
        """碰撞處理測試 - 使用小容量強制碰撞"""
        ht = ChainedHashTable(capacity=1)

        ht.insert("a", 1)
        ht.insert("b", 2)
        ht.insert("c", 3)

        value, _ = ht.search("a")
        assert value == 1
        value, _ = ht.search("b")
        assert value == 2
        value, _ = ht.search("c")
        assert value == 3

    def test_update_existing(self):
        """更新已存在的鍵"""
        ht = ChainedHashTable()
        ht.insert("a", 1)
        ht.insert("a", 100)

        value, _ = ht.search("a")
        assert value == 100
        assert len(ht) == 1

    def test_delete(self):
        """刪除測試"""
        ht = ChainedHashTable()
        ht.insert("a", 1)
        ht.insert("b", 2)

        success, _ = ht.delete("a")
        assert success
        value, _ = ht.search("a")
        assert value is None

    def test_chain_lengths(self):
        """鏈長度統計"""
        ht = ChainedHashTable(capacity=4)
        for i in range(10):
            ht.insert(f"key{i}", i)

        lengths = ht.get_chain_lengths()
        assert sum(lengths) == 10
        assert ht.max_chain_length() >= 1


class TestOpenAddressingLinear:
    """線性探測測試 - Linear probing tests"""

    def test_basic_operations(self):
        """基本操作測試"""
        ht = OpenAddressingHashTable(probe_method=ProbeMethod.LINEAR)
        success, _ = ht.insert("a", 1)
        assert success

        value, _ = ht.search("a")
        assert value == 1

    def test_collision_handling(self):
        """碰撞處理測試"""
        ht = OpenAddressingHashTable(capacity=7, probe_method=ProbeMethod.LINEAR)

        # 插入多個元素
        for i in range(5):
            success, _ = ht.insert(f"key{i}", i)
            assert success

        # 全部可以找到
        for i in range(5):
            value, _ = ht.search(f"key{i}")
            assert value == i

    def test_delete_with_tombstone(self):
        """墓碑刪除測試"""
        ht = OpenAddressingHashTable(capacity=7, probe_method=ProbeMethod.LINEAR)

        ht.insert("a", 1)
        ht.insert("b", 2)
        ht.insert("c", 3)

        # 刪除中間的元素
        success, _ = ht.delete("b")
        assert success

        # 後面的元素仍可找到
        value, _ = ht.search("c")
        assert value == 3

        # 被刪除的找不到
        value, _ = ht.search("b")
        assert value is None

    def test_insert_at_tombstone(self):
        """在墓碑位置插入"""
        ht = OpenAddressingHashTable(capacity=7, probe_method=ProbeMethod.LINEAR)

        ht.insert("a", 1)
        ht.insert("b", 2)
        ht.delete("a")

        # 新元素可能會插入墓碑位置
        ht.insert("c", 3)
        value, _ = ht.search("c")
        assert value == 3

    def test_rehash(self):
        """擴容測試"""
        ht = OpenAddressingHashTable(capacity=4, probe_method=ProbeMethod.LINEAR)

        # 插入足夠多元素觸發擴容
        for i in range(5):
            ht.insert(f"key{i}", i)

        assert ht.capacity > 4

        # 所有元素仍可存取
        for i in range(5):
            value, _ = ht.search(f"key{i}")
            assert value == i


class TestOpenAddressingQuadratic:
    """二次探測測試 - Quadratic probing tests"""

    def test_basic_operations(self):
        """基本操作測試"""
        ht = OpenAddressingHashTable(capacity=11, probe_method=ProbeMethod.QUADRATIC)

        for i in range(5):
            success, _ = ht.insert(f"key{i}", i)
            assert success

        for i in range(5):
            value, _ = ht.search(f"key{i}")
            assert value == i


class TestOpenAddressingDoubleHash:
    """雙重雜湊測試 - Double hashing tests"""

    def test_basic_operations(self):
        """基本操作測試"""
        ht = OpenAddressingHashTable(capacity=11, probe_method=ProbeMethod.DOUBLE_HASH)

        for i in range(5):
            success, _ = ht.insert(f"key{i}", i)
            assert success

        for i in range(5):
            value, _ = ht.search(f"key{i}")
            assert value == i

    def test_distribution(self):
        """分布測試 - 雙重雜湊應該有較好的分布"""
        ht = OpenAddressingHashTable(capacity=23, probe_method=ProbeMethod.DOUBLE_HASH)

        for i in range(15):
            ht.insert(f"key{i}", i)

        # 應該全部成功且效能合理
        assert len(ht) == 15
        assert ht.average_probes() < 3  # 合理的探測次數


class TestCompareProbeStatistics:
    """探測統計比較測試"""

    def test_average_probes_tracking(self):
        """平均探測次數追蹤"""
        ht = ChainedHashTable(capacity=4)

        ht.insert("a", 1)
        ht.insert("b", 2)
        ht.insert("c", 3)

        avg = ht.average_probes()
        assert avg > 0

    def test_probe_count_returned(self):
        """探測次數回傳"""
        ht = OpenAddressingHashTable(capacity=11, probe_method=ProbeMethod.LINEAR)

        _, probes = ht.insert("a", 1)
        assert probes >= 1

        _, probes = ht.search("a")
        assert probes >= 1


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
