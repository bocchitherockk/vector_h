#include <iostream>
#include <cassert>
#define VECTOR_IMPLEMENTATION
#include "../vector.h"

void print_vector_int(int *vec) {
    std::cout << "{" << std::endl;
    std::cout << "    element_size: " << Vector_get_element_size(&vec) << std::endl;
    std::cout << "    length: " << Vector_get_length(&vec) << std::endl;
    std::cout << "    capacity: " << Vector_get_capacity(&vec) << std::endl;
    std::cout << "    data: [";
    if (Vector_get_length(&vec) != 0) {
        for (size_t i = 0; i < Vector_get_length(&vec); i++) {
            std::cout << vec[i] << ", ";
        }
    }
    std::cout << "]\n}" << std::endl;
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
    std::cout << "hello from c++" << std::endl;
    int *vec1 = Vector_init(int);
    int arr[10] = {0};
    for (int i = 0; i < 10; i++) {
        Vector_push(&vec1, i);
        arr[i] = i;
    }

    assert(
        equal_vec_arr(vec1, arr, sizeof(arr) / sizeof(arr[0]))
        == true
    );
    Vector_destroy(&vec1);

    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}
