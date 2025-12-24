"""
碰撞處理：開放定址法（Open Addressing）- Python 實作
Collision Resolution: Open Addressing implementation

實作三種探測方法：線性探測、二次探測、雙重雜湊
Implements three probing methods: Linear, Quadratic, Double Hashing
"""

from typing import Any, Optional, Tuple
from enum import Enum


class ProbeMethod(Enum):
    """探測方法列舉 - Probing method enum"""
    LINEAR = "linear"           # 線性探測
    QUADRATIC = "quadratic"     # 二次探測
    DOUBLE_HASH = "double_hash" # 雙重雜湊


class OpenAddressingHashTable:
    """
    使用開放定址法的雜湊表
    Hash table using open addressing for collision resolution
    """

    # 特殊標記 - Special markers
    EMPTY = object()      # 空槽位
    DELETED = object()    # 墓碑標記（已刪除）

    DEFAULT_CAPACITY = 16
    MAX_LOAD_FACTOR = 0.7  # 開放定址法需要較低的負載因子

    def __init__(self, capacity: int = DEFAULT_CAPACITY,
                 probe_method: ProbeMethod = ProbeMethod.LINEAR) -> None:
        """
        初始化雜湊表
        Initialize hash table

        Args:
            capacity: 容量（建議使用質數）
            probe_method: 探測方法
        """
        if capacity <= 0:
            raise ValueError("容量必須為正整數 / Capacity must be positive")

        self._capacity = capacity
        self._size = 0
        self._probe_method = probe_method
        # 使用兩個陣列分別存 key 和 value
        self._keys = [self.EMPTY] * capacity
        self._values = [None] * capacity
        # 統計資料 - Statistics
        self._total_probes = 0
        self._operations = 0

    def _hash1(self, key: Any) -> int:
        """主雜湊函數 - Primary hash function"""
        return hash(key) % self._capacity

    def _hash2(self, key: Any) -> int:
        """
        次雜湊函數（用於雙重雜湊）
        Secondary hash function for double hashing

        確保回傳值與 capacity 互質
        Ensures return value is coprime with capacity
        """
        # 使用 capacity - 1 確保結果 >= 1
        return 1 + (hash(key) % (self._capacity - 1))

    def _probe(self, key: Any, i: int) -> int:
        """
        計算第 i 次探測的索引
        Calculate index for i-th probe

        Args:
            key: 鍵
            i: 探測次數（從 0 開始）

        Returns:
            探測索引
        """
        h1 = self._hash1(key)

        if self._probe_method == ProbeMethod.LINEAR:
            # 線性探測：h(k) + i
            return (h1 + i) % self._capacity

        elif self._probe_method == ProbeMethod.QUADRATIC:
            # 二次探測：h(k) + i²
            return (h1 + i * i) % self._capacity

        elif self._probe_method == ProbeMethod.DOUBLE_HASH:
            # 雙重雜湊：h₁(k) + i * h₂(k)
            h2 = self._hash2(key)
            return (h1 + i * h2) % self._capacity

        return h1

    def insert(self, key: Any, value: Any) -> Tuple[bool, int]:
        """
        插入鍵值對
        Insert key-value pair

        Returns:
            (success, probes) - 是否成功與探測次數
        """
        if self.load_factor >= self.MAX_LOAD_FACTOR:
            self._rehash()

        first_deleted = -1  # 記錄第一個墓碑位置

        for i in range(self._capacity):
            index = self._probe(key, i)

            if self._keys[index] is self.EMPTY:
                # 找到空位 - Found empty slot
                if first_deleted != -1:
                    index = first_deleted  # 使用之前的墓碑位置
                self._keys[index] = key
                self._values[index] = value
                self._size += 1
                self._total_probes += i + 1
                self._operations += 1
                return True, i + 1

            elif self._keys[index] is self.DELETED:
                # 記錄第一個墓碑 - Record first tombstone
                if first_deleted == -1:
                    first_deleted = index

            elif self._keys[index] == key:
                # 鍵已存在，更新值 - Key exists, update value
                self._values[index] = value
                self._total_probes += i + 1
                self._operations += 1
                return True, i + 1

        # 表滿（不應發生，因為有負載因子限制）
        self._operations += 1
        return False, self._capacity

    def search(self, key: Any) -> Tuple[Optional[Any], int]:
        """
        搜尋鍵對應的值
        Search for value by key

        Returns:
            (value, probes) - 值與探測次數，未找到回傳 (None, probes)
        """
        for i in range(self._capacity):
            index = self._probe(key, i)

            if self._keys[index] is self.EMPTY:
                # 遇到空位，鍵不存在 - Empty slot, key doesn't exist
                self._total_probes += i + 1
                self._operations += 1
                return None, i + 1

            elif self._keys[index] is self.DELETED:
                # 跳過墓碑繼續探測 - Skip tombstone, continue probing
                continue

            elif self._keys[index] == key:
                # 找到鍵 - Found key
                self._total_probes += i + 1
                self._operations += 1
                return self._values[index], i + 1

        self._total_probes += self._capacity
        self._operations += 1
        return None, self._capacity

    def delete(self, key: Any) -> Tuple[bool, int]:
        """
        刪除鍵值對（使用墓碑標記）
        Delete key-value pair (using tombstone marker)

        Returns:
            (success, probes) - 是否成功與探測次數
        """
        for i in range(self._capacity):
            index = self._probe(key, i)

            if self._keys[index] is self.EMPTY:
                # 遇到空位，鍵不存在 - Empty slot, key doesn't exist
                self._total_probes += i + 1
                self._operations += 1
                return False, i + 1

            elif self._keys[index] is self.DELETED:
                # 跳過墓碑 - Skip tombstone
                continue

            elif self._keys[index] == key:
                # 找到鍵，標記為已刪除 - Found key, mark as deleted
                self._keys[index] = self.DELETED
                self._values[index] = None
                self._size -= 1
                self._total_probes += i + 1
                self._operations += 1
                return True, i + 1

        self._total_probes += self._capacity
        self._operations += 1
        return False, self._capacity

    def _rehash(self) -> None:
        """擴容並重新雜湊 - Resize and rehash"""
        old_keys = self._keys
        old_values = self._values
        old_capacity = self._capacity

        # 容量加倍 - Double capacity
        self._capacity *= 2
        self._keys = [self.EMPTY] * self._capacity
        self._values = [None] * self._capacity
        self._size = 0

        # 重新插入（跳過 EMPTY 和 DELETED）
        for i in range(old_capacity):
            if old_keys[i] is not self.EMPTY and old_keys[i] is not self.DELETED:
                self.insert(old_keys[i], old_values[i])

    @property
    def load_factor(self) -> float:
        """負載因子 - Load factor"""
        return self._size / self._capacity

    @property
    def size(self) -> int:
        return self._size

    @property
    def capacity(self) -> int:
        return self._capacity

    @property
    def probe_method(self) -> ProbeMethod:
        return self._probe_method

    def average_probes(self) -> float:
        """平均探測次數 - Average number of probes"""
        if self._operations == 0:
            return 0.0
        return self._total_probes / self._operations

    def __len__(self) -> int:
        return self._size

    def __contains__(self, key: Any) -> bool:
        value, _ = self.search(key)
        return value is not None

    def visualize(self) -> str:
        """視覺化雜湊表狀態 - Visualize hash table state"""
        result = []
        for i in range(self._capacity):
            if self._keys[i] is self.EMPTY:
                result.append(f"[{i}]: EMPTY")
            elif self._keys[i] is self.DELETED:
                result.append(f"[{i}]: DELETED")
            else:
                result.append(f"[{i}]: {self._keys[i]!r} -> {self._values[i]!r}")
        return "\n".join(result)


