# 實作說明（C++）— 雙端佇列（Deque）

## 目標

本單元在 `Deque.hpp` 內用「環狀陣列（circular buffer）」實作 `dequeunit::Deque`：

- `moved`：永遠為 `0`（所有操作只調整索引，不搬移整段元素）
- `copied`：只有擴容（doubling）才會出現，且一次 resize 會複製 `size` 個元素

## 檔案

- `Deque.hpp`：資料結構 + `simulatePushBacks`（成長/複製成本統計）
- `deque_demo.cpp`：示範程式（pushBack 成長表 + 雙端操作小例子）
- `test_deque.cpp`：測試（wrap-around、head≠0 resize 保序、空操作丟例外）
- `CMakeLists.txt`：CMake + CTest

## 核心概念

### 1) indexAt：邏輯 offset → 實體索引

```cpp
int indexAt(int offset) const {
    return (head_ + offset) % capacity_;
}
```

### 2) pushFront / pushBack 都只改索引（moved=0）

pushFront：

```cpp
head_ = (head_ - 1 + capacity_) % capacity_;
data_[static_cast<size_t>(head_)] = value;
size_ += 1;
```

pushBack：

```cpp
int tailIndex = indexAt(size_);
data_[static_cast<size_t>(tailIndex)] = value;
size_ += 1;
```

### 3) resize：依 deque 順序複製並 head=0

```cpp
for (int i = 0; i < size_; i++) {
    newData[static_cast<size_t>(i)] = data_[static_cast<size_t>(indexAt(i))];
}
head_ = 0;
```

## 如何執行

在 `03-stacks-and-queues/04-deque/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/deque_demo
ctest --test-dir build --output-on-failure
```

