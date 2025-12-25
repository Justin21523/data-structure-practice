// 03 雜湊函數示範（C）/ Hash functions demo (C).  // Bilingual file header.

#include "hash_functions.h"  // Include the public API for this unit.

#include <math.h>  // Provide sqrt/floor for multiplication hashing and std deviation.
#include <stddef.h>  // Provide size_t for safe indexing.
#include <stdio.h>  // Provide snprintf for mid-square digit extraction.
#include <stdlib.h>  // Provide abs for digit chunking.
#include <string.h>  // Provide strlen/memcpy for string hashing helpers.

static int positive_mod_int(long long x, int m) {  // Compute x mod m in [0, m) even when x is negative.
    int r = (int)(x % (long long)m);  // Compute remainder in C's signed remainder semantics.
    if (r < 0) {  // Adjust negative remainder to be positive.
        r += m;  // Shift into [0, m).
    }  // Close adjustment branch.
    return r;  // Return positive remainder.
}  // Close positive_mod_int().

int division_hash(int key, int m) {  // Compute division-method hash: h(k) = k mod m.
    if (m <= 0) {  // Validate bucket count to avoid divide-by-zero and invalid ranges.
        return 0;  // Return 0 as a safe fallback (tests only call with valid m).
    }  // Close validation.
    return positive_mod_int((long long)key, m);  // Use positive modulo so result is always in [0, m).
}  // Close division_hash().

int multiplication_hash(int key, int m) {  // Compute multiplication-method hash with default A.
    double A = (sqrt(5.0) - 1.0) / 2.0;  // Use golden ratio conjugate (Knuth suggestion).
    return multiplication_hash_with_a(key, m, A);  // Delegate to the explicit-A variant.
}  // Close multiplication_hash().

int multiplication_hash_with_a(int key, int m, double a) {  // Compute multiplication-method hash: floor(m * frac(k*A)).
    if (m <= 0) {  // Validate bucket count so we return a valid range.
        return 0;  // Return 0 as a safe fallback (tests only call with valid m).
    }  // Close validation.
    double product = ((double)key) * a;  // Multiply key by A in floating-point.
    double fractional = product - floor(product);  // Extract fractional part in [0, 1) (works for negative too).
    int index = (int)floor(((double)m) * fractional);  // Scale by m and take floor.
    if (index >= m) {  // Clamp for rare rounding edge cases (fractional could be ~1.0).
        index = m - 1;  // Clamp to last valid bucket.
    }  // Close clamp branch.
    if (index < 0) {  // Defensive clamp when m is valid but rounding produces negative.
        index = 0;  // Clamp to first bucket.
    }  // Close clamp branch.
    return index;  // Return bucket index.
}  // Close multiplication_hash_with_a().

long long mid_square_hash(int key, int r) {  // Compute mid-square hash by squaring and extracting middle r digits.
    if (r <= 0) {  // Validate digit count so substring extraction is meaningful.
        return 0;  // Return 0 as a safe fallback.
    }  // Close validation.
    long long squared = (long long)key * (long long)key;  // Square as long long to reduce overflow risk for moderate keys.
    char buf[64];  // Fixed buffer for decimal digits (enough for 64-bit integers).
    snprintf(buf, sizeof(buf), "%lld", squared);  // Convert squared value to a decimal string.
    int len = (int)strlen(buf);  // Compute digit string length.
    if (len < r) {  // If there are not enough digits, return the squared value directly (Python reference behavior).
        return squared;  // Return fallback result.
    }  // Close short-length branch.
    int mid = len / 2;  // Find the middle position.
    int start = mid - (r / 2);  // Choose start so we take r digits around the middle.
    int end = start + r;  // Compute exclusive end index.
    char slice[64];  // Buffer for extracted digits.
    int slice_len = 0;  // Track number of copied digits.
    for (int i = start; i < end; i++) {  // Copy the requested digit range.
        slice[slice_len++] = buf[i];  // Copy one digit.
    }  // Close copy loop.
    slice[slice_len] = '\0';  // Null-terminate extracted substring.
    return atoll(slice);  // Parse the extracted digits and return.
}  // Close mid_square_hash().

