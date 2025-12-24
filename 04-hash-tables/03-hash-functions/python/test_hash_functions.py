"""
雜湊函數單元測試
Hash Functions Unit Tests
"""

import pytest
import random
from hash_functions import (
    division_hash, multiplication_hash, mid_square_hash, folding_hash,
    simple_sum_hash, polynomial_hash, djb2_hash, fnv1a_hash,
    jenkins_one_at_a_time, analyze_distribution
)
from universal_hashing import (
    UniversalHashFamily, UniversalStringHashFamily, UniversalHashTable
)


class TestIntegerHashFunctions:
    """整數雜湊函數測試 - Integer hash function tests"""

    def test_division_hash_range(self):
        """除法雜湊值應在範圍內"""
        m = 97
        for key in range(1000):
            h = division_hash(key, m)
            assert 0 <= h < m

    def test_division_hash_deterministic(self):
        """除法雜湊應該是確定性的"""
        m = 97
        key = 12345
        h1 = division_hash(key, m)
        h2 = division_hash(key, m)
        assert h1 == h2

    def test_multiplication_hash_range(self):
        """乘法雜湊值應在範圍內"""
        m = 128
        for key in range(1000):
            h = multiplication_hash(key, m)
            assert 0 <= h < m

    def test_multiplication_hash_deterministic(self):
        """乘法雜湊應該是確定性的"""
        m = 128
        key = 12345
        h1 = multiplication_hash(key, m)
        h2 = multiplication_hash(key, m)
        assert h1 == h2

    def test_mid_square_hash(self):
        """中間平方法測試"""
        h = mid_square_hash(1234, 4)
        assert isinstance(h, int)
        assert h >= 0

    def test_folding_hash_range(self):
        """摺疊法雜湊值應在範圍內"""
        m = 100
        key = 123456789
        h = folding_hash(key, 3, m)
        assert 0 <= h < m


class TestStringHashFunctions:
    """字串雜湊函數測試 - String hash function tests"""

    def test_simple_sum_hash_range(self):
        """簡單加總雜湊值應在範圍內"""
        m = 100
        for s in ["hello", "world", "test", "hash"]:
            h = simple_sum_hash(s, m)
            assert 0 <= h < m

    def test_simple_sum_anagram_collision(self):
        """簡單加總對 anagram 產生碰撞"""
        m = 1000
        h1 = simple_sum_hash("abc", m)
        h2 = simple_sum_hash("bca", m)
        h3 = simple_sum_hash("cab", m)
        assert h1 == h2 == h3  # 這是預期的問題

    def test_djb2_no_anagram_collision(self):
        """DJB2 對 anagram 不應碰撞"""
        h1 = djb2_hash("abc")
        h2 = djb2_hash("bca")
        h3 = djb2_hash("cab")
        assert h1 != h2 and h2 != h3 and h1 != h3

    def test_djb2_deterministic(self):
        """DJB2 應該是確定性的"""
        s = "hello world"
        h1 = djb2_hash(s)
        h2 = djb2_hash(s)
        assert h1 == h2

    def test_fnv1a_deterministic(self):
        """FNV-1a 應該是確定性的"""
        s = "hello world"
        h1 = fnv1a_hash(s)
        h2 = fnv1a_hash(s)
        assert h1 == h2

    def test_fnv1a_32bit(self):
        """FNV-1a 應回傳 32 位元值"""
        h = fnv1a_hash("test string")
        assert 0 <= h <= 0xFFFFFFFF

    def test_jenkins_deterministic(self):
        """Jenkins 應該是確定性的"""
        s = "hello world"
        h1 = jenkins_one_at_a_time(s)
        h2 = jenkins_one_at_a_time(s)
        assert h1 == h2

    def test_polynomial_hash_range(self):
        """多項式雜湊值應在範圍內"""
        m = 1000
        for s in ["test", "hello", "world"]:
            h = polynomial_hash(s, 31, m)
            assert 0 <= h < m

    def test_empty_string(self):
        """空字串測試"""
        assert djb2_hash("") == 5381  # DJB2 初始值
        assert polynomial_hash("", 31, 100) == 0


