# 第一章：複雜度分析（Complexity Analysis）

本章介紹資料結構分析的基礎工具——時間與空間複雜度的概念。這是評估資料結構效能的基石。

## 學習目標

- 理解漸進符號（asymptotic notation）的定義與使用
- 能夠分析基本操作的時間複雜度
- 掌握攤銷分析（amortized analysis）的技巧

## 單元列表

| 單元 | 主題 | 說明 |
|------|------|------|
| 01 | [漸進符號](./01-asymptotic-notation/) | Big-O、Big-Ω、Big-Θ 表示法 |
| 02 | [攤銷分析](./02-amortized-analysis/) | 聚合法、記帳法、位能法 |

## 重點觀念

### 1. 漸進符號（Asymptotic Notation）

用於描述函數成長率的數學工具：

| 符號 | 含義 | 用途 |
|------|------|------|
| O(f(n)) | 上界（upper bound） | 最差情況分析 |
| Ω(f(n)) | 下界（lower bound） | 最佳情況分析 |
| Θ(f(n)) | 緊界（tight bound） | 精確成長率 |

### 2. 常見複雜度等級

```
O(1) < O(log n) < O(n) < O(n log n) < O(n²) < O(n³) < O(2ⁿ) < O(n!)
常數    對數      線性    線性對數     平方     立方     指數    階乘
```

### 3. 資料結構操作複雜度速查

| 資料結構 | 搜尋 | 插入 | 刪除 | 備註 |
|---------|------|------|------|------|
| Array | O(n) | O(n) | O(n) | 隨機存取 O(1) |
| Linked List | O(n) | O(1)* | O(1)* | *已知位置時 |
| Hash Table | O(1) 平均 | O(1) 平均 | O(1) 平均 | 最差 O(n) |
| BST | O(h) | O(h) | O(h) | h 為樹高 |
| Balanced BST | O(log n) | O(log n) | O(log n) | AVL, RBT |
| Heap | O(n) | O(log n) | O(log n) | 取最值 O(1) |

## 考試重點

1. **Big-O 定義的形式化表達**
   - f(n) = O(g(n)) ⟺ ∃c > 0, n₀ > 0, ∀n ≥ n₀: f(n) ≤ c·g(n)

2. **常見遞迴關係式的解**
   - T(n) = T(n/2) + O(1) → O(log n)
   - T(n) = 2T(n/2) + O(n) → O(n log n)
   - T(n) = T(n-1) + O(n) → O(n²)

3. **主定理（Master Theorem）的應用**

## 延伸閱讀

- CLRS Chapter 3: Growth of Functions
- CLRS Chapter 17: Amortized Analysis
