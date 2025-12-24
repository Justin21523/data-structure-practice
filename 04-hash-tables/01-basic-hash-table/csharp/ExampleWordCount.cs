/**
 * 雜湊表應用範例：單字計數（Word Count）
 * Hash Table Application Example: Word Counting
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace DataStructures
{
    public class ExampleWordCount
    {
        /// <summary>
        /// 統計文字中每個單字出現的次數
        /// Count occurrences of each word in text
        /// </summary>
        public static HashTable<string, int> WordCount(string text)
        {
            var counter = new HashTable<string, int>();

            // 使用正則表達式分割單字
            // Use regex to split words
            var words = Regex.Matches(text.ToLower(), @"[a-zA-Z]+")
                            .Cast<Match>()
                            .Select(m => m.Value);

            foreach (var word in words)
            {
                if (counter.TryGetValue(word, out int count))
                {
                    counter[word] = count + 1;
                }
                else
                {
                    counter.Insert(word, 1);
                }
            }

            return counter;
        }

        /// <summary>
        /// 取得出現次數最多的前 n 個單字
        /// Get top n most frequent words
        /// </summary>
        public static List<KeyValuePair<string, int>> GetTopWords(
            HashTable<string, int> counter, int n = 10)
        {
            return counter
                .OrderByDescending(kvp => kvp.Value)
                .Take(n)
                .ToList();
        }

        public static void Main(string[] args)
        {
            string sampleText = @"
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
            ";

            Console.WriteLine("=== 單字計數範例 Word Count Example ===\n");

            // 執行單字計數 - Perform word counting
            var counter = WordCount(sampleText);

            Console.WriteLine($"總共有 {counter.Count} 個不同的單字");
            Console.WriteLine($"負載因子: {counter.LoadFactor:F3}\n");

            // 顯示前 10 個最常出現的單字 - Show top 10 words
            Console.WriteLine("前 10 個最常出現的單字 Top 10 most frequent words:");
            Console.WriteLine(new string('-', 30));

            foreach (var kvp in GetTopWords(counter, 10))
            {
                Console.WriteLine($"  {kvp.Key,-15} : {kvp.Value}");
            }
            Console.WriteLine();

            // 查詢特定單字 - Query specific words
            Console.WriteLine("查詢特定單字 Query specific words:");
            string[] testWords = { "hash", "data", "python", "structures" };

            foreach (var word in testWords)
            {
                if (counter.TryGetValue(word, out int count))
                {
                    Console.WriteLine($"  '{word}' 出現 {count} 次");
                }
                else
                {
                    Console.WriteLine($"  '{word}' 未出現在文字中");
                }
            }
        }
    }
}
