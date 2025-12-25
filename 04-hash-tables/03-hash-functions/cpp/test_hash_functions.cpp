// 03 雜湊函數測試（C++）/ Hash functions tests (C++).  // Bilingual file header.

#include "HashFunctions.hpp"  // Include hash function APIs under test.
#include "UniversalHashing.hpp"  // Include universal hashing APIs under test.

#include <iostream>  // Provide std::cout for status output.
#include <stdexcept>  // Provide exception base types for assertions.
#include <string>  // Provide std::string for test values.
#include <vector>  // Provide std::vector for test key sets.

static void assertTrue(bool condition, const char* message) {  // Minimal assertion helper.
    if (!condition) {  // Fail when condition is false.
        throw std::runtime_error(std::string("FAIL: ") + message);  // Throw to signal test failure.
    }  // Close failure branch.
}  // Close assertTrue().

static void assertEquals(long long expected, long long actual, const char* message) {  // Minimal equality assertion helper.
    if (expected != actual) {  // Fail when values differ.
        throw std::runtime_error(std::string("FAIL: ") + message + " (expected=" + std::to_string(expected) + ", actual=" + std::to_string(actual) + ")");  // Throw mismatch.
    }  // Close mismatch branch.
}  // Close assertEquals().

static void testIntegerHashFunctions() {  // Verify basic properties for integer hash functions.
    int m1 = 97;  // Prime modulus for division hashing.
    for (int key = 0; key < 1000; key++) {  // Check a range of keys.
        int h = hashfunctionsunit::divisionHash(key, m1);  // Compute division hash.
        assertTrue(0 <= h && h < m1, "divisionHash should be in [0, m)");  // Validate range.
    }  // Close loop.
    assertEquals(hashfunctionsunit::divisionHash(12345, m1), hashfunctionsunit::divisionHash(12345, m1), "divisionHash should be deterministic");  // Validate determinism.

    int m2 = 128;  // Power-of-two modulus for multiplication hashing.
    for (int key = 0; key < 1000; key++) {  // Check a range of keys.
        int h = hashfunctionsunit::multiplicationHash(key, m2);  // Compute multiplication hash.
        assertTrue(0 <= h && h < m2, "multiplicationHash should be in [0, m)");  // Validate range.
    }  // Close loop.
    assertEquals(hashfunctionsunit::multiplicationHash(12345, m2), hashfunctionsunit::multiplicationHash(12345, m2), "multiplicationHash should be deterministic");  // Validate determinism.

    long long mid = hashfunctionsunit::midSquareHash(1234, 4);  // Compute mid-square hash for known example.
    assertTrue(mid >= 0, "midSquareHash should be non-negative");  // Validate non-negativity.
    assertEquals(5227, mid, "midSquareHash(1234,4) should match reference digits");  // Validate expected extraction.

    int fold = hashfunctionsunit::foldingHash(123456789, 3, 100);  // Compute folding hash.
    assertTrue(0 <= fold && fold < 100, "foldingHash should be in [0, m)");  // Validate range.
}  // Close testIntegerHashFunctions().

static void testStringHashFunctions() {  // Verify basic properties for string hash functions.
    int m = 100;  // Bucket count for modulo-based hashes.
    for (const std::string& s : std::vector<std::string>{"hello", "world", "test", "hash"}) {  // Test a small set of strings.
        int sum = hashfunctionsunit::simpleSumHash(s, m);  // Compute simple sum hash.
        assertTrue(0 <= sum && sum < m, "simpleSumHash should be in [0, m)");  // Validate range.
        std::uint32_t poly = hashfunctionsunit::polynomialHash(s, 31u, 1000u);  // Compute polynomial hash mod 1000.
        assertTrue(poly < 1000u, "polynomialHash should be in [0, m)");  // Validate range.
        assertEquals(hashfunctionsunit::djb2Hash(s), hashfunctionsunit::djb2Hash(s), "djb2Hash should be deterministic");  // Validate determinism.
        assertEquals(hashfunctionsunit::fnv1aHash(s), hashfunctionsunit::fnv1aHash(s), "fnv1aHash should be deterministic");  // Validate determinism.
        assertEquals(hashfunctionsunit::jenkinsOneAtATime(s), hashfunctionsunit::jenkinsOneAtATime(s), "jenkinsOneAtATime should be deterministic");  // Validate determinism.
    }  // Close loop.

    int mBig = 1000;  // Larger modulus for anagram collision tests.
    int h1 = hashfunctionsunit::simpleSumHash("abc", mBig);  // Hash "abc".
    int h2 = hashfunctionsunit::simpleSumHash("bca", mBig);  // Hash "bca".
    int h3 = hashfunctionsunit::simpleSumHash("cab", mBig);  // Hash "cab".
    assertTrue(h1 == h2 && h2 == h3, "simpleSumHash should collide for anagrams");  // Validate expected collision.

    std::uint32_t d1 = hashfunctionsunit::djb2Hash("abc");  // Hash "abc" with DJB2.
    std::uint32_t d2 = hashfunctionsunit::djb2Hash("bca");  // Hash "bca" with DJB2.
    std::uint32_t d3 = hashfunctionsunit::djb2Hash("cab");  // Hash "cab" with DJB2.
    assertTrue(d1 != d2 && d2 != d3 && d1 != d3, "djb2Hash should not collide for these anagrams");  // Validate non-collision.

    assertEquals(5381, hashfunctionsunit::djb2Hash(""), "djb2Hash(\"\") should equal 5381");  // Validate empty-string base.
    assertEquals(0, hashfunctionsunit::polynomialHash("", 31u, 100u), "polynomialHash(\"\") should equal 0");  // Validate polynomial empty-string base.
}  // Close testStringHashFunctions().

