"""Docstring start
碰撞處理：開放定址法（Open Addressing）- Python 實作
Collision Resolution: Open Addressing implementation

實作三種探測方法：線性探測、二次探測、雙重雜湊 / Implements three probing methods: Linear, Quadratic, Double Hashing
"""  # End of docstring

from typing import Any, Optional, Tuple  # Import modules and symbols needed by this implementation.
from enum import Enum  # Import modules and symbols needed by this implementation.


class ProbeMethod(Enum):  # Declare a class that encapsulates the data structure state and behavior.
    """探測方法列舉 - Probing method enum"""
    LINEAR = "linear"           # 線性探測
    QUADRATIC = "quadratic"     # 二次探測
    DOUBLE_HASH = "double_hash" # 雙重雜湊


class OpenAddressingHashTable:  # Declare a class that encapsulates the data structure state and behavior.
    """Docstring start
    使用開放定址法的雜湊表 / Hash table using open addressing for collision resolution
    """  # End of docstring

    # 特殊標記 - Special markers
    EMPTY = object()      # 空槽位
    DELETED = object()    # 墓碑標記（已刪除）

    DEFAULT_CAPACITY = 16  # Assign or update a variable that represents the current algorithm state.
    MAX_LOAD_FACTOR = 0.7  # 開放定址法需要較低的負載因子

    def __init__(self, capacity: int = DEFAULT_CAPACITY,  # Define a function/method that implements one operation of this unit.
                 probe_method: ProbeMethod = ProbeMethod.LINEAR) -> None:  # Advance or track the probing sequence used by open addressing.
        """Docstring start
        初始化雜湊表 / Initialize hash table

        Args:
            capacity: 容量（建議使用質數）
            probe_method: 探測方法
        """  # End of docstring
        if capacity <= 0:  # Evaluate the condition and branch into the appropriate code path.
            raise ValueError("容量必須為正整數 / Capacity must be positive")  # Execute this statement as part of the data structure implementation.

        self._capacity = capacity  # Assign or update a variable that represents the current algorithm state.
        self._size = 0  # Assign or update a variable that represents the current algorithm state.
        self._probe_method = probe_method  # Advance or track the probing sequence used by open addressing.
        # 使用兩個陣列分別存 key 和 value
        self._keys = [self.EMPTY] * capacity  # Assign or update a variable that represents the current algorithm state.
        self._values = [None] * capacity  # Assign or update a variable that represents the current algorithm state.
        # 統計資料 - Statistics
        self._total_probes = 0  # Advance or track the probing sequence used by open addressing.
        self._operations = 0  # Assign or update a variable that represents the current algorithm state.

    def _hash1(self, key: Any) -> int:  # Define a function/method that implements one operation of this unit.
        """主雜湊函數 - Primary hash function"""
        return hash(key) % self._capacity  # Return the computed result to the caller.

    def _hash2(self, key: Any) -> int:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        次雜湊函數（用於雙重雜湊） / Secondary hash function for double hashing

        確保回傳值與 capacity 互質
        Ensures return value is coprime with capacity
        """  # End of docstring
        # 使用 capacity - 1 確保結果 >= 1
        return 1 + (hash(key) % (self._capacity - 1))  # Return the computed result to the caller.

    def _probe(self, key: Any, i: int) -> int:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        計算第 i 次探測的索引
        Calculate index for i-th probe

        Args:
            key: 鍵
            i: 探測次數（從 0 開始）

        Returns:
            探測索引 / Probe 索引
        """  # End of docstring
        h1 = self._hash1(key)  # Compute a hash-based index so keys map into the table's storage.

        if self._probe_method == ProbeMethod.LINEAR:  # Evaluate the condition and branch into the appropriate code path.
            # 線性探測：h(k) + i
            return (h1 + i) % self._capacity  # Return the computed result to the caller.

        elif self._probe_method == ProbeMethod.QUADRATIC:  # Check the next condition when earlier branches did not match.
            # 二次探測：h(k) + i²
            return (h1 + i * i) % self._capacity  # Return the computed result to the caller.

        elif self._probe_method == ProbeMethod.DOUBLE_HASH:  # Check the next condition when earlier branches did not match.
            # 雙重雜湊：h₁(k) + i * h₂(k)
            h2 = self._hash2(key)  # Compute a hash-based index so keys map into the table's storage.
            return (h1 + i * h2) % self._capacity  # Return the computed result to the caller.

        return h1  # Return the computed result to the caller.

    def insert(self, key: Any, value: Any) -> Tuple[bool, int]:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        插入鍵值對 / Insert key-value pair

        Returns:
            (success, probes) - 是否成功與探測次數
        """  # End of docstring
        if self.load_factor >= self.MAX_LOAD_FACTOR:  # Evaluate the condition and branch into the appropriate code path.
            self._rehash()  # Trigger rehashing: grow capacity and redistribute all existing entries.

        first_deleted = -1  # 記錄第一個墓碑位置

        for i in range(self._capacity):  # Iterate over a range/collection to process each item in sequence.
            index = self._probe(key, i)  # Advance or track the probing sequence used by open addressing.

            if self._keys[index] is self.EMPTY:  # Evaluate the condition and branch into the appropriate code path.
                # 找到空位 - Found empty slot
                if first_deleted != -1:  # Evaluate the condition and branch into the appropriate code path.
                    index = first_deleted  # 使用之前的墓碑位置
                self._keys[index] = key  # Assign or update a variable that represents the current algorithm state.
                self._values[index] = value  # Assign or update a variable that represents the current algorithm state.
                self._size += 1  # Assign or update a variable that represents the current algorithm state.
                self._total_probes += i + 1  # Advance or track the probing sequence used by open addressing.
                self._operations += 1  # Assign or update a variable that represents the current algorithm state.
                return True, i + 1  # Return the computed result to the caller.

            elif self._keys[index] is self.DELETED:  # Check the next condition when earlier branches did not match.
                # 記錄第一個墓碑 - Record first tombstone
                if first_deleted == -1:  # Evaluate the condition and branch into the appropriate code path.
                    first_deleted = index  # Handle tombstones so deletions do not break the probing/search sequence.

            elif self._keys[index] == key:  # Check the next condition when earlier branches did not match.
                # 鍵已存在，更新值 - Key exists, update value
                self._values[index] = value  # Assign or update a variable that represents the current algorithm state.
                self._total_probes += i + 1  # Advance or track the probing sequence used by open addressing.
                self._operations += 1  # Assign or update a variable that represents the current algorithm state.
                return True, i + 1  # Return the computed result to the caller.

        # 表滿（不應發生，因為有負載因子限制） / 表滿 (不應發生，因為有 load factor 限制)
        self._operations += 1  # Assign or update a variable that represents the current algorithm state.
        return False, self._capacity  # Return the computed result to the caller.

    def search(self, key: Any) -> Tuple[Optional[Any], int]:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        搜尋鍵對應的值 / Search for value by key

        Returns:
            (value, probes) - 值與探測次數，未找到回傳 (None, probes)
        """  # End of docstring
        for i in range(self._capacity):  # Iterate over a range/collection to process each item in sequence.
            index = self._probe(key, i)  # Advance or track the probing sequence used by open addressing.

            if self._keys[index] is self.EMPTY:  # Evaluate the condition and branch into the appropriate code path.
                # 遇到空位，鍵不存在 - Empty slot, key doesn't exist
                self._total_probes += i + 1  # Advance or track the probing sequence used by open addressing.
                self._operations += 1  # Assign or update a variable that represents the current algorithm state.
                return None, i + 1  # Return the computed result to the caller.

            elif self._keys[index] is self.DELETED:  # Check the next condition when earlier branches did not match.
                # 跳過墓碑繼續探測 - Skip tombstone, continue probing
                continue  # Skip the remainder of this iteration and continue the loop.

            elif self._keys[index] == key:  # Check the next condition when earlier branches did not match.
                # 找到鍵 - Found key
                self._total_probes += i + 1  # Advance or track the probing sequence used by open addressing.
                self._operations += 1  # Assign or update a variable that represents the current algorithm state.
                return self._values[index], i + 1  # Return the computed result to the caller.

        self._total_probes += self._capacity  # Advance or track the probing sequence used by open addressing.
        self._operations += 1  # Assign or update a variable that represents the current algorithm state.
        return None, self._capacity  # Return the computed result to the caller.

    def delete(self, key: Any) -> Tuple[bool, int]:  # Define a function/method that implements one operation of this unit.
        """Docstring start
        刪除鍵值對（使用墓碑標記） / Delete key-value pair (using tombstone marker)

        Returns:
            (success, probes) - 是否成功與探測次數
        """  # End of docstring
        for i in range(self._capacity):  # Iterate over a range/collection to process each item in sequence.
            index = self._probe(key, i)  # Advance or track the probing sequence used by open addressing.

            if self._keys[index] is self.EMPTY:  # Evaluate the condition and branch into the appropriate code path.
                # 遇到空位，鍵不存在 - Empty slot, key doesn't exist
                self._total_probes += i + 1  # Advance or track the probing sequence used by open addressing.
                self._operations += 1  # Assign or update a variable that represents the current algorithm state.
                return False, i + 1  # Return the computed result to the caller.

            elif self._keys[index] is self.DELETED:  # Check the next condition when earlier branches did not match.
                # 跳過墓碑 - Skip tombstone
                continue  # Skip the remainder of this iteration and continue the loop.

            elif self._keys[index] == key:  # Check the next condition when earlier branches did not match.
                # 找到鍵，標記為已刪除 - Found key, mark as deleted
                self._keys[index] = self.DELETED  # Handle tombstones so deletions do not break the probing/search sequence.
                self._values[index] = None  # Assign or update a variable that represents the current algorithm state.
                self._size -= 1  # Assign or update a variable that represents the current algorithm state.
                self._total_probes += i + 1  # Advance or track the probing sequence used by open addressing.
                self._operations += 1  # Assign or update a variable that represents the current algorithm state.
                return True, i + 1  # Return the computed result to the caller.

        self._total_probes += self._capacity  # Advance or track the probing sequence used by open addressing.
        self._operations += 1  # Assign or update a variable that represents the current algorithm state.
        return False, self._capacity  # Return the computed result to the caller.

    def _rehash(self) -> None:  # Define a function/method that implements one operation of this unit.
        """擴容並重新雜湊 - Resize and rehash"""
        old_keys = self._keys  # Assign or update a variable that represents the current algorithm state.
        old_values = self._values  # Assign or update a variable that represents the current algorithm state.
        old_capacity = self._capacity  # Assign or update a variable that represents the current algorithm state.

        # 容量加倍 - Double capacity
        self._capacity *= 2  # Assign or update a variable that represents the current algorithm state.
        self._keys = [self.EMPTY] * self._capacity  # Assign or update a variable that represents the current algorithm state.
        self._values = [None] * self._capacity  # Assign or update a variable that represents the current algorithm state.
        self._size = 0  # Assign or update a variable that represents the current algorithm state.

        # 重新插入（跳過 EMPTY 和 DELETED）
        for i in range(old_capacity):  # Iterate over a range/collection to process each item in sequence.
            if old_keys[i] is not self.EMPTY and old_keys[i] is not self.DELETED:  # Evaluate the condition and branch into the appropriate code path.
                self.insert(old_keys[i], old_values[i])  # Insert or update the given key with the provided value.

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

    @property  # Execute this statement as part of the data structure implementation.
    def probe_method(self) -> ProbeMethod:  # Define a function/method that implements one operation of this unit.
        return self._probe_method  # Return the computed result to the caller.

    def average_probes(self) -> float:  # Define a function/method that implements one operation of this unit.
        """平均探測次數 - Average number of probes"""
        if self._operations == 0:  # Evaluate the condition and branch into the appropriate code path.
            return 0.0  # Return the computed result to the caller.
        return self._total_probes / self._operations  # Return the computed result to the caller.

    def __len__(self) -> int:  # Define a function/method that implements one operation of this unit.
        return self._size  # Return the computed result to the caller.

    def __contains__(self, key: Any) -> bool:  # Define a function/method that implements one operation of this unit.
        value, _ = self.search(key)  # Look up the current value for the given key (None if the key is missing).
        return value is not None  # Return the computed result to the caller.

    def visualize(self) -> str:  # Define a function/method that implements one operation of this unit.
        """視覺化雜湊表狀態 - Visualize hash table state"""
        result = []  # Assign or update a variable that represents the current algorithm state.
        for i in range(self._capacity):  # Iterate over a range/collection to process each item in sequence.
            if self._keys[i] is self.EMPTY:  # Evaluate the condition and branch into the appropriate code path.
                result.append(f"[{i}]: EMPTY")  # Execute this statement as part of the data structure implementation.
            elif self._keys[i] is self.DELETED:  # Check the next condition when earlier branches did not match.
                result.append(f"[{i}]: DELETED")  # Handle tombstones so deletions do not break the probing/search sequence.
            else:  # Handle the alternative branch when the condition is false.
                result.append(f"[{i}]: {self._keys[i]!r} -> {self._values[i]!r}")  # Execute this statement as part of the data structure implementation.
        return "\n".join(result)  # Return the computed result to the caller.


