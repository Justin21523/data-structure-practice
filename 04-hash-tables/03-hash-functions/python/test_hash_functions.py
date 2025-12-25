"""Docstring start
雜湊函數單元測試 / Hash Functions Unit Tests
"""  # End of docstring

import unittest  # Use Python's built-in unittest so tests run in restricted/offline environments.
import random  # Import modules and symbols needed by this implementation.
from hash_functions import (  # Import modules and symbols needed by this implementation.
    division_hash, multiplication_hash, mid_square_hash, folding_hash,  # Execute this statement as part of the data structure implementation.
    simple_sum_hash, polynomial_hash, djb2_hash, fnv1a_hash,  # Execute this statement as part of the data structure implementation.
    jenkins_one_at_a_time, analyze_distribution  # Execute this statement as part of the data structure implementation.
)  # Execute this statement as part of the data structure implementation.
from universal_hashing import (  # Import modules and symbols needed by this implementation.
    UniversalHashFamily, UniversalStringHashFamily, UniversalHashTable  # Execute this statement as part of the data structure implementation.
)  # Execute this statement as part of the data structure implementation.


class TestIntegerHashFunctions(unittest.TestCase):  # Use unittest.TestCase so unittest discovery can run these tests.
    """整數雜湊函數測試 - Integer hash function tests"""

    def test_division_hash_range(self):  # Define a function/method that implements one operation of this unit.
        """除法雜湊值應在範圍內 / 除法 hash value should 在 range within"""
        m = 97  # Assign or update a variable that represents the current algorithm state.
        for key in range(1000):  # Iterate over a range/collection to process each item in sequence.
            h = division_hash(key, m)  # Compute the hash-based bucket index for the given key.
            assert 0 <= h < m  # Assert an expected condition for this test case.

    def test_division_hash_deterministic(self):  # Define a function/method that implements one operation of this unit.
        """除法雜湊應該是確定性的 / 除法 hash should 該是 deterministic 的"""
        m = 97  # Assign or update a variable that represents the current algorithm state.
        key = 12345  # Assign or update a variable that represents the current algorithm state.
        h1 = division_hash(key, m)  # Compute the hash-based bucket index for the given key.
        h2 = division_hash(key, m)  # Compute the hash-based bucket index for the given key.
        assert h1 == h2  # Assert an expected condition for this test case.

    def test_multiplication_hash_range(self):  # Define a function/method that implements one operation of this unit.
        """乘法雜湊值應在範圍內 / 乘法 hash value should 在 range within"""
        m = 128  # Assign or update a variable that represents the current algorithm state.
        for key in range(1000):  # Iterate over a range/collection to process each item in sequence.
            h = multiplication_hash(key, m)  # Compute the hash-based bucket index for the given key.
            assert 0 <= h < m  # Assert an expected condition for this test case.

    def test_multiplication_hash_deterministic(self):  # Define a function/method that implements one operation of this unit.
        """乘法雜湊應該是確定性的 / 乘法 hash should 該是 deterministic 的"""
        m = 128  # Assign or update a variable that represents the current algorithm state.
        key = 12345  # Assign or update a variable that represents the current algorithm state.
        h1 = multiplication_hash(key, m)  # Compute the hash-based bucket index for the given key.
        h2 = multiplication_hash(key, m)  # Compute the hash-based bucket index for the given key.
        assert h1 == h2  # Assert an expected condition for this test case.

    def test_mid_square_hash(self):  # Define a function/method that implements one operation of this unit.
        """中間平方法測試 / 中間平方法 test"""
        h = mid_square_hash(1234, 4)  # Compute the hash-based bucket index for the given key.
        assert isinstance(h, int)  # Assert an expected condition for this test case.
        assert h >= 0  # Assert an expected condition for this test case.

    def test_folding_hash_range(self):  # Define a function/method that implements one operation of this unit.
        """摺疊法雜湊值應在範圍內 / 摺疊法 hash value should 在 range within"""
        m = 100  # Assign or update a variable that represents the current algorithm state.
        key = 123456789  # Assign or update a variable that represents the current algorithm state.
        h = folding_hash(key, 3, m)  # Compute the hash-based bucket index for the given key.
        assert 0 <= h < m  # Assert an expected condition for this test case.


