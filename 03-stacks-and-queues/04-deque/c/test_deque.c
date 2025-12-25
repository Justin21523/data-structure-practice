// 04 雙端佇列測試（C）/ Tests for deque (C).  // Bilingual file header.

#include "deque.h"  // Include the API under test.

#include <stdio.h>  // Provide printf for status output.
#include <stdlib.h>  // Provide exit for early termination on failure.

static void assert_true(bool condition, const char* message) {  // Minimal assertion helper.
    if (!condition) {  // Fail when condition is false.
        printf("FAIL: %s\n", message);  // Print message.
        exit(1);  // Exit with failure.
    }  // Close failure branch.
    printf("PASS: %s\n", message);  // Print pass status.
}  // Close assert_true().

static void test_push_pop_and_peek(void) {  // Deque should support both-end operations.
    circular_deque d;  // Create deque struct.
    assert_true(circular_deque_init(&d), "init should succeed");  // Initialize deque.

    deque_cost c;  // Cost record for pushes.
    assert_true(circular_deque_push_back(&d, 3, &c), "push_back(3) should succeed");  // Push 3 back.
    assert_true(circular_deque_push_back(&d, 7, &c), "push_back(7) should succeed");  // Push 7 back.
    assert_true(circular_deque_push_front(&d, 1, &c), "push_front(1) should succeed");  // Push 1 front => [1,3,7].

    int front = 0;  // Placeholder for peekFront.
    int back = 0;  // Placeholder for peekBack.
    assert_true(circular_deque_peek_front(&d, &front), "peek_front should succeed");  // Peek front.
    assert_true(circular_deque_peek_back(&d, &back), "peek_back should succeed");  // Peek back.
    assert_true(front == 1, "peek_front should return 1");  // Validate front.
    assert_true(back == 7, "peek_back should return 7");  // Validate back.

    deque_pop_result r;  // Pop result record.
    assert_true(circular_deque_pop_front(&d, &r), "pop_front should succeed");  // Pop front.
    assert_true(r.value == 1, "pop_front should return 1");  // Validate popped value.
    assert_true(r.cost.moved == 0, "pop_front moved should be 0");  // Validate moved=0.
    assert_true(circular_deque_pop_back(&d, &r), "pop_back should succeed");  // Pop back.
    assert_true(r.value == 7, "pop_back should return 7");  // Validate popped value.
    assert_true(r.cost.moved == 0, "pop_back moved should be 0");  // Validate moved=0.
    assert_true(circular_deque_pop_back(&d, &r), "pop_back should succeed");  // Pop last.
    assert_true(r.value == 3, "pop_back should return 3");  // Validate popped value.
    assert_true(d.size == 0, "deque should be empty after removing all");  // Validate emptiness.
    assert_true(d.head == 0, "head should normalize to 0 when empty");  // Validate head normalization.

    circular_deque_destroy(&d);  // Free resources.
}  // Close test_push_pop_and_peek().

static void test_push_resize_copied_counts(void) {  // copied should count resize copies exactly.
    circular_deque d;  // Create deque struct.
    assert_true(circular_deque_init(&d), "init should succeed");  // Initialize deque.

    deque_cost c;  // Cost record.
    assert_true(circular_deque_push_back(&d, 10, &c), "push_back should succeed");  // Push 10.
    assert_true(c.copied == 0, "first push fits without resize");  // Validate copied=0.
    assert_true(circular_deque_push_back(&d, 20, &c), "push_back should succeed");  // Push 20.
    assert_true(c.copied == 1, "resize 1->2 copies 1 element");  // Validate copied=1.
    assert_true(circular_deque_push_back(&d, 30, &c), "push_back should succeed");  // Push 30.
    assert_true(c.copied == 2, "resize 2->4 copies 2 elements");  // Validate copied=2.
    assert_true(circular_deque_push_back(&d, 40, &c), "push_back should succeed");  // Push 40.
    assert_true(c.copied == 0, "push fits in capacity 4");  // Validate copied=0.
    assert_true(circular_deque_push_back(&d, 50, &c), "push_back should succeed");  // Push 50.
    assert_true(c.copied == 4, "resize 4->8 copies 4 elements");  // Validate copied=4.

    assert_true(d.size == 5, "final size should be 5");  // Validate final size.
    assert_true(d.capacity == 8, "final capacity should be 8");  // Validate final capacity.
    assert_true(d.total_copies == 1 + 2 + 4, "total copies should sum resizes");  // Validate total copies.

    int out[8];  // Output buffer for snapshot.
    assert_true(circular_deque_to_array(&d, out, 8), "to_array should succeed");  // Copy snapshot.
    assert_true(out[0] == 10 && out[4] == 50, "snapshot should preserve front->back order");  // Validate endpoints.

    circular_deque_destroy(&d);  // Free resources.
}  // Close test_push_resize_copied_counts().

static void test_wrap_around_works(void) {  // Deque should wrap around when head is not 0.
    circular_deque d;  // Create deque struct.
    assert_true(circular_deque_init(&d), "init should succeed");  // Initialize deque.

    deque_cost c;  // Reusable cost record for pushes.
    for (int v = 0; v < 4; v++) {  // Push 0..3 (capacity should become 4).
        assert_true(circular_deque_push_back(&d, v, &c), "push_back should succeed");  // Push one value.
    }  // Close loop.

    deque_pop_result r;  // Pop result record.
    assert_true(circular_deque_pop_front(&d, &r), "pop_front should succeed");  // Pop 0.
    assert_true(r.value == 0, "pop_front should return 0");  // Validate.
    assert_true(circular_deque_pop_front(&d, &r), "pop_front should succeed");  // Pop 1.
    assert_true(r.value == 1, "pop_front should return 1");  // Validate.

    assert_true(circular_deque_push_back(&d, 4, &c), "push_back should succeed");  // Push 4 (wrap-around).
    assert_true(circular_deque_push_back(&d, 5, &c), "push_back should succeed");  // Push 5 (wrap-around).

    int out[8];  // Output buffer for snapshot.
    assert_true(circular_deque_to_array(&d, out, 8), "to_array should succeed");  // Snapshot.
    assert_true(out[0] == 2 && out[3] == 5, "ordering after wrap-around should be [2,3,4,5]");  // Validate.

    circular_deque_destroy(&d);  // Free resources.
}  // Close test_wrap_around_works().

