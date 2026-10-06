#include <stdio.h>
#include <assert.h>
#define VECTOR_IMPLEMENTATION
#include "../vector.h"

void print_vector(int ***vec) {
    size_t vec_length = Vector_get_length(vec);
    printf("{\n");
    printf("    element_size: %ld,\n", Vector_get_element_size(vec));
    printf("    length: %ld,\n", vec_length);
    printf("    capacity: %ld,\n", Vector_get_capacity(vec));
    printf("    data: [\n");
    for (int i = 0; i < vec_length; i++) {
        int *v = (*vec)[i];
        size_t v_length = Vector_get_length(&v);
        printf("        {\n");
        printf("            element_size: %ld,\n", Vector_get_element_size(&v));
        printf("            length: %ld,\n", v_length);
        printf("            capacity: %ld,\n", Vector_get_capacity(&v));
        printf("            data: [");
        for (int j = 0; j < Vector_get_length(&v); j++) {
            printf("%d%s", v[j], j == v_length - 1 ? "" : ", ");
        }
        printf("]\n");
        printf("        }%s\n", i == vec_length - 1 ? "" : ",");
    }
    printf("    ]\n");
    printf("}\n");
}

void free_fn(void *vec_ptr) {
    int ***temp_vec = (int ***)vec_ptr;
    for (int i = 0; i < Vector_get_length(temp_vec); i++) {
        int *v = (*temp_vec)[i];
        Vector_destroy(&v);
    }
    free(__vector_get_header(temp_vec));
}

bool equal_vec_arr(int *vec, int arr[], size_t arr_length) {
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

int main(void) {
    int **vec_of_vec = Vector_init(int *);
    Vector_set_free_fn(&vec_of_vec, free_fn);

    for (int i = 0; i < 3; i++)
        Vector_push(&vec_of_vec, Vector_init(int));

    // resize each inner vector by increasing its capacity
    // make sure that everything works fine and the pointers are valid
    for (int i =  0; i < 15; i++)
        Vector_push(&vec_of_vec[0], i);
    for (int i = 15; i < 30; i++)
        Vector_push(&vec_of_vec[1], i);
    for (int i = 30; i < 45; i++)
        Vector_push(&vec_of_vec[2], i);

    int n1[][15] = {
        {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14 },
        { 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 },
        { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 }
    };

    assert(
        equal_vec_arr(vec_of_vec[0], n1[0], sizeof(n1[0]) / sizeof(n1[0][0]))
        == true
    );
    assert(
        equal_vec_arr(vec_of_vec[1], n1[1], sizeof(n1[1]) / sizeof(n1[1][0]))
        == true
    );
    assert(
        equal_vec_arr(vec_of_vec[2], n1[2], sizeof(n1[2]) / sizeof(n1[2][0]))
        == true
    );

    // resize vec by increasing the capacity
    // make sure that everything works fine and the pointers are still valid
    Vector_push(&vec_of_vec, Vector_init(int));
    Vector_push(&vec_of_vec, Vector_init(int));
    assert(
        equal_vec_arr(vec_of_vec[0], n1[0], sizeof(n1[0]) / sizeof(n1[0][0]))
        == true
    );
    assert(
        equal_vec_arr(vec_of_vec[1], n1[1], sizeof(n1[1]) / sizeof(n1[1][0]))
        == true
    );
    assert(
        equal_vec_arr(vec_of_vec[2], n1[2], sizeof(n1[2]) / sizeof(n1[2][0]))
        == true
    );

    Vector_destroy(&vec_of_vec);

    printf("All tests passed successfully!\n");
    return 0;
}
