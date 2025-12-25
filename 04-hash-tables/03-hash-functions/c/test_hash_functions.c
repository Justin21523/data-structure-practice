// 03 雜湊函數測試（C）/ Hash functions tests (C).  // Bilingual file header.

#include "hash_functions.h"  // Include hash function APIs under test.
#include "universal_hashing.h"  // Include universal hashing APIs under test.

#include <stdio.h>  // Provide printf for status output.
#include <stdlib.h>  // Provide exit for early termination on failure.
#include <string.h>  // Provide strcmp for string comparisons.

static void assert_true(bool condition, const char* message) {  // Minimal assertion helper.
    if (!condition) {  // Fail when condition is false.
        printf("FAIL: %s\n", message);  // Print message.
        exit(1);  // Exit with failure.
    }  // Close failure branch.
}  // Close assert_true().

static uint32_t djb2_index(const char* s, uint32_t m) {  // Adapter: turn djb2_hash into a (key, m) -> index function.
    return djb2_hash(s) % m;  // Reduce DJB2 hash mod m.
}  // Close djb2_index().

static uint32_t fnv1a_index(const char* s, uint32_t m) {  // Adapter: turn fnv1a_hash into a (key, m) -> index function.
    return fnv1a_hash(s) % m;  // Reduce FNV-1a hash mod m.
}  // Close fnv1a_index().

static void test_integer_hash_functions(void) {  // Verify basic properties for integer hash functions.
    int m1 = 97;  // Prime modulus for division hashing.
    for (int key = 0; key < 1000; key++) {  // Check a range of keys.
        int h = division_hash(key, m1);  // Compute division hash.
        assert_true(0 <= h && h < m1, "division_hash should be in [0, m)");  // Validate range.
    }  // Close loop.
    assert_true(division_hash(12345, m1) == division_hash(12345, m1), "division_hash should be deterministic");  // Validate determinism.

    int m2 = 128;  // Power-of-two modulus for multiplication hashing.
    for (int key = 0; key < 1000; key++) {  // Check a range of keys.
        int h = multiplication_hash(key, m2);  // Compute multiplication hash.
        assert_true(0 <= h && h < m2, "multiplication_hash should be in [0, m)");  // Validate range.
    }  // Close loop.
    assert_true(multiplication_hash(12345, m2) == multiplication_hash(12345, m2), "multiplication_hash should be deterministic");  // Validate determinism.

    long long mid = mid_square_hash(1234, 4);  // Compute mid-square hash for known example.
    assert_true(mid >= 0, "mid_square_hash should be non-negative");  // Validate non-negativity.
    assert_true(mid == 5227, "mid_square_hash(1234,4) should match reference digits");  // Validate expected extraction.

    int fold = folding_hash(123456789, 3, 100);  // Compute folding hash.
    assert_true(0 <= fold && fold < 100, "folding_hash should be in [0, m)");  // Validate range.
}  // Close test_integer_hash_functions().

static void test_string_hash_functions(void) {  // Verify basic properties for string hash functions.
    int m = 100;  // Bucket count for modulo-based hashes.
    const char* samples[] = {"hello", "world", "test", "hash"};  // Sample strings.
    int samples_len = (int)(sizeof(samples) / sizeof(samples[0]));  // Number of samples.
    for (int i = 0; i < samples_len; i++) {  // Check each sample string.
        int h = simple_sum_hash(samples[i], m);  // Compute simple sum hash.
        assert_true(0 <= h && h < m, "simple_sum_hash should be in [0, m)");  // Validate range.
        uint32_t poly = polynomial_hash(samples[i], 31u, 1000u);  // Compute polynomial hash reduced mod 1000.
        assert_true(poly < 1000u, "polynomial_hash should be in [0, m)");  // Validate range.
        assert_true(djb2_hash(samples[i]) == djb2_hash(samples[i]), "djb2_hash should be deterministic");  // Validate determinism.
        assert_true(fnv1a_hash(samples[i]) == fnv1a_hash(samples[i]), "fnv1a_hash should be deterministic");  // Validate determinism.
        assert_true(jenkins_one_at_a_time(samples[i]) == jenkins_one_at_a_time(samples[i]), "jenkins_one_at_a_time should be deterministic");  // Validate determinism.
    }  // Close loop.

    int m_big = 1000;  // Larger modulus for anagram collision tests.
    int h1 = simple_sum_hash("abc", m_big);  // Hash "abc".
    int h2 = simple_sum_hash("bca", m_big);  // Hash "bca".
    int h3 = simple_sum_hash("cab", m_big);  // Hash "cab".
    assert_true(h1 == h2 && h2 == h3, "simple_sum_hash should collide for anagrams");  // Validate expected collision.

    uint32_t d1 = djb2_hash("abc");  // Hash "abc" with DJB2.
    uint32_t d2 = djb2_hash("bca");  // Hash "bca" with DJB2.
    uint32_t d3 = djb2_hash("cab");  // Hash "cab" with DJB2.
    assert_true(d1 != d2 && d2 != d3 && d1 != d3, "djb2_hash should not collide for these anagrams");  // Validate non-collision.

    assert_true(djb2_hash("") == 5381u, "djb2_hash(\"\") should equal 5381");  // Validate DJB2 empty-string base.
    assert_true(polynomial_hash("", 31u, 100u) == 0u, "polynomial_hash(\"\") should equal 0");  // Validate polynomial empty-string base.
    assert_true(fnv1a_hash("test string") <= 0xFFFFFFFFu, "fnv1a_hash should be 32-bit");  // Validate 32-bit range.
}  // Close test_string_hash_functions().

