# 第七章：堆積與優先佇列（Heaps and Priority Queues）

本章介紹堆積（heap）及其最重要的應用——優先佇列（priority queue）。堆積是一種高效支援「取得最大/最小值」操作的資料結構。

## 學習目標

- 理解堆積的性質與陣列表示法
- 掌握 heapify、insert、extract 等核心操作
- 學會使用堆積實作優先佇列

## 單元列表

| 單元 | 主題 | 說明 |
|------|------|------|
| 01 | [二元堆積](./01-binary-heap/) | Max-Heap、Min-Heap、堆積操作 |
| 02 | [優先佇列](./02-priority-queue/) | 使用堆積實作優先佇列 |
| 03 | [d 元堆積](./03-d-ary-heap/) | 多元堆積的變體 |
| 04 | [費波那契堆積](./04-fibonacci-heap/) | 進階堆積結構 |

## 堆積性質

### 二元堆積（Binary Heap）

二元堆積是一棵**完全二元樹**，滿足堆積性質：

| 類型 | 性質 |
|------|------|
| Max-Heap | 父節點 ≥ 子節點（根為最大值） |
| Min-Heap | 父節點 ≤ 子節點（根為最小值） |

```
Max-Heap 範例:

        90
       /  \
      85   70
     / \   /
    60 80 30

Min-Heap 範例:

        10
       /  \
      20   15
     / \   /
    30 25 50
```

### 陣列表示法

完全二元樹可用陣列緊湊表示（index 從 1 開始）：

```
        90
       /  \
      85   70
     / \   /
    60 80 30

陣列: [_, 90, 85, 70, 60, 80, 30]
索引:  0   1   2   3   4   5   6
```

**索引關係**（1-indexed）：
| 節點 | 索引計算 |
|------|---------|
| 父節點 | parent(i) = ⌊i/2⌋ |
| 左子節點 | left(i) = 2i |
| 右子節點 | right(i) = 2i + 1 |

**索引關係**（0-indexed）：
| 節點 | 索引計算 |
|------|---------|
| 父節點 | parent(i) = ⌊(i-1)/2⌋ |
| 左子節點 | left(i) = 2i + 1 |
| 右子節點 | right(i) = 2i + 2 |

## 核心操作

### 操作時間複雜度

| 操作 | 時間複雜度 | 說明 |
|------|-----------|------|
| getMax/getMin | O(1) | 查看根節點 |
| extractMax/extractMin | O(log n) | 移除並回傳根 |
| insert | O(log n) | 插入新元素 |
| heapify | O(log n) | 修復單一違規 |
| buildHeap | O(n) | 從陣列建立堆積 |

### Heapify（向下修復）

從某節點開始，向下修復堆積性質：

```
Max-Heapify(A, i):
    largest = i
    left = 2*i
    right = 2*i + 1

    if left ≤ heap_size and A[left] > A[largest]:
        largest = left
    if right ≤ heap_size and A[right] > A[largest]:
        largest = right

    if largest ≠ i:
        swap A[i] and A[largest]
        Max-Heapify(A, largest)
```

### Build-Heap

從無序陣列建立堆積（O(n) 時間）：

```
Build-Max-Heap(A):
    heap_size = A.length
    for i = ⌊A.length/2⌋ downto 1:
        Max-Heapify(A, i)
```

## 優先佇列（Priority Queue）

抽象資料型別（ADT），支援：
- `insert(x, priority)`：插入元素
- `extractMax()` / `extractMin()`：取出最高/最低優先權元素
- `peek()`：查看最高/最低優先權元素

**應用場景**：
- Dijkstra 最短路徑演算法
- Prim 最小生成樹演算法
- Huffman 編碼
- 工作排程系統

## 堆積變體比較

| 堆積類型 | insert | extractMin | decreaseKey |
|---------|--------|------------|-------------|
| Binary Heap | O(log n) | O(log n) | O(log n) |
| d-ary Heap | O(log_d n) | O(d log_d n) | O(log_d n) |
| Fibonacci Heap | O(1) 攤銷 | O(log n) 攤銷 | O(1) 攤銷 |
| Binomial Heap | O(log n) | O(log n) | O(log n) |

## 考試重點

1. **建堆過程**
   - 給定序列，畫出建堆的每一步
   - Build-Heap 為何是 O(n) 而非 O(n log n)

2. **堆積操作**
   - Insert 後的堆積狀態
   - Extract 後的堆積狀態
   - Heapify 的執行過程

3. **應用題**
   - 找第 k 大/小的元素
   - 合併 k 個已排序串列

## 延伸閱讀

- CLRS Chapter 6: Heapsort
- CLRS Chapter 19: Fibonacci Heaps
- Goodrich Chapter 8: Heaps and Priority Queues
