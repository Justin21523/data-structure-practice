// 04 雜湊函數測試（Java）/ Hash functions tests (Java).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList to build deterministic test input lists.  
import java.util.List;  // Use List for typed collections in tests.  

public final class HashFunctionsTest {  // Provide a minimal test runner without external dependencies.  
    private HashFunctionsTest() {  // Prevent instantiation because this class only hosts static test code.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    private static void assertTrue(boolean condition, String message) {  // Minimal boolean assertion helper.  
        if (!condition) {  // Fail when the condition is false.  
            throw new IllegalStateException("FAIL: " + message);  // Throw to signal test failure.  
        }  // Close failure branch.  
    }  // Close assertTrue().  

    private static void assertEquals(long expected, long actual, String message) {  // Minimal equality assertion helper.  
        if (expected != actual) {  // Fail when values differ.  
            throw new IllegalStateException(message + " (expected=" + expected + ", actual=" + actual + ")");  // Throw mismatch.  
        }  // Close mismatch branch.  
    }  // Close assertEquals().  

    private static void testDivisionHashRangeAndDeterminism() {  // Verify division hashing stays in range and is deterministic.  
        int m = 97;  // Use a prime modulus for division hashing.  
        for (int key = 0; key < 1000; key++) {  // Check a range of keys.  
            int h = HashFunctions.divisionHash(key, m);  // Compute hash value.  
            assertTrue(0 <= h && h < m, "divisionHash should be in [0, m)");  // Validate range.  
        }  // Close loop.  
        int key = 12345;  // Choose one representative key.  
        int h1 = HashFunctions.divisionHash(key, m);  // Hash once.  
        int h2 = HashFunctions.divisionHash(key, m);  // Hash again.  
        assertEquals(h1, h2, "divisionHash should be deterministic");  // Validate determinism.  
    }  // Close testDivisionHashRangeAndDeterminism().  

    private static void testMultiplicationHashRangeAndDeterminism() {  // Verify multiplication hashing stays in range and is deterministic.  
        int m = 128;  // Use a power-of-two bucket count (common for multiplication hashing).  
        for (int key = 0; key < 1000; key++) {  // Check a range of keys.  
            int h = HashFunctions.multiplicationHash(key, m);  // Compute hash value.  
            assertTrue(0 <= h && h < m, "multiplicationHash should be in [0, m)");  // Validate range.  
        }  // Close loop.  
        int key = 12345;  // Choose one representative key.  
        int h1 = HashFunctions.multiplicationHash(key, m);  // Hash once.  
        int h2 = HashFunctions.multiplicationHash(key, m);  // Hash again.  
        assertEquals(h1, h2, "multiplicationHash should be deterministic");  // Validate determinism.  
    }  // Close testMultiplicationHashRangeAndDeterminism().  

    private static void testMidSquareHash() {  // Verify mid-square produces a non-negative integer result.  
        long h = HashFunctions.midSquareHash(1234, 4);  // Compute mid-square hash for a known example.  
        assertTrue(h >= 0, "midSquareHash should be non-negative");  // Validate non-negativity.  
        assertEquals(5227L, h, "midSquareHash(1234,4) should match the reference extraction");  // Validate expected digits.  
    }  // Close testMidSquareHash().  

    private static void testFoldingHashRange() {  // Verify folding hash stays in range.  
        int m = 100;  // Choose a small bucket count for range testing.  
        int key = 123456789;  // Use a multi-digit key to ensure chunking happens.  
        int h = HashFunctions.foldingHash(key, 3, m);  // Compute folding hash.  
        assertTrue(0 <= h && h < m, "foldingHash should be in [0, m)");  // Validate range.  
    }  // Close testFoldingHashRange().  

