# 01 漸進符號（Asymptotic Notation）

本單元用「漸進符號」描述函數成長率，用來分析資料結構/演算法在輸入規模變大時的效率表現。

## 你需要會的三個符號

- **Big-O（O）**：上界（upper bound），常用於最差情況（worst-case）。
- **Big-Ω（Ω）**：下界（lower bound），常用於最佳情況（best-case）。
- **Big-Θ（Θ）**：緊界（tight bound），表示與某函數同階（same order）。

形式化（以 Big-O 為例）：

> f(n) = O(g(n)) ⟺ ∃c > 0, n₀ > 0, ∀n ≥ n₀: f(n) ≤ c·g(n)

## 常見成長率（由小到大）

`O(1) < O(log n) < O(n) < O(n log n) < O(n²) < O(2ⁿ) < O(n!)`

## 程式碼（各語言）

此 repo 會用「操作次數（operation count）」示範成長率，避免實際計時受到硬體與環境干擾。

每個語言資料夾皆提供：

- 示範程式（印出表格）
- 測試程式（驗證操作次數是否符合定義）
- 說明文件：`IMPLEMENTATION.md`

### Python

在 `01-introduction-and-complexity/01-asymptotic-notation/python/`：

```bash
python3 asymptotic_demo.py
python3 -m unittest -v
```

### Java

在 `01-introduction-and-complexity/01-asymptotic-notation/java/`：

```bash
javac AsymptoticDemo.java AsymptoticDemoTest.java
java AsymptoticDemo
java AsymptoticDemoTest
```

### JavaScript（Node.js）

在 `01-introduction-and-complexity/01-asymptotic-notation/javascript/`：

```bash
node asymptoticDemo.js
node --test
```

### C

在 `01-introduction-and-complexity/01-asymptotic-notation/c/`：

```bash
make
make run
make test
```

### C++

在 `01-introduction-and-complexity/01-asymptotic-notation/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/asymptotic_demo
ctest --test-dir build
```

### C#

在 `01-introduction-and-complexity/01-asymptotic-notation/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

如果你在受限環境（例如 sandbox/某些 CI）無法寫入家目錄的 `~/.dotnet`，請改用：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run -- --test
```
