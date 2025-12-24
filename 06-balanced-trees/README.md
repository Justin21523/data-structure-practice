# 第六章：平衡樹（Balanced Trees）

本章介紹自平衡二元搜尋樹（self-balancing BST），這些結構通過維護平衡條件來保證 O(log n) 的操作效能。

## 學習目標

- 理解為什麼需要平衡樹
- 掌握 AVL Tree 與 Red-Black Tree 的平衡機制
- 學會樹旋轉（rotation）操作

## 單元列表

| 單元 | 主題 | 說明 |
|------|------|------|
| 01 | [AVL 樹](./01-avl-tree/) | 嚴格平衡、高度差 ≤ 1 |
| 02 | [紅黑樹](./02-red-black-tree/) | 寬鬆平衡、顏色約束 |
| 03 | [伸展樹](./03-splay-tree/) | 自適應、最近存取優化 |

## 為什麼需要平衡樹？

普通 BST 在最差情況會退化成鏈結串列：

```
插入序列: 1, 2, 3, 4, 5

普通 BST（退化）:          平衡樹:
    1                         3
     \                       / \
      2                     2   4
       \                   /     \
        3                 1       5
         \
          4
           \
            5

高度: O(n)                高度: O(log n)
```

## 平衡樹比較

| 特性 | AVL Tree | Red-Black Tree | Splay Tree |
|------|----------|----------------|------------|
| 平衡條件 | |左高-右高| ≤ 1 | 顏色約束 | 無（攤銷平衡） |
| 搜尋 | O(log n) | O(log n) | O(log n) 攤銷 |
| 插入 | O(log n) | O(log n) | O(log n) 攤銷 |
| 刪除 | O(log n) | O(log n) | O(log n) 攤銷 |
| 旋轉次數（插入） | 最多 2 次 | 最多 2 次 | O(log n) |
| 旋轉次數（刪除） | O(log n) | 最多 3 次 | O(log n) |
| 額外空間 | 高度/平衡因子 | 1 bit 顏色 | 無 |
| 適用場景 | 查詢密集 | 通用、標準庫 | 存取有局部性 |

## 樹旋轉（Tree Rotation）

旋轉是維持平衡的基本操作，分為左旋（left rotation）與右旋（right rotation）。

### 右旋（Right Rotation）

```
    y                x
   / \              / \
  x   C    →       A   y
 / \                  / \
A   B                B   C
```

### 左旋（Left Rotation）

```
  x                  y
 / \                / \
A   y      →       x   C
   / \            / \
  B   C          A   B
```

### 雙旋轉

- **LR 旋轉**：先對左子樹左旋，再對當前節點右旋
- **RL 旋轉**：先對右子樹右旋，再對當前節點左旋

## AVL 樹快速參考

**平衡因子（Balance Factor）**：
```
BF(node) = height(left subtree) - height(right subtree)
合法值: {-1, 0, 1}
```

**四種不平衡情況**：

| 情況 | 條件 | 解法 |
|------|------|------|
| LL | BF > 1 且 BF(left) ≥ 0 | 右旋 |
| LR | BF > 1 且 BF(left) < 0 | 左旋左子 + 右旋 |
| RR | BF < -1 且 BF(right) ≤ 0 | 左旋 |
| RL | BF < -1 且 BF(right) > 0 | 右旋右子 + 左旋 |

## 紅黑樹快速參考

**五個性質**：
1. 每個節點是紅色或黑色
2. 根節點是黑色
3. 所有葉節點（NIL）是黑色
4. 紅色節點的子節點必須是黑色（不能有連續紅色）
5. 從任一節點到其所有後代葉節點的路徑上，黑色節點數相同（black-height）

## 考試重點

1. **AVL 樹**
   - 插入後判斷不平衡類型（LL/LR/RL/RR）
   - 執行旋轉並畫出結果
   - 計算平衡因子

2. **紅黑樹**
   - 驗證是否為合法紅黑樹
   - 插入後的修復（著色、旋轉）
   - 刪除後的修復

3. **比較分析**
   - AVL vs Red-Black 的取捨
   - 何時選用 Splay Tree

## 延伸閱讀

- CLRS Chapter 13: Red-Black Trees
- Goodrich Chapter 10: Search Trees (AVL Trees)
