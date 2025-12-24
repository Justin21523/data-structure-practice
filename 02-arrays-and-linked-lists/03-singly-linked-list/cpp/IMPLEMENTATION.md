# 實作說明（C++）— 單向鏈結串列（Singly Linked List）

## 目標

本單元用教學版 `SinglyLinkedList`（不使用 `std::forward_list`）理解：

- 為何 `pushFront` 是 O(1)
- 為何沒有 tail 時 `pushBack` 需要走訪（O(n)）
- 為何 `get/insertAt/removeAt` 都要從 head 開始走（O(n)）

成本用 `hops` 表示：執行 `current = current->next` 的次數。

## 檔案

- `SinglyLinkedList.hpp`：資料結構 + 成本計數（hops）
- `singly_linked_list_demo.cpp`：示範程式（印出表格）
- `test_singly_linked_list.cpp`：最小化測試（`assert` + `assertThrowsOutOfRange`）
- `CMakeLists.txt`：建置與 `ctest`

## 核心概念

`pushBack` 會走到尾端（沒有 tail 指標）：

```cpp
while (current->next != nullptr) {
    current = current->next;
    hops += 1;
}
```

## 如何執行

在 `02-arrays-and-linked-lists/03-singly-linked-list/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/singly_linked_list_demo
ctest --test-dir build
```