int folding_hash(int key, int chunk_size, int m) {  // Compute folding hash by chunking digits and summing.
    if (chunk_size <= 0 || m <= 0) {  // Validate inputs to avoid invalid loops and modulo.
        return 0;  // Return safe fallback.
    }  // Close validation.
    char buf[64];  // Buffer for decimal digits.
    snprintf(buf, sizeof(buf), "%d", abs(key));  // Convert absolute key to digits.
    int len = (int)strlen(buf);  // Digit string length.
    long long total = 0;  // Accumulate sum of chunks.
    for (int i = 0; i < len; i += chunk_size) {  // Walk the digit string in chunk_size steps.
        int end = i + chunk_size;  // Compute end index for this chunk.
        if (end > len) {  // Clamp the final chunk end.
            end = len;  // Use string end.
        }  // Close clamp branch.
        char chunk[64];  // Buffer for one chunk.
        int chunk_len = 0;  // Track chunk length.
        for (int j = i; j < end; j++) {  // Copy digits for this chunk.
            chunk[chunk_len++] = buf[j];  // Copy one digit.
        }  // Close chunk copy loop.
        chunk[chunk_len] = '\0';  // Null-terminate chunk string.
        total += atoll(chunk);  // Add chunk value to total.
    }  // Close chunk loop.
    return positive_mod_int(total, m);  // Reduce mod m for bucket index.
}  // Close folding_hash().

int simple_sum_hash(const char* s, int m) {  // Compute simple sum-of-chars hash reduced mod m.
    if (s == NULL || m <= 0) {  // Validate inputs to avoid null dereference and invalid modulo.
        return 0;  // Return safe fallback.
    }  // Close validation.
    long long sum = 0;  // Accumulate byte values.
    for (size_t i = 0; s[i] != '\0'; i++) {  // Iterate bytes until null terminator.
        sum += (unsigned char)s[i];  // Add byte value (treat as unsigned).
    }  // Close loop.
    return positive_mod_int(sum, m);  // Reduce mod m for bucket index.
}  // Close simple_sum_hash().

uint32_t polynomial_hash(const char* s, uint32_t a, uint32_t m) {  // Compute polynomial rolling hash reduced mod m.
    if (s == NULL || m == 0) {  // Validate inputs so modulo is defined.
        return 0;  // Return safe fallback.
    }  // Close validation.
    uint64_t h = 0;  // Use 64-bit accumulator to reduce overflow risk between mods.
    for (size_t i = 0; s[i] != '\0'; i++) {  // Iterate bytes until null terminator.
        h = h * (uint64_t)a + (uint64_t)((unsigned char)s[i]);  // Horner step: h = h*a + c.
        h %= (uint64_t)m;  // Reduce mod m to keep accumulator bounded.
    }  // Close loop.
    return (uint32_t)h;  // Return reduced result.
}  // Close polynomial_hash().

uint32_t djb2_hash(const char* s) {  // Compute DJB2 hash (32-bit unsigned).
    if (s == NULL) {  // Validate input pointer.
        return 5381u;  // Return DJB2 offset basis as a safe default.
    }  // Close validation.
    uint32_t h = 5381u;  // Initialize with DJB2 offset basis.
    for (size_t i = 0; s[i] != '\0'; i++) {  // Iterate bytes.
        h = (uint32_t)((h << 5) + h) + (uint32_t)((unsigned char)s[i]);  // Update: h = h*33 + c.
    }  // Close loop.
    return h;  // Return 32-bit hash (wrap-around is intentional).
}  // Close djb2_hash().

uint32_t fnv1a_hash(const char* s) {  // Compute FNV-1a hash (32-bit unsigned).
    const uint32_t FNV_OFFSET_BASIS = 2166136261u;  // Define FNV offset basis constant.
    const uint32_t FNV_PRIME = 16777619u;  // Define FNV prime constant.
    if (s == NULL) {  // Validate input pointer.
        return FNV_OFFSET_BASIS;  // Return offset basis as a safe default.
    }  // Close validation.
    uint32_t h = FNV_OFFSET_BASIS;  // Initialize accumulator.
    for (size_t i = 0; s[i] != '\0'; i++) {  // Iterate bytes.
        h ^= (uint32_t)((unsigned char)s[i]);  // XOR in the next byte.
        h *= FNV_PRIME;  // Multiply by FNV prime (mod 2^32 via overflow).
    }  // Close loop.
    return h;  // Return 32-bit hash.
}  // Close fnv1a_hash().

