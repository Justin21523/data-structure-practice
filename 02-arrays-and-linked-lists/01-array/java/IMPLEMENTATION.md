# 實作說明（Java）— 靜態陣列（Fixed Capacity Array）

## 目標

用 Java 的原生 `int[]` 模擬「**靜態陣列（static array）**」：容量固定、資料連續。  
本單元重點不是效能優化，而是把「插入/刪除為何是 O(n)」用 **搬移次數（moved elements）** 具體呈現。

## 檔案

- `FixedArrayDemo.java`：`FixedArray`（固定容量）+ CLI 印表格
- `FixedArrayDemoTest.java`：自製最小化測試 runner（無 JUnit）

## 搬移次數（為何是 O(n)）

### 插入 `insertAt(i, v)`

右移 `[i, size-1]`，搬移次數：

- `moved = size - i`

程式片段：

```java
for (int i = this.size; i > index; i--) {
    this.data[i] = this.data[i - 1];
}
```

### 刪除 `removeAt(i)`

左移 `[i+1, size-1]`，搬移次數：

- `moved = size - i - 1`

## 如何執行

在 `02-arrays-and-linked-lists/01-array/java/`：

```bash
javac FixedArrayDemo.java FixedArrayDemoTest.java
java FixedArrayDemo
java FixedArrayDemoTest
```

