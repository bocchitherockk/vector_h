#include <iostream>
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

int main(void) {
    std::cout << "hello from c++" << std::endl;
    int *vec1 = Vector_init(int);
    for (int i = 0; i < 10; i++) {
        Vector_push(&vec1, i);
    }
    print_vector_int(vec1);
    Vector_destroy(&vec1);
    return 0;
}