class TestStringHashFunctions(unittest.TestCase):  # Use unittest.TestCase so unittest discovery can run these tests.
    """字串雜湊函數測試 - String hash function tests"""

    def test_simple_sum_hash_range(self):  # Define a function/method that implements one operation of this unit.
        """簡單加總雜湊值應在範圍內 / 簡單加總 hash value should 在 range within"""
        m = 100  # Assign or update a variable that represents the current algorithm state.
        for s in ["hello", "world", "test", "hash"]:  # Iterate over a range/collection to process each item in sequence.
            h = simple_sum_hash(s, m)  # Compute the hash-based bucket index for the given key.
            assert 0 <= h < m  # Assert an expected condition for this test case.

    def test_simple_sum_anagram_collision(self):  # Define a function/method that implements one operation of this unit.
        """簡單加總對 anagram 產生碰撞"""
        m = 1000  # Assign or update a variable that represents the current algorithm state.
        h1 = simple_sum_hash("abc", m)  # Compute the hash-based bucket index for the given key.
        h2 = simple_sum_hash("bca", m)  # Compute the hash-based bucket index for the given key.
        h3 = simple_sum_hash("cab", m)  # Compute the hash-based bucket index for the given key.
        assert h1 == h2 == h3  # 這是預期的問題

    def test_djb2_no_anagram_collision(self):  # Define a function/method that implements one operation of this unit.
        """DJB2 對 anagram 不應碰撞"""
        h1 = djb2_hash("abc")  # Compute the hash-based bucket index for the given key.
        h2 = djb2_hash("bca")  # Compute the hash-based bucket index for the given key.
        h3 = djb2_hash("cab")  # Compute the hash-based bucket index for the given key.
        assert h1 != h2 and h2 != h3 and h1 != h3  # Assert an expected condition for this test case.

    def test_djb2_deterministic(self):  # Define a function/method that implements one operation of this unit.
        """DJB2 應該是確定性的"""
        s = "hello world"  # Assign or update a variable that represents the current algorithm state.
        h1 = djb2_hash(s)  # Compute the hash-based bucket index for the given key.
        h2 = djb2_hash(s)  # Compute the hash-based bucket index for the given key.
        assert h1 == h2  # Assert an expected condition for this test case.

    def test_fnv1a_deterministic(self):  # Define a function/method that implements one operation of this unit.
        """FNV-1a 應該是確定性的"""
        s = "hello world"  # Assign or update a variable that represents the current algorithm state.
        h1 = fnv1a_hash(s)  # Compute the hash-based bucket index for the given key.
        h2 = fnv1a_hash(s)  # Compute the hash-based bucket index for the given key.
        assert h1 == h2  # Assert an expected condition for this test case.

    def test_fnv1a_32bit(self):  # Define a function/method that implements one operation of this unit.
        """FNV-1a 應回傳 32 位元值"""
        h = fnv1a_hash("test string")  # Compute the hash-based bucket index for the given key.
        assert 0 <= h <= 0xFFFFFFFF  # Assert an expected condition for this test case.

    def test_jenkins_deterministic(self):  # Define a function/method that implements one operation of this unit.
        """Jenkins 應該是確定性的"""
        s = "hello world"  # Assign or update a variable that represents the current algorithm state.
        h1 = jenkins_one_at_a_time(s)  # Assign or update a variable that represents the current algorithm state.
        h2 = jenkins_one_at_a_time(s)  # Assign or update a variable that represents the current algorithm state.
        assert h1 == h2  # Assert an expected condition for this test case.

    def test_polynomial_hash_range(self):  # Define a function/method that implements one operation of this unit.
        """多項式雜湊值應在範圍內 / 多項式 hash value should 在 range within"""
        m = 1000  # Assign or update a variable that represents the current algorithm state.
        for s in ["test", "hello", "world"]:  # Iterate over a range/collection to process each item in sequence.
            h = polynomial_hash(s, 31, m)  # Compute the hash-based bucket index for the given key.
            assert 0 <= h < m  # Assert an expected condition for this test case.

    def test_empty_string(self):  # Define a function/method that implements one operation of this unit.
        """空字串測試 / Empty string test"""
        assert djb2_hash("") == 5381  # DJB2 初始值
        assert polynomial_hash("", 31, 100) == 0  # Assert an expected condition for this test case.


