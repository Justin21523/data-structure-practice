"""
雜湊表（Hash Table）- Python 實作
使用鏈結法（chaining）處理碰撞

Hash Table implementation using chaining for collision resolution
"""

from typing import Any, Optional, Iterator, Tuple


class HashTable:
    """
    基本雜湊表實作
    Basic hash table implementation using chaining

    支援操作 Supported operations:
        - insert(key, value): 插入鍵值對
        - search(key): 搜尋鍵對應的值
        - delete(key): 刪除鍵值對
        - contains(key): 檢查鍵是否存在
    """

    # 預設初始容量 - Default initial capacity
    DEFAULT_CAPACITY = 16
    # 負載因子閾值 - Load factor threshold for rehashing
    MAX_LOAD_FACTOR = 0.75

    def __init__(self, capacity: int = DEFAULT_CAPACITY) -> None:
        """
        初始化雜湊表
        Initialize hash table with given capacity

        Args:
            capacity: 桶的數量（number of buckets）
                     建議使用 2 的冪次以優化取餘運算
        """
        if capacity <= 0:
            raise ValueError("容量必須為正整數 / Capacity must be positive")

        self._capacity = capacity
        self._size = 0
        # 使用串列的串列來實作鏈結法
        # Use list of lists for chaining
        self._buckets: list[list[Tuple[Any, Any]]] = [
            [] for _ in range(capacity)
        ]

    def _hash(self, key: Any) -> int:
        """
        計算雜湊值
        Compute hash value for the given key

        使用 Python 內建 hash() 後取餘數
        Uses built-in hash() with modulo

        Args:
            key: 要雜湊的鍵

        Returns:
            桶的索引值（bucket index）
        """
        return hash(key) % self._capacity

    def insert(self, key: Any, value: Any) -> None:
        """
        插入鍵值對（若 key 已存在則更新 value）
        Insert key-value pair (update if key exists)

        時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)

        Args:
            key: 鍵
            value: 值
        """
        index = self._hash(key)
        bucket = self._buckets[index]

        # 檢查 key 是否已存在 - Check if key exists
        for i, (k, v) in enumerate(bucket):
            if k == key:
                bucket[i] = (key, value)  # 更新 - Update existing
                return

        # 新增鍵值對 - Add new key-value pair
        bucket.append((key, value))
        self._size += 1

        # 檢查是否需要擴容 - Check if rehashing needed
        if self.load_factor > self.MAX_LOAD_FACTOR:
            self._rehash()

    def search(self, key: Any) -> Optional[Any]:
        """
        搜尋給定 key 的 value
        Search for value associated with key

        時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)

        Args:
            key: 要搜尋的鍵

        Returns:
            找到則回傳 value，否則回傳 None
            Value if found, None otherwise
        """
        index = self._hash(key)
        bucket = self._buckets[index]

        for k, v in bucket:
            if k == key:
                return v
        return None

    def delete(self, key: Any) -> bool:
        """
        刪除指定的鍵值對
        Delete key-value pair

        時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)

        Args:
            key: 要刪除的鍵

        Returns:
            刪除成功回傳 True，key 不存在回傳 False
            True if deleted, False if key not found
        """
        index = self._hash(key)
        bucket = self._buckets[index]

        for i, (k, v) in enumerate(bucket):
            if k == key:
                del bucket[i]
                self._size -= 1
                return True
        return False

    def contains(self, key: Any) -> bool:
        """
        檢查 key 是否存在於雜湊表中
        Check if key exists in hash table

        Args:
            key: 要檢查的鍵

        Returns:
            存在回傳 True，否則回傳 False
        """
        return self.search(key) is not None

    @property
    def load_factor(self) -> float:
        """
        計算負載因子 α = n / m
        Calculate load factor

        Returns:
            負載因子（load factor）
        """
        return self._size / self._capacity

    @property
    def size(self) -> int:
        """
        回傳元素數量
        Return number of elements
        """
        return self._size

    @property
    def capacity(self) -> int:
        """
        回傳桶的數量
        Return number of buckets
        """
        return self._capacity

    def is_empty(self) -> bool:
        """
        檢查雜湊表是否為空
        Check if hash table is empty
        """
        return self._size == 0

    def clear(self) -> None:
        """
        清空雜湊表
        Clear all elements from hash table
        """
        self._buckets = [[] for _ in range(self._capacity)]
        self._size = 0

    def _rehash(self) -> None:
        """
        擴容並重新雜湊所有元素
        Resize and rehash all elements

        將容量加倍以維持 O(1) 攤銷時間
        Double capacity to maintain O(1) amortized time
        """
        old_buckets = self._buckets
        self._capacity *= 2
        self._buckets = [[] for _ in range(self._capacity)]
        self._size = 0

        # 重新插入所有元素 - Reinsert all elements
        for bucket in old_buckets:
            for key, value in bucket:
                self.insert(key, value)

    def keys(self) -> Iterator[Any]:
        """
        回傳所有鍵的迭代器
        Return iterator over all keys
        """
        for bucket in self._buckets:
            for key, _ in bucket:
                yield key

    def values(self) -> Iterator[Any]:
        """
        回傳所有值的迭代器
        Return iterator over all values
        """
        for bucket in self._buckets:
            for _, value in bucket:
                yield value

    def items(self) -> Iterator[Tuple[Any, Any]]:
        """
        回傳所有鍵值對的迭代器
        Return iterator over all key-value pairs
        """
        for bucket in self._buckets:
            for key, value in bucket:
                yield (key, value)

    # ========== Python 魔術方法 Magic Methods ==========

    def __len__(self) -> int:
        """支援 len() 函數 - Support len() function"""
        return self._size

    def __contains__(self, key: Any) -> bool:
        """支援 'in' 運算子 - Support 'in' operator"""
        return self.contains(key)

    def __getitem__(self, key: Any) -> Any:
        """
        支援 [] 取值語法 - Support [] access syntax

        若 key 不存在則拋出 KeyError
        Raises KeyError if key not found
        """
        value = self.search(key)
        if value is None and not self.contains(key):
            raise KeyError(f"Key not found: {key}")
        return value

    def __setitem__(self, key: Any, value: Any) -> None:
        """支援 [] 設值語法 - Support [] assignment syntax"""
        self.insert(key, value)

    def __delitem__(self, key: Any) -> None:
        """
        支援 del 語法 - Support del syntax

        若 key 不存在則拋出 KeyError
        Raises KeyError if key not found
        """
        if not self.delete(key):
            raise KeyError(f"Key not found: {key}")

    def __iter__(self) -> Iterator[Any]:
        """支援迭代 - Support iteration over keys"""
        return self.keys()

    def __repr__(self) -> str:
        """字串表示 - String representation"""
        items = [f"{k!r}: {v!r}" for k, v in self.items()]
        return "HashTable({" + ", ".join(items) + "})"

    def __str__(self) -> str:
        """人類可讀的字串 - Human readable string"""
        return self.__repr__()