    private static void testStringHashFunctions() {  // Verify basic properties for string hash functions.  
        int m = 100;  // Choose bucket count for modulo-based string hashes.  
        for (String s : new String[] { "hello", "world", "test", "hash" }) {  // Test a small set of strings.  
            int sum = HashFunctions.simpleSumHash(s, m);  // Compute simple sum hash.  
            assertTrue(0 <= sum && sum < m, "simpleSumHash should be in [0, m)");  // Validate range.  
            long poly1 = HashFunctions.polynomialHash(s, 31, m);  // Compute polynomial hash with modulus.  
            long poly2 = HashFunctions.polynomialHash(s, 31, m);  // Compute again.  
            assertEquals(poly1, poly2, "polynomialHash should be deterministic within same parameters");  // Validate determinism.  
            long djb2 = HashFunctions.djb2Hash(s);  // Compute DJB2 hash.  
            long fnv = HashFunctions.fnv1aHash(s);  // Compute FNV-1a hash.  
            long jen = HashFunctions.jenkinsOneAtATime(s);  // Compute Jenkins hash.  
            assertTrue(djb2 >= 0 && djb2 <= 0xFFFFFFFFL, "djb2Hash should be 32-bit unsigned");  // Validate range.  
            assertTrue(fnv >= 0 && fnv <= 0xFFFFFFFFL, "fnv1aHash should be 32-bit unsigned");  // Validate range.  
            assertTrue(jen >= 0 && jen <= 0xFFFFFFFFL, "jenkinsOneAtATime should be 32-bit unsigned");  // Validate range.  
        }  // Close loop.  
    }  // Close testStringHashFunctions().  

    private static void testAnalyzeDistribution() {  // Verify distribution analyzer returns consistent stats.  
        List<String> keys = new ArrayList<>();  // Build a deterministic set of keys.  
        for (int i = 0; i < 1000; i++) {  // Generate 1000 keys.  
            keys.add("key_" + i);  // Add one key string.  
        }  // Close loop.  
        int m = 128;  // Choose bucket count.  
        HashFunctions.DistributionReport r = HashFunctions.analyzeDistribution(  // Analyze distribution for DJB2 reduced mod m.  
            (k, buckets) -> (int)(HashFunctions.djb2Hash(k) % (long)buckets),  // Provide a (key, m) -> index function.  
            keys,  // Provide keys list.  
            m  // Provide bucket count.  
        );  // Close call.  
        assertEquals(1000, r.totalKeys, "totalKeys should match input size");  // Validate total key count.  
        assertEquals(m, r.buckets, "buckets should match m");  // Validate bucket count.  
        assertTrue(r.distribution.length == m, "distribution length should be m");  // Validate distribution length.  
        assertTrue(r.nonEmptyBuckets <= m, "nonEmptyBuckets should be <= m");  // Validate bounds.  
        assertTrue(r.maxBucketSize >= r.minBucketSize, "maxBucketSize should be >= minBucketSize");  // Validate ordering.  
        assertTrue(r.stdDeviation >= 0.0, "stdDeviation should be non-negative");  // Validate std deviation.  
    }  // Close testAnalyzeDistribution().  

    private static void testUniversalHashFamilyBasics() {  // Verify universal hash family properties (range, determinism).  
        int m = 100;  // Choose bucket count.  
        UniversalHashing.UniversalHashFamily uh = new UniversalHashing.UniversalHashFamily(m, 123L);  // Use deterministic seed for stable tests.  
        for (int key = 0; key < 1000; key++) {  // Check a range of keys.  
            int h = uh.hash(key);  // Compute hash value.  
            assertTrue(0 <= h && h < m, "UniversalHashFamily hash should be in [0, m)");  // Validate range.  
        }  // Close loop.  
        int key = 12345;  // Choose one key for determinism check.  
        int h1 = uh.hash(key);  // Hash once.  
        int h2 = uh.hash(key);  // Hash again.  
        assertEquals(h1, h2, "UniversalHashFamily should be deterministic within an instance");  // Validate determinism.  
    }  // Close testUniversalHashFamilyBasics().  

    private static void testUniversalHashFamilyCollisionProbabilityBound() {  // Empirically verify collision probability is small (deterministic simulation).  
        int m = 100;  // Bucket count for probability bound 1/m.  
        int trials = 5000;  // Use a moderate number of trials to keep tests fast.  
        int collisions = 0;  // Count collisions across trials.  
        int x = 42;  // Fixed key x.  
        int y = 137;  // Fixed key y (x != y).  
        for (int t = 0; t < trials; t++) {  // Repeat trials with different deterministic seeds.  
            UniversalHashing.UniversalHashFamily uh = new UniversalHashing.UniversalHashFamily(m, (long)t);  // Use seed=t to vary parameters.  
            if (uh.hash(x) == uh.hash(y)) {  // Count collisions.  
                collisions += 1;  // Increment collision counter.  
            }  // Close collision branch.  
        }  // Close trial loop.  
        double probability = ((double)collisions) / ((double)trials);  // Compute empirical collision probability.  
        assertTrue(probability < (2.0 / (double)m), "collision probability should be < 2/m");  // Allow variance like the Python test.  
    }  // Close testUniversalHashFamilyCollisionProbabilityBound().  

