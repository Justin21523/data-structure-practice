// 03 通用雜湊示範（C）/ Universal hashing demo (C).  // Bilingual file header.

#include "universal_hashing.h"  // Include the public API for this unit.

#include <math.h>  // Provide sqrt for prime checking.
#include <stddef.h>  // Provide size_t for safe indexing.
#include <stdlib.h>  // Provide malloc/free for dynamic allocation.
#include <string.h>  // Provide strlen/memcpy for string duplication.

static char* str_dup(const char* s) {  // Duplicate a C string onto the heap (portable strdup replacement).
    if (s == NULL) {  // Reject null input.
        return NULL;  // Signal invalid input.
    }  // Close validation.
    size_t len = strlen(s);  // Compute string length without null terminator.
    char* out = (char*)malloc(len + 1);  // Allocate output buffer including null terminator.
    if (out == NULL) {  // Handle allocation failure.
        return NULL;  // Signal failure.
    }  // Close allocation check.
    memcpy(out, s, len + 1);  // Copy bytes including null terminator.
    return out;  // Return duplicated string.
}  // Close str_dup().

bool is_prime_int(int n) {  // Check whether n is prime (trial division).
    if (n < 2) {  // Handle 0, 1, and negatives.
        return false;  // Not prime.
    }  // Close base-case branch.
    if (n == 2) {  // Handle the only even prime.
        return true;  // 2 is prime.
    }  // Close base-case branch.
    if (n % 2 == 0) {  // Reject other even numbers quickly.
        return false;  // Not prime.
    }  // Close even branch.
    int limit = (int)floor(sqrt((double)n));  // Compute integer sqrt limit for trial division.
    for (int i = 3; i <= limit; i += 2) {  // Try odd divisors only.
        if (n % i == 0) {  // Detect a divisor.
            return false;  // Not prime.
        }  // Close divisor branch.
    }  // Close loop.
    return true;  // No divisors found => prime.
}  // Close is_prime_int().

int next_prime_int(int n) {  // Find the smallest prime >= n.
    int x = (n < 2) ? 2 : n;  // Ensure we start at least from 2.
    while (!is_prime_int(x)) {  // Repeat until prime.
        x += 1;  // Move to next candidate.
    }  // Close loop.
    return x;  // Return prime candidate.
}  // Close next_prime_int().

void lcg_init(lcg_rng* rng, uint32_t seed) {  // Initialize RNG state from a seed.
    rng->state = seed;  // Store seed directly as state.
}  // Close lcg_init().

uint32_t lcg_next_u32(lcg_rng* rng) {  // Generate next uint32 value using a classic LCG step.
    rng->state = (uint32_t)(1664525u * rng->state + 1013904223u);  // Update state (mod 2^32 via overflow).
    return rng->state;  // Return new state.
}  // Close lcg_next_u32().

uint32_t lcg_next_in_range(lcg_rng* rng, uint32_t min_inclusive, uint32_t max_inclusive) {  // Generate integer in [min, max] (mod bias acceptable).
    uint32_t range = max_inclusive - min_inclusive + 1u;  // Compute range size.
    uint32_t v = lcg_next_u32(rng) % range;  // Reduce to range (small bias is fine for this unit).
    return min_inclusive + v;  // Shift into desired range.
}  // Close lcg_next_in_range().

void universal_hash_family_init(universal_hash_family* uh, int m, int p_or_zero, uint32_t seed) {  // Initialize universal hash family.
    uh->m = m;  // Store bucket count.
    int default_p = next_prime_int((m * 10 > 10007) ? (m * 10) : 10007);  // Choose default p similar to Python reference.
    uh->p = (p_or_zero != 0) ? p_or_zero : default_p;  // Use provided p when non-zero.
    lcg_init(&uh->rng, seed);  // Initialize RNG state.
    universal_hash_family_regenerate(uh);  // Choose initial parameters.
}  // Close universal_hash_family_init().

void universal_hash_family_regenerate(universal_hash_family* uh) {  // Choose new random parameters (a, b).
    uh->a = (int)lcg_next_in_range(&uh->rng, 1u, (uint32_t)(uh->p - 1));  // Pick a in [1, p-1].
    uh->b = (int)lcg_next_in_range(&uh->rng, 0u, (uint32_t)(uh->p - 1));  // Pick b in [0, p-1].
}  // Close universal_hash_family_regenerate().

