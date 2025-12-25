// 03 雜湊函數示範程式（C#）/ Hash functions demo program (C#).  // Bilingual file header.

using System;  // Provide Console I/O and exceptions.  
using System.Collections.Generic;  // Provide List<T> for demo key lists and expected snapshots.  

namespace HashFunctionsUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class Program  // Console entry point for the demo and the built-in tests.  
    {  // Open class scope.  
        private static void AssertTrue(bool condition, string message)  // Minimal boolean assertion helper.  
        {  // Open method scope.  
            if (!condition)  // Fail when condition is false.  
            {  // Open failure scope.  
                throw new InvalidOperationException("FAIL: " + message);  // Throw to signal test failure.  
            }  // Close failure scope.  
        }  // Close AssertTrue.  

        private static void AssertEqual(long expected, long actual, string message)  // Minimal equality assertion helper.  
        {  // Open method scope.  
            if (expected != actual)  // Fail when values differ.  
            {  // Open failure scope.  
                throw new InvalidOperationException($"{message} (expected={expected}, actual={actual})");  // Throw mismatch.  
            }  // Close failure scope.  
        }  // Close AssertEqual.  

        private static void RunTests()  // Run built-in tests (no external packages).  
        {  // Open method scope.  
            {  // Open scope: integer hash tests.  
                int m1 = 97;  // Prime modulus for division hashing.  
                for (int key = 0; key < 1000; key++)  // Check a range of keys.  
                {  // Open loop scope.  
                    int h = HashFunctions.DivisionHash(key, m1);  // Compute division hash.  
                    AssertTrue(0 <= h && h < m1, "DivisionHash should be in [0, m)");  // Validate range.  
                }  // Close loop scope.  
                AssertEqual(HashFunctions.DivisionHash(12345, m1), HashFunctions.DivisionHash(12345, m1), "DivisionHash should be deterministic");  // Validate determinism.  

                int m2 = 128;  // Power-of-two modulus for multiplication hashing.  
                for (int key = 0; key < 1000; key++)  // Check a range of keys.  
                {  // Open loop scope.  
                    int h = HashFunctions.MultiplicationHash(key, m2);  // Compute multiplication hash.  
                    AssertTrue(0 <= h && h < m2, "MultiplicationHash should be in [0, m)");  // Validate range.  
                }  // Close loop scope.  
                AssertEqual(HashFunctions.MultiplicationHash(12345, m2), HashFunctions.MultiplicationHash(12345, m2), "MultiplicationHash should be deterministic");  // Validate determinism.  

                long mid = HashFunctions.MidSquareHash(1234, 4);  // Compute mid-square hash for known example.  
                AssertTrue(mid >= 0, "MidSquareHash should be non-negative");  // Validate non-negativity.  
                AssertEqual(5227, mid, "MidSquareHash(1234,4) should match reference digits");  // Validate expected extraction.  

                int fold = HashFunctions.FoldingHash(123456789, 3, 100);  // Compute folding hash.  
                AssertTrue(0 <= fold && fold < 100, "FoldingHash should be in [0, m)");  // Validate range.  
            }  // Close integer hash scope.  

            {  // Open scope: string hash tests.  
                int m = 100;  // Bucket count for modulo-based hashes.  
                foreach (string s in new[] { "hello", "world", "test", "hash" })  // Test a small set of strings.  
                {  // Open foreach scope.  
                    int sum = HashFunctions.SimpleSumHash(s, m);  // Compute simple sum hash.  
                    AssertTrue(0 <= sum && sum < m, "SimpleSumHash should be in [0, m)");  // Validate range.  
                    uint poly = HashFunctions.PolynomialHash(s, 31u, 1000u);  // Compute polynomial hash mod 1000.  
                    AssertTrue(poly < 1000u, "PolynomialHash should be in [0, m)");  // Validate range.  
                    AssertEqual(HashFunctions.Djb2Hash(s), HashFunctions.Djb2Hash(s), "Djb2Hash should be deterministic");  // Validate determinism.  
                    AssertEqual(HashFunctions.Fnv1aHash(s), HashFunctions.Fnv1aHash(s), "Fnv1aHash should be deterministic");  // Validate determinism.  
                    AssertEqual(HashFunctions.JenkinsOneAtATime(s), HashFunctions.JenkinsOneAtATime(s), "JenkinsOneAtATime should be deterministic");  // Validate determinism.  
                }  // Close foreach scope.  

                int mBig = 1000;  // Larger modulus for anagram collision tests.  
                int h1 = HashFunctions.SimpleSumHash("abc", mBig);  // Hash "abc".  
                int h2 = HashFunctions.SimpleSumHash("bca", mBig);  // Hash "bca".  
                int h3 = HashFunctions.SimpleSumHash("cab", mBig);  // Hash "cab".  
                AssertTrue(h1 == h2 && h2 == h3, "SimpleSumHash should collide for anagrams");  // Validate expected collision.  

                uint d1 = HashFunctions.Djb2Hash("abc");  // Hash "abc" with DJB2.  
                uint d2 = HashFunctions.Djb2Hash("bca");  // Hash "bca" with DJB2.  
                uint d3 = HashFunctions.Djb2Hash("cab");  // Hash "cab" with DJB2.  
                AssertTrue(d1 != d2 && d2 != d3 && d1 != d3, "Djb2Hash should not collide for these anagrams");  // Validate non-collision.  

                AssertEqual(5381, HashFunctions.Djb2Hash(""), "Djb2Hash(\"\") should equal 5381");  // Validate empty-string base.  
                AssertEqual(0, HashFunctions.PolynomialHash("", 31u, 100u), "PolynomialHash(\"\") should equal 0");  // Validate polynomial empty-string base.  
            }  // Close string hash scope.  

            {  // Open scope: distribution analyzer tests.  
                var keys = new List<string>();  // Build deterministic key list.  
                for (int i = 0; i < 1000; i++)  // Generate 1000 keys.  
                {  // Open loop scope.  
                    keys.Add("key_" + i);  // Add one key string.  
                }  // Close loop scope.  
                HashFunctions.DistributionReport r1 = HashFunctions.AnalyzeDistribution((k, m) => (int)(HashFunctions.Djb2Hash(k) % (uint)m), keys, 100);  // Analyze DJB2 reduced mod 100.  
                AssertTrue(r1.NonEmptyBuckets > 60, "DJB2 should fill most buckets for sequential keys");  // Loose non-empty check.  
                AssertTrue(r1.StdDeviation < 10.0, "DJB2 std deviation should be reasonably small");  // Loose std-dev check.  
                HashFunctions.DistributionReport r2 = HashFunctions.AnalyzeDistribution((k, m) => (int)(HashFunctions.Fnv1aHash(k) % (uint)m), keys, 100);  // Analyze FNV-1a reduced mod 100.  
                AssertTrue(r2.StdDeviation < 10.0, "FNV-1a std deviation should be reasonably small");  // Loose std-dev check.  
            }  // Close distribution scope.  

            {  // Open scope: universal hash family tests.  
                int m = 100;  // Bucket count.  
                var uh = new UniversalHashing.UniversalHashFamily(m, seed: 123);  // Use deterministic seed for stable tests.  
                for (int key = 0; key < 1000; key++)  // Check a range of keys.  
                {  // Open loop scope.  
                    int h = uh.Hash(key);  // Compute hash value.  
                    AssertTrue(0 <= h && h < m, "UniversalHashFamily hash should be in [0, m)");  // Validate range.  
                }  // Close loop scope.  
                AssertEqual(uh.Hash(12345), uh.Hash(12345), "UniversalHashFamily should be deterministic within an instance");  // Validate determinism.  

                var results = new List<int>();  // Store hashes across regenerations.  
                for (int i = 0; i < 10; i++)  // Repeat regenerations.  
                {  // Open loop scope.  
                    results.Add(uh.Hash(12345));  // Record hash.  
                    uh.Regenerate();  // Regenerate parameters.  
                }  // Close loop scope.  
                int distinct = 0;  // Count distinct values (simple O(n^2) check for small n).  
                for (int i = 0; i < results.Count; i++)  // Check each result.  
                {  // Open loop scope.  
                    bool seen = false;  // Track whether results[i] was seen before.  
                    for (int j = 0; j < i; j++)  // Compare with earlier results.  
                    {  // Open loop scope.  
                        if (results[j] == results[i])  // Detect duplicate.  
                        {  // Open branch.  
                            seen = true;  // Mark as seen.  
                            break;  // Stop inner loop.  
                        }  // Close branch.  
                    }  // Close loop scope.  
                    if (!seen)  // Count new distinct value.  
                    {  // Open branch.  
                        distinct += 1;  // Increment distinct count.  
                    }  // Close branch.  
                }  // Close loop scope.  
                AssertTrue(distinct > 1, "Regenerate should change hash value sometimes");  // Very unlikely to be all identical.  
            }  // Close universal hash family scope.  

            {  // Open scope: collision probability bound test (deterministic simulation).  
                int m = 50;  // Bucket count for probability bound 1/m.  
                int trials = 5000;  // Moderate trial count to keep tests fast.  
                int collisions = 0;  // Collision counter.  
                int x = 100;  // Key x.  
                int y = 200;  // Key y (x != y).  
                for (int t = 0; t < trials; t++)  // Repeat trials with different deterministic seeds.  
                {  // Open loop scope.  
                    var uh = new UniversalHashing.UniversalHashFamily(m, seed: t);  // Use seed=t to vary parameters.  
                    if (uh.Hash(x) == uh.Hash(y))  // Count collisions.  
                    {  // Open branch.  
                        collisions += 1;  // Increment collision counter.  
                    }  // Close branch.  
                }  // Close loop scope.  
                double probability = (double)collisions / (double)trials;  // Compute empirical collision probability.  
                AssertTrue(probability < (2.0 / (double)m), "collision probability should be < 2/m");  // Allow variance like Python test.  
            }  // Close collision probability scope.  

            {  // Open scope: universal string hash family tests.  
                int m = 100;  // Bucket count.  
                var ush = new UniversalHashing.UniversalStringHashFamily(m, seed: 999);  // Use deterministic seed for stable tests.  
                foreach (string s in new[] { "hello", "world", "test", "hash" })  // Test sample strings.  
                {  // Open foreach scope.  
                    int h = ush.Hash(s);  // Compute hash value.  
                    AssertTrue(0 <= h && h < m, "UniversalStringHashFamily hash should be in [0, m)");  // Validate range.  
                    AssertEqual(ush.Hash(s), ush.Hash(s), "UniversalStringHashFamily should be deterministic within an instance");  // Validate determinism.  
                }  // Close foreach scope.  
            }  // Close universal string hash family scope.  

            {  // Open scope: universal hash table tests.  
                var ht = new UniversalHashing.UniversalHashTable(seed: 123);  // Create empty table with deterministic seed.  
                ht.Insert(1, "one");  // Insert 1 -> "one".  
                ht.Insert(2, "two");  // Insert 2 -> "two".  
                AssertTrue(ht.Search(1) == "one", "Search(1) should return one");  // Validate search.  
                AssertTrue(ht.Search(2) == "two", "Search(2) should return two");  // Validate search.  
                AssertTrue(ht.Search(3) == null, "Search(3) should return null");  // Validate missing.  
                ht.Insert(1, "ONE");  // Update existing key.  
                AssertTrue(ht.Search(1) == "ONE", "Update should overwrite value");  // Validate update.  
                AssertEqual(2, ht.Size, "Size should remain 2 after update");  // Validate size.  
                AssertTrue(ht.Delete(1), "Delete(1) should succeed");  // Delete existing.  
                AssertTrue(ht.Search(1) == null, "Search(1) should be null after delete");  // Validate deletion.  
                AssertTrue(!ht.Delete(1), "Delete(1) should return false when missing");  // Validate missing delete.  
            }  // Close universal hash table scope.  

            {  // Open scope: universal hash table many insertions.  
                var ht = new UniversalHashing.UniversalHashTable(seed: 321);  // Create empty table with deterministic seed.  
                for (int i = 0; i < 100; i++)  // Insert 100 items.  
                {  // Open loop scope.  
                    ht.Insert(i, "value_" + i);  // Insert i -> value_i.  
                }  // Close loop scope.  
                AssertEqual(100, ht.Size, "Size should be 100 after 100 inserts");  // Validate size.  
                for (int i = 0; i < 100; i++)  // Validate lookups.  
                {  // Open loop scope.  
                    AssertTrue(ht.Search(i) == "value_" + i, "Search(i) should return value_i");  // Validate value.  
                }  // Close loop scope.  
            }  // Close universal hash table bulk scope.  
        }  // Close RunTests.  

        private static string FormatDemo()  // Format a small demo output to mirror the Python reference.  
        {  // Open method scope.  
            int testKey = 123456;  // Pick a deterministic integer key for demo output.  
            int mPrime = 97;  // Use a prime bucket count for division hashing.  
            var lines = new List<string>();  // Accumulate demo lines.  
            lines.Add("=== Hash Functions Demo (C#) ===");  // Print header.  
            lines.Add($"Key: {testKey}, m: {mPrime}");  // Print key summary.  
            lines.Add($"  Division:       {HashFunctions.DivisionHash(testKey, mPrime)}");  // Print division hash.  
            lines.Add($"  Multiplication: {HashFunctions.MultiplicationHash(testKey, mPrime)}");  // Print multiplication hash.  
            lines.Add($"  Mid-Square (4): {HashFunctions.MidSquareHash(testKey, 4)}");  // Print mid-square hash.  
            lines.Add($"  Folding (3):    {HashFunctions.FoldingHash(testKey, 3, mPrime)}");  // Print folding hash.  
            lines.Add("");  // Blank line.  
            lines.Add("--- String hashes ---");  // Print section title.  
            foreach (string s in new[] { "hello", "world", "hash", "table", "csharp" })  // Print a few hash values for each string.  
            {  // Open foreach scope.  
                uint djb2 = HashFunctions.Djb2Hash(s);  // Compute DJB2 hash.  
                uint fnv = HashFunctions.Fnv1aHash(s);  // Compute FNV-1a hash.  
                uint poly = HashFunctions.PolynomialHash(s, 31u, 1000003u);  // Compute polynomial hash mod a prime.  
                lines.Add($"{s} => djb2={djb2}, fnv1a={fnv}, poly={poly}");  // Add one line.  
            }  // Close foreach scope.  
            return string.Join(Environment.NewLine, lines);  // Join lines for printing.  
        }  // Close FormatDemo.  

        public static int Main(string[] args)  // Entry point supporting demo and test modes.  
        {  // Open method scope.  
            try  // Catch exceptions for consistent CLI behavior.  
            {  // Open try scope.  
                if (args.Length > 0 && args[0] == "--test")  // Run tests when flag is provided.  
                {  // Open test branch.  
                    RunTests();  // Execute tests.  
                    Console.WriteLine("All tests PASSED.");  // Report success.  
                    return 0;  // Exit success.  
                }  // Close test branch.  

                Console.WriteLine(FormatDemo());  // Print demo output.  
                return 0;  // Exit success.  
            }  // Close try scope.  
            catch (Exception ex)  // Print errors consistently.  
            {  // Open catch scope.  
                Console.Error.WriteLine(ex.Message);  // Print error message.  
                return 1;  // Exit failure.  
            }  // Close catch scope.  
        }  // Close Main.  
    }  // Close class scope.  
}  // Close namespace scope.  

