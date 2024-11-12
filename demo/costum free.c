#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

void print_vector(int ***vec) {
    printf("element_size: %ld\n", Vector_element_size(&vec));
    printf("length: %ld\n", Vector_length(&vec));
    printf("capacity: %ld\n", Vector_capacity(&vec));
    printf("data: [\n");
    for (int i = 0; i < Vector_length(&vec); i++) {
        printf("        [");
        for (int j = 0; j < Vector_length(vec[i]); j++) {
            printf("%d ", (*(vec[i]))[j]);
        }
        printf("]\n");
    }
    printf("]\n");
}

void free_fn(void *vec_ptr) {
    int ****temp_vec = (int ****)vec_ptr;
    printf("vector length: %zu\n", Vector_length(temp_vec));
    for (int i = 0; i < Vector_length(temp_vec); i++) {
        Vector_destroy((*temp_vec)[i]);
        free((*temp_vec)[i]);
    }
    free(__get_vector_header(temp_vec));
}

int main() {
    int ***vec = Vector_init(int**);
    Vector_set_free_fn(&vec, free_fn);

    // // --------- does not work like this ---------
    // for (int i = 0; i < 3; i++) {
    //     int *temp_vec = Vector_init(int);
    //     Vector_push(&vec, &temp_vec);
    // }

    // // --------- instead do it like this ---------
    for (int i = 0; i < 3; i++) {
        int **temp_vec_ptr = malloc(sizeof(int*));
        *temp_vec_ptr = Vector_init(int);
        Vector_push(&vec, temp_vec_ptr);
    }

    print_vector(vec);
    Vector_push(vec[0], 1);
    print_vector(vec);
    Vector_push(vec[0], 2);
    print_vector(vec);
    Vector_push(vec[0], 3);
    print_vector(vec);
    Vector_push(vec[0], 4);
    print_vector(vec);
    Vector_push(vec[0], 5);
    print_vector(vec);

    Vector_destroy(&vec);

    return 0;
}


// this works also but you won't be creating each vector alone, I assume
int main2() {
    int ***vec = Vector_init(int**);

    int *vec1 = Vector_init(int); Vector_push(&vec, &vec1);
    int *vec2 = Vector_init(int); Vector_push(&vec, &vec2);
    int *vec3 = Vector_init(int); Vector_push(&vec, &vec3);

    print_vector(vec);
    Vector_push(vec[0], 1);
    print_vector(vec);
    Vector_push(&vec1, 2);
    print_vector(vec);
    Vector_push(&vec1, 3);
    print_vector(vec);
    Vector_push(&vec1, 4);
    print_vector(vec);
    Vector_push(&vec1, 5);
    print_vector(vec);

    return 0;
}
