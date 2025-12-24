import java.util.*;

/**
 * 雜湊表應用範例：單字計數（Word Count）
 * Hash Table Application Example: Word Counting
 *
 * 展示如何使用雜湊表實作高效的單字頻率統計
 * Demonstrates using hash table for efficient word frequency counting
 */
public class ExampleWordCount {

    /**
     * 統計文字中每個單字出現的次數
     * Count occurrences of each word in text
     *
     * @param text 要分析的文字
     * @return 單字計數的雜湊表
     */
    public static HashTable<String, Integer> wordCount(String text) {
        HashTable<String, Integer> counter = new HashTable<>();

        // 將文字轉為小寫並分割成單字
        // Convert to lowercase and split into words
        String[] words = text.toLowerCase().split("[^a-zA-Z]+");

        for (String word : words) {
            if (!word.isEmpty()) {
                Integer count = counter.search(word);
                if (count == null) {
                    counter.insert(word, 1);
                } else {
                    counter.insert(word, count + 1);
                }
            }
        }

        return counter;
    }

    /**
     * 取得出現次數最多的前 n 個單字
     * Get top n most frequent words
     */
    public static List<Map.Entry<String, Integer>> getTopWords(
            HashTable<String, Integer> counter, int n) {

        // 收集所有單字與計數 - Collect all words and counts
        List<Map.Entry<String, Integer>> wordCounts = new ArrayList<>();

        // 遍歷雜湊表（這裡我們需要一個額外的方法或手動遍歷）
        // 為了示範，我們用一個簡單的方式收集
        // Note: In a complete implementation, we would add an iterator to HashTable

        return wordCounts;
    }

    public static void main(String[] args) {
        // 範例文字 - Sample text
        String sampleText = """
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
            """;

        System.out.println("=== 單字計數範例 Word Count Example ===\n");

        // 執行單字計數 - Perform word counting
        HashTable<String, Integer> counter = wordCount(sampleText);

        System.out.println("總共有 " + counter.size() + " 個不同的單字");
        System.out.printf("負載因子: %.3f%n%n", counter.getLoadFactor());

        // 查詢特定單字 - Query specific words
        System.out.println("查詢特定單字 Query specific words:");
        String[] testWords = {"hash", "data", "python", "structures"};

        for (String word : testWords) {
            Integer count = counter.search(word);
            if (count != null) {
                System.out.println("  '" + word + "' 出現 " + count + " 次");
            } else {
                System.out.println("  '" + word + "' 未出現在文字中");
            }
        }
    }
}
