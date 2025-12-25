// 03 雜湊函數示範程式（C）/ Hash functions demo program (C).  // Bilingual file header.

#include "hash_functions.h"  // Use hash function APIs for this unit.
#include "universal_hashing.h"  // Use universal hashing APIs for this unit.

#include <stdio.h>  // Provide printf for console output.

static uint32_t djb2_index(const char* s, uint32_t m) {  // Adapter: turn djb2_hash into a (key, m) -> index function.
    return djb2_hash(s) % m;  // Reduce DJB2 hash mod m.
}  // Close djb2_index().

int main(void) {  // Entry point for the demo CLI.
    int test_key = 123456;  // Pick a deterministic integer key for demo output.
    int m_prime = 97;  // Use a prime bucket count for division hashing.
    printf("=== Hash Functions Demo (C) ===\n");  // Print header.
    printf("Key: %d, m: %d\n", test_key, m_prime);  // Print key summary.
    printf("  Division:       %d\n", division_hash(test_key, m_prime));  // Print division hash.
    printf("  Multiplication: %d\n", multiplication_hash(test_key, m_prime));  // Print multiplication hash.
    printf("  Mid-Square (4): %lld\n", mid_square_hash(test_key, 4));  // Print mid-square hash.
    printf("  Folding (3):    %d\n", folding_hash(test_key, 3, m_prime));  // Print folding hash.

    const char* strings[] = {"hello", "world", "hash", "table", "c"};  // Sample string list.
    int strings_len = (int)(sizeof(strings) / sizeof(strings[0]));  // Number of sample strings.
    printf("\n--- String hashes ---\n");  // Print section title.
    for (int i = 0; i < strings_len; i++) {  // Print a few hash values for each string.
        const char* s = strings[i];  // Read one string.
        printf("%s => djb2=%u, fnv1a=%u, poly(mod 1000003)=%u\n", s, djb2_hash(s), fnv1a_hash(s), polynomial_hash(s, 31u, 1000003u));  // Print one line.
    }  // Close loop.

    const char* keys[1000];  // Deterministic keys list for distribution demo.
    char buf[1000][16];  // Storage for key strings so pointers stay valid.
    for (int i = 0; i < 1000; i++) {  // Create 1000 keys "key_i".
        snprintf(buf[i], sizeof(buf[i]), "key_%d", i);  // Format one key string.
        keys[i] = buf[i];  // Store pointer to this key.
    }  // Close loop.
    int dist[100];  // Bucket distribution array.
    distribution_report report;  // Report struct to receive computed stats.
    analyze_distribution(djb2_index, keys, 1000, 100, dist, 100, &report);  // Analyze distribution for DJB2 reduced mod 100.
    printf("\n--- Distribution (DJB2, 1000 keys, m=100) ---\n");  // Print section title.
    printf("non_empty=%d, max=%d, std_dev=%.3f\n", report.non_empty_buckets, report.max_bucket_size, report.std_deviation);  // Print summary.

    universal_hash_family uh;  // Universal hash family for integer keys.
    universal_hash_family_init(&uh, 10, 0, 123u);  // Initialize with deterministic seed.
    printf("\n--- UniversalHashFamily (m=10) ---\n");  // Print section title.
    printf("hash(42)=%d, hash(137)=%d\n", universal_hash_family_hash(&uh, 42), universal_hash_family_hash(&uh, 137));  // Print example values.
    return 0;  // Exit success.
}  // Close main().

