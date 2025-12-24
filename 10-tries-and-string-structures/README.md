# 第十章：字典樹與字串結構（Tries and String Structures）

本章介紹專門處理字串的資料結構，它們在字串搜尋、自動補全、字典實作等領域有重要應用。

## 學習目標

- 理解 Trie 的結構與操作
- 掌握空間優化的壓縮 Trie（Radix Tree）
- 學會使用後綴陣列處理字串問題

## 單元列表

| 單元 | 主題 | 說明 |
|------|------|------|
| 01 | [字典樹](./01-trie/) | Trie 的結構與基本操作 |
| 02 | [基數樹](./02-radix-tree/) | 壓縮 Trie、Patricia Tree |
| 03 | [後綴陣列](./03-suffix-array/) | 後綴排序與 LCP 陣列 |

## 字典樹（Trie）

### 結構

Trie（又稱 prefix tree）是一種樹狀結構，每條從根到葉的路徑代表一個字串。

```
儲存字串: ["cat", "car", "card", "dog", "dot"]

            root
           /    \
          c      d
          |      |
          a      o
         / \    / \
        t   r  g   t
            |
            d

• = 單字結尾標記
```

### 基本操作

| 操作 | 時間複雜度 | 說明 |
|------|-----------|------|
| insert(word) | O(m) | m = 字串長度 |
| search(word) | O(m) | 精確查詢 |
| startsWith(prefix) | O(m) | 前綴查詢 |
| delete(word) | O(m) | 刪除字串 |

### 優缺點

| 優點 | 缺點 |
|------|------|
| 前綴搜尋 O(m) | 空間消耗大 |
| 支援自動補全 | 實作較複雜 |
| 字典序遍歷 | Cache 效能差 |

### 應用場景

- 自動補全（autocomplete）
- 拼字檢查（spell checker）
- IP 路由表（longest prefix matching）
- 字串排序

## 基數樹（Radix Tree）

### 壓縮 Trie

將只有單一子節點的路徑壓縮成一個節點：

```
原始 Trie:                壓縮後 (Radix Tree):
    root                      root
      |                      /    \
      r                   rom     rub
     / \                  / \       \
    o   u               an  ulus    y
    |   |
    m   b
   / \   \
  a   u   y
  |   |
  n   l
      |
      u
      |
      s

儲存: ["roman", "romulus", "ruby"]
```

### 優點

- 空間效率更高
- 減少樹的深度
- 適合稀疏的字串集合

## 後綴陣列（Suffix Array）

### 概念

後綴陣列是字串所有後綴的排序索引陣列。

```
字串 S = "banana"

後綴:
0: banana
1: anana
2: nana
3: ana
4: na
5: a

字典序排序:
5: a
3: ana
1: anana
0: banana
4: na
2: nana

後綴陣列 SA = [5, 3, 1, 0, 4, 2]
```

### LCP 陣列

LCP（Longest Common Prefix）陣列記錄相鄰後綴的最長公共前綴長度。

```
SA = [5, 3, 1, 0, 4, 2]

SA[0]=5: a
SA[1]=3: ana      LCP[1] = 1  (a)
SA[2]=1: anana    LCP[2] = 3  (ana)
SA[3]=0: banana   LCP[3] = 0
SA[4]=4: na       LCP[4] = 0
SA[5]=2: nana     LCP[5] = 2  (na)

LCP = [-, 1, 3, 0, 0, 2]
```

### 建構與查詢

| 操作 | 時間複雜度 |
|------|-----------|
| 建構後綴陣列（naive） | O(n² log n) |
| 建構後綴陣列（SA-IS） | O(n) |
| 建構 LCP 陣列（Kasai） | O(n) |
| 子字串搜尋 | O(m log n) |

### 應用

- 子字串搜尋
- 最長重複子字串
- 最長公共子字串
- 字串壓縮

## 比較總結

| 結構 | 空間 | 查詢 | 主要用途 |
|------|------|------|---------|
| Trie | O(ΣΣΣΣ) | O(m) | 前綴搜尋、自動補全 |
| Radix Tree | O(n) | O(m) | 稀疏字串集、路由表 |
| Suffix Array | O(n) | O(m log n) | 子字串搜尋、字串分析 |
| Suffix Tree | O(n) | O(m) | 複雜字串問題 |

Σ = 字母表大小，n = 字串長度，m = 查詢長度

## 考試重點

1. **Trie**
   - 給定字串集合，畫出 Trie
   - 實作 insert、search、startsWith
   - 分析空間複雜度

2. **後綴陣列**
   - 給定字串，寫出後綴陣列
   - 計算 LCP 陣列
   - 使用後綴陣列解決問題

3. **比較分析**
   - Trie vs Hash Table 的取捨
   - 後綴陣列 vs 後綴樹的優缺點

## 延伸閱讀

- Goodrich Chapter 12: Text Processing
- 競賽程式設計中的字串演算法