int universal_hash_family_hash(const universal_hash_family* uh, int key) {  // Compute hash value in [0, m).
    long long linear = (long long)uh->a * (long long)key + (long long)uh->b;  // Compute a*k + b in 64-bit.
    int mod_p = (int)(linear % (long long)uh->p);  // Reduce mod p using C remainder semantics.
    if (mod_p < 0) {  // Adjust negative remainder.
        mod_p += uh->p;  // Shift into [0, p).
    }  // Close adjustment branch.
    return mod_p % uh->m;  // Reduce mod m for bucket index.
}  // Close universal_hash_family_hash().

void universal_string_hash_family_init(universal_string_hash_family* ush, int m, int p_or_zero, uint32_t seed) {  // Initialize string hash family.
    ush->m = m;  // Store bucket count.
    int default_p = next_prime_int((m * 10 > 10007) ? (m * 10) : 10007);  // Choose default p similar to Python reference.
    ush->p = (p_or_zero != 0) ? p_or_zero : default_p;  // Use provided p when non-zero.
    lcg_init(&ush->rng, seed);  // Initialize RNG state.
    universal_string_hash_family_regenerate(ush);  // Choose initial base.
}  // Close universal_string_hash_family_init().

void universal_string_hash_family_regenerate(universal_string_hash_family* ush) {  // Choose new random base a.
    ush->a = (int)lcg_next_in_range(&ush->rng, 1u, (uint32_t)(ush->p - 1));  // Pick a in [1, p-1].
}  // Close universal_string_hash_family_regenerate().

int universal_string_hash_family_hash(const universal_string_hash_family* ush, const char* s) {  // Compute string hash value in [0, m).
    if (s == NULL) {  // Validate input pointer.
        return 0;  // Return safe default.
    }  // Close validation.
    long long h = 0;  // Initialize polynomial accumulator.
    for (size_t i = 0; s[i] != '\0'; i++) {  // Iterate bytes.
        h = (h * (long long)ush->a + (long long)((unsigned char)s[i])) % (long long)ush->p;  // Horner step mod p.
    }  // Close loop.
    return (int)(h % (long long)ush->m);  // Reduce mod m for bucket index.
}  // Close universal_string_hash_family_hash().

static int chain_length(const universal_entry* head) {  // Compute chain length for one bucket.
    int len = 0;  // Count nodes.
    const universal_entry* cur = head;  // Start at head.
    while (cur != NULL) {  // Walk until end of list.
        len += 1;  // Count one node.
        cur = cur->next;  // Advance to next node.
    }  // Close loop.
    return len;  // Return computed length.
}  // Close chain_length().

static void free_chain(universal_entry* head) {  // Free a chain of entries (nodes + values).
    universal_entry* cur = head;  // Start at head.
    while (cur != NULL) {  // Walk until end of list.
        universal_entry* next = cur->next;  // Save next pointer before freeing.
        free(cur->value);  // Free stored value string.
        free(cur);  // Free node itself.
        cur = next;  // Advance to next node.
    }  // Close loop.
}  // Close free_chain().

static void clear_buckets(universal_entry** buckets, int capacity) {  // Free all chains in a bucket array.
    for (int i = 0; i < capacity; i++) {  // Iterate buckets.
        free_chain(buckets[i]);  // Free chain at bucket i.
        buckets[i] = NULL;  // Clear pointer for safety.
    }  // Close loop.
}  // Close clear_buckets().

static void move_all_entries(universal_hash_table* ht, universal_entry** old_buckets, int old_capacity) {  // Move nodes from old buckets into ht->buckets using ht->hash_family.
    for (int i = 0; i < old_capacity; i++) {  // Iterate old buckets.
        universal_entry* node = old_buckets[i];  // Start at head of this bucket.
        while (node != NULL) {  // Walk chain and move nodes.
            universal_entry* next = node->next;  // Save next pointer before relinking.
            int new_index = universal_hash_family_hash(&ht->hash_family, node->key);  // Compute new bucket index.
            node->next = ht->buckets[new_index];  // Insert node at head of new bucket chain.
            ht->buckets[new_index] = node;  // Update bucket head.
            node = next;  // Advance to next node.
        }  // Close node loop.
        old_buckets[i] = NULL;  // Clear old bucket head after moving.
    }  // Close bucket loop.
}  // Close move_all_entries().