# ========== 比較不同探測方法 Compare Probing Methods ==========

def compare_probing_methods():  # Define a function/method that implements one operation of this unit.
    """比較三種探測方法的效能 - Compare performance of probing methods"""
    import random  # Import modules and symbols needed by this implementation.

    print("=== 探測方法比較 Probing Methods Comparison ===\n")  # Print a formatted message for the interactive example output.

    # 產生測試資料 - Generate test data
    random.seed(42)  # Execute this statement as part of the data structure implementation.
    test_keys = [f"key_{i}" for i in range(50)]  # Assign or update a variable that represents the current algorithm state.
    random.shuffle(test_keys)  # Execute this statement as part of the data structure implementation.

    for method in ProbeMethod:  # Iterate over a range/collection to process each item in sequence.
        print(f"\n--- {method.value.upper()} ---")  # Print a formatted message for the interactive example output.
        ht = OpenAddressingHashTable(capacity=67, probe_method=method)  # Create a new hash table instance to store key->value mappings.

        # 插入 - Insert
        for i, key in enumerate(test_keys):  # Iterate over a range/collection to process each item in sequence.
            ht.insert(key, i)  # Insert or update the given key with the provided value.

        print(f"元素數量: {len(ht)}")  # Print a formatted message for the interactive example output.
        print(f"容量: {ht.capacity}")  # Print a formatted message for the interactive example output.
        print(f"負載因子: {ht.load_factor:.3f}")  # Compute/check the load factor (size/capacity) to decide about resizing.
        print(f"平均探測次數: {ht.average_probes():.3f}")  # Print a formatted message for the interactive example output.

        # 搜尋測試 - Search test
        ht._total_probes = 0  # Advance or track the probing sequence used by open addressing.
        ht._operations = 0  # Assign or update a variable that represents the current algorithm state.
        for key in test_keys[:20]:  # Iterate over a range/collection to process each item in sequence.
            ht.search(key)  # Look up the current value for the given key (None if the key is missing).
        print(f"搜尋平均探測: {ht.average_probes():.3f}")  # Print a formatted message for the interactive example output.