# ========== 比較不同探測方法 Compare Probing Methods ==========

def compare_probing_methods():
    """比較三種探測方法的效能 - Compare performance of probing methods"""
    import random

    print("=== 探測方法比較 Probing Methods Comparison ===\n")

    # 產生測試資料 - Generate test data
    random.seed(42)
    test_keys = [f"key_{i}" for i in range(50)]
    random.shuffle(test_keys)

    for method in ProbeMethod:
        print(f"\n--- {method.value.upper()} ---")
        ht = OpenAddressingHashTable(capacity=67, probe_method=method)

        # 插入 - Insert
        for i, key in enumerate(test_keys):
            ht.insert(key, i)

        print(f"元素數量: {len(ht)}")
        print(f"容量: {ht.capacity}")
        print(f"負載因子: {ht.load_factor:.3f}")
        print(f"平均探測次數: {ht.average_probes():.3f}")

        # 搜尋測試 - Search test
        ht._total_probes = 0
        ht._operations = 0
        for key in test_keys[:20]:
            ht.search(key)
        print(f"搜尋平均探測: {ht.average_probes():.3f}")


if __name__ == "__main__":
    print("=== 開放定址法雜湊表示範 Open Addressing Hash Table Demo ===\n")

    # 使用線性探測 - Using linear probing
    ht = OpenAddressingHashTable(capacity=11, probe_method=ProbeMethod.LINEAR)

    # 插入資料 - Insert data
    keys = ["A", "B", "C", "D", "E", "F"]
    print("插入資料 Inserting data:")
    for i, key in enumerate(keys):
        success, probes = ht.insert(key, i * 10)
        print(f"  insert('{key}') - 成功: {success}, 探測: {probes}")

    print(f"\n雜湊表狀態 Hash table state:")
    print(ht.visualize())

    print(f"\n統計 Statistics:")
    print(f"  負載因子: {ht.load_factor:.3f}")
    print(f"  平均探測: {ht.average_probes():.3f}")

    # 刪除測試 - Delete test
    print("\n刪除 'C' Delete 'C':")
    success, probes = ht.delete("C")
    print(f"  成功: {success}, 探測: {probes}")
    print(f"\n刪除後狀態 After deletion:")
    print(ht.visualize())

    # 搜尋測試 - Search test
    print("\n搜尋測試 Search test:")
    for key in ["A", "C", "F", "Z"]:
        value, probes = ht.search(key)
        print(f"  search('{key}') = {value}, 探測: {probes}")

    # 比較探測方法 - Compare probing methods
    print("\n" + "="*50)
    compare_probing_methods()
