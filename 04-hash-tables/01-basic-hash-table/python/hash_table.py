"""Docstring start
雜湊表（Hash Table）- Python 實作
使用鏈結法（chaining）處理碰撞

Hash Table implementation using chaining for collision resolution
"""  # End of docstring

from typing import Any, Optional, Iterator, Tuple  # Import modules and symbols needed by this implementation.


class HashTable:  # Declare a class that encapsulates the data structure state and behavior.
    """Docstring start
    基本雜湊表實作 / Basic hash table implementation using chaining

    支援操作 Supported operations:
        - insert(key, value): 插入鍵值對
        - search(key): 搜尋鍵對應的值
        - delete(key): 刪除鍵值對
        - contains(key): 檢查鍵是否存在
    """  # End of docstring

    # 預設初始容量 - Default initial capacity
    DEFAULT_CAPACITY = 16  # Assign or update a variable that represents the current algorithm state.
    # 負載因子閾值 - Load factor threshold for rehashing
    MAX_LOAD_FACTOR = 0.75  # Assign or update a variable that represents the current algorithm state.

    def __init__(self, capacity: int = DEFAULT_CAPACITY) -> None:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        初始化雜湊表 / Initialize hash table with given capacity

        Args:
            capacity: 桶的數量（number of buckets）
                     建議使用 2 的冪次以優化取餘運算 / 建議 use 2 的冪次以優化取餘運算
        """  # End of docstring
        if capacity <= 0:  # Evaluate the condition and branch into the appropriate code path.
            raise ValueError("容量必須為正整數 / Capacity must be positive")  # Execute this statement as part of the data structure implementation.

        self._capacity = capacity  # Assign or update a variable that represents the current algorithm state.
        self._size = 0  # Assign or update a variable that represents the current algorithm state.
        # 使用串列的串列來實作鏈結法 / Use list of lists for chaining
        self._buckets: list[list[Tuple[Any, Any]]] = [  # Access or update the bucket storage used to hold entries or chains.
            [] for _ in range(capacity)  # Execute this statement as part of the data structure implementation.
        ]  # Execute this statement as part of the data structure implementation.

    def _hash(self, key: Any) -> int:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        計算雜湊值 / Compute hash value for the given key

        使用 Python 內建 hash() 後取餘數
        Uses built-in hash() with modulo

        Args:
            key: 要雜湊的鍵

        Returns:
            桶的索引值（bucket index）
        """  # End of docstring
        return hash(key) % self._capacity  # Return the computed result to the caller.

    def insert(self, key: Any, value: Any) -> None:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        插入鍵值對（若 key 已存在則更新 value）
        Insert key-value pair (update if key exists)

        時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)

        Args:
            key: 鍵
            value: 值
        """  # End of docstring
        index = self._hash(key)  # Compute the hash-based bucket index for the given key.
        bucket = self._buckets[index]  # Access or update the bucket storage used to hold entries or chains.

        # 檢查 key 是否已存在 - Check if key exists
        for i, (k, v) in enumerate(bucket):  # Iterate over a range/collection to process each item in sequence.
            if k == key:  # Evaluate the condition and branch into the appropriate code path.
                bucket[i] = (key, value)  # 更新 - Update existing
                return  # Return the computed result to the caller.

        # 新增鍵值對 - Add new key-value pair
        bucket.append((key, value))  # Access or update the bucket storage used to hold entries or chains.
        self._size += 1  # Assign or update a variable that represents the current algorithm state.

        # 檢查是否需要擴容 - Check if rehashing needed
        if self.load_factor > self.MAX_LOAD_FACTOR:  # Evaluate the condition and branch into the appropriate code path.
            self._rehash()  # Trigger rehashing: grow capacity and redistribute all existing entries.

    def search(self, key: Any) -> Optional[Any]:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        搜尋給定 key 的 value
        Search for value associated with key

        時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)

        Args:
            key: 要搜尋的鍵

        Returns:
            找到則回傳 value，否則回傳 None
            Value if found, None otherwise
        """  # End of docstring
        index = self._hash(key)  # Compute the hash-based bucket index for the given key.
        bucket = self._buckets[index]  # Access or update the bucket storage used to hold entries or chains.

        for k, v in bucket:  # Iterate over a range/collection to process each item in sequence.
            if k == key:  # Evaluate the condition and branch into the appropriate code path.
                return v  # Return the computed result to the caller.
        return None  # Return the computed result to the caller.

    def delete(self, key: Any) -> bool:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        刪除指定的鍵值對 / Delete key-value pair

        時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)

        Args:
            key: 要刪除的鍵

        Returns:
            刪除成功回傳 True，key 不存在回傳 False
            True if deleted, False if key not found
        """  # End of docstring
        index = self._hash(key)  # Compute the hash-based bucket index for the given key.
        bucket = self._buckets[index]  # Access or update the bucket storage used to hold entries or chains.

        for i, (k, v) in enumerate(bucket):  # Iterate over a range/collection to process each item in sequence.
            if k == key:  # Evaluate the condition and branch into the appropriate code path.
                del bucket[i]  # Access or update the bucket storage used to hold entries or chains.
                self._size -= 1  # Assign or update a variable that represents the current algorithm state.
                return True  # Return the computed result to the caller.
        return False  # Return the computed result to the caller.

    def contains(self, key: Any) -> bool:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        檢查 key 是否存在於雜湊表中
        Check if key exists in hash table

        Args:
            key: 要檢查的鍵

        Returns:
            存在回傳 True，否則回傳 False
        """  # End of docstring
        return self.search(key) is not None  # Return the computed result to the caller.

    @property  # Execute this statement as part of the data structure implementation.
    def load_factor(self) -> float:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        計算負載因子 α = n / m
        Calculate load factor

        Returns:
            負載因子（load factor）
        """  # End of docstring
        return self._size / self._capacity  # Return the computed result to the caller.

    @property  # Execute this statement as part of the data structure implementation.
    def size(self) -> int:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        回傳元素數量 / Return number of elements
        """  # End of docstring
        return self._size  # Return the computed result to the caller.

    @property  # Execute this statement as part of the data structure implementation.
    def capacity(self) -> int:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        回傳桶的數量 / Return number of buckets
        """  # End of docstring
        return self._capacity  # Return the computed result to the caller.

    def is_empty(self) -> bool:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        檢查雜湊表是否為空 / Check if hash table is empty
        """  # End of docstring
        return self._size == 0  # Return the computed result to the caller.

    def clear(self) -> None:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        清空雜湊表 / Clear all elements from hash table
        """  # End of docstring
        self._buckets = [[] for _ in range(self._capacity)]  # Access or update the bucket storage used to hold entries or chains.
        self._size = 0  # Assign or update a variable that represents the current algorithm state.

    def _rehash(self) -> None:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        擴容並重新雜湊所有元素 / Resize and rehash all elements

        將容量加倍以維持 O(1) 攤銷時間
        Double capacity to maintain O(1) amortized time
        """  # End of docstring
        old_buckets = self._buckets  # Access or update the bucket storage used to hold entries or chains.
        self._capacity *= 2  # Assign or update a variable that represents the current algorithm state.
        self._buckets = [[] for _ in range(self._capacity)]  # Access or update the bucket storage used to hold entries or chains.
        self._size = 0  # Assign or update a variable that represents the current algorithm state.

        # 重新插入所有元素 - Reinsert all elements
        for bucket in old_buckets:  # Iterate over a range/collection to process each item in sequence.
            for key, value in bucket:  # Iterate over a range/collection to process each item in sequence.
                self.insert(key, value)  # Insert or update the given key with the provided value.

    def keys(self) -> Iterator[Any]:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        回傳所有鍵的迭代器 / Return iterator over all keys
        """  # End of docstring
        for bucket in self._buckets:  # Iterate over a range/collection to process each item in sequence.
            for key, _ in bucket:  # Iterate over a range/collection to process each item in sequence.
                yield key  # Execute this statement as part of the data structure implementation.

    def values(self) -> Iterator[Any]:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        回傳所有值的迭代器 / Return iterator over all values
        """  # End of docstring
        for bucket in self._buckets:  # Iterate over a range/collection to process each item in sequence.
            for _, value in bucket:  # Iterate over a range/collection to process each item in sequence.
                yield value  # Execute this statement as part of the data structure implementation.

    def items(self) -> Iterator[Tuple[Any, Any]]:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        回傳所有鍵值對的迭代器 / Return iterator over all key-value pairs
        """  # End of docstring
        for bucket in self._buckets:  # Iterate over a range/collection to process each item in sequence.
            for key, value in bucket:  # Iterate over a range/collection to process each item in sequence.
                yield (key, value)  # Execute this statement as part of the data structure implementation.

    # ========== Python 魔術方法 Magic Methods ==========

    def __len__(self) -> int:  # Define a function/method that implements one operation of this unit.
        """支援 len() 函數 - Support len() function"""
        return self._size  # Return the computed result to the caller.

    def __contains__(self, key: Any) -> bool:  # Define a function/method that implements one operation of this unit.
        """支援 'in' 運算子 - Support 'in' operator"""
        return self.contains(key)  # Return the computed result to the caller.

    def __getitem__(self, key: Any) -> Any:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        支援 [] 取值語法 - Support [] access syntax

        若 key 不存在則拋出 KeyError
        Raises KeyError if key not found
        """  # End of docstring
        value = self.search(key)  # Look up the current value for the given key (None if the key is missing).
        if value is None and not self.contains(key):  # Evaluate the condition and branch into the appropriate code path.
            raise KeyError(f"Key not found: {key}")  # Execute this statement as part of the data structure implementation.
        return value  # Return the computed result to the caller.

    def __setitem__(self, key: Any, value: Any) -> None:  # Define a function/method that implements one operation of this unit.
        """支援 [] 設值語法 - Support [] assignment syntax"""
        self.insert(key, value)  # Insert or update the given key with the provided value.

    def __delitem__(self, key: Any) -> None:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        支援 del 語法 - Support del syntax

        若 key 不存在則拋出 KeyError
        Raises KeyError if key not found
        """  # End of docstring
        if not self.delete(key):  # Evaluate the condition and branch into the appropriate code path.
            raise KeyError(f"Key not found: {key}")  # Execute this statement as part of the data structure implementation.

    def __iter__(self) -> Iterator[Any]:  # Define a function/method that implements one operation of this unit.
        """支援迭代 - Support iteration over keys"""
        return self.keys()  # Return the computed result to the caller.

    def __repr__(self) -> str:  # Define a function/method that implements one operation of this unit.
        """字串表示 - String representation"""
        items = [f"{k!r}: {v!r}" for k, v in self.items()]  # Iterate over all stored (key, value) pairs in the hash table.
        return "HashTable({" + ", ".join(items) + "})"  # Return the computed result to the caller.

    def __str__(self) -> str:  # Define a function/method that implements one operation of this unit.
        """人類可讀的字串 - Human readable string"""
        return self.__repr__()  # Return the computed result to the caller.


# ========== 簡便函式 Convenience Functions ==========

def create_hash_table(data: dict = None, capacity: int = 16) -> HashTable:  # Define a function/method that implements one operation of this unit.
    """Docstring start
    建立雜湊表的便利函式 / Convenience function to create hash table

    Args:
        data: 初始資料（optional initial data）
        capacity: 初始容量

    Returns:
        HashTable 實例
    """  # End of docstring
    ht = HashTable(capacity)  # Create a new hash table instance to store key->value mappings.
    if data:  # Evaluate the condition and branch into the appropriate code path.
        for key, value in data.items():  # Iterate over a range/collection to process each item in sequence.
            ht.insert(key, value)  # Insert or update the given key with the provided value.
    return ht  # Return the computed result to the caller.


# ========== 範例程式 Example Usage ==========

if __name__ == "__main__":  # Evaluate the condition and branch into the appropriate code path.
    print("=== 雜湊表基本操作示範 Hash Table Basic Operations Demo ===\n")  # Print a formatted message for the interactive example output.

    # 建立雜湊表 - Create hash table
    ht = HashTable()  # Create a new hash table instance to store key->value mappings.

    # 插入操作 - Insert operations
    print("插入鍵值對 Inserting key-value pairs:")  # Print a formatted message for the interactive example output.
    ht.insert("apple", 100)  # Insert or update the given key with the provided value.
    ht.insert("banana", 200)  # Insert or update the given key with the provided value.
    ht.insert("cherry", 300)  # Insert or update the given key with the provided value.
    ht["date"] = 400  # 使用 [] 語法
    print(f"  雜湊表: {ht}")  # Print a formatted message for the interactive example output.
    print(f"  大小: {len(ht)}, 負載因子: {ht.load_factor:.3f}\n")  # Compute/check the load factor (size/capacity) to decide about resizing.

    # 搜尋操作 - Search operations
    print("搜尋操作 Search operations:")  # Print a formatted message for the interactive example output.
    print(f"  search('apple') = {ht.search('apple')}")  # Look up the current value for the given key (None if the key is missing).
    print(f"  ht['banana'] = {ht['banana']}")  # Print a formatted message for the interactive example output.
    print(f"  search('grape') = {ht.search('grape')}")  # Look up the current value for the given key (None if the key is missing).
    print(f"  'cherry' in ht = {'cherry' in ht}\n")  # Print a formatted message for the interactive example output.

    # 更新操作 - Update operation
    print("更新操作 Update operation:")  # Print a formatted message for the interactive example output.
    ht["apple"] = 150  # Assign or update a variable that represents the current algorithm state.
    print(f"  更新 apple 的值為 150")  # Print a formatted message for the interactive example output.
    print(f"  ht['apple'] = {ht['apple']}\n")  # Print a formatted message for the interactive example output.

    # 刪除操作 - Delete operation
    print("刪除操作 Delete operations:")  # Print a formatted message for the interactive example output.
    ht.delete("banana")  # Remove the given key from the table if it exists and report success.
    print(f"  刪除 'banana' 後: {ht}")  # Print a formatted message for the interactive example output.
    print(f"  大小: {len(ht)}\n")  # Print a formatted message for the interactive example output.

    # 迭代操作 - Iteration
    print("迭代操作 Iteration:")  # Print a formatted message for the interactive example output.
    print(f"  所有鍵 Keys: {list(ht.keys())}")  # Print a formatted message for the interactive example output.
    print(f"  所有值 Values: {list(ht.values())}")  # Print a formatted message for the interactive example output.
    print(f"  所有項目 Items: {list(ht.items())}")  # Materialize all (key, value) pairs into a list so we can sort/slice them.