static bool resize_table(universal_hash_table* ht) {  // Double capacity and rehash all entries.
    int new_capacity = ht->capacity * 2;  // Compute new capacity.
    universal_entry** new_buckets = (universal_entry**)malloc((size_t)new_capacity * sizeof(universal_entry*));  // Allocate new bucket array.
    if (new_buckets == NULL) {  // Handle allocation failure.
        return false;  // Signal failure.
    }  // Close allocation check.
    for (int i = 0; i < new_capacity; i++) {  // Initialize new bucket pointers to NULL.
        new_buckets[i] = NULL;  // Clear one bucket.
    }  // Close init loop.

    universal_entry** old_buckets = ht->buckets;  // Save old bucket array pointer.
    int old_capacity = ht->capacity;  // Save old capacity.
    ht->buckets = new_buckets;  // Switch to new buckets.
    ht->capacity = new_capacity;  // Update capacity.
    ht->seed += 1u;  // Change seed so new hash family differs after resize.
    universal_hash_family_init(&ht->hash_family, ht->capacity, 0, ht->seed);  // Create a new hash family for the new capacity.

    move_all_entries(ht, old_buckets, old_capacity);  // Move nodes into the new bucket array.
    free(old_buckets);  // Free old bucket array (nodes were moved).
    return true;  // Report success.
}  // Close resize_table().

static bool regenerate_hash(universal_hash_table* ht) {  // Regenerate (a, b) and rehash without changing capacity.
    universal_entry** new_buckets = (universal_entry**)malloc((size_t)ht->capacity * sizeof(universal_entry*));  // Allocate new bucket array.
    if (new_buckets == NULL) {  // Handle allocation failure.
        return false;  // Signal failure.
    }  // Close allocation check.
    for (int i = 0; i < ht->capacity; i++) {  // Initialize new buckets to NULL.
        new_buckets[i] = NULL;  // Clear one bucket.
    }  // Close init loop.

    universal_entry** old_buckets = ht->buckets;  // Save old bucket array pointer.
    int old_capacity = ht->capacity;  // Save capacity.
    ht->buckets = new_buckets;  // Switch to new bucket array.
    universal_hash_family_regenerate(&ht->hash_family);  // Choose new parameters (a, b) for same m.

    move_all_entries(ht, old_buckets, old_capacity);  // Move nodes into new buckets using new parameters.
    free(old_buckets);  // Free old bucket array.
    ht->rehash_count += 1;  // Count this defensive rehash.
    return true;  // Report success.
}  // Close regenerate_hash().

bool universal_hash_table_init(universal_hash_table* ht, int capacity, uint32_t seed) {  // Initialize table; returns false on allocation failure.
    if (capacity <= 0) {  // Validate capacity.
        return false;  // Signal invalid input.
    }  // Close validation.
    ht->capacity = capacity;  // Store capacity.
    ht->size = 0;  // Start empty.
    ht->rehash_count = 0;  // Start with no rehashes.
    ht->seed = seed;  // Store base seed.
    ht->buckets = (universal_entry**)malloc((size_t)capacity * sizeof(universal_entry*));  // Allocate bucket array.
    if (ht->buckets == NULL) {  // Handle allocation failure.
        return false;  // Signal failure.
    }  // Close allocation check.
    for (int i = 0; i < capacity; i++) {  // Initialize buckets to NULL.
        ht->buckets[i] = NULL;  // Clear one bucket pointer.
    }  // Close init loop.
    universal_hash_family_init(&ht->hash_family, capacity, 0, seed);  // Initialize hash family for this capacity.
    return true;  // Report success.
}  // Close universal_hash_table_init().

void universal_hash_table_destroy(universal_hash_table* ht) {  // Free all memory owned by the table.
    if (ht->buckets != NULL) {  // Free only when bucket array exists.
        clear_buckets(ht->buckets, ht->capacity);  // Free all chains.
        free(ht->buckets);  // Free bucket array.
    }  // Close free branch.
    ht->buckets = NULL;  // Clear pointer.
    ht->capacity = 0;  // Reset capacity.
    ht->size = 0;  // Reset size.
    ht->rehash_count = 0;  // Reset rehash count.
    ht->seed = 0u;  // Reset seed.
}  // Close universal_hash_table_destroy().

static universal_entry* find_entry(universal_entry* head, int key) {  // Find entry with key in a chain (returns pointer or NULL).
    universal_entry* cur = head;  // Start at head.
    while (cur != NULL) {  // Walk list.
        if (cur->key == key) {  // Match found.
            return cur;  // Return node pointer.
        }  // Close match branch.
        cur = cur->next;  // Advance.
    }  // Close loop.
    return NULL;  // Not found.
}  // Close find_entry().

