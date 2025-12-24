"""
雜湊函數（Hash Functions）- Python 實作
Various hash function implementations

包含整數雜湊與字串雜湊函數
Includes integer and string hash functions
"""

import math
from typing import Union


# ============================================================
# 整數雜湊函數 Integer Hash Functions
# ============================================================

def division_hash(key: int, m: int) -> int:
    """
    除法雜湊（Division Method）
    Division hash function

    h(k) = k mod m
    建議 m 為質數 - m should be prime

    Args:
        key: 整數鍵
        m: 雜湊表大小

    Returns:
        雜湊值 0 ~ m-1
    """
    return key % m


def multiplication_hash(key: int, m: int, A: float = None) -> int:
    """
    乘法雜湊（Multiplication Method）
    Multiplication hash function

    h(k) = floor(m * (k * A mod 1))
    Knuth 建議 A = (√5 - 1) / 2 ≈ 0.618

    Args:
        key: 整數鍵
        m: 雜湊表大小
        A: 常數（預設使用黃金比例）

    Returns:
        雜湊值 0 ~ m-1
    """
    if A is None:
        # 黃金比例的倒數 - Golden ratio conjugate
        A = (math.sqrt(5) - 1) / 2

    # k * A mod 1 取小數部分 - Fractional part of k * A
    fractional = (key * A) % 1
    return int(m * fractional)


def mid_square_hash(key: int, r: int) -> int:
    """
    中間平方法（Mid-Square Method）
    Mid-square hash function

    計算 k²，取中間 r 位數字
    Square k and extract middle r digits

    Args:
        key: 整數鍵
        r: 要取的位數

    Returns:
        雜湊值 0 ~ 10^r - 1
    """
    squared = key * key
    squared_str = str(squared)

    # 確保有足夠位數 - Ensure enough digits
    if len(squared_str) < r:
        return squared

    # 取中間 r 位 - Extract middle r digits
    mid = len(squared_str) // 2
    start = mid - r // 2
    end = start + r

    return int(squared_str[start:end])


def folding_hash(key: int, chunk_size: int, m: int) -> int:
    """
    摺疊法（Folding Method）
    Folding hash function

    將數字分段相加後取餘數
    Split number into chunks, sum them, take modulo

    Args:
        key: 整數鍵
        chunk_size: 每段的位數
        m: 雜湊表大小

    Returns:
        雜湊值 0 ~ m-1
    """
    key_str = str(abs(key))
    total = 0

    # 每 chunk_size 位一段 - Split into chunks
    for i in range(0, len(key_str), chunk_size):
        chunk = key_str[i:i + chunk_size]
        total += int(chunk)

    return total % m


# ============================================================
# 字串雜湊函數 String Hash Functions
# ============================================================

def simple_sum_hash(s: str, m: int) -> int:
    """
    簡單加總法（Simple Sum）
    Simple character sum hash

    問題：anagram 會有相同雜湊值
    Issue: anagrams produce same hash

    Args:
        s: 字串
        m: 雜湊表大小

    Returns:
        雜湊值 0 ~ m-1
    """
    return sum(ord(c) for c in s) % m


def polynomial_hash(s: str, a: int = 31, m: int = None) -> int:
    """
    多項式雜湊（Polynomial Hash）
    Polynomial rolling hash

    h(s) = s[0]*a^(n-1) + s[1]*a^(n-2) + ... + s[n-1]
    使用 Horner's method 計算

    Args:
        s: 字串
        a: 基數（常用 31, 33, 37）
        m: 雜湊表大小（None 表示不取餘數）

    Returns:
        雜湊值
    """
    h = 0
    for c in s:
        h = h * a + ord(c)
        if m:
            h %= m
    return h


def djb2_hash(s: str) -> int:
    """
    DJB2 雜湊（Daniel J. Bernstein）
    DJB2 hash function

    經典且高效的字串雜湊函數
    Classic and efficient string hash

    Args:
        s: 字串

    Returns:
        32 位元雜湊值
    """
    h = 5381
    for c in s:
        # h * 33 + c
        h = ((h << 5) + h) + ord(c)
    return h & 0xFFFFFFFF


def sdbm_hash(s: str) -> int:
    """
    SDBM 雜湊
    SDBM hash function

    用於 SDBM 資料庫
    Used in SDBM database

    Args:
        s: 字串

    Returns:
        32 位元雜湊值
    """
    h = 0
    for c in s:
        # h * 65599 + c
        h = ord(c) + (h << 6) + (h << 16) - h
    return h & 0xFFFFFFFF


