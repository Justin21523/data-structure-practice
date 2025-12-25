# 實作說明（C++）— 環狀佇列（Circular Queue）

## 目標

本單元用 `std::unique_ptr<int[]>` 實作 `CircularQueue`，用 head + size 的方式避免 dequeue 左移：

- `moved`：永遠是 0
- `copied`：只在擴容（doubling resize）時出現

## 檔案

- `CircularQueue.hpp`：`CircularQueue` + `simulateEnqueues/simulateDequeueCostAtSize`
- `circular_queue_demo.cpp`：示範程式
- `test_circular_queue.cpp`：最小測試器（無外部測試框架）
- `CMakeLists.txt`：建置設定

## 核心概念

### dequeue 不左移（moved=0）

```cpp
head_ = (head_ + 1) % capacity_;
size_ -= 1;
```

### resize 依佇列順序複製

```cpp
newData[i] = data_[(head_ + i) % capacity_];
```

## 如何執行

在 `03-stacks-and-queues/03-circular-queue/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/circular_queue_demo
ctest --test-dir build
```

