# 實作說明（C#）— 雙向鏈結串列（Doubly Linked List）

## 目標

.NET 有 `LinkedList<T>`，但本單元改用教學版 `DoublyLinkedList`（維護 `head` + `tail`），並用 `Hops` 量化走訪成本：

- `Hops`：做了幾次 `current = current.Next` 或 `current = current.Prev`

## 檔案

- `DoublyLinkedListDemo.cs`：`DoublyLinkedList` 實作（含 hops 計數）
- `Program.cs`：CLI 入口（支援 `--test`）
- `DoublyLinkedList.csproj`：專案檔
- `NuGet.config`：清空套件來源（避免受限環境下嘗試連外）

## 核心概念

### 兩端操作為 O(1)

因為維護 `tail`，尾端插入/刪除不必走訪（`Hops = 0`）。

### get/insert/remove 的 hops（從較近端開始）

```csharp
if (index < _size / 2) {
    for (var i = 0; i < index; i += 1) {
        current = current.Next!;
        hops += 1;
    }
} else {
    var steps = _size - 1 - index;
    for (var i = 0; i < steps; i += 1) {
        current = current.Prev!;
        hops += 1;
    }
}
```

因此 `Hops = min(i, size-1-i)`。

## 如何執行

在 `02-arrays-and-linked-lists/04-doubly-linked-list/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可改用：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run -- --test
```

