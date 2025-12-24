# 雜湊表（Hash Table）

## 1. 動機與概述

### 為什麼需要雜湊表？

在處理「鍵值對（key-value pair）」的存取問題時，我們希望能快速地：
- 根據 key 查找對應的 value
- 插入新的鍵值對
- 刪除指定的鍵值對

| 資料結構 | 搜尋 | 插入 | 刪除 |
|---------|------|------|------|
| 未排序陣列 | O(n) | O(1) | O(n) |
| 已排序陣列 | O(log n) | O(n) | O(n) |
| 鏈結串列 | O(n) | O(1) | O(n) |
| 二元搜尋樹 | O(log n) | O(log n) | O(log n) |
| **雜湊表** | **O(1)** 平均 | **O(1)** 平均 | **O(1)** 平均 |

雜湊表透過**雜湊函數（hash function）**將 key 直接對應到儲存位置，達成平均 O(1) 的操作效率。

### 直接定址表（Direct-Address Table）

最簡單的想法：如果 key 的範圍是 0 ~ m-1，直接用陣列 `T[0..m-1]` 儲存。

```
T[key] = value
```

**限制**：
- key 必須是整數
- key 的範圍不能太大（否則浪費空間）
- 實務上 key 常是字串或複雜物件

**解決方案**：使用雜湊函數將任意 key 轉換為陣列索引。

## 2. 核心概念

### 雜湊函數（Hash Function）

雜湊函數 `h(key)` 將 key 對應到桶（bucket）的索引：

```
h: Key → {0, 1, 2, ..., m-1}
```

```
Key "apple" → h("apple") → 42 → bucket[42] → Value

        hash function
Key ──────────────────→ Index ──→ Bucket ──→ Value
```

### 桶 / 槽位（Bucket / Slot）

雜湊表的儲存單位，每個桶可存放一或多個鍵值對。

```
bucket[0]: (key1, value1) → (key5, value5) → null
bucket[1]: (key2, value2) → null
bucket[2]: null
bucket[3]: (key3, value3) → (key4, value4) → null
...
bucket[m-1]: ...
```

### 負載因子（Load Factor）

衡量雜湊表「擁擠程度」的指標：

```
α = n / m

n = 儲存的元素數量
m = 桶（bucket）的數量
```

- α 越高 → 碰撞越多 → 效能越差
- 一般建議維持 α < 0.75

### 碰撞（Collision）

當兩個不同的 key 被雜湊到同一個桶時，稱為碰撞：

```
h("apple") = 42
h("banana") = 42  ← 碰撞！
```

碰撞是無法避免的（鴿籠原理），必須有策略處理。

## 3. 基本操作與時間複雜度

| 操作 | 平均時間 | 最差時間 | 說明 |
|------|---------|---------|------|
| 搜尋（search） | O(1) | O(n) | 最差：所有 key 碰撞到同一桶 |
| 插入（insert） | O(1) | O(n) | 含搜尋檢查重複 |
| 刪除（delete） | O(1) | O(n) | 需先搜尋 |

**平均時間的前提**：
- 雜湊函數分布均勻
- 負載因子維持在合理範圍

## 4. 碰撞處理概述（Collision Handling）

### 鏈結法（Chaining）

每個桶維護一個鏈結串列，碰撞的元素接在串列後面。

```
bucket[0] → null
bucket[1] → (k1,v1) → null
bucket[2] → (k2,v2) → (k5,v5) → (k8,v8) → null
bucket[3] → (k3,v3) → null
```

**優點**：實作簡單，刪除容易
**缺點**：額外的指標空間，cache 效能較差

### 開放定址法（Open Addressing）

碰撞時探測下一個空位，所有元素都存在桶陣列中。

| 探測方法 | 探測序列 |
|---------|---------|
| 線性探測（Linear Probing） | h(k)+1, h(k)+2, h(k)+3, ... |
| 二次探測（Quadratic Probing） | h(k)+1², h(k)+2², h(k)+3², ... |
| 雙重雜湊（Double Hashing） | h₁(k)+i·h₂(k) |

**詳細實作請參考** `../02-collision-resolution/`