static void testDistributionAnalyzer() {  // Verify distribution analyzer returns reasonable stats for common hashes.
    std::vector<std::string> keys;  // Deterministic keys list.
    for (int i = 0; i < 1000; i++) {  // Build 1000 keys "key_i".
        keys.push_back("key_" + std::to_string(i));  // Append one key.
    }  // Close loop.

    auto r1 = hashfunctionsunit::analyzeDistribution(  // Analyze DJB2 reduced mod 100.
        [](const std::string& k, int m) { return static_cast<int>(hashfunctionsunit::djb2Hash(k) % static_cast<std::uint32_t>(m)); },  // Hash adapter.
        keys, 100  // Provide keys and bucket count.
    );  // Close call.
    assertTrue(r1.nonEmptyBuckets > 60, "djb2 should fill most buckets for sequential keys");  // Loose non-empty check.
    assertTrue(r1.stdDeviation < 10.0, "djb2 std deviation should be reasonably small");  // Loose std-dev check.

    auto r2 = hashfunctionsunit::analyzeDistribution(  // Analyze FNV-1a reduced mod 100.
        [](const std::string& k, int m) { return static_cast<int>(hashfunctionsunit::fnv1aHash(k) % static_cast<std::uint32_t>(m)); },  // Hash adapter.
        keys, 100  // Provide keys and bucket count.
    );  // Close call.
    assertTrue(r2.stdDeviation < 10.0, "fnv1a std deviation should be reasonably small");  // Loose std-dev check.
}  // Close testDistributionAnalyzer().

static void testUniversalHashFamily() {  // Verify universal hash family properties.
    int m = 100;  // Bucket count.
    hashfunctionsunit::UniversalHashFamily uh(m, 123u);  // Use deterministic seed for stable tests.
    for (int key = 0; key < 1000; key++) {  // Check a range of keys.
        int h = uh.hash(key);  // Compute hash value.
        assertTrue(0 <= h && h < m, "UniversalHashFamily hash should be in [0, m)");  // Validate range.
    }  // Close loop.
    assertEquals(uh.hash(12345), uh.hash(12345), "UniversalHashFamily should be deterministic within an instance");  // Validate determinism.

    std::vector<int> results;  // Store hashes across regenerations.
    for (int i = 0; i < 10; i++) {  // Repeat regenerations.
        results.push_back(uh.hash(12345));  // Record hash.
        uh.regenerate();  // Regenerate parameters.
    }  // Close loop.
    int distinct = 0;  // Count distinct values.
    for (size_t i = 0; i < results.size(); i++) {  // Compare each element.
        bool seen = false;  // Track whether results[i] was seen before.
        for (size_t j = 0; j < i; j++) {  // Compare with earlier results.
            if (results[j] == results[i]) {  // Detect duplicate.
                seen = true;  // Mark as seen.
                break;  // Stop inner loop.
            }  // Close duplicate branch.
        }  // Close inner loop.
        if (!seen) {  // Count new distinct value.
            distinct += 1;  // Increment distinct count.
        }  // Close branch.
    }  // Close outer loop.
    assertTrue(distinct > 1, "regenerate should change hash value sometimes");  // Very unlikely to be all identical.
}  // Close testUniversalHashFamily().