class TestHashDistribution:
    """雜湊分布測試 - Hash distribution tests"""

    def test_djb2_distribution(self):
        """DJB2 應有合理的分布"""
        random.seed(42)
        keys = [f"key_{i}_{random.randint(0, 10000)}" for i in range(1000)]
        stats = analyze_distribution(
            lambda k, m: djb2_hash(k) % m,
            keys, 100
        )

        # 標準差不應太大 - Standard deviation shouldn't be too high
        assert stats["std_deviation"] < 5
        # 大部分桶應該有元素 - Most buckets should have elements
        assert stats["non_empty_buckets"] > 80

    def test_fnv1a_distribution(self):
        """FNV-1a 應有合理的分布"""
        random.seed(42)
        keys = [f"key_{i}" for i in range(1000)]
        stats = analyze_distribution(
            lambda k, m: fnv1a_hash(k) % m,
            keys, 100
        )

        assert stats["std_deviation"] < 5


class TestUniversalHashing:
    """通用雜湊測試 - Universal hashing tests"""

    def test_hash_range(self):
        """通用雜湊值應在範圍內"""
        m = 100
        uh = UniversalHashFamily(m)
        for key in range(1000):
            h = uh.hash(key)
            assert 0 <= h < m

    def test_regenerate_changes_hash(self):
        """重新生成應改變雜湊值"""
        m = 100
        uh = UniversalHashFamily(m)
        key = 12345

        results = []
        for _ in range(10):
            results.append(uh.hash(key))
            uh.regenerate()

        # 不太可能 10 次都一樣
        assert len(set(results)) > 1

    def test_collision_probability_bound(self):
        """碰撞機率應該有界"""
        m = 50
        trials = 5000
        collisions = 0
        x, y = 100, 200

        for _ in range(trials):
            uh = UniversalHashFamily(m)
            if uh(x) == uh(y):
                collisions += 1

        # 碰撞機率應接近 1/m
        probability = collisions / trials
        # 允許一些誤差 - Allow some variance
        assert probability < 2 / m

    def test_callable(self):
        """應該可以直接呼叫"""
        uh = UniversalHashFamily(100)
        h = uh(12345)
        assert isinstance(h, int)


class TestUniversalStringHashing:
    """字串通用雜湊測試 - Universal string hashing tests"""

    def test_hash_range(self):
        """字串通用雜湊值應在範圍內"""
        m = 100
        ush = UniversalStringHashFamily(m)
        for s in ["hello", "world", "test", "hash"]:
            h = ush.hash(s)
            assert 0 <= h < m

    def test_deterministic_within_instance(self):
        """同一實例應該是確定性的"""
        ush = UniversalStringHashFamily(100)
        s = "test"
        h1 = ush(s)
        h2 = ush(s)
        assert h1 == h2


class TestUniversalHashTable:
    """通用雜湊表測試 - Universal hash table tests"""

    def test_basic_operations(self):
        """基本操作測試"""
        ht = UniversalHashTable()
        ht.insert(1, "one")
        ht.insert(2, "two")

        assert ht.search(1) == "one"
        assert ht.search(2) == "two"
        assert ht.search(3) is None

    def test_update(self):
        """更新測試"""
        ht = UniversalHashTable()
        ht.insert(1, "one")
        ht.insert(1, "ONE")

        assert ht.search(1) == "ONE"
        assert ht.size == 1

    def test_delete(self):
        """刪除測試"""
        ht = UniversalHashTable()
        ht.insert(1, "one")
        ht.insert(2, "two")

        assert ht.delete(1) is True
        assert ht.search(1) is None
        assert ht.delete(1) is False

    def test_many_insertions(self):
        """大量插入測試"""
        ht = UniversalHashTable()
        for i in range(100):
            ht.insert(i, f"value_{i}")

        assert ht.size == 100
        for i in range(100):
            assert ht.search(i) == f"value_{i}"


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
