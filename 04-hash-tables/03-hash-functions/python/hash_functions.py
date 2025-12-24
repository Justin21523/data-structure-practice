"""Docstring start
雜湊函數（Hash Functions）- Python 實作
Various hash function implementations

包含整數雜湊與字串雜湊函數 / Includes integer and string hash functions
"""  # End of docstring

import math  # Import modules and symbols needed by this implementation.
from typing import Union  # Import modules and symbols needed by this implementation.


# ============================================================
# 整數雜湊函數 Integer Hash Functions
# ============================================================

def division_hash(key: int, m: int) -> int:  # Define a function/method that implements one operation of this unit.
    """Docstring start
    除法雜湊（Division Method）
    Division hash function

    h(k) = k mod m
    建議 m 為質數 - m should be prime

    Args:
        key: 整數鍵
        m: 雜湊表大小

    Returns:
        雜湊值 0 ~ m-1
    """  # End of docstring
    return key % m  # Return the computed result to the caller.


def multiplication_hash(key: int, m: int, A: float = None) -> int:  # Define a function/method that implements one operation of this unit.
    """Docstring start
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
    """  # End of docstring
    if A is None:  # Evaluate the condition and branch into the appropriate code path.
        # 黃金比例的倒數 - Golden ratio conjugate
        A = (math.sqrt(5) - 1) / 2  # Assign or update a variable that represents the current algorithm state.

    # k * A mod 1 取小數部分 - Fractional part of k * A
    fractional = (key * A) % 1  # Assign or update a variable that represents the current algorithm state.
    return int(m * fractional)  # Return the computed result to the caller.


def mid_square_hash(key: int, r: int) -> int:  # Define a function/method that implements one operation of this unit.
    """Docstring start
    中間平方法（Mid-Square Method）
    Mid-square hash function

    計算 k²，取中間 r 位數字
    Square k and extract middle r digits

    Args:
        key: 整數鍵
        r: 要取的位數

    Returns:
        雜湊值 0 ~ 10^r - 1
    """  # End of docstring
    squared = key * key  # Assign or update a variable that represents the current algorithm state.
    squared_str = str(squared)  # Assign or update a variable that represents the current algorithm state.

    # 確保有足夠位數 - Ensure enough digits
    if len(squared_str) < r:  # Evaluate the condition and branch into the appropriate code path.
        return squared  # Return the computed result to the caller.

    # 取中間 r 位 - Extract middle r digits
    mid = len(squared_str) // 2  # Assign or update a variable that represents the current algorithm state.
    start = mid - r // 2  # Assign or update a variable that represents the current algorithm state.
    end = start + r  # Assign or update a variable that represents the current algorithm state.

    return int(squared_str[start:end])  # Return the computed result to the caller.


def folding_hash(key: int, chunk_size: int, m: int) -> int:  # Define a function/method that implements one operation of this unit.
    """Docstring start
    摺疊法（Folding Method）
    Folding hash function

    將數字分段相加後取餘數 / Split number into chunks, sum them, take modulo

    Args:
        key: 整數鍵
        chunk_size: 每段的位數
        m: 雜湊表大小

    Returns:
        雜湊值 0 ~ m-1
    """  # End of docstring
    key_str = str(abs(key))  # Assign or update a variable that represents the current algorithm state.
    total = 0  # Assign or update a variable that represents the current algorithm state.

    # 每 chunk_size 位一段 - Split into chunks
    for i in range(0, len(key_str), chunk_size):  # Iterate over a range/collection to process each item in sequence.
        chunk = key_str[i:i + chunk_size]  # Assign or update a variable that represents the current algorithm state.
        total += int(chunk)  # Assign or update a variable that represents the current algorithm state.

    return total % m  # Return the computed result to the caller.


# ============================================================
# 字串雜湊函數 String Hash Functions
# ============================================================

def simple_sum_hash(s: str, m: int) -> int:  # Define a function/method that implements one operation of this unit.
    """Docstring start
    簡單加總法（Simple Sum）
    Simple character sum hash

    問題：anagram 會有相同雜湊值
    Issue: anagrams produce same hash

    Args:
        s: 字串
        m: 雜湊表大小

    Returns:
        雜湊值 0 ~ m-1
    """  # End of docstring
    return sum(ord(c) for c in s) % m  # Return the computed result to the caller.


def polynomial_hash(s: str, a: int = 31, m: int = None) -> int:  # Define a function/method that implements one operation of this unit.
    """Docstring start
    多項式雜湊（Polynomial Hash）
    Polynomial rolling hash

    h(s) = s[0]*a^(n-1) + s[1]*a^(n-2) + ... + s[n-1]
    使用 Horner's method 計算

    Args:
        s: 字串
        a: 基數（常用 31, 33, 37）
        m: 雜湊表大小（None 表示不取餘數）

    Returns:
        雜湊值 / Hash value
    """  # End of docstring
    h = 0  # Assign or update a variable that represents the current algorithm state.
    for c in s:  # Iterate over a range/collection to process each item in sequence.
        h = h * a + ord(c)  # Assign or update a variable that represents the current algorithm state.
        if m:  # Evaluate the condition and branch into the appropriate code path.
            h %= m  # Assign or update a variable that represents the current algorithm state.
    return h  # Return the computed result to the caller.


