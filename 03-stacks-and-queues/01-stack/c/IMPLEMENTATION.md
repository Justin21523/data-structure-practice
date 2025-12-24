# 實作說明（C）— 堆疊（Stack）

## 目標

本單元用 C 實作陣列堆疊 `array_stack`，並用 `copied` 量化擴容成本：

- `copied`：擴容時複製既有元素次數（resize copy count）

## 檔案

- `stack.h/.c`：`array_stack` + 操作函式（含 copied）
- `stack_main.c`：示範程式（印出 push 成長表格與 LIFO 小例子）
- `test_stack.c`：最小測試器（assert）
- `Makefile`：`make run` / `make test`

## 核心概念

### 1) top 在尾端

top index 固定是 `size-1`；`push` 寫到 `data[size]`，`pop` 先 `size--` 再取 `data[size]`。

### 2) 倍增擴容與 copied

因為要精準計數，我們不用 `realloc`，而是自己 `malloc + copy + free`：

```c
for (int i = 0; i < stack->size; i++) {
    new_data[i] = stack->data[i];
    copied += 1;
}
```

## 如何執行

在 `03-stacks-and-queues/01-stack/c/`：

```bash
make
make run
make test
```

