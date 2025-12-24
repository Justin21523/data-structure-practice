"""Docstring start
通用雜湊（Universal Hashing）- Python 實作
Universal Hashing implementation

從雜湊函數族中隨機選擇函數，保證碰撞機率有界 / Randomly select from a family of hash functions to bound collision probability
"""  # End of docstring

import random  # Import modules and symbols needed by this implementation.
from typing import Any, Optional  # Import modules and symbols needed by this implementation.


def is_prime(n: int) -> bool:  # Define a function/method that implements one operation of this unit.
    """檢查是否為質數 - Check if prime"""
    if n < 2:  # Evaluate the condition and branch into the appropriate code path.
        return False  # Return the computed result to the caller.
    if n == 2:  # Evaluate the condition and branch into the appropriate code path.
        return True  # Return the computed result to the caller.
    if n % 2 == 0:  # Evaluate the condition and branch into the appropriate code path.
        return False  # Return the computed result to the caller.
    for i in range(3, int(n**0.5) + 1, 2):  # Iterate over a range/collection to process each item in sequence.
        if n % i == 0:  # Evaluate the condition and branch into the appropriate code path.
            return False  # Return the computed result to the caller.
    return True  # Return the computed result to the caller.


def next_prime(n: int) -> int:  # Define a function/method that implements one operation of this unit.
    """找到大於等於 n 的最小質數 - Find smallest prime >= n"""
    while not is_prime(n):  # Repeat while the loop condition remains true.
        n += 1  # Assign or update a variable that represents the current algorithm state.
    return n  # Return the computed result to the caller.


class UniversalHashFamily:  # Declare a class that encapsulates the data structure state and behavior.
    """Docstring start
    通用雜湊函數族 / Universal hash function family

    h_{a,b}(k) = ((a * k + b) mod p) mod m

    保證對任意不同的 x, y：
    Pr[h(x) = h(y)] <= 1/m
    """  # End of docstring

    def __init__(self, m: int, p: int = None):  # Define a function/method that implements one operation of this unit.
        """Docstring start
        初始化通用雜湊函數族 / Initialize universal hash family

        Args:
            m: 雜湊表大小
            p: 大質數（應大於所有可能的鍵值）
        """  # End of docstring
        self.m = m  # Assign or update a variable that represents the current algorithm state.
        # 選擇一個大質數 - Choose a large prime
        self.p = p if p else next_prime(max(m * 10, 10007))  # Assign or update a variable that represents the current algorithm state.
        # 隨機選擇參數 - Randomly choose parameters
        self.regenerate()  # Execute this statement as part of the data structure implementation.

    def regenerate(self) -> None:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        重新生成隨機參數 / Regenerate random parameters

        a ∈ {1, 2, ..., p-1}
        b ∈ {0, 1, ..., p-1}
        """  # End of docstring
        self.a = random.randint(1, self.p - 1)  # Assign or update a variable that represents the current algorithm state.
        self.b = random.randint(0, self.p - 1)  # Assign or update a variable that represents the current algorithm state.

    def hash(self, key: int) -> int:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        計算雜湊值 / Compute hash value

        h(k) = ((a * k + b) mod p) mod m

        Args:
            key: 整數鍵

        Returns:
            雜湊值 0 ~ m-1
        """  # End of docstring
        return ((self.a * key + self.b) % self.p) % self.m  # Return the computed result to the caller.

    def __call__(self, key: int) -> int:  # Define a function/method that implements one operation of this unit.
        """允許直接呼叫 - Allow direct call"""
        return self.hash(key)  # Return the computed result to the caller.

    def get_parameters(self) -> dict:  # Define a function/method that implements one operation of this unit.
        """取得目前的參數 - Get current parameters"""
        return {  # Return the computed result to the caller.
            "a": self.a,  # Execute this statement as part of the data structure implementation.
            "b": self.b,  # Execute this statement as part of the data structure implementation.
            "p": self.p,  # Execute this statement as part of the data structure implementation.
            "m": self.m  # Execute this statement as part of the data structure implementation.
        }  # Close the current block scope.


