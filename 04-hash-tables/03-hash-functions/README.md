# 雜湊函數（Hash Functions）

## 1. 概述

### 什麼是雜湊函數？

雜湊函數（hash function）是一個將任意大小的資料映射到固定大小值的函數：

```
h: U → {0, 1, 2, ..., m-1}

U = 所有可能的鍵的集合（universe）
m = 雜湊表的桶數量
```

### 好的雜湊函數特性

| 特性 | 說明 |
|------|------|
| 確定性（Deterministic） | 相同輸入必產生相同輸出 |
| 均勻分布（Uniform Distribution） | 輸出值在範圍內均勻分布，減少碰撞 |
| 高效計算（Efficient） | O(1) 或 O(k) 時間，k 為鍵長度 |
| 雪崩效應（Avalanche Effect） | 輸入微小變化導致輸出大幅改變 |

## 2. 整數雜湊函數

### 2.1 除法（Division Method）

最簡單的方法，直接取餘數：

```
h(k) = k mod m
```

**選擇 m 的原則**：
- 避免 2 的冪次（容易產生模式）
- 避免 10 的冪次（對十進位數字不好）
- **建議使用質數**，且遠離 2 的冪次

```
好的選擇：7, 13, 31, 61, 127, 251, 509, 1021...
不好的選擇：8, 16, 32, 64, 100, 1000...
```

### 2.2 乘法（Multiplication Method）

```
h(k) = ⌊m × (k × A mod 1)⌋

A = 常數，0 < A < 1
```

**Knuth 建議**：A = (√5 - 1) / 2 ≈ 0.6180339887...（黃金比例的倒數）

**優點**：m 的選擇較自由，常用 2 的冪次以便位元運算優化

**實作技巧**（假設 w 位元字組）：
```
h(k) = (k × s) >> (w - p)

s = ⌊A × 2^w⌋
m = 2^p
```

### 2.3 中間平方法（Mid-Square Method）

```
1. 計算 k²
2. 取中間的 r 位數字
3. m = 2^r
```

例如：k = 1234, k² = 1522756, 取中間 227, h(k) = 227

### 2.4 摺疊法（Folding Method）

將鍵分成多段，相加後取餘數：

```
k = 123456789
分段：123 + 456 + 789 = 1368
h(k) = 1368 mod m
```

## 3. 字串雜湊函數

### 3.1 簡單加總（Sum of Characters）

```python
def hash_sum(s, m):
    return sum(ord(c) for c in s) % m
```

**問題**：anagram（字母重排）會產生相同雜湊值
- "abc", "bca", "cab" → 相同雜湊值

### 3.2 多項式雜湊（Polynomial Hash）

將字串視為多項式的係數：

```
h(s) = (s[0] × a^(n-1) + s[1] × a^(n-2) + ... + s[n-1]) mod m
```

**Horner's Method** 高效計算：
```python
def hash_polynomial(s, a, m):
    h = 0
    for c in s:
        h = (h * a + ord(c)) % m
    return h
```

常用的 a 值：31, 33, 37, 39, 41

### 3.3 DJB2 雜湊

由 Daniel J. Bernstein 提出，簡單且效果好：

```python
def djb2(s):
    h = 5381
    for c in s:
        h = ((h << 5) + h) + ord(c)  # h * 33 + c
    return h & 0xFFFFFFFF
```

### 3.4 FNV 雜湊（Fowler–Noll–Vo）

```python
def fnv1a_32(s):
    h = 2166136261  # FNV offset basis
    for c in s:
        h ^= ord(c)
        h *= 16777619  # FNV prime
        h &= 0xFFFFFFFF
    return h
```

### 3.5 MurmurHash

高效能非加密雜湊，廣泛用於：
- Hadoop
- Cassandra
- Redis

特點：速度快、分布均勻、有 32/64/128 位元版本

## 4. 通用雜湊（Universal Hashing）

### 概念

從一族雜湊函數中**隨機選擇**一個使用，可以保證對於任意輸入，碰撞機率有界。

### 定義

雜湊函數族 H 是**通用的（universal）**，若對於任意不同的 x, y：

```
Pr[h(x) = h(y)] ≤ 1/m，其中 h 從 H 中隨機選取
```

### 常見的通用雜湊族

```
h_{a,b}(k) = ((a × k + b) mod p) mod m

p = 大質數，p > |U|
a ∈ {1, 2, ..., p-1}（隨機選擇）
b ∈ {0, 1, ..., p-1}（隨機選擇）
```

### 優點

- 避免對手構造最差情況輸入
- 期望碰撞數有理論保證

## 5. 完美雜湊（Perfect Hashing）

### 概念

當鍵集合 S 已知且固定時，可以設計出**零碰撞**的雜湊函數。

### 靜態完美雜湊

對於 n 個鍵，使用 O(n) 空間，保證：
- 搜尋時間 O(1) 最差情況
- 無碰撞

### 兩層架構（Two-Level Scheme）

1. **第一層**：使用通用雜湊分到 m 個桶
2. **第二層**：每個桶使用獨立的小雜湊表，大小為 nᵢ²

總空間：O(n)（期望值）

## 6. 加密雜湊 vs 非加密雜湊

| 特性 | 非加密雜湊 | 加密雜湊 |
|------|-----------|---------|
| 用途 | 資料結構、快速查找 | 安全、完整性驗證 |
| 速度 | 非常快 | 較慢 |
| 範例 | DJB2, MurmurHash | SHA-256, MD5 |
| 碰撞抵抗 | 不要求 | 強要求 |

**注意**：雜湊表使用非加密雜湊即可，不需要（也不應該）使用加密雜湊。

## 7. 程式檔案清單

| 語言 | 檔案 | 說明 |
|-----|------|-----|
| Python | `hash_functions.py` | 各種雜湊函數實作 |
| Python | `universal_hashing.py` | 通用雜湊實作 |
| Python | `test_hash_functions.py` | 單元測試與分布測試 |
| 其他語言 | ... | 相同結構 |

## 8. 考試重點

1. **計算題**
   - 給定除法/乘法雜湊函數，計算特定鍵的雜湊值
   - 分析碰撞情況

2. **選擇題**
   - 除法中 m 的選擇原則
   - 好的雜湊函數特性

3. **簡答題**
   - 解釋通用雜湊的概念與優點
   - 字串雜湊函數的設計考量

## 9. 延伸閱讀

- 基本雜湊表：見 `../01-basic-hash-table/`
- 碰撞處理：見 `../02-collision-resolution/`
- CLRS Chapter 11.3: Hash Functions
- Knuth, TAOCP Vol. 3, Section 6.4

## 參考資料

- CLRS Chapter 11: Hash Tables
- Knuth, The Art of Computer Programming, Vol. 3
- [SMHasher](https://github.com/aappleby/smhasher) - 雜湊函數測試套件
