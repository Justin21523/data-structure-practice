// 03 通用雜湊示範（C++）/ Universal hashing demo (C++).  // Bilingual header line for this unit.
#ifndef UNIVERSAL_HASHING_HPP  // Header guard to prevent multiple inclusion.
#define UNIVERSAL_HASHING_HPP  // Header guard definition.

#include <algorithm>  // Provide std::max for chain length scanning.
#include <cstdint>  // Provide fixed-width integer types.
#include <cmath>  // Provide std::sqrt for prime checking.
#include <optional>  // Provide std::optional for search results.
#include <random>  // Provide std::mt19937 for deterministic RNG.
#include <stdexcept>  // Provide exceptions for validation.
#include <string>  // Provide std::string for values.
#include <utility>  // Provide std::pair for bucket entries.
#include <vector>  // Provide std::vector for bucket storage.

namespace hashfunctionsunit {  // Use the same namespace as HashFunctions.hpp for this unit.

inline bool isPrime(int n) {  // Check whether n is prime (trial division).
    if (n < 2) {  // Handle 0, 1, and negatives.
        return false;  // Not prime.
    }  // Close base-case branch.
    if (n == 2) {  // Handle the only even prime.
        return true;  // 2 is prime.
    }  // Close base-case branch.
    if (n % 2 == 0) {  // Reject other even numbers quickly.
        return false;  // Not prime.
    }  // Close even branch.
    int limit = static_cast<int>(std::floor(std::sqrt(static_cast<double>(n))));  // Compute sqrt limit.
    for (int i = 3; i <= limit; i += 2) {  // Try odd divisors only.
        if (n % i == 0) {  // Detect divisor.
            return false;  // Not prime.
        }  // Close divisor branch.
    }  // Close loop.
    return true;  // No divisors found => prime.
}  // End isPrime().

inline int nextPrime(int n) {  // Find the smallest prime >= n.
    int x = (n < 2) ? 2 : n;  // Ensure we start at least from 2.
    while (!isPrime(x)) {  // Repeat until prime.
        x += 1;  // Move to next candidate.
    }  // Close loop.
    return x;  // Return prime candidate.
}  // End nextPrime().

class UniversalHashFamily {  // Represent h_{a,b}(k) = ((a*k + b) mod p) mod m.
public:
    explicit UniversalHashFamily(int m, std::uint32_t seed = 0u, std::optional<int> p = std::nullopt)  // Construct family with m and optional seed/p.
        : m_(m),  // Store bucket count.
          p_(p.has_value() ? p.value() : nextPrime(std::max(m * 10, 10007))),  // Choose default p similar to Python reference.
          rng_(seed),  // Seed RNG (deterministic when seed is fixed).
          a_(1),  // Initialize a (will be replaced in regenerate()).
          b_(0) {  // Initialize b.
        if (m_ <= 0) {  // Validate bucket count.
            throw std::invalid_argument("m must be >= 1");  // Signal invalid input.
        }  // Close validation.
        regenerate();  // Choose initial parameters.
    }  // Close constructor.

    void regenerate() {  // Choose new random parameters (a, b).
        std::uniform_int_distribution<int> distA(1, p_ - 1);  // Distribution for a in [1, p-1].
        std::uniform_int_distribution<int> distB(0, p_ - 1);  // Distribution for b in [0, p-1].
        a_ = distA(rng_);  // Sample new a.
        b_ = distB(rng_);  // Sample new b.
    }  // End regenerate().

    int hash(int key) const {  // Compute hash value in [0, m).
        long long linear = static_cast<long long>(a_) * static_cast<long long>(key) + static_cast<long long>(b_);  // Compute a*k + b in 64-bit.
        int modP = static_cast<int>(linear % static_cast<long long>(p_));  // Reduce mod p.
        if (modP < 0) {  // Adjust negative remainder.
            modP += p_;  // Shift into [0, p).
        }  // Close adjustment branch.
        return modP % m_;  // Reduce mod m for bucket index.
    }  // End hash().

