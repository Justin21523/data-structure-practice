# 第十二章：其他進階結構（Other Advanced Structures）

本章介紹幾種實務上重要的進階資料結構，它們在系統設計、快取實作、機率資料結構等領域有廣泛應用。

## 學習目標

- 理解 Skip List 作為平衡樹的替代方案
- 掌握 Bloom Filter 的機率特性與應用
- 學會設計與實作 LRU Cache

## 單元列表

| 單元 | 主題 | 說明 |
|------|------|------|
| 01 | [跳躍表](./01-skip-list/) | 機率平衡的有序結構 |
| 02 | [布隆過濾器](./02-bloom-filter/) | 空間高效的集合成員檢測 |
| 03 | [LRU 快取](./03-lru-cache/) | 最近最少使用快取策略 |

## 跳躍表（Skip List）

### 概念

Skip List 是一種機率性資料結構，提供與平衡樹相當的效能，但實作更簡單。

```
Level 3:  Head ────────────────────────────→ 9 ──→ NIL
Level 2:  Head ──────────→ 3 ─────────────→ 9 ──→ NIL
Level 1:  Head ────→ 2 ──→ 3 ──────→ 6 ───→ 9 ──→ NIL
Level 0:  Head → 1 → 2 → 3 → 4 → 5 → 6 → 7 → 9 → NIL
```

### 時間複雜度

| 操作 | 平均 | 最差 |
|------|------|------|
| 搜尋 | O(log n) | O(n) |
| 插入 | O(log n) | O(n) |
| 刪除 | O(log n) | O(n) |

### 優點

| Skip List | 平衡樹 (如 Red-Black Tree) |
|-----------|---------------------------|
| 實作簡單 | 實作複雜 |
| 易於併發控制 | 併發控制困難 |
| 空間可調整 | 固定額外空間 |
| 區間操作直觀 | 區間操作較複雜 |

### 應用

- Redis 的 Sorted Set
- LevelDB / RocksDB 的 MemTable
- 需要有序集合但不想實作平衡樹時

## 布隆過濾器（Bloom Filter）

### 概念

Bloom Filter 是一種空間高效的機率性資料結構，用於檢測元素是否「可能存在」於集合中。

```
插入 "apple":
hash1("apple") = 2
hash2("apple") = 5
hash3("apple") = 7

位元陣列:
[0, 0, 1, 0, 0, 1, 0, 1, 0, 0]
       ↑        ↑     ↑
```

### 特性

| 查詢結果 | 含義 |
|---------|------|
| 「不存在」 | **確定**不存在 |
| 「存在」 | **可能**存在（有誤判率） |

**誤判率（False Positive Rate）**：
```
p ≈ (1 - e^(-kn/m))^k

m = 位元陣列大小
n = 已插入元素數
k = 雜湊函數數量
```

### 操作

| 操作 | 時間複雜度 | 說明 |
|------|-----------|------|
| 插入 | O(k) | k 次雜湊 |
| 查詢 | O(k) | k 次雜湊 |
| 刪除 | 不支援 | 會影響其他元素 |

### 應用

- 資料庫查詢優化（避免不必要的磁碟讀取）
- 網頁爬蟲 URL 去重
- 垃圾郵件過濾
- 快取穿透防護

## LRU 快取（LRU Cache）

### 概念

LRU（Least Recently Used）是一種快取淘汰策略，當快取滿時，移除最久未使用的項目。

```
容量 = 3

操作: put(1), put(2), put(3), get(1), put(4)

put(1): [1]
put(2): [2, 1]
put(3): [3, 2, 1]
get(1): [1, 3, 2]      ← 1 被存取，移到最前
put(4): [4, 1, 3]      ← 2 最久未用，被淘汰
```

### 實作方式

使用 **雜湊表 + 雙向鏈結串列**：

```
雜湊表: key → 節點指標
雙向鏈結串列: 維護使用順序

Head ↔ [k1,v1] ↔ [k2,v2] ↔ [k3,v3] ↔ Tail
 ↑                                    ↑
最近使用                            最久未用
```

### 操作時間複雜度

| 操作 | 時間複雜度 |
|------|-----------|
| get(key) | O(1) |
| put(key, value) | O(1) |

### API 設計

```
class LRUCache:
    def __init__(self, capacity: int)
    def get(self, key: int) -> int
    def put(self, key: int, value: int) -> None
```

### 應用

- 作業系統頁面置換
- 資料庫緩衝區管理
- Web 應用快取
- CDN 快取策略

## 其他常見快取策略

| 策略 | 全名 | 淘汰規則 |
|------|------|---------|
| LRU | Least Recently Used | 最久未使用 |
| LFU | Least Frequently Used | 使用次數最少 |
| FIFO | First In First Out | 最早進入 |
| MRU | Most Recently Used | 最近使用 |
| ARC | Adaptive Replacement Cache | 自適應混合策略 |

## 考試重點

1. **Skip List**
   - 搜尋路徑的追蹤
   - 與平衡樹的比較
   - 機率分析

2. **Bloom Filter**
   - 誤判率的計算
   - 為何不能刪除元素
   - 應用場景分析

3. **LRU Cache**
   - 實作資料結構的選擇
   - get/put 操作的流程
   - 與其他快取策略的比較

## 延伸閱讀

- Pugh, W. (1990). Skip Lists: A Probabilistic Alternative to Balanced Trees
- Bloom, B. H. (1970). Space/Time Trade-offs in Hash Coding with Allowable Errors
- 系統設計面試相關書籍
