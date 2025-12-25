# 實作說明（C）— 二元樹（Binary Tree）

## 目標

本單元用 C 手刻 `BinaryTree`，練習樹的遞迴與走訪，並且保留「可測試」的輸出：

- `binary_tree_size`：節點數
- `binary_tree_height`：高度**以邊數計**（空樹 `-1`、葉 `0`）
- `binary_tree_count_leaves`：葉節點數
- `preorder / inorder / postorder / level_order`：回傳「值序列」以方便比對

## 檔案

- `binary_tree.h/.c`：資料結構與操作（建樹 / 性質 / 走訪）
- `binary_tree_main.c`：demo（印出 size/height/leaves 與四種走訪）
- `test_binary_tree.c`：測試（不使用外部測試框架）
- `Makefile`：`make run` / `make test`

## 關鍵設計

### 1) 用 `bt_optional_int` 表達 `null`

C 沒有 `null int`，所以用：

```c
typedef struct bt_optional_int { bool is_present; int value; } bt_optional_int;
#define BT_SOME(x) ((bt_optional_int){true, (x)})
#define BT_NONE ((bt_optional_int){false, 0})
```

### 2) level-order 建樹（支援缺洞）

索引規則：`left=2*i+1`、`right=2*i+2`；並且**只配置可達節點**（避免「父節點不存在」造成記憶體洩漏）。

### 3) BFS 用 head/tail 索引（不做 shift）

queue 用固定長度 `n` 的陣列，`head` 代表 dequeue 位置：

```c
while (head < tail) {
    const bt_node* node = queue[head];
    head += 1;
}
```

## 如何執行

在 `05-trees-basic/01-binary-tree/c/`：

```bash
make run
make test
make clean
```

