/** Doc block start
 * 雜湊表應用範例：單字計數（Word Count）
 * Hash Table Application Example: Word Counting
 *(blank line)
 * 展示如何使用雜湊表實作高效的單字頻率統計 / Demonstrates using hash table for efficient word frequency counting
 */  // End of block comment

#include <iostream>  // Execute this statement as part of the data structure implementation.
#include <string>  // Execute this statement as part of the data structure implementation.
#include <sstream>  // Execute this statement as part of the data structure implementation.
#include <algorithm>  // Execute this statement as part of the data structure implementation.
#include <vector>  // Execute this statement as part of the data structure implementation.
#include <cctype>  // Execute this statement as part of the data structure implementation.
#include "HashTable.hpp"  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 將字串轉為小寫 / Convert string to lowercase
 */  // End of block comment
std::string toLower(const std::string& str) {  // Execute this statement as part of the data structure implementation.
    std::string result = str;  // Assign or update a variable that represents the current algorithm state.
    std::transform(result.begin(), result.end(), result.begin(),  // Execute this statement as part of the data structure implementation.
                   [](unsigned char c) { return std::tolower(c); });  // Execute this statement as part of the data structure implementation.
    return result;  // Return the computed result to the caller.
}  // Close the current block scope.

/** Doc block start
 * 清理單字（移除非字母字元） / Clean word (remove non-alphabetic characters)
 */  // End of block comment
