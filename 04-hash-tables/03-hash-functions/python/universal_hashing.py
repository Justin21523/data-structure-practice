"""
通用雜湊（Universal Hashing）- Python 實作
Universal Hashing implementation

從雜湊函數族中隨機選擇函數，保證碰撞機率有界
Randomly select from a family of hash functions to bound collision probability
"""

import random
from typing import Any, Optional


def is_prime(n: int) -> bool:
    """檢查是否為質數 - Check if prime"""
    if n < 2:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    for i in range(3, int(n**0.5) + 1, 2):
        if n % i == 0:
            return False
    return True


def next_prime(n: int) -> int:
    """找到大於等於 n 的最小質數 - Find smallest prime >= n"""
    while not is_prime(n):
        n += 1
    return n


class UniversalHashFamily:
    """
    通用雜湊函數族
    Universal hash function family

    h_{a,b}(k) = ((a * k + b) mod p) mod m

    保證對任意不同的 x, y：
    Pr[h(x) = h(y)] <= 1/m
    """

    def __init__(self, m: int, p: int = None):
        """
        初始化通用雜湊函數族
        Initialize universal hash family

        Args:
            m: 雜湊表大小
            p: 大質數（應大於所有可能的鍵值）
        """
        self.m = m
        # 選擇一個大質數 - Choose a large prime
        self.p = p if p else next_prime(max(m * 10, 10007))
        # 隨機選擇參數 - Randomly choose parameters
        self.regenerate()

    def regenerate(self) -> None:
        """
        重新生成隨機參數
        Regenerate random parameters

        a ∈ {1, 2, ..., p-1}
        b ∈ {0, 1, ..., p-1}
        """
        self.a = random.randint(1, self.p - 1)
        self.b = random.randint(0, self.p - 1)

    def hash(self, key: int) -> int:
        """
        計算雜湊值
        Compute hash value

        h(k) = ((a * k + b) mod p) mod m

        Args:
            key: 整數鍵

        Returns:
            雜湊值 0 ~ m-1
        """
        return ((self.a * key + self.b) % self.p) % self.m

    def __call__(self, key: int) -> int:
        """允許直接呼叫 - Allow direct call"""
        return self.hash(key)

    def get_parameters(self) -> dict:
        """取得目前的參數 - Get current parameters"""
        return {
            "a": self.a,
            "b": self.b,
            "p": self.p,
            "m": self.m
        }


class UniversalStringHashFamily:
    """
    字串的通用雜湊函數族
    Universal hash function family for strings

    將字串視為數字序列，使用多項式雜湊
    Treats string as sequence of numbers, uses polynomial hashing
    """

    def __init__(self, m: int, p: int = None):
        """
        初始化字串通用雜湊
        Initialize universal string hash

        Args:
            m: 雜湊表大小
            p: 大質數
        """
        self.m = m
        self.p = p if p else next_prime(max(m * 10, 10007))
        self.regenerate()

    def regenerate(self) -> None:
        """重新生成隨機參數 - Regenerate random parameters"""
        self.a = random.randint(1, self.p - 1)

    def hash(self, s: str) -> int:
        """
        計算字串雜湊值
        Compute string hash value

        使用多項式雜湊：h(s) = sum(s[i] * a^i) mod p mod m
        Uses polynomial hash

        Args:
            s: 字串

        Returns:
            雜湊值 0 ~ m-1
        """
        h = 0
        for c in s:
            h = (h * self.a + ord(c)) % self.p
        return h % self.m

    def __call__(self, s: str) -> int:
        return self.hash(s)


