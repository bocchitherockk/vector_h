#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../vector.h"

void print_vector(char ***vec) {
    size_t vec_length = Vector_get_length(vec);
    printf("{\n");
    printf("    element_size: %ld,\n", Vector_get_element_size(vec));
    printf("    length: %ld,\n", vec_length);
    printf("    capacity: %ld,\n", Vector_get_capacity(vec));
    printf("    data: [");
    for (int i = 0; i < vec_length; i++) {
        printf("\"%s\"%s", (*vec)[i], vec_length - 1 == i ? "" : ", ");
    }
    printf("]\n");
    printf("}\n");
}

bool equal_vec_arr(char **vec, char *arr[], size_t arr_length) {
    if (Vector_get_length(&vec) != arr_length) {
        return false;
    }
    for (size_t i = 0; i < arr_length; i++) {
        if (strcmp(vec[i], arr[i]) != 0) {
            return false;
        }
    }
    return true;
}


int main(void) {
    char hello[]       = "hello";
    char world[]       = "world";
    char exclamation[] = "!";
    char yassine[]     = "yassine";
    char *hamza        = "hamza"; // this one is read-only due to the nature of C

    char *result1[] = { "hello", "world", "!", "yassine", "hamza" };

    char **vec = Vector_init(char *);
    Vector_push(&vec, hello);
    Vector_push(&vec, world);
    Vector_push(&vec, exclamation);
    Vector_push(&vec, yassine);
    Vector_push(&vec, hamza);
    assert(
        equal_vec_arr(vec, result1, sizeof(result1) / sizeof(result1[0]))
        == true
    );

    char swap[] = "swap";
    vec[0] = swap; // this is valid because the vector stores pointers to the strings, not the strings themselves
    char *result2[] = { "swap", "world", "!", "yassine", "hamza" };
    assert(
        equal_vec_arr(vec, result2, sizeof(result2) / sizeof(result2[0]))
        == true
    );

    vec[0] = "mary"; // this is also valid because the vector stores pointers to the strings, not the strings themselves
    char *result3[] = { "mary", "world", "!", "yassine", "hamza" };
    assert(
        equal_vec_arr(vec, result3, sizeof(result3) / sizeof(result3[0]))
        == true
    );

    // this is not valid because the first string was changed to a read-only string, so we cannot modify it anymore
    // vec[0][0] =  'S';
    // this is valid because the second string is still modifiable
    vec[1][0] = 'W';
    char *result4[] = { "mary", "World", "!", "yassine", "hamza" };
    assert(
        equal_vec_arr(vec, result4, sizeof(result4) / sizeof(result4[0]))
        == true
    );

    printf("All tests passed successfully!\n");

    return 0;
}
