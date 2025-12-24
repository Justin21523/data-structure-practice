# 實作說明（C）— 單向鏈結串列（Singly Linked List）

## 目標

本單元用 C 手刻 `singly_linked_list`：

- `singly_list_node`：`value` + `next`
- `singly_linked_list`：只維護 `head` 與 `size`（不維護 tail）

並用 `hops` 量化走訪成本：

- `hops`：執行 `current = current->next` 的次數

## 檔案

- `singly_linked_list.h` / `singly_linked_list.c`：資料結構與操作 API
- `singly_linked_list_main.c`：示範程式（印出表格）
- `test_singly_linked_list.c`：最小化測試（自製 assert + 印 PASS/FAIL）
- `Makefile`：`make run` / `make test`

## 核心概念：push_back 的走訪

因為沒有 tail 指標，尾端插入必須走到最後一個節點：

```c
while (current->next != NULL) {
    current = current->next;
    hops += 1;
}
```

因此串列長度為 `n` 時，`hops = max(0, n - 1)`。

## 如何執行

在 `02-arrays-and-linked-lists/03-singly-linked-list/c/`：

```bash
make
make run
make test
```

