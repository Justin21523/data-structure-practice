/**
 * 雜湊表應用範例：單字計數（Word Count）
 * Hash Table Application Example: Word Counting
 *
 * 展示如何使用雜湊表實作高效的單字頻率統計
 * Demonstrates using hash table for efficient word frequency counting
 */

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cctype>
#include "HashTable.hpp"

/**
 * 將字串轉為小寫
 * Convert string to lowercase
 */
std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

/**
 * 清理單字（移除非字母字元）
 * Clean word (remove non-alphabetic characters)
 */
std::string cleanWord(const std::string& word) {
    std::string result;
    for (char c : word) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            result += std::tolower(static_cast<unsigned char>(c));
        }
    }
    return result;
}

/**
 * 統計文字中每個單字出現的次數
 * Count occurrences of each word in text
 *
 * @param text 要分析的文字
 * @return 單字計數的雜湊表
 */
HashTable<std::string, int> wordCount(const std::string& text) {
    HashTable<std::string, int> counter;

    std::istringstream stream(text);
    std::string word;

    while (stream >> word) {
        std::string cleaned = cleanWord(word);
        if (!cleaned.empty()) {
            // 使用 [] 運算子：若不存在則初始化為 0
            // Using [] operator: initializes to 0 if not exists
            counter[cleaned]++;
        }
    }

    return counter;
}

/**
 * 取得出現次數最多的前 n 個單字
 * Get top n most frequent words
 */
std::vector<std::pair<std::string, int>> getTopWords(
    HashTable<std::string, int>& counter, size_t n) {

    // 收集所有單字與計數 - Collect all words and counts
    std::vector<std::pair<std::string, int>> wordCounts;
    for (auto& pair : counter) {
        wordCounts.push_back(pair);
    }

    // 按次數排序（降序） - Sort by count (descending)
    std::sort(wordCounts.begin(), wordCounts.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });

    // 只保留前 n 個 - Keep only top n
    if (wordCounts.size() > n) {
        wordCounts.resize(n);
    }

    return wordCounts;
}

int main() {
    // 範例文字 - Sample text
    std::string sampleText = R"(
        Data structures are fundamental building blocks in computer science.
        A hash table is a data structure that implements an associative array,
        also called a dictionary. A dictionary is a collection of key-value pairs.
        Hash tables use a hash function to compute an index into an array of buckets.
        The hash function takes a key as input and returns a hash code.
        Hash tables provide constant time average case for search, insert, and delete.
        This makes hash tables very efficient for many applications.
        Common applications include database indexing, caching, and symbol tables.
        Hash tables are one of the most important data structures in programming.
        Many programming languages provide built-in hash table implementations.
    )";

    std::cout << "=== 單字計數範例 Word Count Example ===" << std::endl
              << std::endl;

    // 執行單字計數 - Perform word counting
    auto counter = wordCount(sampleText);

    std::cout << "總共有 " << counter.size() << " 個不同的單字" << std::endl;
    std::cout << "負載因子: " << counter.loadFactor() << std::endl << std::endl;

    // 顯示前 10 個最常出現的單字 - Show top 10 words
    std::cout << "前 10 個最常出現的單字 Top 10 most frequent words:"
              << std::endl;
    std::cout << "------------------------------" << std::endl;

    auto topWords = getTopWords(counter, 10);
    for (const auto& pair : topWords) {
        std::cout << "  " << pair.first;
        // 對齊輸出 - Align output
        for (size_t i = pair.first.length(); i < 15; ++i) {
            std::cout << " ";
        }
        std::cout << " : " << pair.second << std::endl;
    }
    std::cout << std::endl;

    // 查詢特定單字 - Query specific words
    std::cout << "查詢特定單字 Query specific words:" << std::endl;
    std::vector<std::string> testWords = {"hash", "data", "python", "structures"};

    for (const auto& word : testWords) {
        auto result = counter.search(word);
        if (result.has_value()) {
            std::cout << "  '" << word << "' 出現 " << result.value() << " 次"
                      << std::endl;
        } else {
            std::cout << "  '" << word << "' 未出現在文字中" << std::endl;
        }
    }

    return 0;
}