if __name__ == "__main__":  # Evaluate the condition and branch into the appropriate code path.
    print("=== 開放定址法雜湊表示範 Open Addressing Hash Table Demo ===\n")  # Print a formatted message for the interactive example output.

    # 使用線性探測 - Using linear probing
    ht = OpenAddressingHashTable(capacity=11, probe_method=ProbeMethod.LINEAR)  # Create a new hash table instance to store key->value mappings.

    # 插入資料 - Insert data
    keys = ["A", "B", "C", "D", "E", "F"]  # Assign or update a variable that represents the current algorithm state.
    print("插入資料 Inserting data:")  # Print a formatted message for the interactive example output.
    for i, key in enumerate(keys):  # Iterate over a range/collection to process each item in sequence.
        success, probes = ht.insert(key, i * 10)  # Insert or update the given key with the provided value.
        print(f"  insert('{key}') - 成功: {success}, 探測: {probes}")  # Print a formatted message for the interactive example output.

    print(f"\n雜湊表狀態 Hash table state:")  # Print a formatted message for the interactive example output.
    print(ht.visualize())  # Print a formatted message for the interactive example output.

    print(f"\n統計 Statistics:")  # Print a formatted message for the interactive example output.
    print(f"  負載因子: {ht.load_factor:.3f}")  # Compute/check the load factor (size/capacity) to decide about resizing.
    print(f"  平均探測: {ht.average_probes():.3f}")  # Print a formatted message for the interactive example output.

    # 刪除測試 - Delete test
    print("\n刪除 'C' Delete 'C':")  # Print a formatted message for the interactive example output.
    success, probes = ht.delete("C")  # Remove the given key from the table if it exists and report success.
    print(f"  成功: {success}, 探測: {probes}")  # Print a formatted message for the interactive example output.
    print(f"\n刪除後狀態 After deletion:")  # Print a formatted message for the interactive example output.
    print(ht.visualize())  # Print a formatted message for the interactive example output.

    # 搜尋測試 - Search test
    print("\n搜尋測試 Search test:")  # Print a formatted message for the interactive example output.
    for key in ["A", "C", "F", "Z"]:  # Iterate over a range/collection to process each item in sequence.
        value, probes = ht.search(key)  # Look up the current value for the given key (None if the key is missing).
        print(f"  search('{key}') = {value}, 探測: {probes}")  # Print a formatted message for the interactive example output.

    # 比較探測方法 - Compare probing methods
    print("\n" + "="*50)  # Print a formatted message for the interactive example output.
    compare_probing_methods()  # Advance or track the probing sequence used by open addressing.