static void test_distribution_analyzer(void) {  // Verify distribution analyzer returns reasonable stats for common hashes.
    const int key_count = 1000;  // Number of keys to analyze.
    const int m = 100;  // Bucket count for distribution test.
    static char buf[1000][24];  // Storage for keys so pointers remain valid.
    static const char* keys[1000];  // Pointer list.
    for (int i = 0; i < key_count; i++) {  // Build deterministic keys "key_i".
        snprintf(buf[i], sizeof(buf[i]), "key_%d", i);  // Format one key.
        keys[i] = buf[i];  // Store pointer.
    }  // Close loop.

    int dist1[m];  // Distribution buffer for DJB2.
    distribution_report r1;  // Report struct.
    assert_true(analyze_distribution(djb2_index, keys, key_count, m, dist1, m, &r1), "analyze_distribution should succeed (djb2)");  // Run analysis.
    assert_true(r1.non_empty_buckets > 60, "djb2 should fill most buckets for sequential keys");  // Loose non-empty check (avoid flakiness).
    assert_true(r1.std_deviation < 10.0, "djb2 std deviation should be reasonably small");  // Loose std-dev check.

    int dist2[m];  // Distribution buffer for FNV-1a.
    distribution_report r2;  // Report struct.
    assert_true(analyze_distribution(fnv1a_index, keys, key_count, m, dist2, m, &r2), "analyze_distribution should succeed (fnv1a)");  // Run analysis.
    assert_true(r2.std_deviation < 10.0, "fnv1a std deviation should be reasonably small");  // Loose std-dev check.
}  // Close test_distribution_analyzer().

static void test_universal_hash_family(void) {  // Verify universal hash family properties.
    int m = 100;  // Bucket count.
    universal_hash_family uh;  // Hash family instance.
    universal_hash_family_init(&uh, m, 0, 123u);  // Initialize with deterministic seed.
    for (int key = 0; key < 1000; key++) {  // Check a range of keys.
        int h = universal_hash_family_hash(&uh, key);  // Compute hash value.
        assert_true(0 <= h && h < m, "universal_hash_family_hash should be in [0, m)");  // Validate range.
    }  // Close loop.
    int key = 12345;  // One representative key.
    int h1 = universal_hash_family_hash(&uh, key);  // Hash once.
    int h2 = universal_hash_family_hash(&uh, key);  // Hash again.
    assert_true(h1 == h2, "universal_hash_family_hash should be deterministic within an instance");  // Validate determinism.

    int results[10];  // Store 10 hashes across regenerations.
    for (int i = 0; i < 10; i++) {  // Repeat regenerations.
        results[i] = universal_hash_family_hash(&uh, key);  // Record hash.
        universal_hash_family_regenerate(&uh);  // Regenerate parameters.
    }  // Close loop.
    int distinct = 0;  // Count distinct values (simple O(n^2) check for small n).
    for (int i = 0; i < 10; i++) {  // Check each result.
        bool seen = false;  // Track whether results[i] was seen before.
        for (int j = 0; j < i; j++) {  // Compare with earlier results.
            if (results[j] == results[i]) {  // Detect duplicate.
                seen = true;  // Mark as seen.
                break;  // Stop inner loop.
            }  // Close duplicate branch.
        }  // Close inner loop.
        if (!seen) {  // Count new distinct values.
            distinct += 1;  // Increment distinct count.
        }  // Close branch.
    }  // Close outer loop.
    assert_true(distinct > 1, "regenerate should change hash value sometimes");  // Very unlikely to be all identical.
}  // Close test_universal_hash_family().

static void test_universal_hash_collision_probability_bound(void) {  // Empirically verify collision probability is small (deterministic simulation).
    int m = 50;  // Bucket count for probability bound 1/m.
    int trials = 5000;  // Moderate trial count to keep tests fast.
    int collisions = 0;  // Collision counter.
    int x = 100;  // Key x.
    int y = 200;  // Key y (x != y).
    for (int t = 0; t < trials; t++) {  // Repeat trials with different deterministic seeds.
        universal_hash_family uh;  // Per-trial hash family.
        universal_hash_family_init(&uh, m, 0, (uint32_t)t);  // Use seed=t to vary parameters.
        if (universal_hash_family_hash(&uh, x) == universal_hash_family_hash(&uh, y)) {  // Count collisions.
            collisions += 1;  // Increment collision counter.
        }  // Close collision branch.
    }  // Close trial loop.
    double probability = (double)collisions / (double)trials;  // Compute empirical collision probability.
    assert_true(probability < (2.0 / (double)m), "collision probability should be < 2/m");  // Allow variance like Python test.
}  // Close test_universal_hash_collision_probability_bound().