    int m() const {  // Expose m for inspection.
        return m_;  // Return bucket count.
    }  // End m().

private:
    int m_;  // Bucket count.
    int p_;  // Prime modulus.
    mutable std::mt19937 rng_;  // RNG for regenerate() (mutable to allow regeneration).
    int a_;  // Multiplier parameter.
    int b_;  // Offset parameter.
};  // End UniversalHashFamily.

class UniversalStringHashFamily {  // Represent a polynomial universal-ish hash family for strings.
public:
    explicit UniversalStringHashFamily(int m, std::uint32_t seed = 0u, std::optional<int> p = std::nullopt)  // Construct family with m and optional seed/p.
        : m_(m),  // Store bucket count.
          p_(p.has_value() ? p.value() : nextPrime(std::max(m * 10, 10007))),  // Choose default p similar to Python reference.
          rng_(seed),  // Seed RNG (deterministic when seed is fixed).
          a_(1) {  // Initialize a.
        if (m_ <= 0) {  // Validate bucket count.
            throw std::invalid_argument("m must be >= 1");  // Signal invalid input.
        }  // Close validation.
        regenerate();  // Choose initial base parameter.
    }  // Close constructor.

    void regenerate() {  // Choose a new random base a.
        std::uniform_int_distribution<int> distA(1, p_ - 1);  // Distribution for a in [1, p-1].
        a_ = distA(rng_);  // Sample new a.
    }  // End regenerate().

    int hash(const std::string& s) const {  // Compute string hash value in [0, m).
        long long h = 0;  // Initialize polynomial accumulator.
        for (unsigned char c : s) {  // Iterate bytes.
            h = (h * static_cast<long long>(a_) + static_cast<long long>(c)) % static_cast<long long>(p_);  // Horner step mod p.
        }  // Close loop.
        return static_cast<int>(h % static_cast<long long>(m_));  // Reduce mod m for bucket index.
    }  // End hash().

private:
    int m_;  // Bucket count.
    int p_;  // Prime modulus.
    mutable std::mt19937 rng_;  // RNG for regenerate() (mutable to allow regeneration).
    int a_;  // Base parameter.
};  // End UniversalStringHashFamily.

class UniversalHashTable {  // A simple chained hash table for int keys using universal hashing.
public:
    static constexpr double MAX_LOAD_FACTOR = 0.75;  // Resize threshold.
    static constexpr int MAX_CHAIN_LENGTH = 10;  // Chain length threshold for defensive rehash.

    explicit UniversalHashTable(int capacity = 16, std::uint32_t seed = 0u)  // Construct table with capacity and base seed.
        : capacity_(capacity),  // Store capacity.
          size_(0),  // Start empty.
          buckets_(static_cast<size_t>(capacity), std::vector<std::pair<int, std::string>>{}),  // Allocate empty bucket chains.
          seed_(seed),  // Store base seed.
          hashFamily_(capacity, seed),  // Create hash family for current capacity.
          rehashCount_(0) {  // Start with no rehashes.
        if (capacity_ <= 0) {  // Validate capacity.
            throw std::invalid_argument("capacity must be >= 1");  // Signal invalid input.
        }  // Close validation.
    }  // Close constructor.

    int size() const {  // Expose current number of pairs.
        return size_;  // Return size.
    }  // End size().

    int capacity() const {  // Expose current bucket array length.
        return capacity_;  // Return capacity.
    }  // End capacity().

    int rehashCount() const {  // Expose number of defensive rehashes.
        return rehashCount_;  // Return rehash count.
    }  // End rehashCount().

    double loadFactor() const {  // Compute alpha = size / capacity.
        return static_cast<double>(size_) / static_cast<double>(capacity_);  // Return load factor.
    }  // End loadFactor().

    void insert(int key, const std::string& value) {  // Insert or update key->value.
        int index = hashFamily_.hash(key);  // Compute bucket index.
        auto& bucket = buckets_[static_cast<size_t>(index)];  // Reference bucket chain.

        for (auto& kv : bucket) {  // Search for existing key.
            if (kv.first == key) {  // Update existing key.
                kv.second = value;  // Overwrite value.
                return;  // Size does not change for updates.
            }  // Close match branch.
        }  // Close scan loop.

        bucket.push_back(std::make_pair(key, value));  // Append new entry.
        size_ += 1;  // Increase size.

        if (loadFactor() > MAX_LOAD_FACTOR) {  // Resize when load factor is too high.
            resize();  // Rehash into larger table.
        } else if (static_cast<int>(bucket.size()) > MAX_CHAIN_LENGTH) {  // Regenerate when chain becomes suspiciously long.
            regenerateHash();  // Defensive rehash.
        }  // Close defense branch.
    }  // End insert().