std::string cleanWord(const std::string& word) {  // Execute this statement as part of the data structure implementation.
    std::string result;  // Execute this statement as part of the data structure implementation.
    for (char c : word) {  // Iterate over a range/collection to process each item in sequence.
        if (std::isalpha(static_cast<unsigned char>(c))) {  // Evaluate the condition and branch into the appropriate code path.
            result += std::tolower(static_cast<unsigned char>(c));  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.
    }  // Close the current block scope.
    return result;  // Return the computed result to the caller.
}  // Close the current block scope.

/** Doc block start
 * 統計文字中每個單字出現的次數 / Count occurrences of each word in text
 *(blank line)
 * @param text 要分析的文字
 * @return 單字計數的雜湊表
 */  // End of block comment
HashTable<std::string, int> wordCount(const std::string& text) {  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int> counter;  // Execute this statement as part of the data structure implementation.

    std::istringstream stream(text);  // Execute this statement as part of the data structure implementation.
    std::string word;  // Execute this statement as part of the data structure implementation.

    while (stream >> word) {  // Repeat while the loop condition remains true.
        std::string cleaned = cleanWord(word);  // Assign or update a variable that represents the current algorithm state.
        if (!cleaned.empty()) {  // Evaluate the condition and branch into the appropriate code path.
            // 使用 [] 運算子：若不存在則初始化為 0 / Using [] operator: initializes to 0 if not exists
            counter[cleaned]++;  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.

    return counter;  // Return the computed result to the caller.
}  // Close the current block scope.

/** Doc block start
 * 取得出現次數最多的前 n 個單字
 * Get top n most frequent words
 */  // End of block comment
std::vector<std::pair<std::string, int>> getTopWords(  // Execute this statement as part of the data structure implementation.
    HashTable<std::string, int>& counter, size_t n) {  // Execute this statement as part of the data structure implementation.

    // 收集所有單字與計數 - Collect all words and counts
    std::vector<std::pair<std::string, int>> wordCounts;  // Execute this statement as part of the data structure implementation.
    for (auto& pair : counter) {  // Iterate over a range/collection to process each item in sequence.
        wordCounts.push_back(pair);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    // 按次數排序（降序） - Sort by count (descending)
    std::sort(wordCounts.begin(), wordCounts.end(),  // Execute this statement as part of the data structure implementation.
              [](const auto& a, const auto& b) {  // Execute this statement as part of the data structure implementation.
                  return a.second > b.second;  // Return the computed result to the caller.
              });  // Execute this statement as part of the data structure implementation.

    // 只保留前 n 個 - Keep only top n
    if (wordCounts.size() > n) {  // Evaluate the condition and branch into the appropriate code path.
        wordCounts.resize(n);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    return wordCounts;  // Return the computed result to the caller.
}  // Close the current block scope.

int main() {  // Execute this statement as part of the data structure implementation.
    // 範例文字 - Sample text
    std::string sampleText = R"(  // Assign or update a variable that represents the current algorithm state.
        Data structures are fundamental building blocks in computer science.  // Execute this statement as part of the data structure implementation.
        A hash table is a data structure that implements an associative array,  // Execute this statement as part of the data structure implementation.
        also called a dictionary. A dictionary is a collection of key-value pairs.  // Execute this statement as part of the data structure implementation.
        Hash tables use a hash function to compute an index into an array of buckets.  // Access or update the bucket storage used to hold entries or chains.
        The hash function takes a key as input and returns a hash code.  // Execute this statement as part of the data structure implementation.
        Hash tables provide constant time average case for search, insert, and delete.  // Execute this statement as part of the data structure implementation.
        This makes hash tables very efficient for many applications.  // Execute this statement as part of the data structure implementation.
        Common applications include database indexing, caching, and symbol tables.  // Execute this statement as part of the data structure implementation.
        Hash tables are one of the most important data structures in programming.  // Execute this statement as part of the data structure implementation.
        Many programming languages provide built-in hash table implementations.  // Execute this statement as part of the data structure implementation.
    )";  // Execute this statement as part of the data structure implementation.

    std::cout << "=== 單字計數範例 Word Count Example ===" << std::endl  // Execute this statement as part of the data structure implementation.
              << std::endl;  // Execute this statement as part of the data structure implementation.

    // 執行單字計數 - Perform word counting
    auto counter = wordCount(sampleText);  // Assign or update a variable that represents the current algorithm state.

    std::cout << "總共有 " << counter.size() << " 個不同的單字" << std::endl;  // Execute this statement as part of the data structure implementation.
    std::cout << "負載因子: " << counter.loadFactor() << std::endl << std::endl;  // Execute this statement as part of the data structure implementation.

    // 顯示前 10 個最常出現的單字 - Show top 10 words
    std::cout << "前 10 個最常出現的單字 Top 10 most frequent words:"  // Execute this statement as part of the data structure implementation.
              << std::endl;  // Execute this statement as part of the data structure implementation.
    std::cout << "------------------------------" << std::endl;  // Execute this statement as part of the data structure implementation.

    auto topWords = getTopWords(counter, 10);  // Assign or update a variable that represents the current algorithm state.
    for (const auto& pair : topWords) {  // Iterate over a range/collection to process each item in sequence.
        std::cout << "  " << pair.first;  // Execute this statement as part of the data structure implementation.
        // 對齊輸出 - Align output
        for (size_t i = pair.first.length(); i < 15; ++i) {  // Iterate over a range/collection to process each item in sequence.
            std::cout << " ";  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
        std::cout << " : " << pair.second << std::endl;  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.
    std::cout << std::endl;  // Execute this statement as part of the data structure implementation.

    // 查詢特定單字 - Query specific words
    std::cout << "查詢特定單字 Query specific words:" << std::endl;  // Execute this statement as part of the data structure implementation.
    std::vector<std::string> testWords = {"hash", "data", "python", "structures"};  // Assign or update a variable that represents the current algorithm state.

    for (const auto& word : testWords) {  // Iterate over a range/collection to process each item in sequence.
        auto result = counter.search(word);  // Assign or update a variable that represents the current algorithm state.
        if (result.has_value()) {  // Evaluate the condition and branch into the appropriate code path.
            std::cout << "  '" << word << "' 出現 " << result.value() << " 次"  // Execute this statement as part of the data structure implementation.
                      << std::endl;  // Execute this statement as part of the data structure implementation.
        } else {  // Handle the alternative branch when the condition is false.
            std::cout << "  '" << word << "' 未出現在文字中" << std::endl;  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.

    return 0;  // Return the computed result to the caller.
}  // Close the current block scope.