def djb2_hash(s: str) -> int:  # Define a function/method that implements one operation of this unit.
    """Docstring start
    DJB2 雜湊（Daniel J. Bernstein）
    DJB2 hash function

    經典且高效的字串雜湊函數 / Classic and efficient string hash

    Args:
        s: 字串

    Returns:
        32 位元雜湊值 / 32 位元 hash value
    """  # End of docstring
    h = 5381  # Assign or update a variable that represents the current algorithm state.
    for c in s:  # Iterate over a range/collection to process each item in sequence.
        # h * 33 + c
        h = ((h << 5) + h) + ord(c)  # Assign or update a variable that represents the current algorithm state.
    return h & 0xFFFFFFFF  # Return the computed result to the caller.


def sdbm_hash(s: str) -> int:  # Define a function/method that implements one operation of this unit.
    """Docstring start
    SDBM 雜湊
    SDBM hash function

    用於 SDBM 資料庫
    Used in SDBM database

    Args:
        s: 字串

    Returns:
        32 位元雜湊值 / 32 位元 hash value
    """  # End of docstring
    h = 0  # Assign or update a variable that represents the current algorithm state.
    for c in s:  # Iterate over a range/collection to process each item in sequence.
        # h * 65599 + c
        h = ord(c) + (h << 6) + (h << 16) - h  # Assign or update a variable that represents the current algorithm state.
    return h & 0xFFFFFFFF  # Return the computed result to the caller.


def fnv1a_hash(s: str) -> int:  # Define a function/method that implements one operation of this unit.
    """Docstring start
    FNV-1a 雜湊（Fowler–Noll–Vo）
    FNV-1a hash function

    特點：XOR 在乘法之前
    Feature: XOR before multiply

    Args:
        s: 字串

    Returns:
        32 位元雜湊值 / 32 位元 hash value
    """  # End of docstring
    # FNV-1a 32-bit 參數 - FNV-1a 32-bit parameters
    FNV_OFFSET_BASIS = 2166136261  # Assign or update a variable that represents the current algorithm state.
    FNV_PRIME = 16777619  # Assign or update a variable that represents the current algorithm state.

    h = FNV_OFFSET_BASIS  # Assign or update a variable that represents the current algorithm state.
    for c in s:  # Iterate over a range/collection to process each item in sequence.
        h ^= ord(c)  # Assign or update a variable that represents the current algorithm state.
        h *= FNV_PRIME  # Assign or update a variable that represents the current algorithm state.
        h &= 0xFFFFFFFF  # Assign or update a variable that represents the current algorithm state.
    return h  # Return the computed result to the caller.


def jenkins_one_at_a_time(s: str) -> int:  # Define a function/method that implements one operation of this unit.
    """Docstring start
    Jenkins One-at-a-Time 雜湊
    Jenkins one-at-a-time hash

    由 Bob Jenkins 設計
    Designed by Bob Jenkins

    Args:
        s: 字串

    Returns:
        32 位元雜湊值 / 32 位元 hash value
    """  # End of docstring
    h = 0  # Assign or update a variable that represents the current algorithm state.
    for c in s:  # Iterate over a range/collection to process each item in sequence.
        h += ord(c)  # Assign or update a variable that represents the current algorithm state.
        h += (h << 10)  # Assign or update a variable that represents the current algorithm state.
        h ^= (h >> 6)  # Assign or update a variable that represents the current algorithm state.
        h &= 0xFFFFFFFF  # Assign or update a variable that represents the current algorithm state.

    h += (h << 3)  # Assign or update a variable that represents the current algorithm state.
    h ^= (h >> 11)  # Assign or update a variable that represents the current algorithm state.
    h += (h << 15)  # Assign or update a variable that represents the current algorithm state.
    return h & 0xFFFFFFFF  # Return the computed result to the caller.


# ============================================================
# 雜湊函數分析工具 Hash Function Analysis Tools
# ============================================================