static void testUniversalHashCollisionProbabilityBound() {  // Empirically verify collision probability is small (deterministic simulation).
    int m = 50;  // Bucket count for probability bound 1/m.
    int trials = 5000;  // Moderate trial count to keep tests fast.
    int collisions = 0;  // Collision counter.
    int x = 100;  // Key x.
    int y = 200;  // Key y (x != y).
    for (int t = 0; t < trials; t++) {  // Repeat trials with different deterministic seeds.
        hashfunctionsunit::UniversalHashFamily uh(m, static_cast<std::uint32_t>(t));  // Use seed=t to vary parameters.
        if (uh.hash(x) == uh.hash(y)) {  // Count collisions.
            collisions += 1;  // Increment collision counter.
        }  // Close collision branch.
    }  // Close loop.
    double probability = static_cast<double>(collisions) / static_cast<double>(trials);  // Compute empirical collision probability.
    assertTrue(probability < (2.0 / static_cast<double>(m)), "collision probability should be < 2/m");  // Allow variance like Python test.
}  // Close testUniversalHashCollisionProbabilityBound().

static void testUniversalStringHashFamily() {  // Verify universal string hash family range and determinism.
    int m = 100;  // Bucket count.
    hashfunctionsunit::UniversalStringHashFamily ush(m, 999u);  // Use deterministic seed for stable tests.
    for (const std::string& s : std::vector<std::string>{"hello", "world", "test", "hash"}) {  // Test sample strings.
        int h = ush.hash(s);  // Compute hash.
        assertTrue(0 <= h && h < m, "UniversalStringHashFamily hash should be in [0, m)");  // Validate range.
        assertEquals(ush.hash(s), ush.hash(s), "UniversalStringHashFamily should be deterministic within an instance");  // Validate determinism.
    }  // Close loop.
}  // Close testUniversalStringHashFamily().

static void testUniversalHashTable() {  // Verify universal hash table insert/search/delete/update.
    hashfunctionsunit::UniversalHashTable ht(16, 123u);  // Create empty table with deterministic seed.
    ht.insert(1, "one");  // Insert 1 -> "one".
    ht.insert(2, "two");  // Insert 2 -> "two".
    assertTrue(ht.search(1).has_value() && ht.search(1).value() == "one", "search(1) should return one");  // Validate search.
    assertTrue(ht.search(2).has_value() && ht.search(2).value() == "two", "search(2) should return two");  // Validate search.
    assertTrue(!ht.search(3).has_value(), "search(3) should be missing");  // Validate missing.

    ht.insert(1, "ONE");  // Update existing key.
    assertTrue(ht.search(1).has_value() && ht.search(1).value() == "ONE", "update should overwrite value");  // Validate update.
    assertEquals(2, ht.size(), "size should remain 2 after update");  // Validate size.

    assertTrue(ht.erase(1), "erase(1) should succeed");  // Delete existing.
    assertTrue(!ht.search(1).has_value(), "search(1) should be missing after erase");  // Validate deletion.
    assertTrue(!ht.erase(1), "erase(1) should return false when missing");  // Validate missing delete.
}  // Close testUniversalHashTable().

static void testUniversalHashTableManyInsertions() {  // Verify correctness after many insertions (and resizes).
    hashfunctionsunit::UniversalHashTable ht(16, 321u);  // Create empty table with deterministic seed.
    for (int i = 0; i < 100; i++) {  // Insert 100 items.
        ht.insert(i, "value_" + std::to_string(i));  // Insert i -> value_i.
    }  // Close loop.
    assertEquals(100, ht.size(), "size should be 100 after 100 inserts");  // Validate size.
    for (int i = 0; i < 100; i++) {  // Validate lookups.
        auto v = ht.search(i);  // Search key.
        assertTrue(v.has_value() && v.value() == "value_" + std::to_string(i), "search(i) should return value_i");  // Validate value.
    }  // Close loop.
}  // Close testUniversalHashTableManyInsertions().

int main() {  // Run all tests and print status.
    try {  // Catch failures and print a clean message.
        testIntegerHashFunctions();  // Run integer hash tests.
        testStringHashFunctions();  // Run string hash tests.
        testDistributionAnalyzer();  // Run distribution analyzer tests.
        testUniversalHashFamily();  // Run universal hash family tests.
        testUniversalHashCollisionProbabilityBound();  // Run collision probability bound test.
        testUniversalStringHashFamily();  // Run universal string hash tests.
        testUniversalHashTable();  // Run universal hash table tests.
        testUniversalHashTableManyInsertions();  // Run bulk insert test.
        std::cout << "All tests PASSED.\n";  // Print success.
        return 0;  // Exit success.
    } catch (const std::exception& ex) {  // Print any test failure.
        std::cerr << ex.what() << "\n";  // Print failure message.
        return 1;  // Exit failure.
    }  // Close catch.
}  // Close main().

