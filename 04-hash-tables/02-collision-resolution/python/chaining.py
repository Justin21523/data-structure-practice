"""Docstring start
碰撞處理：鏈結法（Chaining）- Python 實作
Collision Resolution: Chaining implementation

使用鏈結串列處理碰撞 / Uses linked list to handle collisions
"""  # End of docstring

from typing import Any, Optional, List, Tuple  # Import modules and symbols needed by this implementation.


class ChainedHashTable:  # Declare a class that encapsulates the data structure state and behavior.
    """Docstring start
    使用鏈結法的雜湊表 / Hash table using chaining for collision resolution
    """  # End of docstring

    DEFAULT_CAPACITY = 16  # Assign or update a variable that represents the current algorithm state.
    MAX_LOAD_FACTOR = 0.75  # Assign or update a variable that represents the current algorithm state.

    def __init__(self, capacity: int = DEFAULT_CAPACITY) -> None:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        初始化雜湊表 / Initialize hash table
        """  # End of docstring
        if capacity <= 0:  # Evaluate the condition and branch into the appropriate code path.
            raise ValueError("容量必須為正整數 / Capacity must be positive")  # Execute this statement as part of the data structure implementation.

        self._capacity = capacity  # Assign or update a variable that represents the current algorithm state.
        self._size = 0  # Assign or update a variable that represents the current algorithm state.
        # 每個桶是一個串列 - Each bucket is a list
        self._buckets: List[List[Tuple[Any, Any]]] = [  # Access or update the bucket storage used to hold entries or chains.
            [] for _ in range(capacity)  # Execute this statement as part of the data structure implementation.
        ]  # Execute this statement as part of the data structure implementation.
        # 追蹤探測次數（用於分析） - Track probe counts for analysis
        self._total_probes = 0  # Advance or track the probing sequence used by open addressing.
        self._operations = 0  # Assign or update a variable that represents the current algorithm state.

    def _hash(self, key: Any) -> int:  # Define a function/method that implements one operation of this unit.
        """計算雜湊索引 - Compute hash index"""
        return hash(key) % self._capacity  # Return the computed result to the caller.

    def insert(self, key: Any, value: Any) -> int:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        插入鍵值對 / Insert key-value pair

        Returns:
            探測次數 - Number of probes
        """  # End of docstring
        index = self._hash(key)  # Compute the hash-based bucket index for the given key.
        bucket = self._buckets[index]  # Access or update the bucket storage used to hold entries or chains.
        probes = 1  # 至少一次探測

        # 檢查是否已存在 - Check if exists
        for i, (k, v) in enumerate(bucket):  # Iterate over a range/collection to process each item in sequence.
            probes += 1  # Advance or track the probing sequence used by open addressing.
            if k == key:  # Evaluate the condition and branch into the appropriate code path.
                bucket[i] = (key, value)  # Access or update the bucket storage used to hold entries or chains.
                self._total_probes += probes  # Advance or track the probing sequence used by open addressing.
                self._operations += 1  # Assign or update a variable that represents the current algorithm state.
                return probes  # Return the computed result to the caller.

        # 新增到串列尾部 - Add to end of list
        bucket.append((key, value))  # Access or update the bucket storage used to hold entries or chains.
        self._size += 1  # Assign or update a variable that represents the current algorithm state.
        self._total_probes += probes  # Advance or track the probing sequence used by open addressing.
        self._operations += 1  # Assign or update a variable that represents the current algorithm state.

        # 擴容檢查 - Check for rehash
        if self.load_factor > self.MAX_LOAD_FACTOR:  # Evaluate the condition and branch into the appropriate code path.
            self._rehash()  # Trigger rehashing: grow capacity and redistribute all existing entries.

        return probes  # Return the computed result to the caller.

    def search(self, key: Any) -> Tuple[Optional[Any], int]:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        搜尋鍵對應的值 / Search for value by key

        Returns:
            (value, probes) - 值與探測次數
        """  # End of docstring
        index = self._hash(key)  # Compute the hash-based bucket index for the given key.
        bucket = self._buckets[index]  # Access or update the bucket storage used to hold entries or chains.
        probes = 1  # Advance or track the probing sequence used by open addressing.

        for k, v in bucket:  # Iterate over a range/collection to process each item in sequence.
            if k == key:  # Evaluate the condition and branch into the appropriate code path.
                self._total_probes += probes  # Advance or track the probing sequence used by open addressing.
                self._operations += 1  # Assign or update a variable that represents the current algorithm state.
                return v, probes  # Return the computed result to the caller.
            probes += 1  # Advance or track the probing sequence used by open addressing.

        self._total_probes += probes  # Advance or track the probing sequence used by open addressing.
        self._operations += 1  # Assign or update a variable that represents the current algorithm state.
        return None, probes  # Return the computed result to the caller.

    def delete(self, key: Any) -> Tuple[bool, int]:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        刪除鍵值對 / Delete key-value pair

        Returns:
            (success, probes) - 是否成功與探測次數
        """  # End of docstring
        index = self._hash(key)  # Compute the hash-based bucket index for the given key.
        bucket = self._buckets[index]  # Access or update the bucket storage used to hold entries or chains.
        probes = 1  # Advance or track the probing sequence used by open addressing.

        for i, (k, v) in enumerate(bucket):  # Iterate over a range/collection to process each item in sequence.
            if k == key:  # Evaluate the condition and branch into the appropriate code path.
                del bucket[i]  # Access or update the bucket storage used to hold entries or chains.
                self._size -= 1  # Assign or update a variable that represents the current algorithm state.
                self._total_probes += probes  # Advance or track the probing sequence used by open addressing.
                self._operations += 1  # Assign or update a variable that represents the current algorithm state.
                return True, probes  # Return the computed result to the caller.
            probes += 1  # Advance or track the probing sequence used by open addressing.

        self._total_probes += probes  # Advance or track the probing sequence used by open addressing.
        self._operations += 1  # Assign or update a variable that represents the current algorithm state.
        return False, probes  # Return the computed result to the caller.

    def _rehash(self) -> None:  # Define a function/method that implements one operation of this unit.
        """擴容並重新雜湊 - Resize and rehash"""
        old_buckets = self._buckets  # Access or update the bucket storage used to hold entries or chains.
        self._capacity *= 2  # Assign or update a variable that represents the current algorithm state.
        self._buckets = [[] for _ in range(self._capacity)]  # Access or update the bucket storage used to hold entries or chains.
        self._size = 0  # Assign or update a variable that represents the current algorithm state.

        for bucket in old_buckets:  # Iterate over a range/collection to process each item in sequence.
            for key, value in bucket:  # Iterate over a range/collection to process each item in sequence.
                self.insert(key, value)  # Insert or update the given key with the provided value.

    @property  # Execute this statement as part of the data structure implementation.
    def load_factor(self) -> float:  # Define a function/method that implements one operation of this unit.
        """負載因子 - Load factor"""
        return self._size / self._capacity  # Return the computed result to the caller.

    @property  # Execute this statement as part of the data structure implementation.
    def size(self) -> int:  # Define a function/method that implements one operation of this unit.
        return self._size  # Return the computed result to the caller.

    @property  # Execute this statement as part of the data structure implementation.
    def capacity(self) -> int:  # Define a function/method that implements one operation of this unit.
        return self._capacity  # Return the computed result to the caller.

    def average_probes(self) -> float:  # Define a function/method that implements one operation of this unit.
        """平均探測次數 - Average number of probes"""
        if self._operations == 0:  # Evaluate the condition and branch into the appropriate code path.
            return 0.0  # Return the computed result to the caller.
        return self._total_probes / self._operations  # Return the computed result to the caller.

    def get_chain_lengths(self) -> List[int]:  # Define a function/method that implements one operation of this unit.
        """取得每個桶的鏈長度 - Get chain length of each bucket"""
        return [len(bucket) for bucket in self._buckets]  # Return the computed result to the caller.

    def max_chain_length(self) -> int:  # Define a function/method that implements one operation of this unit.
        """最長鏈長度 - Maximum chain length"""
        return max(len(bucket) for bucket in self._buckets)  # Return the computed result to the caller.

    def __len__(self) -> int:  # Define a function/method that implements one operation of this unit.
        return self._size  # Return the computed result to the caller.

    def __contains__(self, key: Any) -> bool:  # Define a function/method that implements one operation of this unit.
        value, _ = self.search(key)  # Look up the current value for the given key (None if the key is missing).
        return value is not None  # Return the computed result to the caller.

    def __repr__(self) -> str:  # Define a function/method that implements one operation of this unit.
        items = []  # Assign or update a variable that represents the current algorithm state.
        for bucket in self._buckets:  # Iterate over a range/collection to process each item in sequence.
            for k, v in bucket:  # Iterate over a range/collection to process each item in sequence.
                items.append(f"{k!r}: {v!r}")  # Execute this statement as part of the data structure implementation.
        return "ChainedHashTable({" + ", ".join(items) + "})"  # Return the computed result to the caller.