## 5. 動態調整大小（Dynamic Resizing）

### 何時擴容？

當負載因子超過閾值（如 0.75）時觸發擴容。

### 擴容策略

1. 建立新的、較大的陣列（通常 2 倍）
2. 重新計算所有元素的雜湊值（rehashing）
3. 將元素放入新陣列

```
擴容前 (m=4, n=3, α=0.75):
[_, (k1,v1), (k2,v2), (k3,v3)]

擴容後 (m=8, n=3, α=0.375):
[_, _, (k1,v1), _, (k2,v2), _, _, (k3,v3)]
```

### 攤銷分析（Amortized Analysis）

單次擴容的時間複雜度是 O(n)，但平均到每次插入：
- 每個元素最多被 rehash O(log n) 次
- 攤銷後每次插入仍是 O(1)

## 6. 使用情境

| 應用 | 說明 |
|------|------|
| 字典 / 映射（Dictionary / Map） | key-value 存取 |
| 集合（Set） | 只存 key，不存 value |
| 快取（Cache） | 快速查找已計算的結果 |
| 去重（Deduplication） | 快速判斷元素是否出現過 |
| 計數器（Frequency Counter） | 統計元素出現次數 |
| 符號表（Symbol Table） | 編譯器中的變數查找 |

## 7. 程式檔案清單

| 語言 | 檔案 | 說明 |
|-----|------|-----|
| Python | `hash_table.py` | 雜湊表類別實作（使用 chaining） |
| Python | `example_word_count.py` | 應用範例：單字計數 |
| Python | `test_hash_table.py` | 單元測試 |
| Java | `HashTable.java` | 雜湊表類別實作 |
| Java | `ExampleWordCount.java` | 應用範例：單字計數 |
| Java | `HashTableTest.java` | 單元測試 |
| JavaScript | `hashTable.js` | 雜湊表類別實作 |
| JavaScript | `exampleWordCount.js` | 應用範例：單字計數 |
| JavaScript | `hashTable.test.js` | 單元測試 |
| C | `hash_table.h` | 標頭檔 |
| C | `hash_table.c` | 雜湊表實作 |
| C | `example_word_count.c` | 應用範例 |
| C | `test_hash_table.c` | 單元測試 |
| C++ | `HashTable.hpp` | 模板類別實作 |
| C++ | `example_word_count.cpp` | 應用範例 |
| C++ | `test_hash_table.cpp` | 單元測試 |
| C# | `HashTable.cs` | 雜湊表類別實作 |
| C# | `ExampleWordCount.cs` | 應用範例 |
| C# | `HashTableTest.cs` | 單元測試 |

## 8. 考試重點提示

### 常考觀念

1. **負載因子與效能的關係**
   - α 增加 → 碰撞增加 → 平均搜尋時間增加
   - Chaining：平均搜尋時間 = 1 + α/2（成功）或 α（失敗）

2. **雜湊函數的設計原則**
   - 確定性：相同輸入必產生相同輸出
   - 均勻分布：減少碰撞
   - 高效計算：O(1) 時間

3. **碰撞處理方式的比較**
   | 特性 | Chaining | Open Addressing |
   |------|----------|-----------------|
   | 空間 | 額外指標 | 無額外空間 |
   | 刪除 | 簡單 | 需標記 |
   | α 限制 | α 可 > 1 | α 必須 < 1 |
   | Cache | 較差 | 較佳（線性探測） |

### 常見題型

1. 給定雜湊函數與碰撞處理方式，畫出插入後的雜湊表
2. 計算平均成功/失敗搜尋長度
3. 設計適合特定資料的雜湊函數
4. 分析特定操作序列的時間複雜度

## 9. 延伸閱讀

- **雜湊函數詳細設計**：見 `../03-hash-functions/`
- **碰撞處理詳細實作**：見 `../02-collision-resolution/`
- **相關演算法實作**：可參考 `tw-gradcs-algorithms` 中對應單元

## 參考資料

- CLRS Chapter 11: Hash Tables
- Goodrich Chapter 9: Maps, Hash Tables, and Skip Lists
- Weiss Chapter 5: Hashing