bool universal_hash_table_insert(universal_hash_table* ht, int key, const char* value) {  // Insert/update key->value; returns false on allocation failure.
    int index = universal_hash_family_hash(&ht->hash_family, key);  // Compute bucket index.
    universal_entry* existing = find_entry(ht->buckets[index], key);  // Look for existing key.
    if (existing != NULL) {  // Update path: replace value string.
        char* copy = str_dup(value);  // Duplicate value.
        if (copy == NULL) {  // Handle allocation failure.
            return false;  // Signal failure.
        }  // Close allocation check.
        free(existing->value);  // Free old value string.
        existing->value = copy;  // Store new value string.
        return true;  // Report success.
    }  // Close update branch.

    universal_entry* node = (universal_entry*)malloc(sizeof(universal_entry));  // Allocate new node.
    if (node == NULL) {  // Handle allocation failure.
        return false;  // Signal failure.
    }  // Close allocation check.
    node->key = key;  // Store key.
    node->value = str_dup(value);  // Duplicate value string.
    if (node->value == NULL) {  // Handle allocation failure.
        free(node);  // Free node.
        return false;  // Signal failure.
    }  // Close allocation check.
    node->next = ht->buckets[index];  // Insert at head of chain.
    ht->buckets[index] = node;  // Update bucket head.
    ht->size += 1;  // Increase size for new key.

    double load = universal_hash_table_load_factor(ht);  // Compute load factor after insertion.
    if (load > 0.75) {  // Resize when load factor is too high.
        if (!resize_table(ht)) {  // Attempt resize.
            return false;  // Signal failure.
        }  // Close resize check.
    } else if (chain_length(ht->buckets[index]) > 10) {  // Regenerate hash when chain becomes suspiciously long.
        if (!regenerate_hash(ht)) {  // Attempt defensive rehash.
            return false;  // Signal failure.
        }  // Close rehash check.
    }  // Close defense branch.

    return true;  // Report success.
}  // Close universal_hash_table_insert().

const char* universal_hash_table_search(const universal_hash_table* ht, int key) {  // Search key; returns stored value pointer (owned by table) or NULL.
    int index = universal_hash_family_hash(&ht->hash_family, key);  // Compute bucket index.
    universal_entry* cur = ht->buckets[index];  // Start at bucket head.
    while (cur != NULL) {  // Walk chain.
        if (cur->key == key) {  // Match found.
            return cur->value;  // Return stored value pointer.
        }  // Close match branch.
        cur = cur->next;  // Advance.
    }  // Close loop.
    return NULL;  // Not found.
}  // Close universal_hash_table_search().

bool universal_hash_table_delete(universal_hash_table* ht, int key) {  // Delete key; returns true if removed.
    int index = universal_hash_family_hash(&ht->hash_family, key);  // Compute bucket index.
    universal_entry* cur = ht->buckets[index];  // Start at head.
    universal_entry* prev = NULL;  // Track previous pointer.
    while (cur != NULL) {  // Walk chain.
        if (cur->key == key) {  // Match found.
            if (prev == NULL) {  // Removing head node.
                ht->buckets[index] = cur->next;  // Update bucket head.
            } else {  // Removing non-head node.
                prev->next = cur->next;  // Bypass current node.
            }  // Close branch.
            free(cur->value);  // Free value string.
            free(cur);  // Free node.
            ht->size -= 1;  // Decrease size.
            return true;  // Report success.
        }  // Close match branch.
        prev = cur;  // Advance prev.
        cur = cur->next;  // Advance cur.
    }  // Close loop.
    return false;  // Not found.
}  // Close universal_hash_table_delete().

double universal_hash_table_load_factor(const universal_hash_table* ht) {  // Compute alpha = size / capacity.
    if (ht->capacity <= 0) {  // Guard against division by zero.
        return 0.0;  // Return safe default.
    }  // Close guard.
    return (double)ht->size / (double)ht->capacity;  // Compute load factor.
}  // Close universal_hash_table_load_factor().

int universal_hash_table_get_max_chain_length(const universal_hash_table* ht) {  // Compute max chain length across buckets.
    int max_len = 0;  // Track maximum chain length.
    for (int i = 0; i < ht->capacity; i++) {  // Scan all buckets.
        int len = chain_length(ht->buckets[i]);  // Compute chain length for bucket i.
        if (len > max_len) {  // Update max when needed.
            max_len = len;  // Store new maximum.
        }  // Close max update.
    }  // Close scan loop.
    return max_len;  // Return maximum chain length.
}  // Close universal_hash_table_get_max_chain_length().

