# 實作說明（C++）— 漸進符號示範（Asymptotic Notation）

## 目標

用 **C++** 以「操作次數」展示成長率，並以最小依賴的方式提供可執行測試（不使用 GoogleTest）。

## 檔案

- `AsymptoticDemo.hpp`：核心計數函式（O(1)、O(log n)、O(n)、O(n log n)、O(n²)）與輸入檢查。
- `asymptotic_demo.cpp`：CLI 程式，印出表格。
- `test_asymptotic_demo.cpp`：自製測試 runner，失敗時丟 `std::runtime_error`。
- `CMakeLists.txt`：建置與 CTest 設定。

## 代表性片段

O(log n) 使用 halving loop：

```cpp
while (current > 1) {
  current /= 2;
  operations += 1;
}
```

## 如何建置與測試

在 `01-introduction-and-complexity/01-asymptotic-notation/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/asymptotic_demo
ctest --test-dir build
```

## 注意事項

- `countLog2Ops` 對 `n < 1` 會丟 `std::invalid_argument`，避免不合理輸入。
- 測試採用 CTest 註冊，方便與其他 C++ 單元一致。

