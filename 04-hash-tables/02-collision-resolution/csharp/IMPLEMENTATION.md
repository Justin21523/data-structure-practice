# 實作說明（C#）— 碰撞處理（Chaining / Open Addressing）

## 檔案與角色

- `ChainedHashTable.cs`：鏈結法雜湊表（並追蹤 probe 統計）。
- `OpenAddressingHashTable.cs`：開放定址雜湊表（含 `ProbeMethod`、tombstone、probe 統計）。

## Chaining

結構與基本雜湊表一致：bucket 是一條 `Entry` 鏈。此版本額外提供統計（例如 TotalProbes、MaxProbes、AverageProbes），用於比較不同策略的平均探測成本。

## Open Addressing

核心觀念：

- 以單一表陣列存放元素。
- 碰撞時依 `ProbeMethod` 產生探測序列。
- 刪除用 tombstone（`IsDeleted`），搜尋不能在 tombstone 停止。

建議對照程式中的 `_deletedCount`（墓碑數量）與 `LoadFactor` 計算方式，理解「已佔用槽位（含墓碑）」對效能與擴容條件的影響。

## 建置/測試

此單元的 C# 檔案為示範碼；若你希望我補齊可直接 `dotnet test` 的專案結構（csproj + 測試專案），告訴我你希望的 .NET 版本與資料夾布局即可。