def analyze_distribution(hash_func, keys: list, m: int) -> dict:  # Define a function/method that implements one operation of this unit.
    """Docstring start
    分析雜湊函數的分布情況 / Analyze distribution of a hash function

    Args:
        hash_func: 雜湊函數 (key, m) -> int
        keys: 鍵的列表
        m: 雜湊表大小

    Returns:
        分布統計資訊 / Distribution count 資訊
    """  # End of docstring
    buckets = [0] * m  # Access or update the bucket storage used to hold entries or chains.

    for key in keys:  # Iterate over a range/collection to process each item in sequence.
        h = hash_func(key, m) if callable(hash_func) else hash_func(key) % m  # Compute a hash-based index so keys map into the table's storage.
        buckets[h] += 1  # Access or update the bucket storage used to hold entries or chains.

    non_empty = sum(1 for b in buckets if b > 0)  # Access or update the bucket storage used to hold entries or chains.
    max_count = max(buckets)  # Access or update the bucket storage used to hold entries or chains.
    min_count = min(buckets)  # Access or update the bucket storage used to hold entries or chains.
    avg_count = len(keys) / m  # Assign or update a variable that represents the current algorithm state.

    # 計算標準差 - Calculate standard deviation
    variance = sum((b - avg_count) ** 2 for b in buckets) / m  # Access or update the bucket storage used to hold entries or chains.
    std_dev = math.sqrt(variance)  # Assign or update a variable that represents the current algorithm state.

    return {  # Return the computed result to the caller.
        "total_keys": len(keys),  # Execute this statement as part of the data structure implementation.
        "buckets": m,  # Access or update the bucket storage used to hold entries or chains.
        "non_empty_buckets": non_empty,  # Access or update the bucket storage used to hold entries or chains.
        "max_bucket_size": max_count,  # Access or update the bucket storage used to hold entries or chains.
        "min_bucket_size": min_count,  # Access or update the bucket storage used to hold entries or chains.
        "avg_bucket_size": avg_count,  # Access or update the bucket storage used to hold entries or chains.
        "std_deviation": std_dev,  # Execute this statement as part of the data structure implementation.
        "distribution": buckets  # Access or update the bucket storage used to hold entries or chains.
    }  # Close the current block scope.


# ============================================================
# 範例與測試 Example and Test
# ============================================================

if __name__ == "__main__":  # Evaluate the condition and branch into the appropriate code path.
    print("=== 雜湊函數示範 Hash Functions Demo ===\n")  # Print a formatted message for the interactive example output.

    # 整數雜湊測試 - Integer hash test
    print("--- 整數雜湊函數 Integer Hash Functions ---")  # Print a formatted message for the interactive example output.
    test_key = 123456  # Assign or update a variable that represents the current algorithm state.
    m = 97  # 質數

    print(f"Key: {test_key}, m: {m}")  # Print a formatted message for the interactive example output.
    print(f"  Division:       {division_hash(test_key, m)}")  # Compute the hash-based bucket index for the given key.
    print(f"  Multiplication: {multiplication_hash(test_key, m)}")  # Compute the hash-based bucket index for the given key.
    print(f"  Mid-Square (4): {mid_square_hash(test_key, 4)}")  # Compute the hash-based bucket index for the given key.
    print(f"  Folding (3):    {folding_hash(test_key, 3, m)}")  # Compute the hash-based bucket index for the given key.

    # 字串雜湊測試 - String hash test
    print("\n--- 字串雜湊函數 String Hash Functions ---")  # Print a formatted message for the interactive example output.
    test_strings = ["hello", "world", "hash", "table", "python"]  # Assign or update a variable that represents the current algorithm state.

    print(f"{'String':<10} {'DJB2':<12} {'FNV-1a':<12} {'Polynomial':<12}")  # Print a formatted message for the interactive example output.
    print("-" * 50)  # Print a formatted message for the interactive example output.
    for s in test_strings:  # Iterate over a range/collection to process each item in sequence.
        print(f"{s:<10} {djb2_hash(s):<12} {fnv1a_hash(s):<12} {polynomial_hash(s):<12}")  # Compute the hash-based bucket index for the given key.

    # Anagram 問題示範 - Anagram problem demo
    print("\n--- Anagram 問題 Anagram Problem ---")  # Print a formatted message for the interactive example output.
    anagrams = ["abc", "bca", "cab"]  # Assign or update a variable that represents the current algorithm state.
    print(f"Simple sum (m=100): {[simple_sum_hash(s, 100) for s in anagrams]}")  # Compute the hash-based bucket index for the given key.
    print(f"DJB2:               {[djb2_hash(s) for s in anagrams]}")  # Compute the hash-based bucket index for the given key.

    # 分布分析 - Distribution analysis
    print("\n--- 分布分析 Distribution Analysis ---")  # Print a formatted message for the interactive example output.
    import random  # Import modules and symbols needed by this implementation.
    random.seed(42)  # Execute this statement as part of the data structure implementation.
    random_strings = [f"key_{i}_{random.randint(0, 1000)}" for i in range(1000)]  # Assign or update a variable that represents the current algorithm state.

    stats = analyze_distribution(  # Assign or update a variable that represents the current algorithm state.
        lambda k, m: djb2_hash(k) % m,  # Compute the hash-based bucket index for the given key.
        random_strings,  # Execute this statement as part of the data structure implementation.
        128  # Execute this statement as part of the data structure implementation.
    )  # Execute this statement as part of the data structure implementation.
    print(f"DJB2 with 1000 keys, 128 buckets:")  # Print a formatted message for the interactive example output.
    print(f"  Non-empty buckets: {stats['non_empty_buckets']}")  # Print a formatted message for the interactive example output.
    print(f"  Max bucket size:   {stats['max_bucket_size']}")  # Print a formatted message for the interactive example output.
    print(f"  Std deviation:     {stats['std_deviation']:.3f}")  # Print a formatted message for the interactive example output.
