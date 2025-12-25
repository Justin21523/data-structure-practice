/* 03 通用雜湊示範（C）/ Universal hashing demo (C). */  // Bilingual header for this unit's public API.
#ifndef UNIVERSAL_HASHING_H  // Header guard to prevent multiple inclusion.
#define UNIVERSAL_HASHING_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure results.
#include <stdint.h>  // Provide fixed-width integer types for deterministic RNG and 32-bit math.

/* ============================================================ */  // Section banner: prime helpers.
/* Prime Helpers */  // Section title.
/* ============================================================ */  // Section banner end.

bool is_prime_int(int n);  // Check whether n is prime (trial division).
int next_prime_int(int n);  // Find the smallest prime >= n.

/* ============================================================ */  // Section banner: deterministic RNG (LCG) for stable tests.
/* Deterministic RNG (LCG) */  // Section title.
/* ============================================================ */  // Section banner end.

typedef struct lcg_rng {  // Tiny deterministic RNG so tests do not depend on libc rand().
    uint32_t state;  // 32-bit internal state.
} lcg_rng;  // End lcg_rng.

void lcg_init(lcg_rng* rng, uint32_t seed);  // Initialize RNG state from a seed.
uint32_t lcg_next_u32(lcg_rng* rng);  // Generate next uint32 value.
uint32_t lcg_next_in_range(lcg_rng* rng, uint32_t min_inclusive, uint32_t max_inclusive);  // Generate integer in [min, max] (mod bias acceptable here).

/* ============================================================ */  // Section banner: universal hash families.
/* Universal Hash Families */  // Section title.
/* ============================================================ */  // Section banner end.

typedef struct universal_hash_family {  // Represent h_{a,b}(k) = ((a*k + b) mod p) mod m.
    int m;  // Bucket count (table size).
    int p;  // Prime modulus.
    int a;  // Multiplier in [1, p-1].
    int b;  // Offset in [0, p-1].
    lcg_rng rng;  // RNG used to pick (a, b).
} universal_hash_family;  // End universal_hash_family.

void universal_hash_family_init(universal_hash_family* uh, int m, int p_or_zero, uint32_t seed);  // Initialize universal hash family (p_or_zero==0 means choose default p).
void universal_hash_family_regenerate(universal_hash_family* uh);  // Choose new random parameters (a, b).
int universal_hash_family_hash(const universal_hash_family* uh, int key);  // Compute hash value in [0, m).

typedef struct universal_string_hash_family {  // Represent a polynomial universal-ish hash family for strings.
    int m;  // Bucket count.
    int p;  // Prime modulus.
    int a;  // Base parameter in [1, p-1].
    lcg_rng rng;  // RNG used to pick a.
} universal_string_hash_family;  // End universal_string_hash_family.

void universal_string_hash_family_init(universal_string_hash_family* ush, int m, int p_or_zero, uint32_t seed);  // Initialize string hash family.
void universal_string_hash_family_regenerate(universal_string_hash_family* ush);  // Choose new random base a.
int universal_string_hash_family_hash(const universal_string_hash_family* ush, const char* s);  // Compute string hash value in [0, m).

/* ============================================================ */  // Section banner: universal hash table (chaining).
/* Universal Hash Table */  // Section title.
/* ============================================================ */  // Section banner end.

typedef struct universal_entry {  // Represent one key-value pair stored in a bucket chain.
    int key;  // Stored key.
    char* value;  // Stored value (heap-owned, null-terminated string).
    struct universal_entry* next;  // Next node in chain.
} universal_entry;  // End universal_entry.

typedef struct universal_hash_table {  // A simple chained hash table for int keys using universal hashing.
    int capacity;  // Number of buckets.
    int size;  // Number of stored entries.
    universal_entry** buckets;  // Array of bucket heads (linked lists).
    universal_hash_family hash_family;  // Hash family used to compute indices.
    int rehash_count;  // How many times we regenerated hash parameters due to long chains.
    uint32_t seed;  // Base seed used to create new hash families on resize.
} universal_hash_table;  // End universal_hash_table.

bool universal_hash_table_init(universal_hash_table* ht, int capacity, uint32_t seed);  // Initialize table; returns false on allocation failure.
void universal_hash_table_destroy(universal_hash_table* ht);  // Free all memory owned by the table.

bool universal_hash_table_insert(universal_hash_table* ht, int key, const char* value);  // Insert/update key->value; returns false on allocation failure.
const char* universal_hash_table_search(const universal_hash_table* ht, int key);  // Search key; returns stored value pointer (owned by table) or NULL.
bool universal_hash_table_delete(universal_hash_table* ht, int key);  // Delete key; returns true if removed.

double universal_hash_table_load_factor(const universal_hash_table* ht);  // Compute alpha = size / capacity.
int universal_hash_table_get_max_chain_length(const universal_hash_table* ht);  // Compute max chain length across buckets.

#endif  // UNIVERSAL_HASHING_H  // End of header guard.

