# 實作說明（C）— 雙向鏈結串列（Doubly Linked List）

## 目標

本單元用 C 手刻 `doubly_linked_list`：

- `dll_node`：`value` + `prev` + `next`
- `doubly_linked_list`：維護 `head`、`tail`、`size`

並用 `hops` 量化走訪成本：

- `hops`：做了幾次 `current = current->next` 或 `current = current->prev`

## 檔案

- `doubly_linked_list.h` / `doubly_linked_list.c`：資料結構與操作 API
- `doubly_linked_list_main.c`：示範程式（印出表格）
- `test_doubly_linked_list.c`：最小化測試（自製 assert + 印 PASS/FAIL）
- `Makefile`：`make run` / `make test`

## 核心概念

### 1) push/pop 兩端皆為 O(1)

因為維護 `tail`，尾端插入/刪除可以直接修改指標，不需要走訪，因此 `hops = 0`。

### 2) nearest-end traversal：`hops = min(i, size-1-i)`

定位 index `i` 時，從 head 或 tail 擇一較近端開始：

```c
if (index < list->size / 2) {
    for (int i = 0; i < index; i++) {
        current = current->next;
        hops += 1;
    }
} else {
    int steps = list->size - 1 - index;
    for (int i = 0; i < steps; i++) {
        current = current->prev;
        hops += 1;
    }
}
```

## 如何執行

在 `02-arrays-and-linked-lists/04-doubly-linked-list/c/`：

```bash
make
make run
make test
```

