# 實作說明（C++）— 佇列（Queue）

## 目標

本單元用 C++ 實作 `ArrayQueue`（naive queue），並量化成本：

- `copied`：擴容（doubling）時複製元素次數
- `moved`：`dequeue` 時左移元素次數（`size-1`）

## 檔案

- `Queue.hpp`：`ArrayQueue` + `simulateEnqueues/simulateDequeueCostAtSize`
- `queue_demo.cpp`：示範程式
- `test_queue.cpp`：最小測試器（無外部測試框架）
- `CMakeLists.txt`：建置設定

## 核心概念

### dequeue 的 moved（左移）

```cpp
int moved = size_ - 1;
for (int i = 1; i < size_; i++) {
    data_[i - 1] = data_[i];
}
```

> 下一單元 `03-circular-queue` 會把 moved 降到 0。

## 如何執行

在 `03-stacks-and-queues/02-queue/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/queue_demo
ctest --test-dir build
```