static void test_universal_string_hash_family(void) {  // Verify universal string hash family range and determinism.
    int m = 100;  // Bucket count.
    universal_string_hash_family ush;  // String hash family instance.
    universal_string_hash_family_init(&ush, m, 0, 999u);  // Initialize with deterministic seed.
    const char* samples[] = {"hello", "world", "test", "hash"};  // Sample strings.
    int samples_len = (int)(sizeof(samples) / sizeof(samples[0]));  // Number of samples.
    for (int i = 0; i < samples_len; i++) {  // Check each sample.
        int h = universal_string_hash_family_hash(&ush, samples[i]);  // Compute hash.
        assert_true(0 <= h && h < m, "universal_string_hash_family_hash should be in [0, m)");  // Validate range.
        assert_true(h == universal_string_hash_family_hash(&ush, samples[i]), "universal_string_hash_family_hash should be deterministic within instance");  // Validate determinism.
    }  // Close loop.
}  // Close test_universal_string_hash_family().

static void test_universal_hash_table(void) {  // Verify universal hash table insert/search/delete/update.
    universal_hash_table ht;  // Table instance.
    assert_true(universal_hash_table_init(&ht, 16, 123u), "universal_hash_table_init should succeed");  // Initialize table.

    assert_true(universal_hash_table_insert(&ht, 1, "one"), "insert(1,one) should succeed");  // Insert 1.
    assert_true(universal_hash_table_insert(&ht, 2, "two"), "insert(2,two) should succeed");  // Insert 2.
    assert_true(strcmp(universal_hash_table_search(&ht, 1), "one") == 0, "search(1) should return one");  // Validate search.
    assert_true(strcmp(universal_hash_table_search(&ht, 2), "two") == 0, "search(2) should return two");  // Validate search.
    assert_true(universal_hash_table_search(&ht, 3) == NULL, "search(3) should return NULL");  // Validate missing.

    assert_true(universal_hash_table_insert(&ht, 1, "ONE"), "update insert(1,ONE) should succeed");  // Update 1.
    assert_true(strcmp(universal_hash_table_search(&ht, 1), "ONE") == 0, "update should overwrite value");  // Validate update.
    assert_true(ht.size == 2, "size should remain 2 after update");  // Validate size.

    assert_true(universal_hash_table_delete(&ht, 1), "delete(1) should succeed");  // Delete existing.
    assert_true(universal_hash_table_search(&ht, 1) == NULL, "search(1) should be NULL after delete");  // Validate deletion.
    assert_true(!universal_hash_table_delete(&ht, 1), "delete(1) should return false when missing");  // Validate missing delete.

    universal_hash_table_destroy(&ht);  // Free resources.
}  // Close test_universal_hash_table().

static void test_universal_hash_table_many_insertions(void) {  // Verify correctness after many insertions (and resizes).
    universal_hash_table ht;  // Table instance.
    assert_true(universal_hash_table_init(&ht, 16, 321u), "universal_hash_table_init should succeed");  // Initialize table.

    char value[32];  // Buffer for formatted values.
    for (int i = 0; i < 100; i++) {  // Insert 100 items.
        snprintf(value, sizeof(value), "value_%d", i);  // Format value string.
        assert_true(universal_hash_table_insert(&ht, i, value), "bulk insert should succeed");  // Insert i -> value_i.
    }  // Close loop.
    assert_true(ht.size == 100, "size should be 100 after 100 inserts");  // Validate size.

    for (int i = 0; i < 100; i++) {  // Validate lookups.
        snprintf(value, sizeof(value), "value_%d", i);  // Format expected value.
        const char* got = universal_hash_table_search(&ht, i);  // Search key.
        assert_true(got != NULL && strcmp(got, value) == 0, "search(i) should return value_i");  // Validate stored value.
    }  // Close loop.

    universal_hash_table_destroy(&ht);  // Free resources.
}  // Close test_universal_hash_table_many_insertions().

int main(void) {  // Run all tests and print status.
    printf("=== Hash Functions Tests (C) ===\n");  // Print header.
    test_integer_hash_functions();  // Run integer hash tests.
    test_string_hash_functions();  // Run string hash tests.
    test_distribution_analyzer();  // Run distribution analyzer tests.
    test_universal_hash_family();  // Run universal hash family tests.
    test_universal_hash_collision_probability_bound();  // Run collision probability bound test.
    test_universal_string_hash_family();  // Run universal string hash family tests.
    test_universal_hash_table();  // Run universal hash table tests.
    test_universal_hash_table_many_insertions();  // Run bulk insert test.
    printf("================================\n");  // Print footer separator.
    printf("All tests PASSED.\n");  // Print success message.
    return 0;  // Exit success.
}  // Close main().
