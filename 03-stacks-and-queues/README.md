# 第三章：堆疊與佇列（Stacks and Queues）

本章介紹兩種受限制的線性資料結構：堆疊（stack）與佇列（queue）。它們對存取順序有特定的限制，廣泛應用於各種演算法中。

## 學習目標

- 理解 LIFO 與 FIFO 的存取原則
- 掌握堆疊與佇列的陣列與鏈結串列實作
- 學會運用這些結構解決實際問題

## 單元列表

| 單元 | 主題 | 說明 |
|------|------|------|
| 01 | [堆疊](./01-stack/) | LIFO 結構、push/pop 操作 |
| 02 | [佇列](./02-queue/) | FIFO 結構、enqueue/dequeue |
| 03 | [環狀佇列](./03-circular-queue/) | 陣列實作的空間優化 |
| 04 | [雙端佇列](./04-deque/) | 兩端皆可操作的結構 |

## 核心概念

### 堆疊（Stack）- LIFO（Last In, First Out）

```
Push 3, Push 7, Push 1:

    ┌───┐
    │ 1 │ ← top
    ├───┤
    │ 7 │
    ├───┤
    │ 3 │
    └───┘

Pop → 回傳 1
```

**基本操作**：
| 操作 | 說明 | 時間複雜度 |
|------|------|-----------|
| push(x) | 將 x 放入頂端 | O(1) |
| pop() | 移除並回傳頂端元素 | O(1) |
| top() / peek() | 查看頂端元素 | O(1) |
| isEmpty() | 檢查是否為空 | O(1) |

### 佇列（Queue）- FIFO（First In, First Out）

```
Enqueue 3, Enqueue 7, Enqueue 1:

    front                 rear
      ↓                    ↓
    ┌───┬───┬───┐
    │ 3 │ 7 │ 1 │
    └───┴───┴───┘

Dequeue → 回傳 3
```

**基本操作**：
| 操作 | 說明 | 時間複雜度 |
|------|------|-----------|
| enqueue(x) | 加入尾端 | O(1) |
| dequeue() | 移除並回傳前端元素 | O(1) |
| front() | 查看前端元素 | O(1) |
| isEmpty() | 檢查是否為空 | O(1) |

### 雙端佇列（Deque）- Double-Ended Queue

支援兩端的插入與刪除：
- `pushFront()` / `popFront()`
- `pushBack()` / `popBack()`

## 應用場景

### 堆疊的應用
- 函式呼叫堆疊（call stack）
- 括號匹配檢查
- 運算式求值（中序轉後序）
- 瀏覽器的上一頁/下一頁
- DFS 的非遞迴實作

### 佇列的應用
- 工作排程（job scheduling）
- BFS 的實作
- 印表機佇列
- 緩衝區（buffer）

## 考試常見題型

1. **基本實作**
   - 用陣列實作堆疊/佇列
   - 用鏈結串列實作堆疊/佇列
   - 用兩個堆疊實作佇列
   - 用兩個佇列實作堆疊

2. **應用題**
   - 括號匹配（含多種括號）
   - 中序轉後序（infix to postfix）
   - 後序運算式求值
   - 最小堆疊（MinStack）

## 延伸閱讀

- Goodrich Chapter 5: Stacks, Queues, and Deques
- CLRS Chapter 10.1: Stacks and Queues
