#include <stdio.h>
#include <assert.h>
#include "../vector.h"

#define ARRAY_SIZE 10

void print_vector(int ***vec) {
    size_t vec_length = Vector_get_length(vec);
    printf("{\n");
    printf("    element_size: %ld,\n", Vector_get_element_size(vec));
    printf("    length: %ld,\n", vec_length);
    printf("    capacity: %ld,\n", Vector_get_capacity(vec));
    printf("    data: [\n");
    for (int i = 0; i < vec_length; i++) {
        printf("        [");
        for (int j = 0; j < 10; j++) {
            printf("%d%s", (*vec)[i][j], j == ARRAY_SIZE - 1 ? "" : ", ");
        }
        printf("]\n");
    }
    printf("    ]\n");
    printf("}\n");
}


bool equal_vec_arr(int **vec_of_arrays, int parent_array[][ARRAY_SIZE], size_t number_of_arrays) {
    if (Vector_get_length(&vec_of_arrays) != number_of_arrays) {
        return false;
    }
    for (size_t current_array_index = 0; current_array_index < number_of_arrays; current_array_index++) {
        int *current_array_in_vec = vec_of_arrays[current_array_index];
        int *current_array_in_parent_array = parent_array[current_array_index];
        for (size_t i = 0; i < ARRAY_SIZE; i++) {
            if (current_array_in_vec[i] != current_array_in_parent_array[i]) {
                return false;
            }
        }
    }
    return true;
}

int main(void) {
    int **vec = Vector_init(int *);

    int a[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6 ,7, 8, 9, 10};
    int n1[][ARRAY_SIZE] = {{1, 2, 3, 4, 5, 6 ,7, 8, 9, 10}};
    Vector_push(&vec, a);

    assert(
        equal_vec_arr(vec, n1, sizeof(n1) / sizeof(n1[0]))
        == true
    );

    int b[ARRAY_SIZE] = { 69, 420, 1337,   69,   420, 1337, 69, 420, 1337,    69 };
    int c[ARRAY_SIZE] = { 10,   9,    8,    7,     6,    5,  4,   3,    2,     1 };
    int d[ARRAY_SIZE] = { 1 ,  11,  111, 1111, 11111,    2, 22, 222, 2222, 22222 };
    int e[ARRAY_SIZE] = { 3 ,  33,  333, 3333, 33333,    4, 44, 444, 4444, 44444 };
    int n2[][ARRAY_SIZE] = {
        {  1,   2,    3,    4,     5,   6 ,  7,   8,    9,    10 },
        { 69, 420, 1337,   69,   420, 1337, 69, 420, 1337,    69 },
        { 10,   9,    8,    7,     6,    5,  4,   3,    2,     1 },
        { 1 ,  11,  111, 1111, 11111,    2, 22, 222, 2222, 22222 },
        { 3 ,  33,  333, 3333, 33333,    4, 44, 444, 4444, 44444 },
    };

    Vector_push(&vec, b);
    Vector_push(&vec, c);
    Vector_push(&vec, d);
    Vector_push(&vec, e);

    assert(
        equal_vec_arr(vec, n2, sizeof(n2) / sizeof(n2[0]))
        == true
    );

    printf("All tests passed successfully!\n");
    return 0;
}
