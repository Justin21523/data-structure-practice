# 實作說明（C#）— 單向鏈結串列（Singly Linked List）

## 目標

.NET 本身有 `LinkedList<T>`，但本單元改用教學版 `SinglyLinkedList`（只存 `head`，不存 `tail`），並用 `Hops` 量化走訪成本：

- `Hops`：做了幾次 `current = current.Next`

這能直覺對應到時間複雜度：`Hops` 越大，越接近 O(n)。

## 檔案

- `SinglyLinkedListDemo.cs`：`SinglyLinkedList` 實作（含成本計數）
- `Program.cs`：CLI 入口（支援 `--test`）
- `SinglyLinkedList.csproj`：專案檔
- `NuGet.config`：清空套件來源（避免受限環境下嘗試連外）

## 核心概念：pushBack 的走訪

沒有 tail 指標時，尾端插入必須一路走到最後：

```csharp
while (current.Next != null) {
    current = current.Next;
    hops += 1;
}
```

## 如何執行

在 `02-arrays-and-linked-lists/03-singly-linked-list/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可改用：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run -- --test
```

