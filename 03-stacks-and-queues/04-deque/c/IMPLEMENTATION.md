# 實作說明（C）— 雙端佇列（Deque）

## 目標

本單元用「環狀陣列（circular buffer）」實作 `circular_deque`，支援兩端插入/刪除並維持 **O(1)**（攤銷）：

- `moved`：永遠為 `0`（不做 shift）
- `copied`：只在擴容（doubling）時出現，且每次 resize 會複製 `size` 個元素

## 檔案

- `deque.h` / `deque.c`：資料結構與 API
- `deque_main.c`：示範程式（pushBack 成長表 + 雙端操作小例子）
- `test_deque.c`：測試（wrap-around、head≠0 resize 保序、moved=0）
- `Makefile`：`make run` / `make test` / `make clean`

## 核心概念

### 1) head + size 的表示法

- `head`：front 的實體索引
- `size`：元素數量
- `capacity`：buffer 大小

```c
static int index_at(const circular_deque* deque, int offset) {
    return (deque->head + offset) % deque->capacity;
}
```

### 2) 兩端 push 都只改索引（moved=0）

pushFront（head 往左繞回去）：

```c
deque->head = (deque->head - 1 + deque->capacity) % deque->capacity;
deque->data[deque->head] = value;
deque->size += 1;
```

pushBack（寫到 tail）：

```c
int tail_index = index_at(deque, deque->size);
deque->data[tail_index] = value;
deque->size += 1;
```

### 3) resize：依 deque 順序複製並 head=0

即使 head 不在 0，也要用 `i=0..size-1` 的邏輯順序複製：

```c
for (int i = 0; i < deque->size; i++) {
    new_data[i] = deque->data[index_at(deque, i)];
}
deque->head = 0;
```

## 如何執行

在 `03-stacks-and-queues/04-deque/c/`：

```bash
make
make run
make test
make clean
```