static void test_resize_when_head_not_zero_preserves_order(void) {  // Resize should preserve order even when head is offset.
    circular_deque d;  // Create deque struct.
    assert_true(circular_deque_init(&d), "init should succeed");  // Initialize deque.

    deque_cost c;  // Reusable cost record for pushes.
    for (int v = 0; v < 4; v++) {  // Push 0..3 (capacity should become 4).
        assert_true(circular_deque_push_back(&d, v, &c), "push_back should succeed");  // Push one value.
    }  // Close loop.

    deque_pop_result r;  // Pop result record.
    circular_deque_pop_front(&d, &r);  // Pop 0 (head moves).
    circular_deque_pop_front(&d, &r);  // Pop 1 (head moves).

    assert_true(circular_deque_push_back(&d, 4, &c), "push_back should succeed");  // Push 4.
    assert_true(circular_deque_push_back(&d, 5, &c), "push_back should succeed");  // Push 5 (deque is full).

    assert_true(circular_deque_push_back(&d, 6, &c), "push_back should succeed");  // Push 6 (triggers resize).
    assert_true(c.copied == 4, "resize should copy exactly size elements (4)");  // Validate copied count.
    int out1[16];  // Output buffer.
    assert_true(circular_deque_to_array(&d, out1, 16), "to_array should succeed");  // Snapshot deque.
    assert_true(out1[0] == 2 && out1[4] == 6, "order should be preserved after push_back resize");  // Validate endpoints.

    assert_true(circular_deque_push_front(&d, 1, &c), "push_front should succeed");  // Push 1 to front.
    assert_true(c.copied == 0, "push_front should not resize after growth");  // Validate no resize now.
    int out2[16];  // Output buffer.
    assert_true(circular_deque_to_array(&d, out2, 16), "to_array should succeed");  // Snapshot deque.
    assert_true(out2[0] == 1 && out2[5] == 6, "push_front should add element to the front");  // Validate endpoints.

    circular_deque_destroy(&d);  // Free resources.
}  // Close test_resize_when_head_not_zero_preserves_order().

static void test_push_front_triggers_resize_and_preserves_order(void) {  // push_front should also preserve order when it triggers resize.
    circular_deque d;  // Create deque struct.
    assert_true(circular_deque_init(&d), "init should succeed");  // Initialize deque.

    deque_cost c;  // Reusable cost record for pushes.
    for (int v = 0; v < 4; v++) {  // Push 0..3 (capacity should become 4).
        assert_true(circular_deque_push_back(&d, v, &c), "push_back should succeed");  // Push one value.
    }  // Close loop.

    deque_pop_result r;  // Pop result record.
    circular_deque_pop_front(&d, &r);  // Pop 0 (head moves).
    circular_deque_pop_front(&d, &r);  // Pop 1 (head moves).
    circular_deque_push_back(&d, 4, &c);  // Push 4.
    circular_deque_push_back(&d, 5, &c);  // Push 5 (deque is full again).

    assert_true(circular_deque_push_front(&d, 1, &c), "push_front should succeed");  // Push 1 (triggers resize).
    assert_true(c.copied == 4, "push_front resize should copy exactly size elements (4)");  // Validate copied count.

    int out[16];  // Output buffer.
    assert_true(circular_deque_to_array(&d, out, 16), "to_array should succeed");  // Snapshot deque.
    assert_true(out[0] == 1 && out[4] == 5, "order should be [1,2,3,4,5] after resize");  // Validate endpoints.

    circular_deque_destroy(&d);  // Free resources.
}  // Close test_push_front_triggers_resize_and_preserves_order().

static void test_empty_operations_fail(void) {  // peek/pop should reject empty deque.
    circular_deque d;  // Create deque struct.
    assert_true(circular_deque_init(&d), "init should succeed");  // Initialize deque.

    int v = 0;  // Placeholder for peek.
    deque_pop_result r;  // Placeholder for pop results.
    assert_true(!circular_deque_peek_front(&d, &v), "peek_front should fail on empty");  // Invalid peek.
    assert_true(!circular_deque_peek_back(&d, &v), "peek_back should fail on empty");  // Invalid peek.
    assert_true(!circular_deque_pop_front(&d, &r), "pop_front should fail on empty");  // Invalid pop.
    assert_true(!circular_deque_pop_back(&d, &r), "pop_back should fail on empty");  // Invalid pop.

    circular_deque_destroy(&d);  // Free resources.
}  // Close test_empty_operations_fail().

int main(void) {  // Run all tests and print status.
    printf("=== Deque Tests (C) ===\n");  // Print header.
    test_push_pop_and_peek();  // Run basic operations test.
    test_push_resize_copied_counts();  // Run resize copy tests.
    test_wrap_around_works();  // Run wrap-around test.
    test_resize_when_head_not_zero_preserves_order();  // Run resize-with-offset-head test.
    test_push_front_triggers_resize_and_preserves_order();  // Run push_front-triggered resize test.
    test_empty_operations_fail();  // Run empty-operation tests.
    printf("========================\n");  // Print footer separator.
    printf("All tests PASSED.\n");  // Print success message.
    return 0;  // Exit success.
}  // Close main().