class UniversalStringHashFamily:  # Declare a class that encapsulates the data structure state and behavior.
    """Docstring start
    字串的通用雜湊函數族 / Universal hash function family for strings

    將字串視為數字序列，使用多項式雜湊 / Treats string as sequence of numbers, uses polynomial hashing
    """  # End of docstring

    def __init__(self, m: int, p: int = None):  # Define a function/method that implements one operation of this unit.
        """Docstring start
        初始化字串通用雜湊 / Initialize universal string hash

        Args:
            m: 雜湊表大小
            p: 大質數
        """  # End of docstring
        self.m = m  # Assign or update a variable that represents the current algorithm state.
        self.p = p if p else next_prime(max(m * 10, 10007))  # Assign or update a variable that represents the current algorithm state.
        self.regenerate()  # Execute this statement as part of the data structure implementation.

    def regenerate(self) -> None:  # Define a function/method that implements one operation of this unit.
        """重新生成隨機參數 - Regenerate random parameters"""
        self.a = random.randint(1, self.p - 1)  # Assign or update a variable that represents the current algorithm state.

    def hash(self, s: str) -> int:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        計算字串雜湊值 / Compute string hash value

        使用多項式雜湊：h(s) = sum(s[i] * a^i) mod p mod m
        Uses polynomial hash

        Args:
            s: 字串

        Returns:
            雜湊值 0 ~ m-1
        """  # End of docstring
        h = 0  # Assign or update a variable that represents the current algorithm state.
        for c in s:  # Iterate over a range/collection to process each item in sequence.
            h = (h * self.a + ord(c)) % self.p  # Assign or update a variable that represents the current algorithm state.
        return h % self.m  # Return the computed result to the caller.

    def __call__(self, s: str) -> int:  # Define a function/method that implements one operation of this unit.
        return self.hash(s)  # Return the computed result to the caller.


class UniversalHashTable:  # Declare a class that encapsulates the data structure state and behavior.
    """Docstring start
    使用通用雜湊的雜湊表 / Hash table using universal hashing

    可以在偵測到攻擊（過多碰撞）時重新選擇雜湊函數 / Can regenerate hash function when detecting attacks (too many collisions)
    """  # End of docstring

    DEFAULT_CAPACITY = 16  # Assign or update a variable that represents the current algorithm state.
    MAX_LOAD_FACTOR = 0.75  # Assign or update a variable that represents the current algorithm state.
    MAX_CHAIN_LENGTH = 10  # 觸發重新雜湊的閾值

    def __init__(self, capacity: int = DEFAULT_CAPACITY):  # Define a function/method that implements one operation of this unit.
        """初始化雜湊表 - Initialize hash table"""
        self._capacity = capacity  # Assign or update a variable that represents the current algorithm state.
        self._size = 0  # Assign or update a variable that represents the current algorithm state.
        self._buckets = [[] for _ in range(capacity)]  # Access or update the bucket storage used to hold entries or chains.
        self._hash_family = UniversalHashFamily(capacity)  # Compute a hash-based index so keys map into the table's storage.
        self._rehash_count = 0  # Trigger rehashing: grow capacity and redistribute all existing entries.

    def _hash(self, key: int) -> int:  # Define a function/method that implements one operation of this unit.
        """計算雜湊值 - Compute hash"""
        return self._hash_family(key)  # Return the computed result to the caller.

    def insert(self, key: int, value: Any) -> None:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        插入鍵值對 / Insert key-value pair
        """  # End of docstring
        index = self._hash(key)  # Compute the hash-based bucket index for the given key.
        bucket = self._buckets[index]  # Access or update the bucket storage used to hold entries or chains.

        # 檢查是否已存在 - Check if exists
        for i, (k, v) in enumerate(bucket):  # Iterate over a range/collection to process each item in sequence.
            if k == key:  # Evaluate the condition and branch into the appropriate code path.
                bucket[i] = (key, value)  # Access or update the bucket storage used to hold entries or chains.
                return  # Return the computed result to the caller.

        bucket.append((key, value))  # Access or update the bucket storage used to hold entries or chains.
        self._size += 1  # Assign or update a variable that represents the current algorithm state.

        # 檢查是否需要擴容或重新雜湊 - Check for resize or rehash
        if self.load_factor > self.MAX_LOAD_FACTOR:  # Evaluate the condition and branch into the appropriate code path.
            self._resize()  # Execute this statement as part of the data structure implementation.
        elif len(bucket) > self.MAX_CHAIN_LENGTH:  # Check the next condition when earlier branches did not match.
            # 鏈太長，可能遭受攻擊，重新選擇雜湊函數 / Chain too long, possible attack, regenerate hash
            self._regenerate_hash()  # Compute the hash-based bucket index for the given key.

    def search(self, key: int) -> Optional[Any]:  # Define a function/method that implements one operation of this unit.
        """搜尋鍵對應的值 - Search for value"""
        index = self._hash(key)  # Compute the hash-based bucket index for the given key.
        for k, v in self._buckets[index]:  # Iterate over a range/collection to process each item in sequence.
            if k == key:  # Evaluate the condition and branch into the appropriate code path.
                return v  # Return the computed result to the caller.
        return None  # Return the computed result to the caller.

    def delete(self, key: int) -> bool:  # Define a function/method that implements one operation of this unit.
        """刪除鍵值對 - Delete key-value pair"""
        index = self._hash(key)  # Compute the hash-based bucket index for the given key.
        bucket = self._buckets[index]  # Access or update the bucket storage used to hold entries or chains.

        for i, (k, v) in enumerate(bucket):  # Iterate over a range/collection to process each item in sequence.
            if k == key:  # Evaluate the condition and branch into the appropriate code path.
                del bucket[i]  # Access or update the bucket storage used to hold entries or chains.
                self._size -= 1  # Assign or update a variable that represents the current algorithm state.
                return True  # Return the computed result to the caller.
        return False  # Return the computed result to the caller.

    def _resize(self) -> None:  # Define a function/method that implements one operation of this unit.
        """擴容 - Resize"""
        old_buckets = self._buckets  # Access or update the bucket storage used to hold entries or chains.
        self._capacity *= 2  # Assign or update a variable that represents the current algorithm state.
        self._buckets = [[] for _ in range(self._capacity)]  # Access or update the bucket storage used to hold entries or chains.
        self._hash_family = UniversalHashFamily(self._capacity)  # Compute a hash-based index so keys map into the table's storage.
        self._size = 0  # Assign or update a variable that represents the current algorithm state.

        for bucket in old_buckets:  # Iterate over a range/collection to process each item in sequence.
            for key, value in bucket:  # Iterate over a range/collection to process each item in sequence.
                self.insert(key, value)  # Insert or update the given key with the provided value.

    def _regenerate_hash(self) -> None:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        重新選擇雜湊函數並重新分配所有元素 / Regenerate hash function and redistribute all elements
        """  # End of docstring
        self._rehash_count += 1  # Trigger rehashing: grow capacity and redistribute all existing entries.
        old_buckets = self._buckets  # Access or update the bucket storage used to hold entries or chains.
        self._buckets = [[] for _ in range(self._capacity)]  # Access or update the bucket storage used to hold entries or chains.
        self._hash_family.regenerate()  # Compute a hash-based index so keys map into the table's storage.
        self._size = 0  # Assign or update a variable that represents the current algorithm state.

        for bucket in old_buckets:  # Iterate over a range/collection to process each item in sequence.
            for key, value in bucket:  # Iterate over a range/collection to process each item in sequence.
                self.insert(key, value)  # Insert or update the given key with the provided value.

    @property  # Execute this statement as part of the data structure implementation.
    def load_factor(self) -> float:  # Define a function/method that implements one operation of this unit.
        return self._size / self._capacity  # Return the computed result to the caller.

    @property  # Execute this statement as part of the data structure implementation.
    def size(self) -> int:  # Define a function/method that implements one operation of this unit.
        return self._size  # Return the computed result to the caller.

    @property  # Execute this statement as part of the data structure implementation.
    def rehash_count(self) -> int:  # Define a function/method that implements one operation of this unit.
        """重新雜湊次數 - Number of rehashes"""
        return self._rehash_count  # Return the computed result to the caller.

    def get_max_chain_length(self) -> int:  # Define a function/method that implements one operation of this unit.
        """最長鏈長度 - Maximum chain length"""
        return max(len(bucket) for bucket in self._buckets)  # Return the computed result to the caller.


# ============================================================
# 範例與測試 Example and Test
# ============================================================

if __name__ == "__main__":  # Evaluate the condition and branch into the appropriate code path.
    print("=== 通用雜湊示範 Universal Hashing Demo ===\n")  # Print a formatted message for the interactive example output.

    # 基本通用雜湊 - Basic universal hashing
    print("--- 通用雜湊函數族 Universal Hash Family ---")  # Print a formatted message for the interactive example output.
    m = 10  # Assign or update a variable that represents the current algorithm state.
    uh = UniversalHashFamily(m)  # Assign or update a variable that represents the current algorithm state.
    print(f"Parameters: {uh.get_parameters()}")  # Print a formatted message for the interactive example output.

    test_keys = [10, 20, 30, 40, 50, 100, 200, 300]  # Assign or update a variable that represents the current algorithm state.
    print(f"Keys: {test_keys}")  # Print a formatted message for the interactive example output.
    print(f"Hash values: {[uh(k) for k in test_keys]}")  # Print a formatted message for the interactive example output.

    print("\n重新生成參數後 After regenerating:")  # Print a formatted message for the interactive example output.
    uh.regenerate()  # Execute this statement as part of the data structure implementation.
    print(f"Parameters: {uh.get_parameters()}")  # Print a formatted message for the interactive example output.
    print(f"Hash values: {[uh(k) for k in test_keys]}")  # Print a formatted message for the interactive example output.

    # 碰撞機率驗證 - Collision probability verification
    print("\n--- 碰撞機率驗證 Collision Probability ---")  # Print a formatted message for the interactive example output.
    m = 100  # Assign or update a variable that represents the current algorithm state.
    trials = 10000  # Assign or update a variable that represents the current algorithm state.
    collisions = 0  # Assign or update a variable that represents the current algorithm state.
    x, y = 42, 137  # 兩個固定的不同鍵

    for _ in range(trials):  # Iterate over a range/collection to process each item in sequence.
        uh = UniversalHashFamily(m)  # Assign or update a variable that represents the current algorithm state.
        if uh(x) == uh(y):  # Evaluate the condition and branch into the appropriate code path.
            collisions += 1  # Assign or update a variable that represents the current algorithm state.

    expected = trials / m  # Assign or update a variable that represents the current algorithm state.
    print(f"鍵 x={x}, y={y}, m={m}")  # Print a formatted message for the interactive example output.
    print(f"試驗次數: {trials}")  # Print a formatted message for the interactive example output.
    print(f"碰撞次數: {collisions}")  # Print a formatted message for the interactive example output.
    print(f"碰撞機率: {collisions/trials:.4f}")  # Print a formatted message for the interactive example output.
    print(f"理論上界: {1/m:.4f}")  # Print a formatted message for the interactive example output.
    print(f"期望碰撞: {expected:.1f}")  # Print a formatted message for the interactive example output.

    # 使用通用雜湊的雜湊表 - Hash table with universal hashing
    print("\n--- 使用通用雜湊的雜湊表 ---")  # Print a formatted message for the interactive example output.
    ht = UniversalHashTable()  # Create a new hash table instance to store key->value mappings.

    for i in range(50):  # Iterate over a range/collection to process each item in sequence.
        ht.insert(i * 7, f"value_{i}")  # Insert or update the given key with the provided value.

    print(f"元素數量: {ht.size}")  # Print a formatted message for the interactive example output.
    print(f"負載因子: {ht.load_factor:.3f}")  # Compute/check the load factor (size/capacity) to decide about resizing.
    print(f"最長鏈長: {ht.get_max_chain_length()}")  # Print a formatted message for the interactive example output.
    print(f"重新雜湊次數: {ht.rehash_count}")  # Trigger rehashing: grow capacity and redistribute all existing entries.

    # 搜尋測試 / Search test
    print(f"search(14) = {ht.search(14)}")  # Look up the current value for the given key (None if the key is missing).
    print(f"search(999) = {ht.search(999)}")  # Look up the current value for the given key (None if the key is missing).
