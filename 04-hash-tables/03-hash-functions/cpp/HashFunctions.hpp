// 03 雜湊函數示範（C++）/ Hash functions demo (C++).  // Bilingual header line for this unit.
#ifndef HASH_FUNCTIONS_HPP  // Header guard to prevent multiple inclusion.
#define HASH_FUNCTIONS_HPP  // Header guard definition.

#include <algorithm>  // Provide std::max and std::min for stats computations.
#include <cmath>  // Provide std::sqrt and std::floor for hashing and std deviation.
#include <cstdlib>  // Provide std::llabs for absolute value of long long.
#include <cstdint>  // Provide uint32_t/uint64_t for 32-bit hashes.
#include <limits>  // Provide std::numeric_limits for NaN default handling.
#include <stdexcept>  // Provide exceptions for validation.
#include <string>  // Provide std::string for string hashing.
#include <vector>  // Provide std::vector for distribution storage.

namespace hashfunctionsunit {  // Use a small namespace to avoid polluting the global namespace.

inline int positiveMod(long long x, int m) {  // Compute x mod m in [0, m) even when x is negative.
    int r = static_cast<int>(x % static_cast<long long>(m));  // Compute remainder with signed semantics.
    if (r < 0) {  // Adjust negative remainder to be positive.
        r += m;  // Shift into [0, m).
    }  // Close adjustment branch.
    return r;  // Return positive remainder.
}  // End positiveMod().

// ============================================================  // Section banner: integer hash functions.
// Integer Hash Functions  // Section title.
// ============================================================  // Section banner end.

inline int divisionHash(int key, int m) {  // Compute division-method hash: h(k) = k mod m.
    if (m <= 0) {  // Validate bucket count to avoid divide-by-zero and invalid ranges.
        throw std::invalid_argument("m must be >= 1");  // Signal invalid input.
    }  // Close validation.
    return positiveMod(static_cast<long long>(key), m);  // Use positive modulo so result is always in [0, m).
}  // End divisionHash().

inline int multiplicationHash(int key, int m, double A = std::numeric_limits<double>::quiet_NaN()) {  // Compute multiplication-method hash: floor(m * frac(k*A)).
    if (m <= 0) {  // Validate bucket count so we return a valid index range.
        throw std::invalid_argument("m must be >= 1");  // Signal invalid input.
    }  // Close validation.
    double aConst = std::isnan(A) ? ((std::sqrt(5.0) - 1.0) / 2.0) : A;  // Use golden ratio conjugate when A is not provided.
    double product = static_cast<double>(key) * aConst;  // Multiply key by A in floating-point.
    double fractional = product - std::floor(product);  // Extract fractional part in [0, 1) (works for negative too).
    int index = static_cast<int>(std::floor(static_cast<double>(m) * fractional));  // Scale by m and take floor.
    if (index >= m) {  // Clamp for rare rounding edge cases (fractional could be ~1.0).
        index = m - 1;  // Clamp to last valid bucket.
    }  // Close clamp branch.
    if (index < 0) {  // Defensive clamp for negative rounding edge cases.
        index = 0;  // Clamp to first bucket.
    }  // Close clamp branch.
    return index;  // Return bucket index.
}  // End multiplicationHash().

inline long long midSquareHash(int key, int r) {  // Compute mid-square hash by squaring and extracting middle r digits.
    if (r <= 0) {  // Validate digit count so substring extraction is meaningful.
        throw std::invalid_argument("r must be >= 1");  // Signal invalid input.
    }  // Close validation.
    long long squared = static_cast<long long>(key) * static_cast<long long>(key);  // Square as 64-bit integer.
    std::string s = std::to_string(squared);  // Convert squared value to decimal string.
    if (static_cast<int>(s.size()) < r) {  // If there are not enough digits, return the squared value directly.
        return squared;  // Return fallback result.
    }  // Close short-length branch.
    int mid = static_cast<int>(s.size()) / 2;  // Find the middle position.
    int start = mid - (r / 2);  // Choose start so we take r digits around the middle.
    std::string slice = s.substr(static_cast<size_t>(start), static_cast<size_t>(r));  // Extract the middle digits.
    return std::stoll(slice);  // Parse extracted digits and return.
}  // End midSquareHash().

inline int foldingHash(int key, int chunkSize, int m) {  // Compute folding hash by chunking digits and summing.
    if (chunkSize <= 0) {  // Validate chunk size to avoid infinite loops.
        throw std::invalid_argument("chunkSize must be >= 1");  // Signal invalid input.
    }  // Close validation.
    if (m <= 0) {  // Validate bucket count so we return a valid range.
        throw std::invalid_argument("m must be >= 1");  // Signal invalid input.
    }  // Close validation.
    std::string digits = std::to_string(std::llabs(static_cast<long long>(key)));  // Convert abs(key) to digits string.
    long long total = 0;  // Accumulate chunk sums.
    for (size_t i = 0; i < digits.size(); i += static_cast<size_t>(chunkSize)) {  // Walk the string in chunkSize steps.
        size_t end = std::min(i + static_cast<size_t>(chunkSize), digits.size());  // Clamp end for final chunk.
        std::string chunk = digits.substr(i, end - i);  // Extract one chunk.
        total += std::stoll(chunk);  // Add chunk value.
    }  // Close loop.
    return positiveMod(total, m);  // Reduce mod m for bucket index.
}  // End foldingHash().

// ============================================================  // Section banner: string hash functions.
// String Hash Functions  // Section title.
// ============================================================  // Section banner end.

inline int simpleSumHash(const std::string& s, int m) {  // Compute simple sum-of-chars hash reduced mod m.
    if (m <= 0) {  // Validate bucket count so we return a valid range.
        throw std::invalid_argument("m must be >= 1");  // Signal invalid input.
    }  // Close validation.
    long long sum = 0;  // Accumulate byte values (treat char as unsigned).
    for (unsigned char c : s) {  // Iterate bytes in the string.
        sum += static_cast<long long>(c);  // Add one byte value.
    }  // Close loop.
    return positiveMod(sum, m);  // Reduce mod m for bucket index.
}  // End simpleSumHash().

inline std::uint32_t polynomialHash(const std::string& s, std::uint32_t a, std::uint32_t m) {  // Compute polynomial rolling hash reduced mod m.
    if (m == 0u) {  // Validate modulo so division is defined.
        throw std::invalid_argument("m must be >= 1");  // Signal invalid input.
    }  // Close validation.
    std::uint64_t h = 0u;  // Use 64-bit accumulator to reduce overflow risk between mods.
    for (unsigned char c : s) {  // Iterate bytes.
        h = h * static_cast<std::uint64_t>(a) + static_cast<std::uint64_t>(c);  // Horner step: h = h*a + c.
        h %= static_cast<std::uint64_t>(m);  // Reduce mod m each step.
    }  // Close loop.
    return static_cast<std::uint32_t>(h);  // Return reduced value.
}  // End polynomialHash().

inline std::uint32_t djb2Hash(const std::string& s) {  // Compute DJB2 hash (32-bit unsigned).
    std::uint32_t h = 5381u;  // Initialize with DJB2 offset basis.
    for (unsigned char c : s) {  // Iterate bytes.
        h = static_cast<std::uint32_t>((h << 5) + h) + static_cast<std::uint32_t>(c);  // Update: h = h*33 + c.
    }  // Close loop.
    return h;  // Return 32-bit hash (wrap-around is intentional).
}  // End djb2Hash().

inline std::uint32_t fnv1aHash(const std::string& s) {  // Compute FNV-1a hash (32-bit unsigned).
    const std::uint32_t FNV_OFFSET_BASIS = 2166136261u;  // Define FNV offset basis constant.
    const std::uint32_t FNV_PRIME = 16777619u;  // Define FNV prime constant.
    std::uint32_t h = FNV_OFFSET_BASIS;  // Initialize accumulator.
    for (unsigned char c : s) {  // Iterate bytes.
        h ^= static_cast<std::uint32_t>(c);  // XOR in the next byte.
        h *= FNV_PRIME;  // Multiply by FNV prime (mod 2^32 via overflow).
    }  // Close loop.
    return h;  // Return 32-bit hash.
}  // End fnv1aHash().

inline std::uint32_t jenkinsOneAtATime(const std::string& s) {  // Compute Jenkins one-at-a-time hash (32-bit unsigned).
    std::uint32_t h = 0u;  // Initialize accumulator.
    for (unsigned char c : s) {  // Iterate bytes.
        h += static_cast<std::uint32_t>(c);  // Add byte value.
        h += (h << 10);  // Mix with left shift.
        h ^= (h >> 6);  // Mix with right shift.
    }  // Close loop.
    h += (h << 3);  // Final avalanche step 1.
    h ^= (h >> 11);  // Final avalanche step 2.
    h += (h << 15);  // Final avalanche step 3.
    return h;  // Return 32-bit hash.
}  // End jenkinsOneAtATime().

// ============================================================  // Section banner: distribution analysis.
// Hash Function Analysis Tools  // Section title.
// ============================================================  // Section banner end.

struct DistributionReport {  // Report distribution statistics for a set of keys.
    int totalKeys;  // Total number of keys analyzed.
    int buckets;  // Number of buckets (m).
    int nonEmptyBuckets;  // Count of buckets with at least one key.
    int maxBucketSize;  // Maximum bucket occupancy.
    int minBucketSize;  // Minimum bucket occupancy (often 0).
    double avgBucketSize;  // Average bucket occupancy (n/m).
    double stdDeviation;  // Standard deviation of bucket occupancies.
    std::vector<int> distribution;  // Raw bucket counts.
};  // End DistributionReport.

template <typename HashFunc>  // Template for generic (key, m) -> index hash functions.
inline DistributionReport analyzeDistribution(HashFunc hashFunc, const std::vector<std::string>& keys, int m) {  // Analyze bucket distribution for a string hash function.
    if (m <= 0) {  // Validate bucket count so arrays have a valid size.
        throw std::invalid_argument("m must be >= 1");  // Signal invalid input.
    }  // Close validation.

    std::vector<int> buckets(static_cast<size_t>(m), 0);  // Allocate bucket counters.
    for (const std::string& key : keys) {  // Hash each key into a bucket.
        int h = static_cast<int>(hashFunc(key, m));  // Compute bucket index for this key.
        buckets[static_cast<size_t>(h)] += 1;  // Increment bucket occupancy.
    }  // Close loop.

    int nonEmpty = 0;  // Count non-empty buckets.
    int maxCount = 0;  // Track maximum bucket size.
    int minCount = buckets[0];  // Track minimum bucket size.
    for (int b : buckets) {  // Scan bucket counts.
        if (b > 0) {  // Identify non-empty buckets.
            nonEmpty += 1;  // Increment non-empty counter.
        }  // Close non-empty branch.
        maxCount = std::max(maxCount, b);  // Update maximum.
        minCount = std::min(minCount, b);  // Update minimum.
    }  // Close scan loop.

    double avg = static_cast<double>(keys.size()) / static_cast<double>(m);  // Compute average bucket size (n/m).
    double variance = 0.0;  // Accumulate variance over buckets.
    for (int b : buckets) {  // Compute squared deviations from the mean.
        double diff = static_cast<double>(b) - avg;  // Compute deviation.
        variance += diff * diff;  // Accumulate squared deviation.
    }  // Close loop.
    variance /= static_cast<double>(m);  // Convert sum to average.
    double stdDev = std::sqrt(variance);  // Compute standard deviation.

    return DistributionReport{static_cast<int>(keys.size()), m, nonEmpty, maxCount, minCount, avg, stdDev, buckets};  // Return report.
}  // End analyzeDistribution().

}  // namespace hashfunctionsunit  // Close namespace.

#endif  // HASH_FUNCTIONS_HPP  // End of header guard.
