# 實作說明（Java）— 動態陣列（Dynamic Array）

## 目標

本單元用 Java 的原生 `int[]` 當作底層連續 buffer，手動實作 `DynamicArray`，重點是把「動態陣列的成本」量化成可測試的數字：

- `copied`：擴容（resize）時，複製既有元素的次數
- `moved`：`insertAt/removeAt` 為了 shift 而移動元素的次數

容量採用 **倍增（doubling）**：`size == capacity` 時把容量乘 2，使 `append` 具備攤銷（amortized）O(1)。

## 檔案

- `DynamicArrayDemo.java`：`DynamicArray` + CLI（印 capacity/copies/moved 表格）
- `DynamicArrayDemoTest.java`：自製最小化測試 runner（無 JUnit）

## 核心概念

### 1) 擴容（resize）與 copied

擴容的本質是「配置新陣列 + 複製舊資料」：

```java
for (int i = 0; i < this.size; i++) {
    newData[i] = this.data[i];
    copied += 1;
}
```

倍增策略下，總 copied 會呈線性成長（可用 `finalCapacity - 1` 驗證），因此 `append` 的平均成本是常數。

### 2) insertAt / removeAt 與 moved

- `insertAt(i, v)`：右移 `[i, size-1]`，`moved = size - i`
- `removeAt(i)`：左移 `[i+1, size-1]`，`moved = size - i - 1`

## 如何執行

在 `02-arrays-and-linked-lists/02-dynamic-array/java/`：

```bash
javac DynamicArrayDemo.java DynamicArrayDemoTest.java
java DynamicArrayDemo
java DynamicArrayDemoTest
```

