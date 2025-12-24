# 實作說明（C++）— 雙向鏈結串列（Doubly Linked List）

## 目標

本單元以教學目的實作 `DoublyLinkedList`（不使用 `std::list`），並用 `hops` 呈現走訪成本：

- `hops`：每次沿著 `next/prev` 前進都算 1 hop

藉由維護 `head` + `tail`，你會看到兩個重點：

- 兩端操作（push/pop）都是 O(1)（`hops=0`）
- `get/insertAt/removeAt` 仍需走訪，但可從較近端開始走（`hops = min(i, size-1-i)`）

## 檔案

- `DoublyLinkedList.hpp`：資料結構 + hops 計數
- `doubly_linked_list_demo.cpp`：示範程式（印出表格）
- `test_doubly_linked_list.cpp`：最小化測試（`assert` + `assertThrowsOutOfRange`）
- `CMakeLists.txt`：建置與 `ctest`

## 核心概念：從較近端開始走

```cpp
if (index < size_ / 2) {
    for (int i = 0; i < index; i += 1) {
        current = current->next;
        hops += 1;
    }
} else {
    int steps = size_ - 1 - index;
    for (int i = 0; i < steps; i += 1) {
        current = current->prev;
        hops += 1;
    }
}
```

因此 `hops = min(i, size-1-i)`。

## 如何執行

在 `02-arrays-and-linked-lists/04-doubly-linked-list/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/doubly_linked_list_demo
ctest --test-dir build
```