# ========== 範例與統計 Example and Statistics ==========

if __name__ == "__main__":  # Evaluate the condition and branch into the appropriate code path.
    print("=== 鏈結法雜湊表示範 Chaining Hash Table Demo ===\n")  # Print a formatted message for the interactive example output.

    ht = ChainedHashTable(8)  # Create a new hash table instance to store key->value mappings.

    # 插入資料 - Insert data
    keys = ["apple", "banana", "cherry", "date", "elderberry",  # Assign or update a variable that represents the current algorithm state.
            "fig", "grape", "honeydew", "kiwi", "lemon"]  # Execute this statement as part of the data structure implementation.

    print("插入資料 Inserting data:")  # Print a formatted message for the interactive example output.
    for i, key in enumerate(keys):  # Iterate over a range/collection to process each item in sequence.
        probes = ht.insert(key, i * 10)  # Insert or update the given key with the provided value.
        print(f"  insert('{key}') - 探測次數: {probes}")  # Print a formatted message for the interactive example output.

    print(f"\n統計 Statistics:")  # Print a formatted message for the interactive example output.
    print(f"  元素數量: {len(ht)}")  # Print a formatted message for the interactive example output.
    print(f"  桶數量: {ht.capacity}")  # Print a formatted message for the interactive example output.
    print(f"  負載因子: {ht.load_factor:.3f}")  # Compute/check the load factor (size/capacity) to decide about resizing.
    print(f"  最長鏈長度: {ht.max_chain_length()}")  # Print a formatted message for the interactive example output.
    print(f"  平均探測次數: {ht.average_probes():.3f}")  # Print a formatted message for the interactive example output.

    # 鏈長度分布 - Chain length distribution
    lengths = ht.get_chain_lengths()  # Assign or update a variable that represents the current algorithm state.
    print(f"\n鏈長度分布 Chain lengths: {lengths}")  # Print a formatted message for the interactive example output.

    # 搜尋測試 - Search test
    print("\n搜尋測試 Search test:")  # Print a formatted message for the interactive example output.
    for key in ["apple", "fig", "mango"]:  # Iterate over a range/collection to process each item in sequence.
        value, probes = ht.search(key)  # Look up the current value for the given key (None if the key is missing).
        print(f"  search('{key}') = {value}, 探測: {probes}")  # Print a formatted message for the interactive example output.
