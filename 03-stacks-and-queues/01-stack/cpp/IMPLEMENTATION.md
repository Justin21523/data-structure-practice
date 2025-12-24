# 實作說明（C++）— 堆疊（Stack）

## 目標

本單元用 `std::unique_ptr<int[]>` 實作 `ArrayStack`，並用 `copied` 量化擴容成本：

- `copied`：擴容時複製既有元素次數

`top` 位於陣列尾端（`size-1`），因此 `push/pop/peek` 都是 O(1)，只有 `push` 在擴容當下會複製 O(n) 個元素，但整體是攤銷 O(1)（doubling）。

## 檔案

- `Stack.hpp`：`ArrayStack` + `simulatePushes`（含 copied）
- `stack_demo.cpp`：示範程式
- `test_stack.cpp`：最小測試器（無外部測試框架）
- `CMakeLists.txt`：建置設定

## 核心概念

### 倍增擴容與 copied

```cpp
for (int i = 0; i < size_; i++) {
    newData[i] = data_[i];
    copied += 1;
}
```

## 如何執行

在 `03-stacks-and-queues/01-stack/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/stack_demo
ctest --test-dir build
```

