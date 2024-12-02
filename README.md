# Vector.h

## Generic Vector Library in C

This Generic Vector Library provides a flexible and efficient way to manage dynamic arrays in C. It supports useful necessary operations from initialization to insertion, deletion, alongside utility functions for sorting, filtering, mapping, and reducing vectors.

### Installation

To integrate the Generic Vector Library into your project, follow these steps:

1. Clone the repository into your project directory.
2. Cd into the 'modules' directory and execute the python script to git clone the necessary dependencies, (you can do it manually if you want, they are listed in the modules.json file).
3. Cd back and build the source code (create dynamic and static libraries) by executing the command `make export`
4. Include the vector.h header file in your own project and don't forget to compile it without linking with one of the libraries (you can instead compile the vector.c with no need to create a library and link with it). `gcc my_files.c -l:libvector.a -Lpath/to/lib -Wl,-rpath=path/to/lib -o out`
5. Happy coding 🤓

### Compilers

This library uses preprocessor directives defined in the `system_env` module to ensure compatibility with different C compilers by checking support for:

-   Statement expressions
-   Typeof keyword
-   Built-in functions like `__builtin_clz`

This library provides suppport for `c++` as well by preventing name mangling.

### How to use

#### 1. Initialization and Destruction

```c
#include "./vector.h"
int main() {
    int *vec = Vector_init(int);
    // Now you can use `vec` with other vector functions
    Vector_destroy(&vec);
    // The vector gets freed from the memory and cannot be used again as it points to NULL, unless reinitialized
}
```

#### 2. General information

```c
    size_t length         = Vector_length(&vec);
    size_t element_size   = Vector_element_size(&vec);
    size_t capacity       = Vector_capacity(&vec);
    bool   is_full        = Vector_is_full(&vec);
    bool   is_underfilled = Vector_is_underfilled(&vec);
    bool   is_empty       = Vector_is_empty(&vec);
```

#### 3. Adding Elements

```c
    Vector_push(&vec, 10); // Adds an element to the end of the vector
    Vector_insert_at(&vec, 1, 100); // inserts 100 at index 1
    int insertion_index = Vector_insert_sorted(&vec, 5, lambda(int, (int value_in_vec, int value_as_param), { return value_in_vec - value_as_param; }));
    // inserts 5 in it's sorted position, considering that the vector is already sorted and returns the index inserted in
    Vector_concat(&vec, &vec2); // pushes the elements in vec2 to the end of vec
```

#### 4. Accessing Elements

```c
    int value = vec[0]; // Access the first element
    vec[3]    = 15; // setting the value at index 3 to 15
    int *ptr  = &vec[0]; // this is a wrong approach, as this pointer will be pointing to a wrong address when the vector gets resized
```

#### 5. Removing Elements

```c
    int value, index;
    value = Vector_pop(&vec); // Removes the last element and returns it
    value = Vector_remove_at(&vec, 3); // Removes the value at the index 3 and returns it
    index = Vector_remove_value(&vec, 10, lambda(bool, (int value_in_vec, int value_as_param), { return value_in_vec == value_as_param; }))
    // Removes a given value from the vector and returns it's index
    // A function is provided as an argument because costum data types do not get compared using ==
    // Also this adds flexibility in terms of using other logic rather than a simple ==
    // if no value matches the callback function, an assertion error will be raised
    Vector_clear(&vec); // Clears all values from the vector
```

#### 6. Utility Functions

```c
    int index = Vector_index(&vec, 10, lambda(bool, (int value_in_vec, int value_as_param), { return value_in_vec == value_as_param; })) // returns the index of the first value that validates the compare function. If none, raise an assertion error
    int count = Vector_count(&vec, 10, lambda(bool, (int value_in_vec, int value_as_param), { return value_in_vec == value_as_param; })) // returns the number of elements that validate the compare function
    int *vec2 = Vector_copy(&vec) // returns a shallow copy of the vector
    Vector_reverse(&vec) // reverses the vector in place
    Vector_sort(&vec, lambda(int, (int value_in_vec, int value_as_param), { return value_in_vec - value_as_param; })); // Sorts the vector in place according to the sorting function given using merge sort algorithm
    int *vec3 = Vector_filter(&vec, lambda(bool, (int value_in_vec), { return int value_in_vec % 2 == 0; })); // returns a new filtered vector of even numbers
    Vector_foreach(&vec, lambda(void, (int *value_in_vec_ptr), { *value_in_vec_ptr *= 2; })); // multiplies each value in the vector by 2, modifies the vector in place
    int *vec4 = Vector_map(&vec, lambda(int, (int value_in_vec), { return value_in_vec + 2; }), int); // Returns a new vector with each value mapped by the mapper function, the result of the map function is a vector and it's type is specified in the third parameter
    int sum = Vector_reduce(&vec, lambda(int, (int accumulator, int value_in_vec), { return accumulator + value_in_vec; }), 0); // Calculates the sum of elements
    bool all = Vector_all(&vec, lambda(bool, (int value_in_vec), { return value_in_vec == 10; })); // checks to see if all values in the vector verify the callback function (the callback returns true)
    bool any = Vector_any(&vec, lambda(bool, (int value_in_vec), { return value_in_vec == 10; })); // checks to see if any of the values in the vector verify the callback function
    int *vec5 = Vector_slice(&vec, 0, Vector_length(&vec), 2); // slices vec from index 0 to Vector_length(vec) by a step 2 and returns it
```
