// 01 堆疊測試（C）/ Tests for stack (C).  // Bilingual file header.

#include "stack.h"  // Include the API under test.

#include <stdio.h>  // Provide printf for status output.
#include <stdlib.h>  // Provide exit for early termination on failure.

static void assert_true(bool condition, const char* message) {  // Minimal assertion helper.
    if (!condition) {  // Fail when condition is false.
        printf("FAIL: %s\n", message);  // Print message.
        exit(1);  // Exit with failure.
    }  // Close failure branch.
    printf("PASS: %s\n", message);  // Print pass status.
}  // Close assert_true().

static void test_lifo_push_pop_and_peek(void) {  // Stack should obey LIFO semantics.
    array_stack s;  // Create stack struct.
    assert_true(array_stack_init(&s), "init should succeed");  // Initialize stack.

    stack_cost c;  // Cost record for pushes.
    assert_true(array_stack_push(&s, 10, &c), "push(10) should succeed");  // Push 10.
    assert_true(array_stack_push(&s, 20, &c), "push(20) should succeed");  // Push 20.
    assert_true(array_stack_push(&s, 30, &c), "push(30) should succeed");  // Push 30.

    int top = 0;  // Store peeked value.
    assert_true(array_stack_peek(&s, &top), "peek should succeed");  // Peek top.
    assert_true(top == 30, "peek should return last pushed value");  // Validate peek value.

    stack_pop_result r;  // Pop result.
    assert_true(array_stack_pop(&s, &r), "pop should succeed");  // Pop first.
    assert_true(r.value == 30, "first pop should return 30");  // Validate first pop.
    assert_true(array_stack_pop(&s, &r), "pop should succeed");  // Pop second.
    assert_true(r.value == 20, "second pop should return 20");  // Validate second pop.
    assert_true(array_stack_pop(&s, &r), "pop should succeed");  // Pop third.
    assert_true(r.value == 10, "third pop should return 10");  // Validate third pop.
    assert_true(s.size == 0, "stack should be empty after popping all");  // Validate emptiness.

    array_stack_destroy(&s);  // Free resources.
}  // Close test_lifo_push_pop_and_peek().

static void test_push_resize_copied_counts(void) {  // copied should count resize copies exactly.
    array_stack s;  // Create stack struct.
    assert_true(array_stack_init(&s), "init should succeed");  // Initialize stack.

    stack_cost c;  // Cost record.
    assert_true(array_stack_push(&s, 10, &c), "push should succeed");  // Push 10.
    assert_true(c.copied == 0, "first push fits without resize");  // Validate copied=0.
    assert_true(array_stack_push(&s, 20, &c), "push should succeed");  // Push 20.
    assert_true(c.copied == 1, "resize 1->2 copies 1 element");  // Validate copied=1.
    assert_true(array_stack_push(&s, 30, &c), "push should succeed");  // Push 30.
    assert_true(c.copied == 2, "resize 2->4 copies 2 elements");  // Validate copied=2.
    assert_true(array_stack_push(&s, 40, &c), "push should succeed");  // Push 40.
    assert_true(c.copied == 0, "push fits in capacity 4");  // Validate copied=0.
    assert_true(array_stack_push(&s, 50, &c), "push should succeed");  // Push 50.
    assert_true(c.copied == 4, "resize 4->8 copies 4 elements");  // Validate copied=4.

    assert_true(s.size == 5, "final size should be 5");  // Validate final size.
    assert_true(s.capacity == 8, "final capacity should be 8");  // Validate final capacity.
    assert_true(s.total_copies == 1 + 2 + 4, "total copies should sum resizes");  // Validate total copies.

    int out[8];  // Output buffer for snapshot.
    assert_true(array_stack_to_array(&s, out, 8), "to_array should succeed");  // Copy snapshot.
    assert_true(out[0] == 10 && out[4] == 50, "snapshot should preserve bottom->top order");  // Validate snapshot endpoints.

    array_stack_destroy(&s);  // Free resources.
}  // Close test_push_resize_copied_counts().

static void test_empty_operations_fail(void) {  // pop/peek should reject empty stack.
    array_stack s;  // Create stack struct.
    assert_true(array_stack_init(&s), "init should succeed");  // Initialize stack.

    int top = 0;  // Placeholder for peek value.
    assert_true(!array_stack_peek(&s, &top), "peek should fail on empty");  // Invalid peek.
    stack_pop_result r;  // Placeholder for pop result.
    assert_true(!array_stack_pop(&s, &r), "pop should fail on empty");  // Invalid pop.

    array_stack_destroy(&s);  // Free resources.
}  // Close test_empty_operations_fail().

int main(void) {  // Run all tests and print status.
    printf("=== ArrayStack Tests (C) ===\n");  // Print header.
    test_lifo_push_pop_and_peek();  // Run LIFO tests.
    test_push_resize_copied_counts();  // Run resize cost tests.
    test_empty_operations_fail();  // Run empty-operation tests.
    printf("============================\n");  // Print footer separator.
    printf("All tests PASSED.\n");  // Print success message.
    return 0;  // Exit success.
}  // Close main().