class TestHashDistribution(unittest.TestCase):  # Use unittest.TestCase so unittest discovery can run these tests.
    """雜湊分布測試 - Hash distribution tests"""

    def test_djb2_distribution(self):  # Define a function/method that implements one operation of this unit.
        """DJB2 應有合理的分布"""
        random.seed(42)  # Execute this statement as part of the data structure implementation.
        keys = [f"key_{i}_{random.randint(0, 10000)}" for i in range(1000)]  # Assign or update a variable that represents the current algorithm state.
        stats = analyze_distribution(  # Assign or update a variable that represents the current algorithm state.
            lambda k, m: djb2_hash(k) % m,  # Compute the hash-based bucket index for the given key.
            keys, 100  # Execute this statement as part of the data structure implementation.
        )  # Execute this statement as part of the data structure implementation.

        # 標準差不應太大 - Standard deviation shouldn't be too high
        assert stats["std_deviation"] < 5  # Assert an expected condition for this test case.
        # 大部分桶應該有元素 - Most buckets should have elements
        assert stats["non_empty_buckets"] > 80  # Assert an expected condition for this test case.

    def test_fnv1a_distribution(self):  # Define a function/method that implements one operation of this unit.
        """FNV-1a 應有合理的分布"""
        random.seed(42)  # Execute this statement as part of the data structure implementation.
        keys = [f"key_{i}" for i in range(1000)]  # Assign or update a variable that represents the current algorithm state.
        stats = analyze_distribution(  # Assign or update a variable that represents the current algorithm state.
            lambda k, m: fnv1a_hash(k) % m,  # Compute the hash-based bucket index for the given key.
            keys, 100  # Execute this statement as part of the data structure implementation.
        )  # Execute this statement as part of the data structure implementation.

        assert stats["std_deviation"] < 5  # Assert an expected condition for this test case.


class TestUniversalHashing(unittest.TestCase):  # Use unittest.TestCase so unittest discovery can run these tests.
    """通用雜湊測試 - Universal hashing tests"""

    def test_hash_range(self):  # Define a function/method that implements one operation of this unit.
        """通用雜湊值應在範圍內 / 通用 hash value should 在 range within"""
        m = 100  # Assign or update a variable that represents the current algorithm state.
        uh = UniversalHashFamily(m)  # Assign or update a variable that represents the current algorithm state.
        for key in range(1000):  # Iterate over a range/collection to process each item in sequence.
            h = uh.hash(key)  # Compute a hash-based index so keys map into the table's storage.
            assert 0 <= h < m  # Assert an expected condition for this test case.

    def test_regenerate_changes_hash(self):  # Define a function/method that implements one operation of this unit.
        """重新生成應改變雜湊值 / Regenerate should change hash value"""
        m = 100  # Assign or update a variable that represents the current algorithm state.
        uh = UniversalHashFamily(m)  # Assign or update a variable that represents the current algorithm state.
        key = 12345  # Assign or update a variable that represents the current algorithm state.

        results = []  # Assign or update a variable that represents the current algorithm state.
        for _ in range(10):  # Iterate over a range/collection to process each item in sequence.
            results.append(uh.hash(key))  # Compute a hash-based index so keys map into the table's storage.
            uh.regenerate()  # Execute this statement as part of the data structure implementation.

        # 不太可能 10 次都一樣 / Very unlikely 10 次都一樣
        assert len(set(results)) > 1  # Assert an expected condition for this test case.

    def test_collision_probability_bound(self):  # Define a function/method that implements one operation of this unit.
        """碰撞機率應該有界 / Collision probability should 該 bounded"""
        m = 50  # Assign or update a variable that represents the current algorithm state.
        trials = 5000  # Assign or update a variable that represents the current algorithm state.
        collisions = 0  # Assign or update a variable that represents the current algorithm state.
        x, y = 100, 200  # Assign or update a variable that represents the current algorithm state.

        for _ in range(trials):  # Iterate over a range/collection to process each item in sequence.
            uh = UniversalHashFamily(m)  # Assign or update a variable that represents the current algorithm state.
            if uh(x) == uh(y):  # Evaluate the condition and branch into the appropriate code path.
                collisions += 1  # Assign or update a variable that represents the current algorithm state.

        # 碰撞機率應接近 1/m
        probability = collisions / trials  # Advance or track the probing sequence used by open addressing.
        # 允許一些誤差 - Allow some variance
        assert probability < 2 / m  # Assert an expected condition for this test case.

    def test_callable(self):  # Define a function/method that implements one operation of this unit.
        """應該可以直接呼叫 / Should 該可以直接呼叫"""
        uh = UniversalHashFamily(100)  # Assign or update a variable that represents the current algorithm state.
        h = uh(12345)  # Assign or update a variable that represents the current algorithm state.
        assert isinstance(h, int)  # Assert an expected condition for this test case.