    private static void testUniversalStringHashFamilyBasics() {  // Verify universal string hash family range and determinism.  
        int m = 100;  // Bucket count for range checks.  
        UniversalHashing.UniversalStringHashFamily ush = new UniversalHashing.UniversalStringHashFamily(m, 999L);  // Use deterministic seed for stable tests.  
        for (String s : new String[] { "hello", "world", "test", "hash" }) {  // Test a small set of strings.  
            int h = ush.hash(s);  // Compute hash value.  
            assertTrue(0 <= h && h < m, "UniversalStringHashFamily hash should be in [0, m)");  // Validate range.  
            int h1 = ush.hash(s);  // Compute again.  
            int h2 = ush.hash(s);  // Compute again.  
            assertEquals(h1, h2, "UniversalStringHashFamily should be deterministic within an instance");  // Validate determinism.  
        }  // Close loop.  
    }  // Close testUniversalStringHashFamilyBasics().  

    private static void testUniversalHashTableOperations() {  // Verify insert/search/delete semantics for the universal hash table.  
        UniversalHashing.UniversalHashTable<String> ht = new UniversalHashing.UniversalHashTable<>();  // Create empty table.  
        ht.insert(1, "one");  // Insert 1 -> "one".  
        ht.insert(2, "two");  // Insert 2 -> "two".  
        assertTrue("one".equals(ht.search(1)), "search(1) should return \"one\"");  // Validate search.  
        assertTrue("two".equals(ht.search(2)), "search(2) should return \"two\"");  // Validate search.  
        assertTrue(ht.search(3) == null, "search(3) should return null");  // Validate missing.  

        ht.insert(1, "ONE");  // Update existing key 1.  
        assertTrue("ONE".equals(ht.search(1)), "update should overwrite value");  // Validate update.  
        assertEquals(2, ht.size(), "size should remain 2 after update");  // Validate size.  

        assertTrue(ht.delete(1), "delete(1) should return true");  // Validate delete success.  
        assertTrue(ht.search(1) == null, "search(1) should be null after delete");  // Validate removal.  
        assertTrue(!ht.delete(1), "delete(1) should return false when missing");  // Validate delete miss.  
    }  // Close testUniversalHashTableOperations().  

    private static void testUniversalHashTableManyInsertions() {  // Verify correctness under many insertions (and possible resizes).  
        UniversalHashing.UniversalHashTable<String> ht = new UniversalHashing.UniversalHashTable<>();  // Create empty table.  
        for (int i = 0; i < 100; i++) {  // Insert 100 items.  
            ht.insert(i, "value_" + i);  // Insert i -> "value_i".  
        }  // Close loop.  
        assertEquals(100, ht.size(), "size should be 100 after 100 inserts");  // Validate size.  
        for (int i = 0; i < 100; i++) {  // Validate all lookups.  
            String v = ht.search(i);  // Read value.  
            assertTrue(("value_" + i).equals(v), "search(i) should return value_i");  // Validate value.  
        }  // Close loop.  
    }  // Close testUniversalHashTableManyInsertions().  

    public static void main(String[] args) {  // Run all tests and print a concise success message.  
        testDivisionHashRangeAndDeterminism();  // Run division hash tests.  
        testMultiplicationHashRangeAndDeterminism();  // Run multiplication hash tests.  
        testMidSquareHash();  // Run mid-square test.  
        testFoldingHashRange();  // Run folding hash test.  
        testStringHashFunctions();  // Run string hash tests.  
        testAnalyzeDistribution();  // Run distribution analyzer tests.  
        testUniversalHashFamilyBasics();  // Run universal hash family basic tests.  
        testUniversalHashFamilyCollisionProbabilityBound();  // Run empirical collision bound test.  
        testUniversalStringHashFamilyBasics();  // Run universal string hash tests.  
        testUniversalHashTableOperations();  // Run universal hash table operations tests.  
        testUniversalHashTableManyInsertions();  // Run universal hash table bulk test.  
        System.out.println("All tests PASSED.");  // Report success.  
    }  // Close main().  
}  // End HashFunctionsTest.  