uint32_t jenkins_one_at_a_time(const char* s) {  // Compute Jenkins one-at-a-time hash (32-bit unsigned).
    if (s == NULL) {  // Validate input pointer.
        return 0u;  // Return 0 as a safe default for null.
    }  // Close validation.
    uint32_t h = 0u;  // Initialize accumulator.
    for (size_t i = 0; s[i] != '\0'; i++) {  // Iterate bytes.
        h += (uint32_t)((unsigned char)s[i]);  // Add byte value.
        h += (h << 10);  // Mix with left shift.
        h ^= (h >> 6);  // Mix with right shift.
    }  // Close loop.
    h += (h << 3);  // Final avalanche step 1.
    h ^= (h >> 11);  // Final avalanche step 2.
    h += (h << 15);  // Final avalanche step 3.
    return h;  // Return 32-bit hash.
}  // Close jenkins_one_at_a_time().

bool analyze_distribution(hash_index_func hash_func, const char** keys, int key_count, int m, int* out_distribution, int out_len, distribution_report* out_report) {  // Analyze bucket distribution for a string hash function.
    if (hash_func == NULL || keys == NULL || out_distribution == NULL || out_report == NULL) {  // Validate pointers.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (m <= 0 || out_len < m || key_count < 0) {  // Validate sizes so buffers are valid.
        return false;  // Signal invalid input.
    }  // Close validation.

    for (int i = 0; i < m; i++) {  // Clear bucket counters.
        out_distribution[i] = 0;  // Set bucket count to 0.
    }  // Close clear loop.

    for (int i = 0; i < key_count; i++) {  // Hash each key into a bucket.
        uint32_t h = hash_func(keys[i], (uint32_t)m);  // Compute bucket index for this key.
        out_distribution[(int)h] += 1;  // Increment bucket occupancy.
    }  // Close key loop.

    int non_empty = 0;  // Count non-empty buckets.
    int max_count = 0;  // Track maximum bucket size.
    int min_count = out_distribution[0];  // Track minimum bucket size (often 0).
    for (int i = 0; i < m; i++) {  // Scan bucket counts to compute summary stats.
        int b = out_distribution[i];  // Read one bucket count.
        if (b > 0) {  // Identify non-empty buckets.
            non_empty += 1;  // Increment non-empty counter.
        }  // Close non-empty branch.
        if (b > max_count) {  // Update max when needed.
            max_count = b;  // Store new maximum.
        }  // Close max update.
        if (b < min_count) {  // Update min when needed.
            min_count = b;  // Store new minimum.
        }  // Close min update.
    }  // Close scan loop.

    double avg = (m == 0) ? 0.0 : ((double)key_count / (double)m);  // Compute average bucket size (n/m).
    double variance = 0.0;  // Accumulate variance over buckets.
    for (int i = 0; i < m; i++) {  // Compute squared deviations from the mean.
        double diff = ((double)out_distribution[i]) - avg;  // Compute deviation.
        variance += diff * diff;  // Accumulate squared deviation.
    }  // Close variance loop.
    variance /= (double)m;  // Convert sum to average.
    double std_dev = sqrt(variance);  // Compute standard deviation.

    out_report->total_keys = key_count;  // Store total key count.
    out_report->buckets = m;  // Store bucket count.
    out_report->non_empty_buckets = non_empty;  // Store non-empty bucket count.
    out_report->max_bucket_size = max_count;  // Store max bucket size.
    out_report->min_bucket_size = min_count;  // Store min bucket size.
    out_report->avg_bucket_size = avg;  // Store average bucket size.
    out_report->std_deviation = std_dev;  // Store standard deviation.
    out_report->distribution = out_distribution;  // Point to caller-owned distribution buffer.
    return true;  // Report success.
}  // Close analyze_distribution().

