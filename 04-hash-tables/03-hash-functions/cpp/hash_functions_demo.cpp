// 03 雜湊函數示範程式（C++）/ Hash functions demo program (C++).  // Bilingual file header.

#include "HashFunctions.hpp"  // Import hash functions and analysis helpers.
#include "UniversalHashing.hpp"  // Import universal hashing helpers.

#include <iostream>  // Provide std::cout for console output.
#include <vector>  // Provide std::vector for demo key lists.

int main() {  // Entry point for the demo CLI.
    int testKey = 123456;  // Pick a deterministic integer key for demo output.
    int mPrime = 97;  // Use a prime bucket count for division hashing.
    std::cout << "=== Hash Functions Demo (C++) ===\n";  // Print header.
    std::cout << "Key: " << testKey << ", m: " << mPrime << "\n";  // Print key summary.
    std::cout << "  Division:       " << hashfunctionsunit::divisionHash(testKey, mPrime) << "\n";  // Print division hash.
    std::cout << "  Multiplication: " << hashfunctionsunit::multiplicationHash(testKey, mPrime) << "\n";  // Print multiplication hash.
    std::cout << "  Mid-Square (4): " << hashfunctionsunit::midSquareHash(testKey, 4) << "\n";  // Print mid-square hash.
    std::cout << "  Folding (3):    " << hashfunctionsunit::foldingHash(testKey, 3, mPrime) << "\n";  // Print folding hash.

    std::vector<std::string> strings = {"hello", "world", "hash", "table", "cpp"};  // Sample strings.
    std::cout << "\n--- String hashes ---\n";  // Print section title.
    for (const std::string& s : strings) {  // Print a few hash values for each string.
        std::uint32_t djb2 = hashfunctionsunit::djb2Hash(s);  // Compute DJB2 hash.
        std::uint32_t fnv = hashfunctionsunit::fnv1aHash(s);  // Compute FNV-1a hash.
        std::uint32_t poly = hashfunctionsunit::polynomialHash(s, 31u, 1000003u);  // Compute polynomial hash mod a prime.
        std::cout << s << " => djb2=" << djb2 << ", fnv1a=" << fnv << ", poly=" << poly << "\n";  // Print one line.
    }  // Close loop.

    std::vector<std::string> keys;  // Deterministic keys list for distribution demo.
    for (int i = 0; i < 1000; i++) {  // Create 1000 keys.
        keys.push_back("key_" + std::to_string(i));  // Append one key.
    }  // Close loop.
    auto report = hashfunctionsunit::analyzeDistribution(  // Analyze distribution for DJB2 reduced mod 100.
        [](const std::string& k, int m) { return static_cast<int>(hashfunctionsunit::djb2Hash(k) % static_cast<std::uint32_t>(m)); },  // Hash adapter.
        keys, 100  // Provide keys and bucket count.
    );  // Close call.
    std::cout << "\n--- Distribution (DJB2, 1000 keys, m=100) ---\n";  // Print section title.
    std::cout << "non_empty=" << report.nonEmptyBuckets << ", max=" << report.maxBucketSize << ", std_dev=" << report.stdDeviation << "\n";  // Print summary.

    hashfunctionsunit::UniversalHashFamily uh(10, 123u);  // Universal hash family for ints with deterministic seed.
    std::cout << "\n--- UniversalHashFamily (m=10) ---\n";  // Print section title.
    std::cout << "hash(42)=" << uh.hash(42) << ", hash(137)=" << uh.hash(137) << "\n";  // Print example values.
    return 0;  // Exit success.
}  // Close main().