class UniversalHashTable:
    """
    使用通用雜湊的雜湊表
    Hash table using universal hashing

    可以在偵測到攻擊（過多碰撞）時重新選擇雜湊函數
    Can regenerate hash function when detecting attacks (too many collisions)
    """

    DEFAULT_CAPACITY = 16
    MAX_LOAD_FACTOR = 0.75
    MAX_CHAIN_LENGTH = 10  # 觸發重新雜湊的閾值

    def __init__(self, capacity: int = DEFAULT_CAPACITY):
        """初始化雜湊表 - Initialize hash table"""
        self._capacity = capacity
        self._size = 0
        self._buckets = [[] for _ in range(capacity)]
        self._hash_family = UniversalHashFamily(capacity)
        self._rehash_count = 0

    def _hash(self, key: int) -> int:
        """計算雜湊值 - Compute hash"""
        return self._hash_family(key)

    def insert(self, key: int, value: Any) -> None:
        """
        插入鍵值對
        Insert key-value pair
        """
        index = self._hash(key)
        bucket = self._buckets[index]

        # 檢查是否已存在 - Check if exists
        for i, (k, v) in enumerate(bucket):
            if k == key:
                bucket[i] = (key, value)
                return

        bucket.append((key, value))
        self._size += 1

        # 檢查是否需要擴容或重新雜湊 - Check for resize or rehash
        if self.load_factor > self.MAX_LOAD_FACTOR:
            self._resize()
        elif len(bucket) > self.MAX_CHAIN_LENGTH:
            # 鏈太長，可能遭受攻擊，重新選擇雜湊函數
            # Chain too long, possible attack, regenerate hash
            self._regenerate_hash()

    def search(self, key: int) -> Optional[Any]:
        """搜尋鍵對應的值 - Search for value"""
        index = self._hash(key)
        for k, v in self._buckets[index]:
            if k == key:
                return v
        return None

    def delete(self, key: int) -> bool:
        """刪除鍵值對 - Delete key-value pair"""
        index = self._hash(key)
        bucket = self._buckets[index]

        for i, (k, v) in enumerate(bucket):
            if k == key:
                del bucket[i]
                self._size -= 1
                return True
        return False

    def _resize(self) -> None:
        """擴容 - Resize"""
        old_buckets = self._buckets
        self._capacity *= 2
        self._buckets = [[] for _ in range(self._capacity)]
        self._hash_family = UniversalHashFamily(self._capacity)
        self._size = 0

        for bucket in old_buckets:
            for key, value in bucket:
                self.insert(key, value)

    def _regenerate_hash(self) -> None:
        """
        重新選擇雜湊函數並重新分配所有元素
        Regenerate hash function and redistribute all elements
        """
        self._rehash_count += 1
        old_buckets = self._buckets
        self._buckets = [[] for _ in range(self._capacity)]
        self._hash_family.regenerate()
        self._size = 0

        for bucket in old_buckets:
            for key, value in bucket:
                self.insert(key, value)

    @property
    def load_factor(self) -> float:
        return self._size / self._capacity

    @property
    def size(self) -> int:
        return self._size

    @property
    def rehash_count(self) -> int:
        """重新雜湊次數 - Number of rehashes"""
        return self._rehash_count

    def get_max_chain_length(self) -> int:
        """最長鏈長度 - Maximum chain length"""
        return max(len(bucket) for bucket in self._buckets)


# ============================================================
# 範例與測試 Example and Test
# ============================================================

if __name__ == "__main__":
    print("=== 通用雜湊示範 Universal Hashing Demo ===\n")

    # 基本通用雜湊 - Basic universal hashing
    print("--- 通用雜湊函數族 Universal Hash Family ---")
    m = 10
    uh = UniversalHashFamily(m)
    print(f"Parameters: {uh.get_parameters()}")

    test_keys = [10, 20, 30, 40, 50, 100, 200, 300]
    print(f"Keys: {test_keys}")
    print(f"Hash values: {[uh(k) for k in test_keys]}")

    print("\n重新生成參數後 After regenerating:")
    uh.regenerate()
    print(f"Parameters: {uh.get_parameters()}")
    print(f"Hash values: {[uh(k) for k in test_keys]}")

    # 碰撞機率驗證 - Collision probability verification
    print("\n--- 碰撞機率驗證 Collision Probability ---")
    m = 100
    trials = 10000
    collisions = 0
    x, y = 42, 137  # 兩個固定的不同鍵

    for _ in range(trials):
        uh = UniversalHashFamily(m)
        if uh(x) == uh(y):
            collisions += 1

    expected = trials / m
    print(f"鍵 x={x}, y={y}, m={m}")
    print(f"試驗次數: {trials}")
    print(f"碰撞次數: {collisions}")
    print(f"碰撞機率: {collisions/trials:.4f}")
    print(f"理論上界: {1/m:.4f}")
    print(f"期望碰撞: {expected:.1f}")

    # 使用通用雜湊的雜湊表 - Hash table with universal hashing
    print("\n--- 使用通用雜湊的雜湊表 ---")
    ht = UniversalHashTable()

    for i in range(50):
        ht.insert(i * 7, f"value_{i}")

    print(f"元素數量: {ht.size}")
    print(f"負載因子: {ht.load_factor:.3f}")
    print(f"最長鏈長: {ht.get_max_chain_length()}")
    print(f"重新雜湊次數: {ht.rehash_count}")

    # 搜尋測試
    print(f"search(14) = {ht.search(14)}")
    print(f"search(999) = {ht.search(999)}")