def fnv1a_hash(s: str) -> int:
    """
    FNV-1a 雜湊（Fowler–Noll–Vo）
    FNV-1a hash function

    特點：XOR 在乘法之前
    Feature: XOR before multiply

    Args:
        s: 字串

    Returns:
        32 位元雜湊值
    """
    # FNV-1a 32-bit 參數 - FNV-1a 32-bit parameters
    FNV_OFFSET_BASIS = 2166136261
    FNV_PRIME = 16777619

    h = FNV_OFFSET_BASIS
    for c in s:
        h ^= ord(c)
        h *= FNV_PRIME
        h &= 0xFFFFFFFF
    return h


def jenkins_one_at_a_time(s: str) -> int:
    """
    Jenkins One-at-a-Time 雜湊
    Jenkins one-at-a-time hash

    由 Bob Jenkins 設計
    Designed by Bob Jenkins

    Args:
        s: 字串

    Returns:
        32 位元雜湊值
    """
    h = 0
    for c in s:
        h += ord(c)
        h += (h << 10)
        h ^= (h >> 6)
        h &= 0xFFFFFFFF

    h += (h << 3)
    h ^= (h >> 11)
    h += (h << 15)
    return h & 0xFFFFFFFF


# ============================================================
# 雜湊函數分析工具 Hash Function Analysis Tools
# ============================================================

def analyze_distribution(hash_func, keys: list, m: int) -> dict:
    """
    分析雜湊函數的分布情況
    Analyze distribution of a hash function

    Args:
        hash_func: 雜湊函數 (key, m) -> int
        keys: 鍵的列表
        m: 雜湊表大小

    Returns:
        分布統計資訊
    """
    buckets = [0] * m

    for key in keys:
        h = hash_func(key, m) if callable(hash_func) else hash_func(key) % m
        buckets[h] += 1

    non_empty = sum(1 for b in buckets if b > 0)
    max_count = max(buckets)
    min_count = min(buckets)
    avg_count = len(keys) / m

    # 計算標準差 - Calculate standard deviation
    variance = sum((b - avg_count) ** 2 for b in buckets) / m
    std_dev = math.sqrt(variance)

    return {
        "total_keys": len(keys),
        "buckets": m,
        "non_empty_buckets": non_empty,
        "max_bucket_size": max_count,
        "min_bucket_size": min_count,
        "avg_bucket_size": avg_count,
        "std_deviation": std_dev,
        "distribution": buckets
    }


# ============================================================
# 範例與測試 Example and Test
# ============================================================

if __name__ == "__main__":
    print("=== 雜湊函數示範 Hash Functions Demo ===\n")

    # 整數雜湊測試 - Integer hash test
    print("--- 整數雜湊函數 Integer Hash Functions ---")
    test_key = 123456
    m = 97  # 質數

    print(f"Key: {test_key}, m: {m}")
    print(f"  Division:       {division_hash(test_key, m)}")
    print(f"  Multiplication: {multiplication_hash(test_key, m)}")
    print(f"  Mid-Square (4): {mid_square_hash(test_key, 4)}")
    print(f"  Folding (3):    {folding_hash(test_key, 3, m)}")

    # 字串雜湊測試 - String hash test
    print("\n--- 字串雜湊函數 String Hash Functions ---")
    test_strings = ["hello", "world", "hash", "table", "python"]

    print(f"{'String':<10} {'DJB2':<12} {'FNV-1a':<12} {'Polynomial':<12}")
    print("-" * 50)
    for s in test_strings:
        print(f"{s:<10} {djb2_hash(s):<12} {fnv1a_hash(s):<12} {polynomial_hash(s):<12}")

    # Anagram 問題示範 - Anagram problem demo
    print("\n--- Anagram 問題 Anagram Problem ---")
    anagrams = ["abc", "bca", "cab"]
    print(f"Simple sum (m=100): {[simple_sum_hash(s, 100) for s in anagrams]}")
    print(f"DJB2:               {[djb2_hash(s) for s in anagrams]}")

    # 分布分析 - Distribution analysis
    print("\n--- 分布分析 Distribution Analysis ---")
    import random
    random.seed(42)
    random_strings = [f"key_{i}_{random.randint(0, 1000)}" for i in range(1000)]

    stats = analyze_distribution(
        lambda k, m: djb2_hash(k) % m,
        random_strings,
        128
    )
    print(f"DJB2 with 1000 keys, 128 buckets:")
    print(f"  Non-empty buckets: {stats['non_empty_buckets']}")
    print(f"  Max bucket size:   {stats['max_bucket_size']}")
    print(f"  Std deviation:     {stats['std_deviation']:.3f}")
