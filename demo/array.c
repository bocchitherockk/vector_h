#include <stdio.h>
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


int main(void) {
    int a[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6 ,7, 8, 9, 10};
    int b[ARRAY_SIZE] = {69, 420, 1337, 69, 420, 1337, 69, 420, 1337, 69};
    int c[ARRAY_SIZE] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int d[ARRAY_SIZE] = {1, 11, 111, 1111, 11111, 2, 22, 222, 2222, 22222};
    int e[ARRAY_SIZE] = {3, 33, 333, 3333, 33333, 4, 44, 444, 4444, 44444};

    int **vec = Vector_init(int *);
    Vector_push(&vec, a);
    Vector_push(&vec, b);
    Vector_push(&vec, c);
    Vector_push(&vec, d);
    print_vector(&vec);

    Vector_push(&vec, e);
    print_vector(&vec);

    return 0;
}
