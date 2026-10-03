#define SYSTEM_ENV_H // this is used to uninclude the "system_env.h", so then i will define various compiler features for testing
#define VALUE 0b100
#define COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS (VALUE & 0b100)
#define COMPILER_SUPPORTS_TYPEOF (VALUE & 0b010)
#define COMPILER_SUPPORTS_BUILTIN_CLZ (VALUE & 0b001)

#include <stdio.h>
#include <assert.h>
#include "../vector.h"

void print_vector_int(int *vec) {
    printf("{\n");
    printf("    element_size: %ld\n", Vector_get_element_size(&vec));
    printf("    length: %ld\n", Vector_get_length(&vec));
    printf("    capacity: %ld\n", Vector_get_capacity(&vec));
    printf("    data: [");
    if (Vector_get_length(&vec) != 0) {
        printf("%d", vec[0]);
        for (size_t i = 1; i < Vector_get_length(&vec); i++) {
            printf(", %d", vec[i]);
        }
    }
    printf("]\n}\n");
}

bool equal_vec_arr(int *vec, int *arr, size_t arr_length) {
    if (Vector_get_length(&vec) != arr_length) {
        return false;
    }
    for (size_t i = 0; i < arr_length; i++) {
        if (vec[i] != arr[i]) {
            return false;
        }
    }
    return true;
}

bool equal_vec_vec(int *vec1, int *vec2) {
    if (Vector_get_length(&vec1) != Vector_get_length(&vec2)) {
        return false;
    }
    for (size_t i = 0; i < Vector_get_length(&vec1); i++) {
        if (vec1[i] != vec2[i]) {
            return false;
        }
    }
    return true;
}

// selects the numbers that are equal to the given value
int int_boolean_comparator(int a, int b) { return a == b; }
// sorts in ascending order
int int_ordering_comparator(int a, int b) { return a - b; }
// selects even numbers
bool filter(int value) { return value % 2 == 0; }
// doubles the value
void for_each(int *value) { *value *= 2; }
// adds 2 to the value
int map(int x) { return x + 2; }
// calculates the sum
int sum_reducer(int acc, int x) { return acc + x; }
// checks if the value is equal to 10
bool any_all(int value) { return value == 10; }
// sets the value to 10
void set_all_to_10(int *value) { *value = 10; }