class TestUniversalStringHashing(unittest.TestCase):  # Use unittest.TestCase so unittest discovery can run these tests.
    """字串通用雜湊測試 - Universal string hashing tests"""

    def test_hash_range(self):  # Define a function/method that implements one operation of this unit.
        """字串通用雜湊值應在範圍內 / String 通用 hash value should 在 range within"""
        m = 100  # Assign or update a variable that represents the current algorithm state.
        ush = UniversalStringHashFamily(m)  # Assign or update a variable that represents the current algorithm state.
        for s in ["hello", "world", "test", "hash"]:  # Iterate over a range/collection to process each item in sequence.
            h = ush.hash(s)  # Compute a hash-based index so keys map into the table's storage.
            assert 0 <= h < m  # Assert an expected condition for this test case.

    def test_deterministic_within_instance(self):  # Define a function/method that implements one operation of this unit.
        """同一實例應該是確定性的 / 同一實例 should 該是 deterministic 的"""
        ush = UniversalStringHashFamily(100)  # Assign or update a variable that represents the current algorithm state.
        s = "test"  # Assign or update a variable that represents the current algorithm state.
        h1 = ush(s)  # Assign or update a variable that represents the current algorithm state.
        h2 = ush(s)  # Assign or update a variable that represents the current algorithm state.
        assert h1 == h2  # Assert an expected condition for this test case.


class TestUniversalHashTable(unittest.TestCase):  # Use unittest.TestCase so unittest discovery can run these tests.
    """通用雜湊表測試 - Universal hash table tests"""

    def test_basic_operations(self):  # Define a function/method that implements one operation of this unit.
        """基本操作測試 / Basic operations test"""
        ht = UniversalHashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert(1, "one")  # Insert or update the given key with the provided value.
        ht.insert(2, "two")  # Insert or update the given key with the provided value.

        assert ht.search(1) == "one"  # Assert an expected condition for this test case.
        assert ht.search(2) == "two"  # Assert an expected condition for this test case.
        assert ht.search(3) is None  # Assert an expected condition for this test case.

    def test_update(self):  # Define a function/method that implements one operation of this unit.
        """更新測試 / Update test"""
        ht = UniversalHashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert(1, "one")  # Insert or update the given key with the provided value.
        ht.insert(1, "ONE")  # Insert or update the given key with the provided value.

        assert ht.search(1) == "ONE"  # Assert an expected condition for this test case.
        assert ht.size == 1  # Assert an expected condition for this test case.

    def test_delete(self):  # Define a function/method that implements one operation of this unit.
        """刪除測試 / Delete test"""
        ht = UniversalHashTable()  # Create a new hash table instance to store key->value mappings.
        ht.insert(1, "one")  # Insert or update the given key with the provided value.
        ht.insert(2, "two")  # Insert or update the given key with the provided value.

        assert ht.delete(1) is True  # Assert an expected condition for this test case.
        assert ht.search(1) is None  # Assert an expected condition for this test case.
        assert ht.delete(1) is False  # Assert an expected condition for this test case.

    def test_many_insertions(self):  # Define a function/method that implements one operation of this unit.
        """大量插入測試 / Bulk insert test"""
        ht = UniversalHashTable()  # Create a new hash table instance to store key->value mappings.
        for i in range(100):  # Iterate over a range/collection to process each item in sequence.
            ht.insert(i, f"value_{i}")  # Insert or update the given key with the provided value.

        assert ht.size == 100  # Assert an expected condition for this test case.
        for i in range(100):  # Iterate over a range/collection to process each item in sequence.
            assert ht.search(i) == f"value_{i}"  # Assert an expected condition for this test case.


if __name__ == "__main__":  # Evaluate the condition and branch into the appropriate code path.
    unittest.main(verbosity=2)  # Run unittest with verbose output for a study-friendly test log.
