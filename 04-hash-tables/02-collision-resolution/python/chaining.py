"""
碰撞處理：鏈結法（Chaining）- Python 實作
Collision Resolution: Chaining implementation

使用鏈結串列處理碰撞
Uses linked list to handle collisions
"""

from typing import Any, Optional, List, Tuple


class ChainedHashTable:
    """
    使用鏈結法的雜湊表
    Hash table using chaining for collision resolution
    """

    DEFAULT_CAPACITY = 16
    MAX_LOAD_FACTOR = 0.75

    def __init__(self, capacity: int = DEFAULT_CAPACITY) -> None:
        """
        初始化雜湊表
        Initialize hash table
        """
        if capacity <= 0:
            raise ValueError("容量必須為正整數 / Capacity must be positive")

        self._capacity = capacity
        self._size = 0
        # 每個桶是一個串列 - Each bucket is a list
        self._buckets: List[List[Tuple[Any, Any]]] = [
            [] for _ in range(capacity)
        ]
        # 追蹤探測次數（用於分析） - Track probe counts for analysis
        self._total_probes = 0
        self._operations = 0

    def _hash(self, key: Any) -> int:
        """計算雜湊索引 - Compute hash index"""
        return hash(key) % self._capacity

    def insert(self, key: Any, value: Any) -> int:
        """
        插入鍵值對
        Insert key-value pair

        Returns:
            探測次數 - Number of probes
        """
        index = self._hash(key)
        bucket = self._buckets[index]
        probes = 1  # 至少一次探測

        # 檢查是否已存在 - Check if exists
        for i, (k, v) in enumerate(bucket):
            probes += 1
            if k == key:
                bucket[i] = (key, value)
                self._total_probes += probes
                self._operations += 1
                return probes

        # 新增到串列尾部 - Add to end of list
        bucket.append((key, value))
        self._size += 1
        self._total_probes += probes
        self._operations += 1

        # 擴容檢查 - Check for rehash
        if self.load_factor > self.MAX_LOAD_FACTOR:
            self._rehash()

        return probes

    def search(self, key: Any) -> Tuple[Optional[Any], int]:
        """
        搜尋鍵對應的值
        Search for value by key

        Returns:
            (value, probes) - 值與探測次數
        """
        index = self._hash(key)
        bucket = self._buckets[index]
        probes = 1

        for k, v in bucket:
            if k == key:
                self._total_probes += probes
                self._operations += 1
                return v, probes
            probes += 1

        self._total_probes += probes
        self._operations += 1
        return None, probes

    def delete(self, key: Any) -> Tuple[bool, int]:
        """
        刪除鍵值對
        Delete key-value pair

        Returns:
            (success, probes) - 是否成功與探測次數
        """
        index = self._hash(key)
        bucket = self._buckets[index]
        probes = 1

        for i, (k, v) in enumerate(bucket):
            if k == key:
                del bucket[i]
                self._size -= 1
                self._total_probes += probes
                self._operations += 1
                return True, probes
            probes += 1

        self._total_probes += probes
        self._operations += 1
        return False, probes

    def _rehash(self) -> None:
        """擴容並重新雜湊 - Resize and rehash"""
        old_buckets = self._buckets
        self._capacity *= 2
        self._buckets = [[] for _ in range(self._capacity)]
        self._size = 0

        for bucket in old_buckets:
            for key, value in bucket:
                self.insert(key, value)

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

    def average_probes(self) -> float:
        """平均探測次數 - Average number of probes"""
        if self._operations == 0:
            return 0.0
        return self._total_probes / self._operations

    def get_chain_lengths(self) -> List[int]:
        """取得每個桶的鏈長度 - Get chain length of each bucket"""
        return [len(bucket) for bucket in self._buckets]

    def max_chain_length(self) -> int:
        """最長鏈長度 - Maximum chain length"""
        return max(len(bucket) for bucket in self._buckets)

    def __len__(self) -> int:
        return self._size

    def __contains__(self, key: Any) -> bool:
        value, _ = self.search(key)
        return value is not None

    def __repr__(self) -> str:
        items = []
        for bucket in self._buckets:
            for k, v in bucket:
                items.append(f"{k!r}: {v!r}")
        return "ChainedHashTable({" + ", ".join(items) + "})"


# ========== 範例與統計 Example and Statistics ==========

if __name__ == "__main__":
    print("=== 鏈結法雜湊表示範 Chaining Hash Table Demo ===\n")

    ht = ChainedHashTable(8)

    # 插入資料 - Insert data
    keys = ["apple", "banana", "cherry", "date", "elderberry",
            "fig", "grape", "honeydew", "kiwi", "lemon"]

    print("插入資料 Inserting data:")
    for i, key in enumerate(keys):
        probes = ht.insert(key, i * 10)
        print(f"  insert('{key}') - 探測次數: {probes}")

    print(f"\n統計 Statistics:")
    print(f"  元素數量: {len(ht)}")
    print(f"  桶數量: {ht.capacity}")
    print(f"  負載因子: {ht.load_factor:.3f}")
    print(f"  最長鏈長度: {ht.max_chain_length()}")
    print(f"  平均探測次數: {ht.average_probes():.3f}")

    # 鏈長度分布 - Chain length distribution
    lengths = ht.get_chain_lengths()
    print(f"\n鏈長度分布 Chain lengths: {lengths}")

    # 搜尋測試 - Search test
    print("\n搜尋測試 Search test:")
    for key in ["apple", "fig", "mango"]:
        value, probes = ht.search(key)
        print(f"  search('{key}') = {value}, 探測: {probes}")
