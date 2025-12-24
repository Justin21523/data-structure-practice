# 第四章：雜湊表（Hash Tables）

本章介紹雜湊表——一種提供平均 O(1) 時間複雜度進行搜尋、插入、刪除的高效資料結構。

## 學習目標

- 理解雜湊表的運作原理
- 掌握不同碰撞處理策略的實作與比較
- 學會設計良好的雜湊函數

## 單元列表

| 單元 | 主題 | 說明 |
|------|------|------|
| 01 | [基本雜湊表](./01-basic-hash-table/) | 雜湊表原理、基本操作 |
| 02 | [碰撞處理](./02-collision-resolution/) | Chaining、Open Addressing |
| 03 | [雜湊函數](./03-hash-functions/) | 除法、乘法、通用雜湊 |

## 核心概念

### 雜湊表原理

```
Key "apple" → hash("apple") → 42 → bucket[42] → Value

        hash function
Key ──────────────────→ Index ──→ Bucket ──→ Value
```

### 時間複雜度

| 操作 | 平均 | 最差 |
|------|------|------|
| 搜尋（search） | O(1) | O(n) |
| 插入（insert） | O(1) | O(n) |
| 刪除（delete） | O(1) | O(n) |

### 負載因子（Load Factor）

```
α = n / m

n = 儲存的元素數量
m = 桶（bucket）的數量

建議保持 α < 0.75 以維持效能
```

### 碰撞處理方法

#### 1. 鏈結法（Chaining）

每個桶維護一個鏈結串列：

```
bucket[0] → (k1,v1) → (k5,v5) → null
bucket[1] → (k2,v2) → null
bucket[2] → null
bucket[3] → (k3,v3) → (k4,v4) → (k6,v6) → null
```

#### 2. 開放定址法（Open Addressing）

碰撞時探測下一個空位：

| 探測方法 | 探測序列 | 特點 |
|---------|---------|------|
| 線性探測（Linear Probing） | h+1, h+2, h+3, ... | 簡單，但易群聚 |
| 二次探測（Quadratic Probing） | h+1², h+2², h+3², ... | 減少群聚 |
| 雙重雜湊（Double Hashing） | h₁ + i·h₂ | 效果最佳 |

## 重要議題

### 動態調整大小（Rehashing）

當負載因子超過閾值時：
1. 建立新的、較大的陣列（通常 2 倍）
2. 重新計算所有元素的雜湊值
3. 將元素放入新陣列

時間複雜度：單次 O(n)，攤銷 O(1)

### 良好雜湊函數的特性

1. **確定性（Deterministic）**：相同輸入產生相同輸出
2. **均勻分布（Uniform Distribution）**：減少碰撞
3. **高效計算（Efficient）**：O(1) 計算時間

## 考試重點

1. **計算題**
   - 給定雜湊函數與碰撞處理方式，畫出最終雜湊表
   - 計算平均搜尋長度（ASL）

2. **分析題**
   - 比較 chaining 與 open addressing 的優缺點
   - 分析不同負載因子下的效能

3. **設計題**
   - 設計適合特定資料的雜湊函數
   - 實作支援特定操作的雜湊表

## 延伸閱讀

- CLRS Chapter 11: Hash Tables
- Goodrich Chapter 9: Maps, Hash Tables, and Skip Lists