# ========== 簡便函式 Convenience Functions ==========

def create_hash_table(data: dict = None, capacity: int = 16) -> HashTable:
    """
    建立雜湊表的便利函式
    Convenience function to create hash table

    Args:
        data: 初始資料（optional initial data）
        capacity: 初始容量

    Returns:
        HashTable 實例
    """
    ht = HashTable(capacity)
    if data:
        for key, value in data.items():
            ht.insert(key, value)
    return ht


# ========== 範例程式 Example Usage ==========

if __name__ == "__main__":
    print("=== 雜湊表基本操作示範 Hash Table Basic Operations Demo ===\n")

    # 建立雜湊表 - Create hash table
    ht = HashTable()

    # 插入操作 - Insert operations
    print("插入鍵值對 Inserting key-value pairs:")
    ht.insert("apple", 100)
    ht.insert("banana", 200)
    ht.insert("cherry", 300)
    ht["date"] = 400  # 使用 [] 語法
    print(f"  雜湊表: {ht}")
    print(f"  大小: {len(ht)}, 負載因子: {ht.load_factor:.3f}\n")

    # 搜尋操作 - Search operations
    print("搜尋操作 Search operations:")
    print(f"  search('apple') = {ht.search('apple')}")
    print(f"  ht['banana'] = {ht['banana']}")
    print(f"  search('grape') = {ht.search('grape')}")
    print(f"  'cherry' in ht = {'cherry' in ht}\n")

    # 更新操作 - Update operation
    print("更新操作 Update operation:")
    ht["apple"] = 150
    print(f"  更新 apple 的值為 150")
    print(f"  ht['apple'] = {ht['apple']}\n")

    # 刪除操作 - Delete operation
    print("刪除操作 Delete operations:")
    ht.delete("banana")
    print(f"  刪除 'banana' 後: {ht}")
    print(f"  大小: {len(ht)}\n")

    # 迭代操作 - Iteration
    print("迭代操作 Iteration:")
    print(f"  所有鍵 Keys: {list(ht.keys())}")
    print(f"  所有值 Values: {list(ht.values())}")
    print(f"  所有項目 Items: {list(ht.items())}")
