# 實作說明（C++）— 環狀鏈結串列（Circular Linked List）

## 目標

本單元用 C++ 實作 `CircularLinkedList`（單向環狀鏈結串列），並用 `hops`（走訪 `next` 次數）量化成本：

- `hops`：每做一次 `current = current->next` 就 +1

## 檔案

- `CircularLinkedList.hpp`：資料結構（tail-only）+ 操作（含 hops）
- `circular_linked_list_demo.cpp`：示範程式
- `test_circular_linked_list.cpp`：最小測試器（無外部測試框架）
- `CMakeLists.txt`：建置設定

## 核心概念

### 1) tail-only：`head = tail->next`

非空時 head 不需要額外指標，直接取 `tail->next`。

### 2) 走訪用 size 限制

環狀結構沒有終點，`toVector()` / 搜尋都以 `size` 次數限制走訪。

### 3) 成本公式（hops）

- `pushFront / pushBack / popFront`：`hops = 0`
- `popBack`：找 predecessor → `hops = max(0, n-2)`
- `rotate(k)`：`hops = k % size`，且 `k < 0` 會丟 `std::invalid_argument`

## 如何執行

在 `02-arrays-and-linked-lists/05-circular-linked-list/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/circular_linked_list_demo
ctest --test-dir build
```

