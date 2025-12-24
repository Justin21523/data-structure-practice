# 實作說明（C++）— 靜態陣列（Fixed Capacity Array）

## 目標

用 C++ 實作固定容量陣列，並把插入/刪除的 shift 成本用 **moved elements（搬移次數）** 具體表達。

為了讓「成本可預測」，本單元不使用 `std::vector` 的自動擴容，而是用 `std::unique_ptr<int[]>` 管理固定大小的 backing buffer。

## 檔案

- `FixedArray.hpp`：`FixedArray` + `RemoveResult` + `simulateInsertMoves/simulateRemoveMoves`
- `fixed_array_demo.cpp`：CLI 印表格
- `test_fixed_array.cpp`：最小化測試 runner（丟出例外代表失敗）
- `CMakeLists.txt`：建置與 CTest

## 搬移次數（公式）

- 插入 `insertAt(i)`：`moved = size - i`
- 刪除 `removeAt(i)`：`moved = size - i - 1`

## 如何執行

在 `02-arrays-and-linked-lists/01-array/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/fixed_array_demo
ctest --test-dir build
```

