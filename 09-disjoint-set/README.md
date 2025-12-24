# 第九章：互斥集合（Disjoint Set）

本章介紹互斥集合（disjoint set）資料結構，也稱為並查集（Union-Find）。它是處理動態連通性問題的高效工具。

## 學習目標

- 理解互斥集合的概念與應用場景
- 掌握 Union-Find 的實作與優化技巧
- 學會分析 Union-Find 的時間複雜度

## 單元列表

| 單元 | 主題 | 說明 |
|------|------|------|
| 01 | [Union-Find](./01-union-find/) | 互斥集合的實作與優化 |

## 核心概念

### 互斥集合（Disjoint Set）

維護一組不相交的集合，支援以下操作：
- **MakeSet(x)**：建立只包含 x 的新集合
- **Find(x)**：找出 x 所屬集合的代表元素
- **Union(x, y)**：合併 x 和 y 所屬的兩個集合

```
初始狀態（每個元素獨立）:
{1}, {2}, {3}, {4}, {5}

Union(1, 2):
{1, 2}, {3}, {4}, {5}

Union(3, 4):
{1, 2}, {3, 4}, {5}

Union(1, 3):
{1, 2, 3, 4}, {5}
```

### 樹狀表示法

每個集合用一棵樹表示，根節點為代表元素：

```
Union(1, 2), Union(3, 4), Union(5, 6), Union(1, 3):

    1           5
   / \          |
  2   3         6
      |
      4

集合1: {1, 2, 3, 4}，代表元素 = 1
集合2: {5, 6}，代表元素 = 5
```

## 實作與優化

### 基本實作

```
parent[i] = i 的父節點（若為根則 parent[i] = i）

Find(x):
    while parent[x] ≠ x:
        x = parent[x]
    return x

Union(x, y):
    rootX = Find(x)
    rootY = Find(y)
    if rootX ≠ rootY:
        parent[rootX] = rootY
```

### 優化技巧

#### 1. 按秩合併（Union by Rank）

總是將較小的樹接到較大的樹上：

```
rank[x] = x 為根時的樹高度（或大小）

Union(x, y):
    rootX = Find(x)
    rootY = Find(y)
    if rootX ≠ rootY:
        if rank[rootX] < rank[rootY]:
            parent[rootX] = rootY
        elif rank[rootX] > rank[rootY]:
            parent[rootY] = rootX
        else:
            parent[rootY] = rootX
            rank[rootX] += 1
```

#### 2. 路徑壓縮（Path Compression）

在 Find 時將路徑上的節點直接連到根：

```
Find(x):
    if parent[x] ≠ x:
        parent[x] = Find(parent[x])  # 遞迴壓縮
    return parent[x]
```

**圖示**：
```
壓縮前:               壓縮後:
    1                    1
    |                  / | \
    2                 2  3  4
    |
    3
    |
    4

Find(4) 後，2, 3, 4 都直接連到 1
```

### 時間複雜度

| 實作方式 | 單次操作 | m 次操作 |
|---------|---------|---------|
| 無優化 | O(n) | O(mn) |
| 只有 Union by Rank | O(log n) | O(m log n) |
| 只有 Path Compression | O(log n) 攤銷 | O(m log n) |
| 兩者皆有 | O(α(n)) 攤銷 | O(m α(n)) |

其中 α(n) 是阿克曼函數的反函數，實務上可視為常數（n = 10^80 時 α(n) < 5）。

## 應用場景

### 1. 連通分量（Connected Components）

判斷無向圖中兩點是否連通：

```python
def is_connected(u, v):
    return find(u) == find(v)

def add_edge(u, v):
    union(u, v)
```

### 2. Kruskal 最小生成樹

```
Kruskal(G):
    edges = sort G.edges by weight
    MST = []
    for each edge (u, v, w) in edges:
        if Find(u) ≠ Find(v):
            MST.append((u, v, w))
            Union(u, v)
    return MST
```

### 3. 其他應用

- 偵測無向圖中的環
- 最小公共祖先（LCA）的 Tarjan 演算法
- 等價類別的維護

## 考試重點

1. **基本操作**
   - 給定操作序列，畫出 Union-Find 的樹狀結構
   - 計算 Find 操作的比較次數

2. **優化分析**
   - 解釋按秩合併如何限制樹高
   - 解釋路徑壓縮的攤銷分析

3. **應用題**
   - 使用 Union-Find 解決連通性問題
   - Kruskal 演算法的實作

## 延伸閱讀

- CLRS Chapter 21: Data Structures for Disjoint Sets
- Tarjan, R. E. (1975). Efficiency of a Good But Not Linear Set Union Algorithm
