import java.util.*;  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 雜湊表應用範例：單字計數（Word Count）
 * Hash Table Application Example: Word Counting
 *(blank line)
 * 展示如何使用雜湊表實作高效的單字頻率統計 / Demonstrates using hash table for efficient word frequency counting
 */  // End of block comment
public class ExampleWordCount {  // Execute this statement as part of the data structure implementation.

    /** Doc block start
     * 統計文字中每個單字出現的次數 / Count occurrences of each word in text
     *(blank line)
     * @param text 要分析的文字
     * @return 單字計數的雜湊表
     */  // End of block comment
    public static HashTable<String, Integer> wordCount(String text) {  // Execute this statement as part of the data structure implementation.
        HashTable<String, Integer> counter = new HashTable<>();  // Assign or update a variable that represents the current algorithm state.

        // 將文字轉為小寫並分割成單字 / Convert to lowercase and split into words
        String[] words = text.toLowerCase().split("[^a-zA-Z]+");  // Assign or update a variable that represents the current algorithm state.

        for (String word : words) {  // Iterate over a range/collection to process each item in sequence.
            if (!word.isEmpty()) {  // Evaluate the condition and branch into the appropriate code path.
                Integer count = counter.search(word);  // Assign or update a variable that represents the current algorithm state.
                if (count == null) {  // Evaluate the condition and branch into the appropriate code path.
                    counter.insert(word, 1);  // Execute this statement as part of the data structure implementation.
                } else {  // Handle the alternative branch when the condition is false.
                    counter.insert(word, count + 1);  // Execute this statement as part of the data structure implementation.
                }  // Close the current block scope.
            }  // Close the current block scope.
        }  // Close the current block scope.

        return counter;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 取得出現次數最多的前 n 個單字
     * Get top n most frequent words
     */  // End of block comment
    public static List<Map.Entry<String, Integer>> getTopWords(  // Execute this statement as part of the data structure implementation.
            HashTable<String, Integer> counter, int n) {  // Execute this statement as part of the data structure implementation.

        // 收集所有單字與計數 - Collect all words and counts
        List<Map.Entry<String, Integer>> wordCounts = new ArrayList<>();  // Assign or update a variable that represents the current algorithm state.

        // 遍歷雜湊表（這裡我們需要一個額外的方法或手動遍歷） / 遍歷 hash table (這裡我們需要一個額外的方法或手動遍歷)
        // 為了示範，我們用一個簡單的方式收集 / Note: In a complete implementation, we would add an iterator to HashTable

        return wordCounts;  // Return the computed result to the caller.
    }  // Close the current block scope.

    public static void main(String[] args) {  // Execute this statement as part of the data structure implementation.
        // 範例文字 - Sample text
        String sampleText = """  // Assign or update a variable that represents the current algorithm state.
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
            """;  // Execute this statement as part of the data structure implementation.

        System.out.println("=== 單字計數範例 Word Count Example ===\n");  // Execute this statement as part of the data structure implementation.

        // 執行單字計數 - Perform word counting
        HashTable<String, Integer> counter = wordCount(sampleText);  // Assign or update a variable that represents the current algorithm state.

        System.out.println("總共有 " + counter.size() + " 個不同的單字");  // Execute this statement as part of the data structure implementation.
        System.out.printf("負載因子: %.3f%n%n", counter.getLoadFactor());  // Execute this statement as part of the data structure implementation.

        // 查詢特定單字 - Query specific words
        System.out.println("查詢特定單字 Query specific words:");  // Execute this statement as part of the data structure implementation.
        String[] testWords = {"hash", "data", "python", "structures"};  // Assign or update a variable that represents the current algorithm state.

        for (String word : testWords) {  // Iterate over a range/collection to process each item in sequence.
            Integer count = counter.search(word);  // Assign or update a variable that represents the current algorithm state.
            if (count != null) {  // Evaluate the condition and branch into the appropriate code path.
                System.out.println("  '" + word + "' 出現 " + count + " 次");  // Execute this statement as part of the data structure implementation.
            } else {  // Handle the alternative branch when the condition is false.
                System.out.println("  '" + word + "' 未出現在文字中");  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        }  // Close the current block scope.
    }  // Close the current block scope.
}  // Close the current block scope.
