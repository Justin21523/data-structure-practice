/* 03 雜湊函數示範（C）/ Hash functions demo (C). */  // Bilingual header for this unit's public API.
#ifndef HASH_FUNCTIONS_H  // Header guard to prevent multiple inclusion.
#define HASH_FUNCTIONS_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure results.
#include <stdint.h>  // Provide fixed-width integer types for 32-bit hashes.

/* ============================================================ */  // Section banner: integer hash functions.
/* Integer Hash Functions */  // Section title.
/* ============================================================ */  // Section banner end.

int division_hash(int key, int m);  // Compute division-method hash: h(k) = k mod m (returns in [0, m)).
int multiplication_hash(int key, int m);  // Compute multiplication-method hash with default A (golden ratio conjugate).
int multiplication_hash_with_a(int key, int m, double a);  // Compute multiplication-method hash with explicit A.
long long mid_square_hash(int key, int r);  // Compute mid-square hash by extracting middle r digits from k^2.
int folding_hash(int key, int chunk_size, int m);  // Compute folding hash by chunking digits and summing.

/* ============================================================ */  // Section banner: string hash functions.
/* String Hash Functions */  // Section title.
/* ============================================================ */  // Section banner end.

int simple_sum_hash(const char* s, int m);  // Compute simple sum-of-chars hash reduced mod m.
uint32_t polynomial_hash(const char* s, uint32_t a, uint32_t m);  // Compute polynomial rolling hash reduced mod m.
uint32_t djb2_hash(const char* s);  // Compute DJB2 hash (32-bit unsigned).
uint32_t fnv1a_hash(const char* s);  // Compute FNV-1a hash (32-bit unsigned).
uint32_t jenkins_one_at_a_time(const char* s);  // Compute Jenkins one-at-a-time hash (32-bit unsigned).

/* ============================================================ */  // Section banner: distribution analysis.
/* Hash Function Analysis Tools */  // Section title.
/* ============================================================ */  // Section banner end.

typedef uint32_t (*hash_index_func)(const char* key, uint32_t m);  // Function pointer type for (string, m) -> bucket index.

typedef struct distribution_report {  // Report distribution statistics for a set of keys.
    int total_keys;  // Total number of keys analyzed.
    int buckets;  // Number of buckets (m).
    int non_empty_buckets;  // Count of buckets with at least one key.
    int max_bucket_size;  // Maximum bucket occupancy.
    int min_bucket_size;  // Minimum bucket occupancy (often 0).
    double avg_bucket_size;  // Average bucket occupancy (n/m).
    double std_deviation;  // Standard deviation of bucket occupancies.
    const int* distribution;  // Pointer to the bucket counts array (owned by caller).
} distribution_report;  // End of distribution_report.

bool analyze_distribution(hash_index_func hash_func, const char** keys, int key_count, int m, int* out_distribution, int out_len, distribution_report* out_report);  // Fill out_distribution and out_report; returns false on invalid input.

#endif  // HASH_FUNCTIONS_H  // End of header guard.

