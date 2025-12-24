# 實作說明（C++）— 動態陣列（Dynamic Array）

## 目標

本單元以教學目的實作 `DynamicArray`（不使用 `std::vector`），用「可觀察的成本」理解動態陣列：

- `copied`：擴容（resize）時複製既有元素次數
- `moved`：插入/刪除時 shift 的搬移次數

容量策略採 **倍增（doubling）**：容量不足時 `capacity *= 2`，讓 `append` 呈現攤銷（amortized）O(1)。

## 檔案

- `DynamicArray.hpp`：`DynamicArray` 實作（含成本計數）
- `dynamic_array_demo.cpp`：示範程式（印出表格）
- `test_dynamic_array.cpp`：最小化測試（以 `assert` 為主）
- `CMakeLists.txt`：建置與 `ctest`

## 核心概念

### resize（計 copied）

擴容時配置新 buffer，複製 `0..size-1`：

```cpp
for (int i = 0; i < size_; i += 1) {
    new_data[i] = data_[i];
    copied += 1;
}
```

### insertAt / removeAt（計 moved）

- `insertAt(i, v)`：右移 `[i, size-1]`，`moved = size - i`
- `removeAt(i)`：左移 `[i+1, size-1]`，`moved = size - i - 1`

## 如何執行

在 `02-arrays-and-linked-lists/02-dynamic-array/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/dynamic_array_demo
ctest --test-dir build
```