int main(void) {
    printf("COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS: %s\n", COMPILER_SUPPORTS_STATEMENT_EXPRESSIONS ? "true" : "false");
    printf("COMPILER_SUPPORTS_TYPEOF: %s\n", COMPILER_SUPPORTS_TYPEOF ? "true" : "false");
    printf("COMPILER_SUPPORTS_BUILTIN_CLZ: %s\n", COMPILER_SUPPORTS_BUILTIN_CLZ ? "true" : "false");

    int *vec1 = Vector_init(int);

    int n1[] = {4, 9, 2, 0, 7, 5, 8, 1, 3, 6, 9, 4, 0, 8, 2, 5, 1, 7, 6, 3};
    for (size_t i = 0; i < sizeof(n1) / sizeof(n1[0]); i++) {
        Vector_push(&vec1, n1[i]);
    }
    assert(equal_vec_arr(vec1, n1, sizeof(n1) / sizeof(n1[0])));

    // getting the index of the first 5
    int index = Vector_index_of(&vec1, 5, int_boolean_comparator);
    assert(index == 5);

    // getting the count of 5s
    int count = Vector_count(&vec1, 5, int_boolean_comparator);
    assert(count == 2);


    // inserting 100 at index 5
    Vector_insert_at(&vec1, 5, 100);
    int n2[] = {4, 9, 2, 0, 7, 100, 5, 8, 1, 3, 6, 9, 4, 0, 8, 2, 5, 1, 7, 6, 3};
    assert(equal_vec_arr(vec1, n2, sizeof(n2) / sizeof(n2[0])));

    // popping the last value
    int popped = Vector_pop(&vec1, int);
    int n3[] = {4, 9, 2, 0, 7, 100, 5, 8, 1, 3, 6, 9, 4, 0, 8, 2, 5, 1, 7, 6};
    assert(popped == 3);
    assert(equal_vec_arr(vec1, n3, sizeof(n3) / sizeof(n3[0])));

    // removing the value at index 0
    int removed = Vector_remove_at(&vec1, 0, int);
    int n4[] = {9, 2, 0, 7, 100, 5, 8, 1, 3, 6, 9, 4, 0, 8, 2, 5, 1, 7, 6};
    assert(removed == 4);
    assert(equal_vec_arr(vec1, n4, sizeof(n4) / sizeof(n4[0])));

    // removing the first occurrence of 5
    index = Vector_remove_value(&vec1, 5, int_boolean_comparator, int);
    int n5[] = {9, 2, 0, 7, 100, 8, 1, 3, 6, 9, 4, 0, 8, 2, 5, 1, 7, 6};
    assert(index == 5);
    assert(equal_vec_arr(vec1, n5, sizeof(n5) / sizeof(n5[0])));

    // copying vec1 to vec2
    int *vec2 = Vector_copy(&vec1);
    assert(equal_vec_vec(vec1, vec2));

    // removing the value at index 3 unordered from vec1
    removed = Vector_remove_at_unordered(&vec1, 3, int);
    int n6[] = {9, 2, 0, 6, 100, 8, 1, 3, 6, 9, 4, 0, 8, 2, 5, 1, 7};
    assert(removed == 7);
    assert(equal_vec_arr(vec1, n6, sizeof(n6) / sizeof(n6[0])));

    // removing the first occurrence of 5 unordered from vec1
    index = Vector_remove_value_unordered(&vec1, 5, int_boolean_comparator, int);
    int n7[] = {9, 2, 0, 6, 100, 8, 1, 3, 6, 9, 4, 0, 8, 2, 7, 1};
    assert(index == 14);
    assert(equal_vec_arr(vec1, n7, sizeof(n7) / sizeof(n7[0])));

    // sorting vec2
    Vector_sort(&vec2, int_ordering_comparator, int);
    int n8[] = {0, 0, 1, 1, 2, 2, 3, 4, 5, 6, 6, 7, 7, 8, 8, 9, 9, 100};
    assert(equal_vec_arr(vec2, n8, sizeof(n8) / sizeof(n8[0])));


    // inserting 5 in its sorted position in vec2
    index = Vector_insert_sorted(&vec2, 5, int_ordering_comparator);
    int n9[] = {0, 0, 1, 1, 2, 2, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 100};
    assert(index == 8);
    assert(equal_vec_arr(vec2, n9, sizeof(n9) / sizeof(n9[0])));


    // reversing vec2
    Vector_reverse(&vec2, int);
    int n10[] = {100, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 4, 3, 2, 2, 1, 1, 0, 0};
    assert(equal_vec_arr(vec2, n10, sizeof(n10) / sizeof(n10[0])));

    // filtering vec2 for even numbers and storing the result in vec3
    int *vec3 = Vector_filter(&vec2, filter, int);
    int n11[] = {100, 8, 8, 6, 6, 4, 2, 2, 0, 0};
    assert(equal_vec_arr(vec3, n11, sizeof(n11) / sizeof(n11[0])));

    // doubling the values in vec3
    Vector_foreach(&vec3, for_each);
    int n12[] = {200, 16, 16, 12, 12, 8, 4, 4, 0, 0};
    assert(equal_vec_arr(vec3, n12, sizeof(n12) / sizeof(n12[0])));

    // mapping vec3 by adding 2 to each value and storing the result in vec4
    int *vec4 = Vector_map(&vec3, map, int);
    int n13[] = {202, 18, 18, 14, 14, 10, 6, 6, 2, 2};
    assert(equal_vec_arr(vec4, n13, sizeof(n13) / sizeof(n13[0])));

    // calculating the sum of vec4
    int sum = Vector_reduce(&vec4, sum_reducer, 0, int);
    assert(sum == 202 + 18 + 18 + 14 + 14 + 10 + 6 + 6 + 2 + 2);

    // checking if any value in vec4 is equal to 10
    bool any = Vector_any(&vec4, any_all);
    assert(any == true);

    // checking if all values in vec4 are equal to 10
    bool all = Vector_all(&vec4, any_all);
    assert(all == false);

    // setting all values in vec4 to 10
    Vector_foreach(&vec4, set_all_to_10);
    int n14[] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
    assert(equal_vec_arr(vec4, n14, sizeof(n14) / sizeof(n14[0])));

    // checking if all values in vec4 are equal to 10
    all = Vector_all(&vec4, any_all);
    assert(all == true);

    // slicing vec1 from index 0 to the end with a step of 2 and storing the result in vec5
    int *vec5 = Vector_slice(&vec1, 0, Vector_get_length(&vec1), 2, int);
    int n15[] = {9, 0, 100, 1, 6, 4, 8, 7};
    assert(equal_vec_arr(vec5, n15, sizeof(n15) / sizeof(n15[0])));

    Vector_destroy(&vec1);
    Vector_destroy(&vec2);
    Vector_destroy(&vec3);
    Vector_destroy(&vec4);
    Vector_destroy(&vec5);

    printf("All tests passed successfully!\n");
    return 0;
}
