/* 01 漸進符號示範（C）/ Asymptotic notation demo (C). */  // Bilingual header comment for this module.
#ifndef ASYMPTOTIC_DEMO_H  // Header guard to prevent multiple inclusion.
#define ASYMPTOTIC_DEMO_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for simple success/failure signaling.
#include <stddef.h>  // Provide size_t for array sizes and indices.

long long count_constant_ops(int n);  // Return a constant operation count (O(1)) for any n.
long long count_log2_ops(int n);  // Return floor(log2(n)) via repeated halving; returns -1 when n < 1.
long long count_linear_ops(int n);  // Return n via a single loop; returns -1 when n < 0.
long long count_n_log2_n_ops(int n);  // Return n * floor(log2(n)) via nested loops; returns -1 when n < 0.
long long count_quadratic_ops(int n);  // Return n^2 via double loop; returns -1 when n < 0.

#endif  // ASYMPTOTIC_DEMO_H  // End of header guard.