    std::optional<std::string> search(int key) const {  // Search key and return value (nullopt when missing).
        int index = hashFamily_.hash(key);  // Compute bucket index.
        const auto& bucket = buckets_[static_cast<size_t>(index)];  // Reference bucket chain.
        for (const auto& kv : bucket) {  // Scan chain.
            if (kv.first == key) {  // Match found.
                return kv.second;  // Return stored value.
            }  // Close match branch.
        }  // Close scan loop.
        return std::nullopt;  // Not found.
    }  // End search().

    bool erase(int key) {  // Delete key; return true if removed.
        int index = hashFamily_.hash(key);  // Compute bucket index.
        auto& bucket = buckets_[static_cast<size_t>(index)];  // Reference bucket chain.
        for (size_t i = 0; i < bucket.size(); i++) {  // Scan chain for key.
            if (bucket[i].first == key) {  // Match found.
                bucket.erase(bucket.begin() + static_cast<std::ptrdiff_t>(i));  // Remove entry.
                size_ -= 1;  // Decrease size.
                return true;  // Report success.
            }  // Close match branch.
        }  // Close loop.
        return false;  // Not found.
    }  // End erase().

    int getMaxChainLength() const {  // Compute maximum chain length across buckets.
        int maxLen = 0;  // Track maximum chain length.
        for (const auto& bucket : buckets_) {  // Scan all buckets.
            maxLen = std::max(maxLen, static_cast<int>(bucket.size()));  // Update max.
        }  // Close scan loop.
        return maxLen;  // Return maximum chain length.
    }  // End getMaxChainLength().

private:
    int capacity_;  // Number of buckets.
    int size_;  // Number of stored entries.
    std::vector<std::vector<std::pair<int, std::string>>> buckets_;  // Bucket array: each bucket is a vector chain.
    std::uint32_t seed_;  // Base seed for deterministic rehashing in tests.
    mutable UniversalHashFamily hashFamily_;  // Hash family for bucket indices (mutable to allow regenerate).
    int rehashCount_;  // Count of defensive rehashes.

    void resize() {  // Double capacity and reinsert all entries (rehash).
        std::vector<std::vector<std::pair<int, std::string>>> oldBuckets = buckets_;  // Copy old buckets.
        capacity_ *= 2;  // Double capacity.
        buckets_.assign(static_cast<size_t>(capacity_), std::vector<std::pair<int, std::string>>{});  // Allocate new empty buckets.
        seed_ += 1u;  // Change seed so new hash family differs after resize.
        hashFamily_ = UniversalHashFamily(capacity_, seed_);  // Create new hash family for new capacity.
        size_ = 0;  // Reset size and reinsert.

        for (const auto& bucket : oldBuckets) {  // Reinsert entries into new buckets.
            for (const auto& kv : bucket) {  // Reinsert one entry.
                insert(kv.first, kv.second);  // Use insert to maintain invariants.
            }  // Close entry loop.
        }  // Close bucket loop.
    }  // End resize().

    void regenerateHash() {  // Regenerate parameters and redistribute without changing capacity.
        rehashCount_ += 1;  // Count this defensive rehash.
        std::vector<std::vector<std::pair<int, std::string>>> oldBuckets = buckets_;  // Copy old buckets.
        buckets_.assign(static_cast<size_t>(capacity_), std::vector<std::pair<int, std::string>>{});  // Allocate new empty buckets.
        hashFamily_.regenerate();  // Choose new parameters.
        size_ = 0;  // Reset size and reinsert.

        for (const auto& bucket : oldBuckets) {  // Reinsert entries.
            for (const auto& kv : bucket) {  // Reinsert one entry.
                insert(kv.first, kv.second);  // Use insert to maintain invariants.
            }  // Close entry loop.
        }  // Close bucket loop.
    }  // End regenerateHash().
};  // End UniversalHashTable.

}  // namespace hashfunctionsunit  // Close namespace.

#endif  // UNIVERSAL_HASHING_HPP  // End of header guard.

